//
// C++ Implementation: UMAC
//

//
//
#include "UMAC.h"
#include <QCoreApplication>
/*The constructor of UMAC class.You can use it to create an object with a connection to the device.
The default timeout is 5 second and that means if there is anything wrong on hardware(bad cabel,
failed power supply,incorrect ip adress,etc..),the soft will try to connect it within 5 seconds.
After that,if still unconnected,a QString except will be thrown out with the infomation on the error.
You can catch and handle it in youe app.You can only create 5 in your app and that should be enough.*/
UMAC::UMAC ( const std::string& ip_address,const unsigned int port )
{
    client.sin_addr.s_addr = inet_addr ( ip_address.c_str() );
    client.sin_family = PF_INET;
    client.sin_port = htons ( port );
    memset(client.sin_zero,'\0',sizeof(client.sin_zero));

    if(connectDevice()==ERROR)
        throw QString(strerror(errno))+"   connection to UMAC error!";
}

UMAC::~UMAC()
{
    cleanclose();
    //qDebug("Delete UMAC");
}

/*This is a private one to be invoked by the constructor to implement the connection to the device.
DO NOT call it in your app directly via some hack way. */
int UMAC::connectDevice() //for the timeout of connect,use select
{
    int length,result;
    unsigned long block = 1;
    fd_set set;
    struct timeval time_out;
    int ret = 0;
    length = sizeof ( client );

    if ( ( sock = socket ( PF_INET,SOCK_STREAM, 0 ) ) ==-1 )
        goto unsocketed;

    if ( setTimeoutForSendRecv() )
        return ERROR;

    ioctl ( sock,FIONBIO,&block );//set UNBLOCK MODE
    result = connect ( sock, ( __CONST_SOCKADDR_ARG ) &client, ( socklen_t ) length );
    if ( result ==-1 )
    {
        if ( errno!=EINPROGRESS )
            //	return ( perror ( "connectDevice(connect):error" ),ERROR );;//need to check late.this is caused by time out.incorrect ip adress?maybe...
            goto unconnected;
        time_out.tv_sec = CONNECT_TIME_OUT;
        time_out.tv_usec = 0;
        FD_ZERO ( &set );
        FD_SET ( sock,&set );
        ret = select ( sock+1,NULL,&set,NULL,&time_out );//Check if it is writable.
        if ( ret==0 )              //timeout error
            goto unconnected;
        else if ( ret==-1 )      //select error
        {
            int error;
            getsockopt(sock,SOL_SOCKET,SO_ERROR,&error,(socklen_t*)&length);
            if(error==0)
                goto connected;
            else
                goto unconnected;
        }
        else goto connected;     //only one sock,do not need FD_ISSET...
    }
connected:
    block = 0;
    ioctl ( sock,FIONBIO,&block );//set BLOCK MODE
    return 0;
unsocketed:
    cleanclose();
    return ( perror ( "connectDevice(socket):error" ),ERROR );
unconnected:
    cleanclose();
    return ( perror ( "connectDevice(connect):error" ),ERROR );
}

/*This function is for user to send proper commands to the device and receive the corresponsed responses.
The first parameter is for your command and you will have the answer in the second parameter which is a QString.
If sent command success completely,0 is returned,else if any error occurs,-1 will return and the errno will be set to show the info of this error.
NOTES
1.The max bytes of the command sent to the device is 255.If more,the fun will return ERROR with a message show on the console.
2.Only common command is allowed here.If you want to send ctrl charactors,please refer to ctrlCmdPMAC(...).
3.More than one commands sent in one string is supported and you have to separate the answers with each other yourself from cResponse if you need.*/
int UMAC::commCmdPMAC ( const char cCommand[],QString& cResponse ) //cCommand max is 255.
{
    if ( !cCommand || strlen ( cCommand ) >256 )
        return ( perror ( "Invalid parameter.Should not be NULL or more than 255 charactors" ),ERROR );
    cResponse.clear();

    ETHERNETCMD EthCmd;
    char temp[MAXDATA+1];
    int ret;

    EthCmd.RequestType = VR_DOWNLOAD;
    EthCmd.Request     = VR_PMAC_GETRESPONSE;
    EthCmd.wValue      = 0;
    EthCmd.wIndex      = 0;
    EthCmd.wLength     = htons ( ( u_int16_t ) strlen ( ( const char* ) cCommand ) );
    //strncpy ( &EthCmd.bData[0],cCommand,strlen ( cCommand ) );//need +1?
    strncpy ( (char*)&EthCmd.bData[0],cCommand ,strlen ( cCommand ));//need +1?  change  strlen ( cCommand )

#ifdef CHECK_DATA
    if ( ( cleanDataAvailable() ) ==-1 )
        return ERROR;
#endif

    if ( send ( sock,&EthCmd,ETHERNETCMDSIZE + strlen ( cCommand ),0 ) ==-1 )
        return ( perror ( "commCmdPMAC(send):error" ),ERROR );
    do // i do not know if i should use GETBUFFER or use like this to get all available data.need to check late.
    {
        memset ( temp,'\0',sizeof ( temp ) );
        ret=recv ( sock,temp,MAXDATA,0 );
        if ( ret == -1 )
            return ( perror ( "commCmdPMAC(recv):error" ),ERROR );
        //		printf("%d",strlen(temp));
        temp[strlen(temp)-2]='\0';//to delete the ACK charactor
        cResponse += temp;
    }
    while ( ret == MAXDATA );
    return 0;
}

/*This function is for user to send control commands to the device.Just use any command you want list in UMAC.h as
the first parameter and you can get the answer in the second parameter which is a QString.
If your first input is invalid(not refered in the head file),-1 will return without sending anything.
If success,0 return,if any error,-1 return with the error info set in errno.*/
int UMAC::ctrlCmdPMAC ( const int cCommand,QString& cResponse )
{
    if ( cCommand < 0 || cCommand > 26 )
        return ( perror ( "Invalid parameter.Should not be less than 0 or more than 26." ),ERROR );
    cResponse.clear();

    ETHERNETCMD EthCmd;
    char temp[MAXDATA+1];
    int ret = 0;
    EthCmd.RequestType = VR_UPLOAD;
    EthCmd.Request     = VR_CTRL_RESPONSE;
    EthCmd.wValue      = htons ( cCommand );//here i do not know
    EthCmd.wIndex      = 0;
    EthCmd.wLength     = htons ( MAXDATA );

#ifdef CHECK_DATA
    if ( ( cleanDataAvailable() ) ==-1 )
        return ERROR;
#endif

    if ( send ( sock,&EthCmd,ETHERNETCMDSIZE,0 ) ==-1 )
        return ( perror ( "ctrlCmdPMAC(send):error" ),ERROR );
    do // i do not know if i should use GETBUFFER or use like this to get all available data.need to check late.
    {
        memset ( temp,'\0',sizeof ( temp ) );
        ret=recv ( sock,temp,MAXDATA,0 );
        if ( ret == -1 )
            return ( perror ( "ctrlCmdPMAC(recv):error" ),ERROR );
        temp[strlen(temp)-2]='\0';//to delete the ACK charactor
        cResponse += temp;
    }
    while ( ret == MAXDATA );
    return 0;
}

/*This function is for user to send some datas to DPRAM.The first parameter is the buffer for tha datas to be sent
and the second one is the address you want to put your data which is the offset from the base address of the DPRAM.
Success and fail handle are as the same as above.*/
// int UMAC::sendDataDPRAM ( const char cData[],long offset )
// {
// 	int data_length = strlen ( cData );
// 	int len=0;//to decide where the copy start that is the offset
// 	char data;//for repley
// 
// 	ETHERNETCMD EthCmd;
// 	EthCmd.RequestType = VR_UPLOAD;
// 	EthCmd.Request     = VR_PMAC_SETMEM;
// 	EthCmd.wIndex      = 0;
// 
// #ifdef CHECK_DATA
// 	if ( ( cleanDataAvailable() ) ==-1 )
// 		return ERROR;
// #endif
// 
// 	do
// 	{
// 		EthCmd.wValue= htons ( offset ); //
// 		EthCmd.wLength= htons ( data_length >= MAXDATA ? ( offset+=MAXDATA,data_length-=MAXDATA,MAXDATA ) : data_length );
// 		strncpy ( &EthCmd.bData[0],cData+len,EthCmd.wLength );
// 		len+=EthCmd.wLength;
// 		if ( send ( sock, &EthCmd,ETHERNETCMDSIZE+ EthCmd.wLength,0 ) ==-1 )
// 			return ( perror ( "sendDataDPRAM(send):error" ),ERROR );
// 		if ( recv ( sock, &data,MAXDATA,0 ) ==-1 )
// 			return ( perror ( "sendDataDPRAM(recv):error" ),ERROR );
// 
// 	}
// 	while ( EthCmd.wLength==MAXDATA && data_length );//both for more than 1400 and equal 1400 :-)
// 	return 0;
// }

// int UMAC::sendDataDPRAM ( const char cData[],long offset,const int command_type )
// {
// 	int data_length = strlen ( cData );
// 	int len;
// 	char data;
// 	EthCmd.RequestType = VR_UPLOAD;
// 	EthCmd.Request= command_type;
// 	EthCmd.wIndex= 0;
// 	if ( setTimeoutForSendRecv() )
// 		return OTHER_ERROR;
// 	while ( len=data_length )
// 	{
// 
// 		EthCmd.wValue= htons ( offset ); //
// 		EthCmd.wLength= htons ( data_length > MAXDATA ? ( offset+=MAXDATA,data_length-=MAXDATA,MAXDATA ) :(data_length-=data_length,len) );
// 		strncpy ( &EthCmd.bData[0],cData,EthCmd.wLength );
// 		if ( send ( sock, &EthCmd,ETHERNETCMDSIZE+ EthCmd.wLength,0 ) ==-1 )
// 			return SEND_ERROR;
// 		if ( recv ( sock, &data,MAXDATA,0 ) ==-1 )
// 			return RECV_ERROR;
// 
// 	}
// 	return 0;
// }


/*This function is for people to get some datas from a proper address of DPRAM.The first parameter is for the
storage of the coming datas and the second one is the base address where the copy start which is an offset
value with the same meaning descriputed above in sendDataDPRAM and last is the length of the datas you want to copy.
No specical for success and fail handle.*/
// int UMAC::getDataDPRAM ( QString& data,long offset,int data_length )
// {
// 	char temp[MAXDATA+1];
// 	data.clear();
// 
// 	ETHERNETCMD EthCmd;
// 	EthCmd.RequestType = VR_UPLOAD;
// 	EthCmd.Request     = VR_PMAC_GETMEM;
// 	EthCmd.wIndex      = 0;
// 
// #ifdef CHECK_DATA
// 	if ( ( cleanDataAvailable() ) ==-1 )
// 		return ERROR;
// #endif
// 
// 	do
// 	{
// 		memset ( temp,'\0',sizeof ( temp ) );
// 		EthCmd.wValue= htons ( offset ); //i do not know if this variable is const or not,need to be check late.
// 		EthCmd.wLength= htons ( data_length >= MAXDATA ? ( offset+=MAXDATA,data_length-=MAXDATA,MAXDATA ) : data_length );
// 		if ( send ( sock,&EthCmd,ETHERNETCMDSIZE,0 ) ==-1 )
// 			return ( perror ( "getDataDPRAM(send):error" ),ERROR );
// 		if ( recv ( sock, temp,MAXDATA,0 ) == -1 ) //blocked ,timeout 1 second.
// 			return ( perror ( "getDataDPRAM(recv):error" ),ERROR );
// 		temp[strlen(temp)-2]='\0';//to delete the ACK charactor 
// 		data+=temp;
// 	}
// 	while ( EthCmd.wLength==MAXDATA && data_length );
// 	return 0;
// }

// int UMAC::download_file ( char * file_path )
// {
//
// }

/*This is for user to clean any available data inside the UMAC and close the connection to it.You can call it before the
end of you app.*/
void UMAC::cleanclose()
{
    int i = 0;
    char buf[100];
    shutdown ( sock, SHUT_WR );
    do
    {
        i = recv ( sock, buf, 100, 0 );
    }
    while ( i > 0 );
    shutdown ( sock, SHUT_RD );

    close ( sock );
}

/*This is a private function to set timeout value for send() and recv().Only for library developer,not for library user.*/
// int UMAC::setTimeoutForSendRecv()
// {
// 	struct timeval send_time_out;
// 	send_time_out.tv_sec=SEND_TIME_OUTS;
// 	send_time_out.tv_usec=SEND_TIME_OUTU;
// 	if ( setsockopt ( sock,SOL_SOCKET,SO_SNDTIMEO, ( char * ) &send_time_out.tv_sec,sizeof ( struct timeval ) ) ==-1 )
// 		return 1;
// 	struct timeval recv_time_out;
// 	recv_time_out.tv_sec=RECV_TIME_OUTS;
// 	recv_time_out.tv_usec=RECV_TIME_OUTU;
// 	if ( setsockopt ( sock,SOL_SOCKET,SO_RCVTIMEO, ( char * ) &recv_time_out.tv_sec,sizeof ( struct timeval ) ) ==-1 )
// 		return 1;
// 
// 	int send_buffed = 0;//for performance
// 	if ( setsockopt ( sock,SOL_SOCKET,SO_SNDBUF, ( char * ) &send_buffed,sizeof ( send_buffed ) ) ==-1 )
// 		return 1;
// 
// 	int recv_buffed = 0; //for performance
// 	if ( setsockopt ( sock,SOL_SOCKET,SO_RCVBUF, ( char * ) &recv_buffed,sizeof ( recv_buffed ) ) ==-1 )
// 		return 1;
// 
// 	return 0;
// }

/*This is a private function to set timeout value for send() and recv().Only for library developer,not for library user.*/
int UMAC::setTimeoutForSendRecv()
{
    struct timeval send_time_out;
    send_time_out.tv_sec=SEND_TIME_OUTS;
    send_time_out.tv_usec=SEND_TIME_OUTU;
    if ( setsockopt ( sock,SOL_SOCKET,SO_SNDTIMEO, ( char * ) &send_time_out.tv_sec,sizeof ( struct timeval ) ) ==-1 )
        return 1;
    struct timeval recv_time_out;
    recv_time_out.tv_sec=RECV_TIME_OUTS;
    recv_time_out.tv_usec=RECV_TIME_OUTU;
    if ( setsockopt ( sock,SOL_SOCKET,SO_RCVTIMEO, ( char * ) &recv_time_out.tv_sec,sizeof ( struct timeval ) ) ==-1 )
        return 1;

    int send_buffed = 32 * 1024;              //0;//for performance
    if ( setsockopt ( sock,SOL_SOCKET,SO_SNDBUF, ( char * ) &send_buffed,sizeof ( send_buffed ) ) ==-1 )
        return 1;

    int recv_buffed = 32 * 1024;  //0; //for performance
    if ( setsockopt ( sock,SOL_SOCKET,SO_RCVBUF, ( char * ) &recv_buffed,sizeof ( recv_buffed ) ) ==-1 )
        return 1;

    return 0;
}
/*Check if there is any data inside the UMAC.Private fun only for library developer.*/
int UMAC::isDataAvailable()
{
    char data[2];
    ETHERNETCMD EthCmd;
    EthCmd.RequestType = VR_UPLOAD;
    EthCmd.Request     = VR_PMAC_READREADY;
    EthCmd.wValue      = 0;
    EthCmd.wIndex      = 0;
    EthCmd.wLength     = htons ( 2 );

    if ( send ( sock,&EthCmd,ETHERNETCMDSIZE,0 ) ==-1 )
        return ( perror ( "isDataAvailable(send):error" ),ERROR );
    if ( recv ( sock, data,2,0 ) == -1 ) //blocked ,timeout 1 second.
        return ( perror ( "isDataAvailable(recv):error" ),ERROR );
    return data[0];
}

/*Get all available data inside the UMAC.Private fun only for library developer.*/
int UMAC::getDataAvailable()
{
    char data[MAXDATA+1];
    memset ( data,'\0',sizeof ( data ) );

    ETHERNETCMD EthCmd;
    EthCmd.RequestType = VR_DOWNLOAD;
    EthCmd.Request     = VR_PMAC_GETBUFFER;
    EthCmd.wValue      = 0;
    EthCmd.wIndex      = 0;
    EthCmd.wLength     = htons ( strlen ( "a" ) );//here i am not very sure...

    if ( send ( sock,&EthCmd,ETHERNETCMDSIZE,0 ) ==-1 )
        return ( perror ( "getDataAvailable(send):error" ),ERROR );
    if ( recv ( sock, data,MAXDATA,0 ) == -1 ) //blocked ,timeout 1 second.
        return ( perror ( "getDataAvailable(recv):error" ),ERROR );
    return data[0];
}

/*A light package of the above two funs to check and clean any available data in the UMAC.These three funs is only
an insurence before we send command to the device and it is not a must.To make sure there is no other unrelated data
which may affect the coming sent command's response stuffs .If you want it work in the library,
just add #define CHECK_DATA in your app.By default,it will be forbidden for performance reason.Because one call will
cause send two different packages whick may delay the app.*/
int UMAC::cleanDataAvailable()
{
    int ret;

    if ( ( ret=isDataAvailable() ) ==-1 )
        return ERROR;
    else if ( ret )
        if ( cleanDataAvailable() ==-1 )
            return ERROR;
    return 0;
}





////////////sendDataDPRAM-int*

int UMAC::sendDataDPRAM(const int iData [],uint16_t offset, int iDatalength)
{
    uint16_t data_length = iDatalength;
    uint16_t datalength;
    ETHERNETINT EthCmd;

    int len=0;//to decide where the copy start that is the offset,
    int data[1400];///for repley     10 ?

    EthCmd.RequestType = VR_UPLOAD;
    EthCmd.Request     = VR_PMAC_SETMEM;
    EthCmd.wIndex      = 0;

#ifdef CHECK_DATA
    if ( ( cleanDataAvailable() ) ==-1 )
        return ERROR;
#endif

    do
    {

        EthCmd.wValue= htons ( offset ); //

        datalength = data_length >= FOURMAXDATA ? ( offset+=4*FOURMAXDATA, data_length-=FOURMAXDATA,FOURMAXDATA ) : data_length;
        EthCmd.wLength= htons ( 4*datalength );


        //memcpy ( EthCmd.bData,iData+len,datalength );
        for (int i=0,j= len; i < datalength; i++,j++)
        {

            EthCmd.bData[i] = iData[j];

        }
        len+=datalength;


        if ( send (sock, &EthCmd,ETHERNETCMDSIZE+ 4*datalength,0 ) ==-1 )
            return ( perror ( "sendDataDPRAM(send):error" ),ERROR );

        if ( recv (sock, &data,140,0 ) == -1 )
            return ( perror ( "sendDataDPRAM(recv):error" ),ERROR );

    }
    while ( datalength == FOURMAXDATA && data_length );//both for more than 1400 and equal 1400 :-)


    //if ( setTimeoutForSendRecv() )   //change
    //	return ERROR;
    //@@

    //	qDebug("send Done");
    return 0;
}     


//////////////////////////////getdataDPRAM----int*
// int UMAC::getDataDPRAM (int* data , uint16_t offset, int length)
// {
// 	
// 	int temp[1400];
// 	int  nRecvBuf; 
// 	int  datalength;  
// 	ETHERNETINT EthCmd;
// 	QString str;		
// 	int data_length = length;   
// 	
// 	int len =0;
// 	memset ( data,0,sizeof ( data ) );  
// 	
// 
// 	EthCmd.RequestType = VR_UPLOAD;
// 	EthCmd.Request     = VR_PMAC_GETMEM;
// 	EthCmd.wIndex      = 0;
// 
// #ifdef CHECK_DATA
// 	if ( ( cleanDataAvailable() ) ==-1 )
// 		return ERROR;
// #endif
// 
// 	do
// 	{
// 		if(getDataDPRAM(str,0x0678,4,1) != 1) /// check
// 		//if(getDataDPRAM(str,0x1140,1,1) != 1) /// check
// 		{
// 			qDebug("sleep(0)");
// 			sleep(0);
// 			//QCoreApplication::processEvents();
// 		}
// 
// 		memset ( temp,0,sizeof ( temp ) ); 
// 		//bIsMax = false;
// 		
// 		
// 		EthCmd.wValue= htons ( offset ); //i do not know if this variable is const or not,need to be check late.
// 		                                    //vary @
// 
// 		// the following fixed
// 		 
// 		datalength = (data_length >= FOURMAXDATA ? ( offset+=(FOURMAXDATA*4),data_length-=FOURMAXDATA, FOURMAXDATA ) : data_length ); 
// 		
// 		EthCmd.wLength= htons ( 4*datalength );
// 		
// 		
// 		if ( send ( sock,&EthCmd,ETHERNETCMDSIZE,0 ) ==-1 ) 
// 			return ( perror ( "getDataDPRAM(send):error" ),ERROR );	
// 		
// 		
// 		if ( recv ( sock, temp,1400 ,0 ) == -1 ) 
// 			return ( perror ( "getDataDPRAM(recv):error" ),ERROR );
//                		
// 		for(int i = 0,j = len; i < datalength; i++, j++)
// 		{
// 			data[j] = temp[i];
// 			
// 		}
// 		len+=datalength;
// 		
// 
// 		
// 	}while ( datalength == FOURMAXDATA && data_length );  // change && bIsMax
// 	
// 	return 0;
// }	



int UMAC::getDataDPRAM (int* data , uint16_t offset, int length)
{
    int temp[1400];// '\0' buffer  @@@@
    int  nRecvBuf; //iTimeout,
    int  datalength;  // add datalength
    ETHERNETINT EthCmd;
    int data_length = length;   //@try@: data_length = 2,3,4;
    int len =0;
    memset ( data,0,sizeof ( data ) );  //data.clear();//@@@length*sizeof (int)
    int intTemp;
    QString str;
    char set[2];
    set[0]=0x80;
    set[1]=0x00;
    char clear[2] ;
    clear[0] = 0x10;
    clear[1] = 0x00;

    EthCmd.RequestType = VR_UPLOAD;
    EthCmd.Request     = VR_PMAC_GETMEM;
    EthCmd.wIndex      = 0;

#ifdef CHECK_DATA
    if ( ( cleanDataAvailable() ) ==-1 )
        return ERROR;
#endif

    do
    {


        while(getDataDPRAM(str,0x006C,4,1) !=1) /// check 0x067B 0x1140
        {
            char s[255];
            sprintf ( s,"get 0x006c,4,1 error,at offset %04X",offset);
            qDebug(s);
            sleep(0);
            // 			QCoreApplication::processEvents();
        }

        sendDataDPRAM(set, 0x006B);

        memset ( temp,0,sizeof ( temp ) );


        EthCmd.wValue= htons ( offset ); //i do not know if this variable is const or not,need to be check late.
        //vary @


        datalength = (data_length >= FOURMAXDATA ? ( offset+=(FOURMAXDATA*4),data_length-=FOURMAXDATA, FOURMAXDATA ) : data_length );
        EthCmd.wLength= htons ( 4*datalength );

        // while (one send , and one recv),  offset vary  @
        if ( send ( sock,&EthCmd,ETHERNETCMDSIZE,0 ) ==-1 )
            return ( perror ( "getDataDPRAM(send):error" ),ERROR );

        if ( recv ( sock, temp,1400 ,0 ) == -1 ) //blocked ,timeout 1 second.  change MAXDATA
            return ( perror ( "getDataDPRAM(recv):error" ),ERROR );




        for(int i = 0,j = len; i < datalength; i++, j++)
        {
            data[j] = temp[i];

        }
        len+=datalength;


    }while ( datalength == FOURMAXDATA && data_length );  // change && bIsMax

    sendDataDPRAM(clear,0x006B);

    return 0;
}




//////////////////////////////////////////////////////////////////////////////////////////////////

////////////sendDataDPRAM-Float*

int UMAC::sendDataDPRAM(const float fData [],uint16_t offset, int iDatalength)
{
    uint16_t data_length = iDatalength;
    uint16_t datalength;
    ETHERNETFLOAT EthCmd;
    //	int rc,nSendBuf,nRecvBuf;// iTimeout, nSendBuf;

    //qDebug("send data_length: %d", data_length);



    int len=0;//to decide where the copy start that is the offset,
    float data[1400];//for repley    1400 ?

    EthCmd.RequestType = VR_UPLOAD;
    EthCmd.Request     = VR_PMAC_SETMEM;
    EthCmd.wIndex      = 0;

#ifdef CHECK_DATA
    if ( ( cleanDataAvailable() ) ==-1 )
        return ERROR;
#endif


    do
    {

        EthCmd.wValue= htons ( offset ); //

        datalength = data_length >= FOURMAXDATA ? ( offset+=4*FOURMAXDATA, data_length-=FOURMAXDATA,FOURMAXDATA ) : data_length; //INT
        EthCmd.wLength= htons ( 4*datalength );


        //memcpy ( EthCmd.bData,iData+len,datalength );
        for (int f=0,i=0,j= len; f < datalength; i++,j++,f++)    //EthCmd.bData 两位   iData 一位
        {

            EthCmd.bData[i] = fData[j];

        }
        len+=datalength;


        if ( send (sock, &EthCmd,ETHERNETCMDSIZE+ 4*datalength,0 ) ==-1 )  //change EthCmd.wLength
            return ( perror ( "sendDataDPRAM(send):error" ),ERROR );

        if ( recv (sock, &data,140,0 ) == -1 )		///  1400 ?
            return ( perror ( "FLoatsendDataDPRAM(recv):error" ),ERROR );

    }
    while ( datalength == FOURMAXDATA && data_length );//both for more than 1400 and equal 1400 :-)


    ///if ( setTimeoutForSendRecv() )   //change
    //	qDebug("send Done");
    return 0;
}     


//////////////////////////////getdataDPRAM----float*
int UMAC::getDataDPRAM (float * data , uint16_t offset, int length)
{
    float temp[1401];
    int  nRecvBuf;
    int  datalength;
    ETHERNETFLOAT EthCmd;
    int data_length = length;
    int len =0;
    static int getDataDPRAM_first =0;


    memset ( data,0,sizeof ( data ) );

    EthCmd.RequestType = VR_UPLOAD;
    EthCmd.Request     = VR_PMAC_GETMEM;
    EthCmd.wIndex      = 0;

#ifdef CHECK_DATA
    if ( ( cleanDataAvailable() ) ==-1 )
        return ERROR;
#endif


    do
    {
        memset ( temp,0,sizeof ( temp ) );


        EthCmd.wValue= htons ( offset ); //i do not know if this variable is const or not,need to be check late.

        // the following fixed

        datalength = (data_length >= FOURMAXDATA ? ( offset+=(FOURMAXDATA*4),data_length-=FOURMAXDATA, FOURMAXDATA ) : data_length );

        EthCmd.wLength= htons ( 4*datalength );


        if ( send ( sock,&EthCmd,ETHERNETCMDSIZE,0 ) ==-1 )
            return ( perror ( "getDataDPRAM(send):error" ),ERROR );


        if ( recv ( sock, temp,1400 ,0 ) == -1 )
            return ( perror ( "getDataDPRAM(recv):error" ),ERROR );

        // data[] vary @
        for(int i = 0,j = len; i < datalength; i++, j++)
        {
            data[j] = temp[i];

        }
        len+=datalength;



    }
    while ( datalength == FOURMAXDATA && data_length );  // change && bIsMax
    //	qDebug ("receive \n");
    return 0;

}



///////////////////////////////*
/*This function is for user to send some datas to DPRAM.The first parameter is the buffer for tha datas to be sent
and the second one is the address you want to put your data which is the offset from the base address of the DPRAM.
Success and fail handle are as the same as above.*/
int UMAC::sendDataDPRAM ( const char cData[], uint16_t offset )
{

    uint16_t data_length = (uint16_t )strlen ( cData );
    uint16_t datalength;
    ETHERNETCMD EthCmd;
    int rc,nSendBuf;
    int len=0;//to decide where the copy start that is the offset,����
    char data;//for repley

    EthCmd.RequestType = VR_UPLOAD;
    EthCmd.Request     = VR_PMAC_SETMEM;
    EthCmd.wIndex      = 0;

#ifdef CHECK_DATA
    if ( ( cleanDataAvailable() ) ==-1 )
        return ERROR;
#endif



    do
    {

        EthCmd.wValue= htons ( offset ); //

        datalength = data_length >= MAXDATA ? ( offset+=(MAXDATA/2),data_length-=MAXDATA,MAXDATA ) : data_length;
        EthCmd.wLength= htons ( datalength );


        memcpy ( EthCmd.bData,cData+len,datalength );  // May be EthCmd.wLength
        len+=datalength;


        if ( send ( sock, &EthCmd,ETHERNETCMDSIZE+ datalength,0 ) ==-1 )  //change EthCmd.wLength
            return ( perror ( "sendDataDPRAM(send):error" ),ERROR );

        if ( recv ( sock, &data,1,0 ) == -1 )
            return ( perror ( "sendDataDPRAM(recv):error" ),ERROR );

    }
    while ( datalength == MAXDATA && data_length );//both for more than 1400 and equal 1400 :-)

    ///if ( setTimeoutForSendRecv() )   //change
    //return ERROR;
    //cleanclose();
    //connectDevice();

    return 0;
} //changed


/*This function is for people to get some datas from a proper address of DPRAM.The first parameter is for the
storage of the coming datas and the second one is the base address where the copy start which is an offset value
with the same meaning descriputed above in sendDataDPRAM and last is the length of the datas you want to copy.
No specical for success and fail handle.*/
int UMAC::getDataDPRAM ( QString& data,uint16_t offset,int data_length,int check )
{

    char temp[MAXDATA+1];// '\0' buffer
    int  nRecvBuf; //iTimeout,
    int  datalength;  // add datalength
    ETHERNETCMD EthCmd;


    data.clear();


    EthCmd.RequestType = VR_UPLOAD;
    EthCmd.Request     = VR_PMAC_GETMEM;
    EthCmd.wIndex      = 0;

#ifdef CHECK_DATA
    if ( ( cleanDataAvailable() ) ==-1 )
        return ERROR;
#endif


    do
    {
        memset ( temp,'\0',sizeof ( temp ) );
        EthCmd.wValue= htons ( offset ); //i do not know if this variable is const or not,need to be check late.

        datalength = (data_length >= RECVMAXDATA ? ( offset+=RECVMAXDATA,data_length-=RECVMAXDATA, RECVMAXDATA ) : data_length );  // change MAXDATA
        EthCmd.wLength= htons ( datalength );


        if ( send ( sock,&EthCmd,ETHERNETCMDSIZE,0 ) ==-1 )
            return ( perror ( "getDataDPRAM(send):error" ),ERROR );

        if ( recv ( sock, temp,datalength ,0 ) == -1 ) //blocked ,timeout 1 second.  change MAXDATA
            return ( perror ( "getDataDPRAM(recv):error" ),ERROR );




        //temp[strlen(temp)-2]='\0';//to delete the ACK charactor
        data+=temp;

    }
    while ( datalength == RECVMAXDATA && data_length );  // change && bIsMax

    if(check)
    {
        bool flag = false;
        (temp[3]&0x80) ? flag = true: flag = false;
        return flag;
    }

    return 0;
}
/*
int UMAC::getDataDPRAM(long long * data, uint16_t offset, int length)
{
 INT48 temp[200];///add change // '\0' buffer
 int  nRecvBuf; //iTimeout,
 int  datalength;  // add datalength
 ETHERNETINT48 EthCmd;
 int data_length = length;
 int len =0;
 static int getDataDPRAM_first =0;
 //bool bIsMax = false;
 QString str;

 memset ( data,0,sizeof ( data ) );  //data.clear();



 EthCmd.RequestType = VR_UPLOAD;
 EthCmd.Request     = VR_PMAC_GETMEM;
 EthCmd.wIndex      = 0;

#ifdef CHECK_DATA
 if ( ( cleanDataAvailable() ) ==-1 )
  return ERROR;
#endif


 do
 {
  if(getDataDPRAM(str,0x0678,4,1) != 1) /// check
  //if(getDataDPRAM(str,0x1140,1,1) != 1) /// check
  {
   qDebug("sleep(0)");
   sleep(0);
   //QCoreApplication::processEvents();
  }

  memset ( temp,0,sizeof ( temp ) ); //给这个数组初始化
  //bIsMax = false;


  EthCmd.wValue= htons ( offset ); //i do not know if this variable is const or not,need to be check late.
                                      //vary @


  datalength = (data_length >= DOUBLEMAXDATA ? ( offset+=(DOUBLEMAXDATA*8),data_length-=DOUBLEMAXDATA, DOUBLEMAXDATA ) : data_length );
  EthCmd.wLength= htons ( 8*datalength );



   if ( send ( sock,&EthCmd,ETHERNETCMDSIZE,0 ) ==-1 )
    return ( perror ( "getDataDPRAM(send):error" ),ERROR );


   if ( recv ( sock, temp,200 ,0 ) == -1 ) //blocked ,timeout 1 second.  change
    return ( perror ( "getDataDPRAM(recv):error" ),ERROR );

   // data[] vary @
   for(int i = 0,j = len; i < datalength; i++, j++)
   {
    data[j] = changeType(temp[i]);

   }
   len+=datalength;


 }
 while ( datalength == DOUBLEMAXDATA && data_length );  // change && bIsMax
 return 0;

}*/

int UMAC::getinformation(uint16_t offset,long long & followingError, int & servoCommand,
                         int& servoStatus, int& generalStatus, long long& positionBias,
                         int& filteredActuralVelocity, long long& masterPosition, long long& netActualPosition)
{
    int intRe[12];
    long long ll; //temp change long long
    if(getDataDPRAM(intRe, offset, 12) == -1)
        return -1;

    INT48 int48;
    int48.one = intRe[0];
    int48.two = intRe[1];
    ll = typeChange(int48);
    followingError = ll;
    //qDebug("followingError: %lld",followingError);
    int48.one = intRe[5];
    int48.two = intRe[6];
    ll = typeChange(int48);
    positionBias = ll;
    //qDebug("positionBias: %lld",positionBias);
    int48.one = intRe[8];
    int48.two = intRe[9];
    ll = typeChange(int48);
    masterPosition = ll;
    //qDebug("masterPosition: %lld",masterPosition);
    int48.one = intRe[10];
    int48.two = intRe[11];
    ll = typeChange(int48);
    netActualPosition =ll;
    //qDebug("netActualPosition: %lld",netActualPosition);
    //qDebug("netActualPosition: %llf \n",netActualPosition/3072*0.225/3600); //0.0140625
    ////3072*0.0140625/3600
    servoCommand = intRe[2];
    servoStatus = intRe[3];
    generalStatus = intRe[4];
    filteredActuralVelocity = intRe[7];

    return 0;
}





INT48 UMAC::changeType(const long long& sendData)
{
    INT48 buffer;
    int f;
    buffer.one = sendData & 0x00FFFFFF;
    buffer.one = (sendData >> 63) ? (buffer.one | 0xFF000000):(buffer.one & 0x00FFFFFF);

    buffer.two = (sendData >> 24) & 0xFFFFFF;
    buffer.two = (sendData >> 63) ?(buffer.two | 0xFF000000):(buffer.two & 0x00FFFFFF);

    return buffer;
}

long long UMAC::changeType(const INT48& receiveData)
{
    long long buffer=0;
    long long tmp=0;
    buffer = receiveData.one & 0xFFFFFF;
    buffer = ((tmp |( receiveData.two & 16777215)) * 16777216) + buffer;
    buffer = (receiveData.two & 0xFF000000)?(buffer|0xFFFF000000000000) : buffer;
    return buffer;

}

INT48 UMAC::typeChange(const long long& sendData)
{
    return changeType(sendData);
}

long long UMAC::typeChange(const INT48& receiveData)
{
    return changeType(receiveData);
}




int UMAC::getDataDPRAM(long long * data, uint16_t offset, int length)
{
    INT48 temp[200];///add change // '\0' buffer
    int  nRecvBuf; //iTimeout,
    int  datalength;  // add datalength
    ETHERNETINT48 EthCmd;
    int data_length = length;
    int len =0;
    static int getDataDPRAM_first =0;
    //bool bIsMax = false;
    QString str;
    char set[2] ;
    set[0]=0x80;
    set[1]=0x00;
    char clear[2] ;
    clear[0] = 0x10;
    clear[1] = 0x00;

    memset ( data,0,sizeof ( data ) );  //data.clear();



    EthCmd.RequestType = VR_UPLOAD;
    EthCmd.Request     = VR_PMAC_GETMEM;
    EthCmd.wIndex      = 0;

#ifdef CHECK_DATA
    if ( ( cleanDataAvailable() ) ==-1 )
        return ERROR;
#endif


    do
    {
        while(getDataDPRAM(str,0x006C,4,1) !=1) /// check 0x067B 0x1140
        {

            qDebug("sleep(0)");
            sleep(0);
            // 			QCoreApplication::processEvents();
        }
        sendDataDPRAM(set,0x006B);
        for(int i= 0; i<200;i++)
        {
            temp[i].one = 0;
            temp[i].two =0;
        }
        ///memset ( temp,0,sizeof ( temp ) ); //给这个数组初始化

        //bIsMax = false;


        EthCmd.wValue= htons ( offset ); //i do not know if this variable is const or not,need to be check late.
        //vary @


        datalength = (data_length >= DOUBLEMAXDATA ? ( offset+=(DOUBLEMAXDATA*8),data_length-=DOUBLEMAXDATA, DOUBLEMAXDATA ) : data_length );
        EthCmd.wLength= htons ( 8*datalength );



        if ( send ( sock,&EthCmd,ETHERNETCMDSIZE,0 ) ==-1 )
            return ( perror ( "getDataDPRAM(send):error" ),ERROR );


        if ( recv ( sock, temp,200 ,0 ) == -1 ) //blocked ,timeout 1 second.  change
            return ( perror ( "getDataDPRAM(recv):error" ),ERROR );

        // data[] vary @
        for(int i = 0,j = len; i < datalength; i++, j++)
        {
            data[j] = changeType(temp[i]);

        }
        len+=datalength;


    }
    while ( datalength == DOUBLEMAXDATA && data_length );  // change && bIsMax

    sendDataDPRAM(clear,0x006B);
    return 0;

}



