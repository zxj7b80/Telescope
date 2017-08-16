//
// C++ Implementation: direct_azalt.cpp


#include "main_window.h"
#include "direct_azalt.h"
#include "signs.h"
#include "server.h"
#include "sendthread.h"
#include <QSound>
#include <QApplication>
#include <QtTest/QTest>
#include <QDateTime>


#define ERROR   5
#define MaxBufferSize 1024
#define LONG_MESSAGE_LENGTH 512

#define az_vel_spec_BOTTOM     			0
#define az_vel_spec_TOP         		40000
#define az_vel_spec_decimals      		3


#define alt_vel_spec_BOTTOM     		0
#define alt_vel_spec_TOP         		40000
#define alt_vel_spec_decimals      		3

#define az_targetD_BOTTOM       		-60
#define az_targetD_TOP          		450
#define az_targetD_decimals      		10


#define az_targetM_BOTTOM        		-59
#define az_targetM_TOP          		59

#define az_targetS_BOTTOM        		-59.9999
#define az_targetS_TOP          		59.9999
#define az_targetS_decimals      		4

#define alt_targetD_BOTTOM       		-25
#define alt_targetD_TOP           		205
#define alt_targetD_decimals      		10

#define alt_targetM_BOTTOM        		-59
#define alt_targetM_TOP          		59

#define alt_targetS_BOTTOM        		-59.9999
#define alt_targetS_TOP          		59.9999
#define alt_targetS_decimals      		4

#define AzAltCheckStatusClock   		2000	       	//time for timer_AzAltCheckStatus,2500ms
#define MountInfoClock      			1000		//time for timer_MountInfo,1000ms
#define MountSaveClock    			1000		//time for timer_save_data,1000ms
#define MountCheckCommandFromOCSClock    	1000		//time for timer_CheckCommandFromOCS,1000ms
#define MountGoPointClock    			5000		//time for timer_gopoint,5000ms
#define AZreplyOCSClock				5000		//time for timer2_AZreplyOCS,5000ms
#define ALTreplyOCSClock			5000		//time for timer2_ALTreplyOCS,5000ms

extern int OcsPort;						//Port of OCS (TCP/IP)
extern QString OcsIP; 						//IP of OCS(TCP/IP)
extern QString	DataBackToOcs;
extern TrackStar trackstar;

bool UMAC_ETHERNET=TRUE;					// FLAG OF COMMUNICATION WITH UMAC
QString path="";						//current dir
extern FlagTCS flagtcs;
sAxis  	s_Az,s_Alt;							//structure of AZ
//structure of ALTCheckCommandFromOCS()
OcsCOM OcsCom;							//structure of comminication with OCS/TCS
bool flag_ALtbeyondRang=false;
extern long nConnection;					//server connection neumber0
extern QString LocalhostIP;

CSendMessageTCS::CSendMessageTCS ( char* temp )
{
    strcpy ( c_sentstr,temp );
    clientsocket = new QTcpSocket;
    //clientsocket->abort();
    QHostAddress *serverIP;
    serverIP = new QHostAddress();
    serverIP->setAddress ( OcsIP );

    clientsocket->connectToHost ( *serverIP,OcsPort );
    clientsocket->write ( c_sentstr );
    clientsocket->close();
}

DirectAzalt::DirectAzalt ( MainWindow *parent ) :az_error ( ERROR,false ),az_error_info ( ERROR,"" ),alt_error ( ERROR,false ),alt_error_info ( ERROR,"" ) /*:QScrollArea(parent)*/
{
    m_parent = parent;
    setupUi ( this );
    //	connectSignals();
    AzAltCheckStatus();
    showMountInfo();

    timer_MountInfo = new QTimer();
    connect ( timer_MountInfo,SIGNAL ( timeout() ),this,SLOT ( showMountInfo() ) );
    timer_MountInfo->start ( MountInfoClock );

    timer_save_data = new QTimer();
    connect ( timer_save_data,SIGNAL ( timeout() ),this,SLOT ( saveData() ) );
    timer_save_data->start ( MountSaveClock );
    initUi();

    timer2_AZreplyOCS=new QTimer ( this );
    timer2_AZreplyOCS->connect ( timer2_AZreplyOCS, SIGNAL ( timeout() ), this, SLOT ( AZreplyOCS() ) );

    timer2_ALTreplyOCS=new QTimer ( this );
    timer2_ALTreplyOCS->connect ( timer2_ALTreplyOCS, SIGNAL ( timeout() ), this, SLOT ( ALTreplyOCS() ) );

    timer_CheckCommandFromOCS=new QTimer ( this );				// check command from OCS every  second
    timer_CheckCommandFromOCS->connect ( timer_CheckCommandFromOCS, SIGNAL ( timeout() ), this, SLOT ( CheckCommandFromOCS() ) );
    timer_CheckCommandFromOCS->start ( MountCheckCommandFromOCSClock );

    timer_gopoint=new QTimer ( this );
    timer_gopoint->connect ( timer_gopoint, SIGNAL ( timeout() ), this, SLOT ( gopoint() ) );

    timer_AzAltCheckStatus = new QTimer();					//check Az/Alt status
    connect ( timer_AzAltCheckStatus,SIGNAL ( timeout() ),this,SLOT ( AzAltCheckStatus() ) );
    timer_AzAltCheckStatus->start ( AzAltCheckStatusClock );

    flagtcs.IsOpened_azalt =true;
    flagtcs.Az_Homed=false;
    flagtcs.Alt_Homed=false;

    az_hometime=0;
    alt_hometime=0;
    setValidator();
    //writelog *writeLog = new writelog();
    //UMAC_ETHERNET=true;

    //by zxj-----
    datacount=0;

    QString path("/home/book/workspace/QtCode/telescope/data");
    QString datatime = QDateTime::currentDateTime().toString("yyyyMMdd_hhmm");
    mFilename = path + "/" + datatime + "curvedata.txt";

    dataFile = new QFile(mFilename);
    dataFile->open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
    dataFile->flush();
    dataFile->close();
}



void DirectAzalt::CheckTarget()								//make sure targets are in range
{
    if (  s_Alt.PosReal_Degree>90|| s_Alt.PosReal_Degree<3)
    {
        if(!flag_ALtbeyondRang)
        {
            on_pushButton_alt_stop_clicked();

            flag_ALtbeyondRang=true;
        }
    }
}


void DirectAzalt::CheckSoftLimit()	
{
    ///////////Soft LIM+   Az ////////////////////////////
    if (  s_Az.PosReal_Degree>=AzSoftLimitP&&!flagtcs.Az_SoftLimit_P)
    {
        flagtcs.Az_SoftLimit_P=true;
        on_pushButton_az_stop_clicked();
        label_az_soft_limitP->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
        writeLog->writeerror("AZ  Soft LIMIT+","	AZ Soft LIMIT+");
    }
    else if ( s_Az.PosReal_Degree<AzSoftLimitP && flagtcs.Az_SoftLimit_P)
    {
        label_az_soft_limitP->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
        flagtcs.Az_SoftLimit_P=false;
    }
    ///////////Soft LIM-   Az ////////////////////////////
    if (  s_Az.PosReal_Degree<=AzSoftLimitN&&!flagtcs.Az_SoftLimit_N)
    {
        flagtcs.Az_SoftLimit_N=true;
        on_pushButton_az_stop_clicked();
        label_az_soft_limitN->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
        writeLog->writeerror("AZ  Soft LIMIT-","	AZ Soft LIMIT-");
    }
    else if (  s_Az.PosReal_Degree>AzSoftLimitN && flagtcs.Az_SoftLimit_N)
    {
        label_az_soft_limitN->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
        flagtcs.Az_SoftLimit_N=false;
    }

    ///////////Soft LIM+   Alt ////////////////////////////
    if (  s_Alt.PosReal_Degree>=AltSoftLimitP&&!flagtcs.Alt_SoftLimit_P)
    {
        flagtcs.Alt_SoftLimit_P=true;
        on_pushButton_alt_stop_clicked();
        label_alt_soft_limitP->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
        writeLog->writeerror("ALT  Soft LIMIT+","	ALT Soft LIMIT+");

    }
    else if (  s_Alt.PosReal_Degree<AltSoftLimitP && flagtcs.Alt_SoftLimit_P )
    {
        label_alt_soft_limitP->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
        flagtcs.Alt_SoftLimit_P=false;
    }
    ///////////Soft LIM-   Alt ////////////////////////////
    if (  s_Alt.PosReal_Degree<=AltSoftLimitN &&!flagtcs.Alt_SoftLimit_N)
    {
        flagtcs.Alt_SoftLimit_N=true;
        on_pushButton_alt_stop_clicked();
        label_alt_soft_limitN->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
        writeLog->writeerror("ALT  Soft LIMIT-","	ALT Soft LIMIT-");
    }
    else if (  s_Alt.PosReal_Degree>AltSoftLimitN && flagtcs.Alt_SoftLimit_N)
    {
        label_alt_soft_limitN->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
        flagtcs.Alt_SoftLimit_N=false;
    }

}

DirectAzalt::~DirectAzalt()
{
    qDebug ( "Close DirectAzalt" );
    timer_save_data->deleteLater();
    //deleteLater();
    flagtcs.IsOpened_azalt =false;
}

void DirectAzalt::iniNetwork()
{
    OcsIP=SMySetting::instance_p()->value ( "misc/OcsIP" ).toString();
    OcsPort=SMySetting::instance_p()->value ( "misc/OcsPort" ).toInt();
}
/*input:
 OcsCom.flag_PointByOCS_AZ ,Global Variable
 OcsCom.flag_PointByOCS_ALT ,Global Variable
 OcsCom.flag_PointStopByOCS_AZ,Global Variable
 OcsCom.flag_PointStopByOCS_ALT,Global Variable
 OcsCom.flag_TcsCheckAz,Global Variable
 OcsCom.flag_TcsCheckAlt,Global Variable
*/
void DirectAzalt::CheckCommandFromOCS()
{
    if(OcsCom.flag_TcsEmergenStop_AzAlt)
    {
        on_pushButton_az_stop_clicked();
        on_pushButton_alt_stop_clicked();
        OcsCom.flag_TcsEmergenStop_AzAlt=false;
        OcsCom.flag_PointByOCS_AZ=false;
        OcsCom.flag_PointStopByOCS_AZ=false;
        OcsCom.flag_PointingByOCS_AZ=false;
        OcsCom.flag_TcsHomingAz=false;
        OcsCom.flag_PointByOCS_ALT=false;
        OcsCom.flag_PointStopByOCS_ALT=false;
        OcsCom.flag_PointingByOCS_ALT=false;
        OcsCom.flag_TcsHomingAlt=false;
        return;
    }
    if(OcsCom.flag_TcsGetAzAltPosition)
    {
        OcsCom.flag_TcsGetAzAltPosition=false;
        GetAzAltPositionByOCS_AZ();
    }
    if (OcsCom.flag_TcsEnableAz)
    {
        EnableByOCS_AZ();
        OcsCom.flag_TcsEnableAz=false;
    }
    else if (OcsCom.flag_TcsDisableAz)
    {
        DisableByOCS_AZ();
        OcsCom.flag_TcsDisableAz=false;
    }
    else if ( OcsCom.flag_TcsCheckAz )
    {
        TcsCheckAz();
        OcsCom.flag_TcsCheckAz=false;
    }
    else if ( OcsCom.flag_TcsHomeAz )
    {
        HomeByOCS_AZ();
        OcsCom.flag_TcsHomeAz=false;
        OcsCom.flag_TcsHomingAz=true;
    }
    else if ( OcsCom.flag_PointByOCS_AZ )
    {
        PointByOCS_AZ();
        OcsCom.flag_PointByOCS_AZ=false;
        OcsCom.flag_PointingByOCS_AZ=true;
        OcsCom.flag_PointStopByOCS_AZ=false;
    }
    else if ( OcsCom.flag_PointStopByOCS_AZ )
    {
        PointStopByOCS_AZ();
        OcsCom.flag_PointByOCS_AZ=false;
        OcsCom.flag_PointStopByOCS_AZ=false;
        OcsCom.flag_PointingByOCS_AZ=false;
        OcsCom.flag_TcsHomingAz=false;
    }

    if (OcsCom.flag_TcsEnableAlt)
    {
        EnableByOCS_ALT();
        OcsCom.flag_TcsEnableAlt=false;
    }
    else if (OcsCom.flag_TcsDisableAlt)
    {
        DisableByOCS_ALT();
        OcsCom.flag_TcsDisableAlt=false;
    }
    else if ( OcsCom.flag_TcsCheckAlt )
    {
        TcsCheckAlt();
        OcsCom.flag_TcsCheckAlt=false;
    }
    else if ( OcsCom.flag_TcsHomeAlt )
    {
        HomeByOCS_ALT();
        OcsCom.flag_TcsHomeAlt =false;
        OcsCom.flag_TcsHomingAlt=true;
    }
    else if ( OcsCom.flag_PointByOCS_ALT ) 			//ALT start
    {
        PointByOCS_ALT();
        OcsCom.flag_PointByOCS_ALT=false;
        OcsCom.flag_PointingByOCS_ALT=true;					//pointing
        OcsCom.flag_PointStopByOCS_ALT=false;
    }
    else if ( OcsCom.flag_PointStopByOCS_ALT )
    {
        PointStopByOCS_ALT();
        OcsCom.flag_PointByOCS_ALT=false;
        OcsCom.flag_PointStopByOCS_ALT=false;
        OcsCom.flag_PointingByOCS_ALT=false;
        OcsCom.flag_TcsHomingAlt=false;
    }

    else if (OcsCom.flag_TcsUnlockAlt)
    {
        UnlockByOCS_ALT();
        OcsCom.flag_TcsUnlockAlt=false;
    }
    else if (OcsCom.flag_TcsLockAlt)
    {
        LockByOCS_ALT();
        OcsCom.flag_TcsLockAlt=false;
    }
}

/* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
   StatusCUID:return to OCS, new time
   ExeStatus:Start , Active , Done
 RV:    0:working normally;1:warning;-1:fault
       Info:   if no error:"OK"; or,imformation of error
 bExeStatus:  start working or finish working:"true";still working:false
 DataBack:	Additional data back to OCS,eg. positon

*/

void DirectAzalt::MakeString ( char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp )
{
    QDateTime datetime;
    QString s_datetime;

    datetime = QDateTime::currentDateTime();
    s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

    QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +s_datetime+
            QString ( "><ExeStatus=%1><RV=%2><Info=%3><bExeStatus=%4>" ).arg ( ExeStatus ).arg ( RV ).arg ( Info ).arg ( bExeStatus ) +DataBack;
    ( *s_temp ).append ( str );
}


/* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
   StatusCUID:return to OCS, new time
   ExeStatus:Start , Active , Done
 RV:    0:working normally;1:warning;-1:fault
       Info:   if no error:"OK"; or,imformation of error
 bExeStatus:  start working or finish working:"true";still working:false
 DataBack:	Additional data back to OCS,eg. positon
*/
/*
   input:OcsCom.StatusCUID_MA,Gloable Variable

*/
void DirectAzalt::GetAzAltPositionByOCS_AZ()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";

    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    if ( flagtcs.Az_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );
    DataBack=QString ( "<Az=%L1><Alt=%L2>" ).arg ( s_Az.PosReal_Degree,0,'f',8 ).arg ( s_Alt.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void DirectAzalt::EnableByOCS_AZ()
{
    // char StatusCUID_MA[100]="2.9.0.1.1.20061123.123456.4";
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";


    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_Az,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    on_pushButton_az_enable_clicked();
    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    if ( flagtcs.Az_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );
    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    MakeString ( OcsCom.StatusCUID_Az,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_Az,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void DirectAzalt::DisableByOCS_AZ()
{
    // char StatusCUID_MA[100]="2.9.0.1.1.20061123.123456.4";
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";


    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_Az,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);
    on_pushButton_az_disable_clicked();

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    if ( flagtcs.Az_Amplifier )
        sprintf ( Info,"OK Disable" );
    else
        sprintf ( Info,"Open Loop" );
    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );
    MakeString ( OcsCom.StatusCUID_Az,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_Az,ExeStatus,bExeStatus,&s_temp2 );
    SendToOCS ( s_temp2 );
}

void DirectAzalt::TcsCheckAz()
{
    // char StatusCUID_MA[100]="2.9.0.1.1.20061123.123456.4";
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.Az_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    //qDebug()<<"Info"<<QString(Info)<<"amplifier"<<flagtcs.Az_Amplifier;
    //MakeString ( OcsCom.StatusCUID_Az,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_Az,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    DataBack=QString ( "<Pos=%1>" ).arg ( s_Az.PosReal_Degree,0,'f',8 );
    if(flagtcs.Az_Homed) DataBack+="<Homed=true>";
    else DataBack+="<Homed=false>";
    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    MakeString ( OcsCom.StatusCUID_Az,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_Az,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void DirectAzalt::CombineString ( char StatusCUID[100],char ExeStatus[50],char bExeStatus[10],QString* s_temp )
{
    QDateTime datetime;
    QString s_datetime;

    datetime = QDateTime::currentDateTime();
    s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

    QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +
            s_datetime+QString ( "><ExeStatus=%1><bExeStatus=true>" ).arg ( ExeStatus );
    ( *s_temp ).append ( str );
}

// void   DirectAzalt::SendToOCS ( QString str )
// {
// //  	m_parent->server->tcp_socket->connectToHost (OcsIP,OcsPort);
// // 	QByteArray cmdSend;
// // 	QDataStream out(&cmdSend,QIODevice::ReadWrite);
// // 	out.setVersion ( QDataStream::Qt_4_0 );
// // 	out << (quint16)0;
// // 	out << str;
// // 	out.device()->seek(0);
// // 	out << ( quint16 ) ( cmdSend.size() - sizeof ( quint16 ) );
// // 	m_parent->server->tcp_socket->write(cmdSend);
// //         m_parent->server->tcp_socket->close();
// 	m_parent->server->ConnectServer ( str );
// 	DataBackToOcs=str;
// }


void DirectAzalt::SendToOCS(QString str)
{
    sendthread *send = new sendthread(m_parent,str,OcsIP,OcsPort);
    //sendthread *send = new sendthread(m_parent,str,LocalhostIP,OcsPort);
    send->run();

}




/*input:
 OcsCom.StatusCUID_MA ,Global Variable
 flagtcs.Alt_Amplifier ,Global Variable

*/
void DirectAzalt::EnableByOCS_ALT()
{
    // char StatusCUID_MA[100]="2.9.0.1.1.20061123.123456.4";
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";

    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_Alt,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    SendToOCS ( s_temp );
    QTest::qWait(250);
    on_pushButton_alt_enable_clicked();

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    if ( flagtcs.Az_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    MakeString ( OcsCom.StatusCUID_Alt,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_Alt,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void DirectAzalt::DisableByOCS_ALT()
{
    // char StatusCUID_MA[100]="2.9.0.1.1.20061123.123456.4";
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";


    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_Alt,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    SendToOCS ( s_temp );
    QTest::qWait(250);
    on_pushButton_alt_disable_clicked();

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    if ( flagtcs.Az_Amplifier )
        sprintf ( Info,"OK Disable" );
    else
        sprintf ( Info,"Open Loop" );
    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    MakeString ( OcsCom.StatusCUID_Alt,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_Alt,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void DirectAzalt::TcsCheckAlt()
{
    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
    StatusCUID:return to OCS, new time
    ExeStatus:Start , Active , Done
  RV:    0:working normally;1:warning;-1:fault
        Info:   if no error:"OK"; or,imformation of error
  bExeStatus:  start working or finish working:"true";still working:false
   DataBack:	Additional data back to OCS,eg. positon
 */
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";


    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_Alt,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    if ( flagtcs.Alt_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );
    DataBack=QString ( "<Pos=%1>" ).arg ( s_Alt.PosReal_Degree,0,'f',8 );
    if(flagtcs.Alt_Homed) DataBack+="<Homed=true>";
    else DataBack+="<Homed=false>";
    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    MakeString ( OcsCom.StatusCUID_Alt,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_Alt,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );

    /*	QDateTime datetime;
  QString s_datetime;
  QString s_temp;
  datetime = QDateTime::currentDateTime();
  s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );

  s_temp = "<RelatedCmdCUID=";

  QString temp1;
  temp1.sprintf ( "%s",OcsCom.StatusCUID_MA );
  //qDebug()<<temp1;
  s_temp.append ( temp1 );
  s_temp.append ( "><StatusCUID=" );
  s_temp.append ( s_datetime );

  s_temp.append ( "><ExeStatus=Start><RV=0><Info=" );

  if ( flagtcs.Alt_Amplifier )
   s_temp.append ( "OK>" );
  else
   s_temp.append ( "Fault>" );
 */

    // 	QByteArray ba;
    // 	char* comm;
    // 	ba= s_temp.toLatin1();
    // 	comm = ba.data();
    // 	CSendMessageTCS *sendMessageTCS ;
    // 	sendMessageTCS = new CSendMessageTCS ( comm );


    // ClientSocket->write ( comm );

    //qDebug() <<s_temp;

}

void DirectAzalt::UnlockByOCS_ALT()
{
    QString s_temp,s_temp1,s_temp2,s_temp3;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";

    CombineString ( OcsCom.StatusCUID_AltUnlock,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    on_pushButton_alt_brake_clicked();

    sprintf ( ExeStatus, "Active" );
    sprintf ( Info,"Release start" );
    sprintf ( RV, "1" );
    MakeString ( OcsCom.StatusCUID_AltUnlock,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    QTime dieTime;
    dieTime = QTime::currentTime().addMSecs(20000);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );

    if ( flagtcs.Alt_Release )
        sprintf ( Info,"Release OK" );
    else
        sprintf ( Info,"Release fault" );

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    MakeString ( OcsCom.StatusCUID_AltUnlock,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    SendToOCS ( s_temp2 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_AltUnlock,ExeStatus,bExeStatus,&s_temp3 );
    SendToOCS ( s_temp3 );
}

void DirectAzalt::LockByOCS_ALT()
{
    QString s_temp,s_temp1,s_temp2,s_temp3;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";

    CombineString ( OcsCom.StatusCUID_AltLock,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );

    on_pushButton_alt_brake_clicked();
    sprintf ( ExeStatus, "Active" );
    sprintf ( Info,"Brake start" );
    sprintf ( RV, "1" );
    MakeString ( OcsCom.StatusCUID_AltLock,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    QTime dieTime;
    dieTime = QTime::currentTime().addMSecs(20000);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );

    if ( flagtcs.Alt_Brake )
        sprintf ( Info,"Brake OK" );
    else
        sprintf ( Info,"Brake fault" );

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    MakeString ( OcsCom.StatusCUID_AltLock,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    SendToOCS ( s_temp2 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_AltLock,ExeStatus,bExeStatus,&s_temp3 );
    SendToOCS ( s_temp3 );
}

/*

input:  Az.Az_point,Gloable Variable
 OcsCom.flag_PointByOCS_AZ ,Global Variable
 OcsCom.flag_PointingByOCS_AZ,Global Variable
 OcsCom.flag_PointStopByOCS_AZ,Global Variable
*/


void DirectAzalt::PointByOCS_AZ()
{

    int AZ_d=0,AZ_m=0;
    double AZ_s=0;
    char s[255];
    QDateTime datetime;
    QString s_datetime;

    QString s_temp,s_temp1;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[20];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.Az_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    //MakeString ( OcsCom.StatusCUID_AzPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_AzPoint,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    SendToOCS ( s_temp );
    QTest::qWait(250);

    if(s_Az.point<390 && s_Az.point>-30)
    {
        D2DMS ( s_Az.point,&AZ_d,&AZ_m,&AZ_s );
        sprintf ( s,"%3d",int ( AZ_d ) );//target,
        lineEdit_az_targetD->setText ( s );
        sprintf ( s,"%3d",int ( AZ_m ) );
        lineEdit_az_targetM->setText ( s );
        sprintf ( s,"%6.3f",AZ_s );
        lineEdit_az_targetS->setText ( s );
        on_radioButton_az_vel_fast_toggled (true);
        on_pushButton_az_go_clicked();
    }
    else
    {
        OcsCom.flag_PointingByOCS_AZ=false;
        sprintf ( RV,"-1" );
        sprintf ( ExeStatus,"Active" );
        sprintf ( Info,"Beyond Az range" );
        MakeString ( OcsCom.StatusCUID_AzPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        SendToOCS ( s_temp );
        QTest::qWait(250);

        sprintf ( ExeStatus,"Done" );
        sprintf ( bExeStatus,"true" );
        CombineString ( OcsCom.StatusCUID_AzPoint,ExeStatus,bExeStatus,&s_temp1);
        SendToOCS ( s_temp1 );
    }

    if ( !timer2_AZreplyOCS->isActive () )
        timer2_AZreplyOCS->start ( AZreplyOCSClock );//start check every 5 seconds

    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
    StatusCUID:return to OCS, new time
    ExeStatus:Start , Active , Done
  RV:    0:working normally;1:warning;-1:fault
        Info:   if no error:"OK"; or,imformation of error
  bExeStatus:  start working or finish working:"true";still working:false
   DataBack:	Additional data back to OCS,eg. positon
 */



}


/*input: az.azPosReal_Degree ,Global Variable
 Az.Az_point,Global Variable
        OcsCom.StatusCUID_MA ,Global Variable
        OcsCom.flag_PointingByOCS_AZ,Global Variable
*/
void DirectAzalt::PointingByOCS_AZ()
{
    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
    StatusCUID:return to OCS, new time
    ExeStatus:Start , Active , Done
  RV:    0:working normally;1:warning;-1:fault
        Info:   if no error:"OK"; or,imformation of error
  bExeStatus:  start working or finish working:"true";still working:false
   DataBack:	Additional data back to OCS,eg. positon
 */
    QString s_temp,s_temp1;
    //QString temp1;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.Az_Amplifier )
        sprintf ( Info,"Pointing" );
    else
        sprintf ( Info,"Open Loop" );

    DataBack=QString ( "<Pos=%1>" ).arg ( s_Az.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_AzPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);
    if ( fabs ( s_Az.PosReal_Degree-s_Az.point ) <=0.01 ) 			//az reach target
    {
        OcsCom.flag_PointingByOCS_AZ=false;
        //if ( timer2_AZreplyOCS->isActive () )
        //	timer2_AZreplyOCS->stop();

        sprintf ( ExeStatus,"Done" );
        sprintf ( bExeStatus,"true" );
        CombineString ( OcsCom.StatusCUID_AzPoint,ExeStatus,bExeStatus,&s_temp1);
        SendToOCS ( s_temp1 );
    }
}
/*input:
 OcsCom.StatusCUID_MA ,Global Variable
 OcsCom.flag_PointByOCS_AZ ,Global Variable
 OcsCom.flag_PointStopByOCS_AZ,Global Variable
*/

void DirectAzalt::PointStopByOCS_AZ()
{
    OcsCom.flag_TcsHomingAz=false;
    OcsCom.flag_PointingByOCS_AZ=false;
    on_pushButton_az_stop_clicked();

    /*QDateTime datetime;
 QString s_datetime;
 QString s_temp2;
 QString temp1;
    QByteArray ba;*/
    //char* comm;


    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
    StatusCUID:return to OCS, new time
    ExeStatus:Start , Active , Done
  RV:    0:working normally;1:warning;-1:fault
        Info:   if no error:"OK"; or,imformation of error
  bExeStatus:  start working or finish working:"true";still working:false
   DataBack:	Additional data back to OCS,eg. positon
 */
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";


    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_Az,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    if ( flagtcs.Az_Amplifier )
        sprintf ( Info,"OK Stop" );
    else
        sprintf ( Info,"Open Loop" );
    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    MakeString ( OcsCom.StatusCUID_Az,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_Az,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );

}

/*
   input:OcsCom.StatusCUID_MA,Gloable Variable

*/

void     DirectAzalt::HomeByOCS_AZ()
{
    QString s_temp;
    char ExeStatus[50]="Start";
    //char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    //QString DataBack="";
    on_pushButton_az_home_clicked();

    sprintf ( Info,"OK" );

    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_AzHome,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );

    if ( !timer2_AZreplyOCS->isActive () )
        timer2_AZreplyOCS->start ( AZreplyOCSClock );				//start check every 5 seconds
}

/*input: az.azPosReal_Degree ,Global Variable
 Az.Az_point,Global Variable
        OcsCom.StatusCUID_MA ,Global Variable
        OcsCom.flag_PointingByOCS_AZ,Global Variable
*/
void DirectAzalt::HomingByOCS_AZ()
{
    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
    StatusCUID:return to OCS, new time
    ExeStatus:Start , Active , Done
    RV:    0:working normally;1:warning;-1:fault
    Info:   if no error:"OK"; or,imformation of error
    bExeStatus:  start working or finish working:"true";still working:false
   DataBack:	Additional data back to OCS,eg. positon
 */
    QString s_temp;
    QString s_temp1;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.Az_Amplifier )
        sprintf ( Info,"Homing" );
    else
        sprintf ( Info,"Open Loop" );

    DataBack=QString ( "<Pos=%1>" ).arg ( s_Az.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_AzHome,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
    if (flagtcs.Az_Homed ) 			//az reach target
    {
        //if ( timer2_AZreplyOCS->isActive () )
        //	timer2_AZreplyOCS->stop();
        QTest::qWait(250);
        sprintf ( bExeStatus,"true" );
        sprintf ( ExeStatus,"Done" );
        OcsCom.flag_TcsHomingAz=false;
        CombineString ( OcsCom.StatusCUID_AzHome,ExeStatus,bExeStatus,&s_temp1 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp1 );
        return;
    }
    else if( OcsCom.flag_HomeErrorAz)
    {
        on_pushButton_az_stop_clicked();
        //if ( timer2_AZreplyOCS->isActive () )
        //	timer2_AZreplyOCS->stop();
        OcsCom.flag_TcsHomingAz=false;
        sprintf ( bExeStatus,"true" );
        sprintf ( Info,"Fault" );
        MakeString ( OcsCom.StatusCUID_AzHome,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        SendToOCS ( s_temp );
        QTest::qWait(250);
        sprintf ( ExeStatus,"Done" );
        CombineString ( OcsCom.StatusCUID_AzHome,ExeStatus,bExeStatus,&s_temp1 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp1 );
        return;
    }
}


void     DirectAzalt::HomeByOCS_ALT()
{
    QString s_temp;
    char ExeStatus[50]="Start";
    //char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    //QString DataBack="";
    on_pushButton_alt_home_clicked();

    sprintf ( Info,"OK" );

    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_AltHome,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );

    if ( !timer2_ALTreplyOCS->isActive () )
        timer2_ALTreplyOCS->start ( ALTreplyOCSClock );				//start check every 5 seconds
}


void DirectAzalt::HomingByOCS_ALT()
{
    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
       StatusCUID:return to OCS, new time
       ExeStatus:Start , Active , Done
       RV:    0:working normally;1:warning;-1:fault
       Info:   if no error:"OK"; or,imformation of error
       bExeStatus:  start working or finish working:"true";still working:false
      DataBack:	Additional data back to OCS,eg. positon
    */
    QString s_temp;
    QString s_temp1;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.Alt_Amplifier )
        sprintf ( Info,"Homing" );
    else
        sprintf ( Info,"Open Loop" );

    DataBack=QString ( "<Pos=%1>" ).arg ( s_Alt.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_AltHome,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
    if (flagtcs.Alt_Homed ) 			//az reach target
    {
        //if ( timer2_ALTreplyOCS->isActive () )
        //    timer2_ALTreplyOCS->stop();
        QTest::qWait(250);
        sprintf ( bExeStatus,"true" );
        sprintf ( ExeStatus,"Done" );
        OcsCom.flag_TcsHomingAlt=false;
        CombineString ( OcsCom.StatusCUID_AltHome,ExeStatus,bExeStatus,&s_temp1 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp1 );
        return;
    }
    else if( OcsCom.flag_HomeErrorAlt)
    {
        on_pushButton_alt_stop_clicked();
        //if ( timer2_AZreplyOCS->isActive () )
        //	timer2_AZreplyOCS->stop();
        OcsCom.flag_TcsHomingAlt=false;
        sprintf ( bExeStatus,"true" );
        sprintf ( Info,"Fault" );
        MakeString ( OcsCom.StatusCUID_AltHome,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        SendToOCS ( s_temp );
        QTest::qWait(250);
        sprintf ( ExeStatus,"Done" );
        CombineString ( OcsCom.StatusCUID_AltHome,ExeStatus,bExeStatus,&s_temp1 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp1 );
        return;
    }
}
/*input:   OcsCom.StatusCUID_MA ,Global Variable
   OcsCom.flag_PointByOCS_ALT,Global Variable
 OcsCom.flag_PointingByOCS_ALT,Global Variable
 OcsCom.flag_PointStopByOCS_ALT,Global Variable
*/

void DirectAzalt::PointByOCS_ALT()
{
    //double tmp1;
    int ALT_d=0,ALT_m=0;
    double ALT_s=0;
    char s[255];
    QDateTime datetime;
    QString s_datetime;
    //CSendMessageTCS *sendMessageTCS ;


    D2DMS ( s_Alt.point,&ALT_d,&ALT_m,&ALT_s );
    sprintf ( s,"%3d",int ( ALT_d ) );
    lineEdit_alt_targetD->setText ( s );
    sprintf ( s,"%3d",int ( ALT_m ) );
    lineEdit_alt_targetM->setText ( s );
    sprintf ( s,"%6.3f",ALT_s );
    lineEdit_alt_targetS->setText ( s );
    on_radioButton_alt_vel_fast_toggled (true);
    on_pushButton_alt_go_clicked();

    if ( !timer2_ALTreplyOCS->isActive () )
        timer2_ALTreplyOCS->start ( ALTreplyOCSClock );			//start check every 5 seconds

    QString s_temp;
    char ExeStatus[50]="Start";
    char bExeStatus[10]="true";


    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_AltPoint,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    SendToOCS ( s_temp );

}


/*input:  	OcsCom.StatusCUID_MA ,Global Variable
  OcsCom.flag_PointByOCS_ALT,Global Variable
  OcsCom.flag_PointingByOCS_ALT,Global Variable
  OcsCom.flag_PointStopByOCS_ALT,Global Variable
*/
void DirectAzalt::PointStopByOCS_ALT()
{
    OcsCom.flag_TcsHomingAlt=false;
    OcsCom.flag_PointingByOCS_ALT=false;
    on_pushButton_alt_stop_clicked();
    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
    StatusCUID:return to OCS, new time
    ExeStatus:Start , Active , Done
  RV:    0:working normally;1:warning;-1:fault
        Info:   if no error:"OK"; or,imformation of error
  bExeStatus:  start working or finish working:"true";still working:false
   DataBack:	Additional data back to OCS,eg. positon
 */
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";

    CombineString ( OcsCom.StatusCUID_Alt,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    if ( flagtcs.Alt_Amplifier )
        sprintf ( Info,"OK Stop" );
    else
        sprintf ( Info,"Open Loop" );
    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    MakeString ( OcsCom.StatusCUID_Alt,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf ( ExeStatus,"Done" );
    CombineString ( OcsCom.StatusCUID_Alt,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

/*input: Alt.altPosReal_Degree ,Global Variable
  Alt.Alt_point ,Global Variable
 OcsCom.StatusCUID_MA ,Global Variable
 OcsCom.flag_PointingByOCS_ALT,Global Variable
*/
void DirectAzalt::PointingByOCS_ALT()
{
    QString s_temp,s_temp1;
    //QString temp1;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.Alt_Amplifier )
        sprintf ( Info,"Pointing" );
    else
        sprintf ( Info,"Open Loop" );

    DataBack=QString ( "<Pos=%1>" ).arg ( s_Alt.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_AltPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );

    if ( fabs ( s_Alt.PosReal_Degree-s_Alt.point ) <=0.01 ) 			//az reach target
    {
        QTest::qWait(250);
        OcsCom.flag_PointingByOCS_ALT=false;
        //if ( timer2_ALTreplyOCS->isActive () )
        //	timer2_ALTreplyOCS->stop();
        sprintf ( ExeStatus,"Done" );
        sprintf ( bExeStatus,"true" );
        CombineString ( OcsCom.StatusCUID_AltPoint,ExeStatus,bExeStatus,&s_temp1);
        SendToOCS ( s_temp1 );
    }
}



/*input
OcsCom.flag_PointingByOCS_AZ,Global Variable
*/
void DirectAzalt::AZreplyOCS() // replay OCS every 5 seconds,timer2_AZreplyOCS
{
    if ( OcsCom.flag_PointingByOCS_AZ ) 				//AZ pointing
        PointingByOCS_AZ();

    if ( OcsCom.flag_TcsHomingAz ) 					//AZ homing
        HomingByOCS_AZ();
    if(!OcsCom.flag_PointingByOCS_AZ && !OcsCom.flag_TcsHomingAz)timer2_AZreplyOCS->stop();

}
/*input
OcsCom.flag_PointingByOCS_ALT,Global Variable
*/
void DirectAzalt::ALTreplyOCS() // replay OCS every 5 seconds,timer2_ALTreplyOCS
{   
    if ( OcsCom.flag_PointingByOCS_ALT ) //ALT pointing
        PointingByOCS_ALT();
    if ( OcsCom.flag_TcsHomingAlt ) 					//AZ homing
        HomingByOCS_ALT();
    if(!OcsCom.flag_PointingByOCS_ALT && !OcsCom.flag_TcsHomingAlt)timer2_ALTreplyOCS->stop();
}



void DirectAzalt::initUi()
{
    //QDir dir;
    //dir.mkpath ( "/home/telescope/telescope" );
    path = QDir::currentPath();
    //qDebug() <<path;


    radioButton_az_vel_slow->setChecked ( TRUE );
    radioButton_az_stepMid->setChecked ( TRUE );
    radioButton_alt_vel_slow->setChecked ( TRUE );
    radioButton_alt_stepMid->setChecked ( TRUE );

    radioButton_az_stepLong->setText ( QString::number ( SMySetting::instance_p()->value ( "az_alt/az_stepLong" ).toDouble() ) );
    radioButton_az_stepMid->setText ( QString::number ( SMySetting::instance_p()->value ( "az_alt/az_stepMid" ).toDouble() ) );
    radioButton_az_stepShort->setText ( QString::number ( SMySetting::instance_p()->value ( "az_alt/az_stepShort" ).toDouble() ) );

    radioButton_alt_stepLong->setText ( QString::number ( SMySetting::instance_p()->value ( "az_alt/alt_stepLong" ).toDouble() ) );
    radioButton_alt_stepMid->setText ( QString::number ( SMySetting::instance_p()->value ( "az_alt/alt_stepMid" ).toDouble() ) );
    radioButton_alt_stepShort->setText ( QString::number ( SMySetting::instance_p()->value ( "az_alt/alt_stepShort" ).toDouble() ) );

    CheckAzHoming();
    CheckAltHoming();


}
void DirectAzalt::closeEvent ( QCloseEvent *event )
{
    //parentWidget()->hide();
    //event->ignore();
}

/*
input: Az.azPointer,,Global Variable

*/
void DirectAzalt::saveData()
{
    if ( !radioButton_save->isChecked() )
        return;

    /*    if ( s_Az.Pointer>299 )
 {
                storeAzData();
                s_Az.Pointer=0;

 }

        if ( s_Alt.Pointer>299 )
 {
  storeAltData();
                s_Alt.Pointer=0;
    }*/

    QDateTime dt=QDateTime::currentDateTime();
    QTime time=dt.time();
    QString dtstr;
    dtstr=dt.toString ( "MM_dd_yyyy" );
    QString fileName ="/home/scope/telescope/data/test"+dtstr+".txt";
    QFile file ( fileName );
    if ( !file.open ( QFile::Append|QFile::Truncate ) )
    {
        QMessageBox::warning ( this,
                               tr ( "Warn" ),
                               tr ( "open error!" ),
                               QMessageBox::Yes );
        //qDebug("AzAlt can't open file for save.");
        return;
    }
    QTextStream out ( &file );

    //out<<"time Az_position Az_velocity Az_error Alt_position Alt_velocity Alt_error\n";
    double utc_now=time.hour()  *3600+time.minute() *60.+time.second()+time.msec()/1000.;
    /*sprintf ( s_1,"  %10.6f",s_Az.PosReal_Degree );
        sprintf ( s_2,"  %8.2f",azVelocity );
        sprintf ( s_2,"  %6.2f",azFolErrUMAC );
        sprintf ( s_3,"  %10.6f",s_Alt.PosReal_Degree );
        sprintf ( s_2,"  %8.2f",altVelocity );
        sprintf ( s_4,"  %6.2f",altFolErrUMAC );
        sprintf ( s_0,"%s%s%s%s       ",s_1,s_2,s_3,s_4 );
        QString string = QString ( QLatin1String ( s_0 ) ) + DataTime[i];*/
    char strdata[600];
    sprintf ( strdata,"%7.2f\t %10.7f\t %8.2f\t %8.2f\t %10.7f\t %8.2f\t %8.2f\t  \n",utc_now,
              s_Az.PosReal_Degree,s_Az.Velocity,s_Az.error,
              s_Alt.PosReal_Degree,s_Alt.Velocity,s_Alt.error);
    /*sprintf ( strdata,"%7.2f\t %10.7f\t %8.2f\t %8.3f \n",utc_now,
                          s_Az.PosReal_Degree,s_Az.Velocity,s_Az.error);*/
    out<<strdata;



    file.close();
}


/*
 input:	Az.AzDataTime,Global Variable
        Az.AzData,Global Variable
*/
bool DirectAzalt::storeAzData()
{
    //AzDatamm_dd_yy.txt
    QString FileName="/AzData";
    DataSave ( s_Az.Data,s_Az.DataTime,FileName,N );
    return true;
}

/*
 input:	Alt.AltDataTime,Global Variable
        Alt.AltData,Global Variable
*/
bool DirectAzalt::storeAltData()
{
    //AzDatamm_dd_yy.txt
    QString FileName="/AltData";
    DataSave ( s_Alt.Data,s_Alt.DataTime,FileName,N );
    return true;
}

/*  function: save data to a file
//input:
 int num       //matrix Az.AzData rows,#define N 300
double ( *p ) [5]:   save data, Az.AzData[N][5] ,AltData[N][5]
QString DataTime[]:save time
QString FileName: file name

*/

bool DirectAzalt::DataSave ( double ( *p ) [6],QString DataTime[],QString FileName,int num )
{
    char s_0[400] ;
    char s_1[60] ;
    char s_2[60];
    char s_3[60];
    char s_4[60];
    int i,j;
    double t_end,t_start,t;

    QStringList  list1 = DataTime[299].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );

    int it=0;
    for ( QStringList::iterator point = list1.begin(); point != list1.end();++it, ++point )
    {}
    it=0;
    QStringList  list_time =  list1[1].split ( ":" );
    for ( QStringList::iterator point = list_time.begin(); point != list_time.end();++it, ++point )
    {}
    t_end=list_time[0].toDouble() *3600+list_time[1].toDouble() *60+list_time[2].toDouble();
    //start time
    list1 = DataTime[0].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );

    it=0;
    for ( QStringList::iterator point = list1.begin(); point != list1.end();++it, ++point )
    {}
    it=0;
    list_time =  list1[1].split ( ":" );
    for ( QStringList::iterator point = list_time.begin(); point != list_time.end();++it, ++point )
    {}
    t_start=list_time[0].toDouble() *3600+list_time[1].toDouble() *60+list_time[2].toDouble();

    t=t_end-t_start;
    if ( t<0 )
        t=30;
    ////////////following error/////////////////////////////////////////
    double speed= ( * ( * ( p+num-1 ) +1 ) - * ( * ( p+0 ) +1 ) ) /t;			//Data[num-1][1]-Data[0][1]   get speed

    //get RMS of tracking error
    double Xi=0,Yi=0;
    for ( j=0;j<num;j++ ) 									//  num:   matrix Az.AzData rows
        Xi= * ( * ( p+j ) +2 ) +Xi;							//Data[j][2]

    Xi=Xi/num;

    for ( j=0;j<num;j++ )
        Yi= ( * ( * ( p+j ) +2 )-Xi ) * ( * ( * ( p+j ) +2 )-Xi ) +Yi;			//Data[j][2]
    if ( num>=1 )
        Yi=sqrt ( Yi/num );
    else
        Yi=0;

    //PV
    double min,max;
    double PV;
    max=min=* ( * ( p+0 ) +2 );								//Data[0][2]

    for ( j=0;j<num;j++ )
    {
        if ( max<=* ( * ( p+j ) +2 ) )     						//Data[j][2]
        {
            max=* ( * ( p+j ) +2 );             					//Data[j][2]
        }
        if ( min>* ( * ( p+j ) +2 ) )							//Data[j][2]
        {
            min=* ( * ( p+j ) +2 );							//Data[j][2]

        }
    }
    PV=max-min;



    ////////////// save file////////////////////
    QDateTime dt=QDateTime::currentDateTime();
    QTime time=dt.time();
    QString dtstr;
    dtstr=dt.toString ( "MM_dd_yyyy" );
    QString fileName =SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString()+FileName+dtstr+".txt";
    QFile file ( fileName );
    if ( !file.open ( QFile::Append|QFile::Truncate ) )
    {
        /*QMessageBox::warning ( this,
                         tr ( "Warn" ),
                         tr ( "open error!" ),
                               QMessageBox::Yes );*/
        qDebug("AzAlt can't open file for save.");
        return false;
    }
    QTextStream out ( &file );

    for ( i=0;i<num;i++ )
    {
        out<<"\n";
        sprintf ( s_1,"  %10.6f",* ( * ( p+i ) +0 ) /3600 );				//command position（UMAC,"）     Data[i][0]
        sprintf ( s_2,"  %10.6f",* ( * ( p+i ) +1 ) /3600 );				//real position,"                    Data[i][1]
        sprintf ( s_3,"  %10.4f",* ( * ( p+i ) +3 ) );					//following error,"  ,  PC             Data[i][2]
        sprintf ( s_4,"  %10.4f",* ( * ( p+i ) +2 ) );					//following error,"  ,  UMAC             Data[i][2]
        sprintf ( s_0,"%s%s%s%s       ",s_1,s_2,s_3,s_4 );
        QString string = QString ( QLatin1String ( s_0 ) ) + DataTime[i];
        out<<string;
    }

    sprintf ( s_1," V=  %6.3f''/s,",speed );
    sprintf ( s_2,"RMS=  %6.4f'' ",Yi );
    sprintf ( s_3,"PV=  %6.4f'' ",PV );
    sprintf ( s_0,"%s%s%s ",s_1,s_2,s_3 );
    out<<tr ( s_0 );
    sprintf ( s_1," max=  %6.4f'',",max );
    sprintf ( s_2," min=  %6.4f''",min );
    out<<tr ( s_1 );
    out<<tr ( s_2 );
    file.close();
    return true;


}


void DirectAzalt::writeErrorToOCS ( QString error )
{
    error="";
}



void DirectAzalt::show_MountData()
{

    nConnection++;
    if ( nConnection>=65530 )
        nConnection=0;
}

/*

*/
void DirectAzalt::showMountInfo()
{

    // 	if ( !UMAC_ETHERNET )
    // 	{
    // 		timer_MountInfo->deleteLater();
    // 		return;
    // 	}
    if(!trackstar.flagAzTracking)
    {
        GetAzPosition();
        GetAltPosition();
        //show_MountData();
        GetAzReadHead();
        GetAltReadHead();
    }
    GetMotorCurrent();
    CheckTarget();
}


void DirectAzalt::AzAltCheckStatus()
{

    //if(flagtcs.Az_Homing)CheckAzHoming();	//CHECK HOMING
    //if(flagtcs.Alt_Homing)CheckAltHoming();
    CheckAzHoming();
    CheckAltHoming();
    CheckAmplifier();//Check amplifier status of Az and Alt
    CheckError();//Check error status of Az and Alt
    CheckLoopStatus();
    CheckLimit();
    //CheckSoftLimit();
    CheckAltBrake();
}


void DirectAzalt::on_pushButton_az_enable_clicked()
{
    QString str;
    char cmd[255];
    //sprintf ( cmd,"ENABLE PLC11" );  // ENABLE AZ MOTOR 1#
    sprintf ( cmd,"#21$" );  // ENABLE AZ MOTOR 1#
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}

void DirectAzalt::on_pushButton_az_disable_clicked()
{
    QString str;
    char cmd[255];
    sprintf ( cmd,"#21K" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    /*
//	AzEnabledCheck->setChecked ( FALSE );
//	Az_HomedCheck->setChecked ( FALSE );
 QString str;
 char cmd[255];
    sprintf ( cmd,CmdKillAz );  // ENABLE PLC13
 SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
 sprintf ( cmd,"DISABLE PLC5" );  // STOP AZ HOMING
 SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
 sprintf ( cmd,ClearAzFlag);
 SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
*/
}

void DirectAzalt::on_pushButton_az_home_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC("M2145=0#21HM", str);
    //flagtcs.Az_Homing=true;
    //flagtcs.Az_Homed=false;
    //OcsCom.flag_HomeErrorAz=false;
    //az_hometime=0;
    /*
 QString str;
    SQUMAC::instance_p()->QcommCmdPMAC ( AzPointParam,str );
        SQUMAC::instance_p()->QcommCmdPMAC ( "DISABLE PLC5",str );
    //SQUMAC::instance_p()->QcommCmdPMAC ( "P529=3280088",str );//3280088.9
    //SQUMAC::instance_p()->QcommCmdPMAC ( "P529=3336581.83",str );//offset=-2750s, 2014/05/13
    SQUMAC::instance_p()->QcommCmdPMAC ( "P529=3336314.77",str );//offset=13s, dero, 2014/07/07
    SQUMAC::instance_p()->QcommCmdPMAC ( "P505=3",str );
 SQUMAC::instance_p()->QcommCmdPMAC ( "ENABLE PLC5",str );
    //SQUMAC::instance_p()->QcommCmdPMAC ( "P801=0",str );
    flagtcs.Az_Homing=true;
    flagtcs.Az_Homed=false;
    OcsCom.flag_HomeErrorAz=false;
    az_hometime=0;
    */
}

void DirectAzalt::on_pushButton_alt_home_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC("M1745=0#17HM", str);
    //flagtcs.Alt_Homing=true;
    //flagtcs.Alt_Homed=false;
    //OcsCom.flag_HomeErrorAlt=false;
    //alt_hometime=0;

    /*
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC ( AltPointParam,str );
 SQUMAC::instance_p()->QcommCmdPMAC ( "DISABLE PLC6",str );
    //SQUMAC::instance_p()->QcommCmdPMAC ( "P629=2247197",str );	//at nanjing
    //SQUMAC::instance_p()->QcommCmdPMAC ( "P629=-11064803",str );  //first at delingha, 180degree diffrence
    //SQUMAC::instance_p()->QcommCmdPMAC ( "P629=-11071787",str );//zero at 340as, by cross wire, without mirror
    //SQUMAC::instance_p()->QcommCmdPMAC ( "P629=-11074046.75",str );//offset=110s, 2014/05/13
    //SQUMAC::instance_p()->QcommCmdPMAC ( "P629=-11073828.38",str );//offset=-10.63s, 2014/05/17
    SQUMAC::instance_p()->QcommCmdPMAC ( "P629=-11074218.70",str );//offset=19s, dero, 2014/07/07
    SQUMAC::instance_p()->QcommCmdPMAC ( "P605=3",str );
    SQUMAC::instance_p()->QcommCmdPMAC ( "P624=0",str );
    SQUMAC::instance_p()->QcommCmdPMAC ( "ENABLE PLC6",str );
    //SQUMAC::instance_p()->QcommCmdPMAC ( "P807=0",str );							//flag
    flagtcs.Alt_Homing=true;
    flagtcs.Alt_Homed=false;
    OcsCom.flag_HomeErrorAlt=false;
    alt_hometime=0;
    */
}


void DirectAzalt::on_radioButton_az_vel_fast_toggled ( bool on )
{
    if ( on )
    {
        char cmd[255];
        QString str;
        az_vel = SMySetting::instance_p()->value ( "az_alt/az_vel_fast" ).toDouble();
        lineEdit_az_vel_spec->setText ( QString::number ( az_vel ) );
        az_vel=az_vel/ ( CTS_az*1000 );
        sprintf ( cmd,"I2122=%f",az_vel );
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );


    }
}

void DirectAzalt::on_radioButton_az_vel_mid_toggled ( bool on )
{
    if ( on )
    {
        char cmd[255];
        QString str;
        az_vel = SMySetting::instance_p()->value ( "az_alt/az_vel_mid" ).toDouble();
        lineEdit_az_vel_spec->setText ( QString::number ( az_vel ) );
        az_vel=az_vel/ ( CTS_az*1000 );

        sprintf ( cmd,"I2122=%f",az_vel );
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );


    }
}

void DirectAzalt::on_radioButton_az_vel_slow_toggled ( bool on )
{
    if ( on )
    {
        char cmd[255];
        QString str;
        az_vel = SMySetting::instance_p()->value ( "az_alt/az_vel_slow" ).toDouble();
        lineEdit_az_vel_spec->setText ( QString::number ( az_vel ) );
        az_vel=az_vel/ ( CTS_az*1000 );

        sprintf ( cmd,"I2122=%f",az_vel );
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );

        //  lineEdit_az_vel_spec->setText(QApplication::translate("DirectAzalt", "15", 0, QApplication::UnicodeUTF8));
    }
}

void DirectAzalt::on_lineEdit_az_vel_spec_returnPressed()
{
    //	emit az_vel_spec ( lineEdit_az_vel_spec->text().toFloat() );
    double azVelocityCmd=0;
    char cmd[255];
    QString str;
    azVelocityCmd=lineEdit_az_vel_spec->text().toFloat();
    az_vel = azVelocityCmd/ ( CTS_az*1000 );
    sprintf ( cmd,"I2122=%f",az_vel );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );

}



void DirectAzalt::on_pushButton_az_dirP_pressed() //positive direct
{
    QString str;
    on_lineEdit_az_vel_spec_returnPressed();
    //setPIDparam_Az_going();
    SQUMAC::instance_p()->QcommCmdPMAC ( "#21j+",str );

}
void DirectAzalt::on_pushButton_az_dirP_released()
{
    on_pushButton_az_stop_clicked();

}
void DirectAzalt::on_pushButton_az_dirM_pressed() //negative direct
{
    QString str;
    on_lineEdit_az_vel_spec_returnPressed();
    //setPIDparam_Az_going();
    SQUMAC::instance_p()->QcommCmdPMAC ( "#21j-",str );
}
void DirectAzalt::on_pushButton_az_dirM_released()
{
    on_pushButton_az_stop_clicked();
}


void DirectAzalt::setPIDparam_Az_going()
{
    //AZ
    QString str;
    double azVelocityCmd=0;

    azVelocityCmd=lineEdit_az_vel_spec->text().toFloat();
    if ( fabs ( azVelocityCmd ) >=900 )
        SQUMAC::instance_p()->QcommCmdPMAC (AzPointParam,str );
    else if ( fabs ( azVelocityCmd ) >=100&&fabs ( azVelocityCmd ) <900 )
        SQUMAC::instance_p()->QcommCmdPMAC (AzTrackParam,str );
    else if ( fabs ( azVelocityCmd ) >=30&&fabs ( azVelocityCmd ) <100 )
        SQUMAC::instance_p()->QcommCmdPMAC ( AzTrackParam,str );
    else  if ( fabs ( azVelocityCmd ) <30 )
        SQUMAC::instance_p()->QcommCmdPMAC (AzTrackParam,str );

}

void DirectAzalt::on_pushButton_az_park_clicked()
{
    double az_ParkTarget;
    QString str;

    //az_ParkTarget= SMySetting::instance_p()->value ( "az_alt/az_park" ).toDouble();
    //if(s_Az.PosReal_Degree<0 || s_Az.PosReal_Degree>200)az_ParkTarget= 270;
    //else az_ParkTarget= 90;
    az_ParkTarget= 180;
    on_radioButton_az_vel_fast_toggled ( true );
    on_lineEdit_az_vel_spec_returnPressed();
    //setPIDparam_Az_going();
    ShowAzTarget ( az_ParkTarget );//show park target
    on_pushButton_az_go_clicked();
}

void DirectAzalt::ShowAzTarget ( double AzTarget ) //show Az target:degree to (degree,minute,second)
{
    double AZ_d=0,AZ_m=0;
    double AZ_s=0;
    char s[255];

    AZ_d=int ( AzTarget );
    AZ_m=int ( ( AzTarget-AZ_d ) *60 );
    AZ_s= ( ( AzTarget-AZ_d ) *60-AZ_m ) *60;

    sprintf ( s,"%3d",int ( AZ_d ) );// target,
    lineEdit_az_targetD->setText ( s ); //degree
    sprintf ( s,"%3d",int ( AZ_m ) );
    lineEdit_az_targetM->setText ( s );//minute
    sprintf ( s,"%6.3f",AZ_s );
    lineEdit_az_targetS->setText ( s );//second
}



void DirectAzalt::AZstep2target ( int direction,double realpos )
{
    double AzStep=0;
    double Aztarget;
    try
    {
        if ( radioButton_az_stepLong->isChecked() )
            AzStep =  SMySetting::instance_p()->value ( "az_alt/az_stepLong" ).toDouble();
        else if ( radioButton_az_stepMid->isChecked() )
            AzStep =  SMySetting::instance_p()->value ( "az_alt/az_stepMid" ).toDouble();
        else if ( radioButton_az_stepShort->isChecked() )
            AzStep =  SMySetting::instance_p()->value ( "az_alt/az_stepShort" ).toDouble();
        else if ( radioButton_az_step_spec->isChecked() )
            AzStep =  lineEdit_az_step_spec->text().toFloat();

        AzStep=AzStep*direction;
        Aztarget=realpos+AzStep/3600;//  unit:degree
        ShowAzTarget ( Aztarget );//show  target
        on_pushButton_az_go_clicked();

    }
    catch ( std::string Str ) {qDebug() << QString::fromStdString ( Str );}

}
void DirectAzalt::ALTstep2target ( int direction,double realpos )
{
    double AltStep=0;
    //double Alttarget;
    try
    {
        if ( radioButton_alt_stepLong->isChecked() )
            AltStep =  SMySetting::instance_p()->value ( "az_alt/alt_stepLong" ).toDouble();
        else if ( radioButton_alt_stepMid->isChecked() )
            AltStep =  SMySetting::instance_p()->value ( "az_alt/alt_stepMid" ).toDouble();
        else if ( radioButton_alt_stepShort->isChecked() )
            AltStep =  SMySetting::instance_p()->value ( "az_alt/alt_stepShort" ).toDouble();
        else if ( radioButton_alt_step_spec->isChecked() )
            AltStep =  lineEdit_alt_step_spec->text().toFloat();

        AltStep=AltStep*direction;
        AltStep=realpos+AltStep/3600;//  unit:degree
        ShowAltTarget ( AltStep );//show  target
        on_pushButton_alt_go_clicked();

    }
    catch ( std::string Str ) {qDebug() << QString::fromStdString ( Str );}

}

//step ,positive
//input: az.azPosReal_Degree ,Global Variable
void DirectAzalt::on_pushButton_az_stepP_clicked()
{
    int direction=1;
    double realpostion=s_Az.PosReal_Degree;
    AZstep2target ( direction,realpostion );
}

//step ,minus
//input: az.azPosReal_Degree ,Global Variable
void DirectAzalt::on_pushButton_az_stepM_clicked()
{
    int direction=-1;
    double realpostion=s_Az.PosReal_Degree;
    AZstep2target ( direction,realpostion );
}

void DirectAzalt::on_radioButton_az_stepLong_toggled ( bool on )
{
    if ( on )
    {
        QString str;
        double AzStep;
        AzStep = SMySetting::instance_p()->value ( "az_alt/az_stepLong" ).toDouble();
        lineEdit_az_step_spec->setText ( QString::number ( AzStep ) );

    }
}

void DirectAzalt::on_radioButton_az_stepMid_toggled ( bool on )
{
    if ( on )
    {
        QString str;
        double AzStep;
        AzStep = SMySetting::instance_p()->value ( "az_alt/az_stepMid" ).toDouble();
        lineEdit_az_step_spec->setText ( QString::number ( AzStep ) );

    }
}

void DirectAzalt::on_radioButton_az_stepShort_toggled ( bool on )
{
    if ( on )
    {
        QString str;
        double AzStep;
        AzStep = SMySetting::instance_p()->value ( "az_alt/az_stepShort" ).toDouble();
        lineEdit_az_step_spec->setText ( QString::number ( AzStep ) );

    }
}

//Alt step ,positive
//input: Alt.altPosReal_Degree ,Global Variable
void DirectAzalt::on_pushButton_alt_stepP_clicked()
{
    int direction=1;
    double realpostion=s_Alt.PosReal_Degree;
    ALTstep2target ( direction,realpostion );
}

//Alt step ,minus
//input: Alt.altPosReal_Degree ,Global Variable
void DirectAzalt::on_pushButton_alt_stepM_clicked()
{
    int direction=-1;
    double realpostion=s_Alt.PosReal_Degree;
    ALTstep2target ( direction,realpostion );
}

void DirectAzalt::on_radioButton_alt_stepLong_toggled ( bool on )
{
    if ( on )
    {
        QString str;
        double AltStep;
        AltStep = SMySetting::instance_p()->value ( "az_alt/alt_stepLong" ).toDouble();
        lineEdit_alt_step_spec->setText ( QString::number ( AltStep ) );
    }
}

void DirectAzalt::on_radioButton_alt_stepMid_toggled ( bool on )
{
    if ( on )
    {
        QString str;
        double AltStep;
        AltStep = SMySetting::instance_p()->value ( "az_alt/alt_stepMid" ).toDouble();
        lineEdit_alt_step_spec->setText ( QString::number ( AltStep ) );
    }
}


void DirectAzalt::on_radioButton_alt_stepShort_toggled ( bool on )
{
    if ( on )
    {
        QString str;
        double AltStep;
        AltStep = SMySetting::instance_p()->value ( "az_alt/alt_stepShort" ).toDouble();
        lineEdit_alt_step_spec->setText ( QString::number ( AltStep ) );

    }
}


void DirectAzalt::gopoint()
{

    double azVelocityCmd;
    timer_gopoint->stop();
    QString str;

    on_lineEdit_az_vel_spec_returnPressed();
    //SQUMAC::instance_p()->QcommCmdPMAC ( "I122=3.2",str );
    //SQUMAC::instance_p()->QcommCmdPMAC ( "I130=-28500I131=120I132=140I133=350I135=30",str );
    char cmd[255];
    // QString str;
    az_vel=45;
    azVelocityCmd=az_vel;
    lineEdit_az_vel_spec->setText ( QString::number ( az_vel ) );
    az_vel=az_vel/ ( CTS_az*1000 );
    sprintf ( cmd,"I522=%f",az_vel );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );

    //QString str;

    //setPIDparam_Az_going();
    //SQUMAC::instance_p()->QcommCmdPMAC ( "I130=-16500I131=1100I132=1100I133=1200I135=10I136=0.02I137=0.02I138=0.74I139=0.42I168=0I119=0.5I121=10000 ",str );// with load and roller,
    sprintf ( cmd,"#5J+" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    //	flag_gopoint=true;
}


void DirectAzalt::on_pushButton_az_stop_clicked()
{
    //QApplication::beep();
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC ( "#21j/",str );
}

void DirectAzalt::on_pushButton_alt_enable_clicked()
{
    QString str;
    char cmd[255];
    //sprintf ( cmd,"ENABLE PLC12" );  // ENABLE ALT MOTOR 2# 3#
    sprintf ( cmd,"#17$" );  // ENABLE AZ MOTOR 1#
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}
/*void DirectAzalt::on_pushButton_alt_phase_clicked()
{
 QString str;
 char cmd[255];
 sprintf ( cmd,"ENABLE PLC19" );  // phase ALT MOTOR 2# 3#
 SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}
void DirectAzalt::on_pushButton_az_phase_clicked()
{
 QString str;
 char cmd[255];
 sprintf ( cmd,"ENABLE PLC18" );  // phase AZ MOTOR 1#
 SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}*/
void DirectAzalt::on_pushButton_alt_disable_clicked()
{
    QString str;
    char cmd[255];
    sprintf ( cmd,"#17K" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    /*
 QString str;
 char cmd[255];
    sprintf ( cmd,CmdKillAlt );  // ENABLE PLC14
 SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
 sprintf ( cmd,"DISABLE PLC6" );  // STOP ALT  HOMING
 SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
 //SQUMAC::instance_p()->QcommCmdPMAC ( "P218=0",str );
        */
}



void DirectAzalt::on_radioButton_alt_vel_fast_toggled ( bool on )
{
    if ( on )
    {
        char cmd[255];
        QString str;
        alt_vel = SMySetting::instance_p()->value ( "az_alt/alt_vel_fast" ).toDouble() ;
        lineEdit_alt_vel_spec->setText ( QString::number ( alt_vel ) );
        alt_vel=alt_vel/ ( CTS_alt*1000 );
        sprintf ( cmd,"I1722=%f",alt_vel );
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    }
}

void DirectAzalt::on_radioButton_alt_vel_mid_toggled ( bool on )
{
    if ( on )
    {
        char cmd[255];
        QString str;
        alt_vel = SMySetting::instance_p()->value ( "az_alt/alt_vel_mid" ).toDouble()  ;
        lineEdit_alt_vel_spec->setText ( QString::number ( alt_vel ) );
        alt_vel=alt_vel/ ( CTS_alt*1000 );
        sprintf ( cmd,"I1722=%f",alt_vel );
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    }
}

void DirectAzalt::on_radioButton_alt_vel_slow_toggled ( bool on )
{
    if ( on )
    {
        char cmd[255];
        QString str;
        alt_vel = SMySetting::instance_p()->value ( "az_alt/alt_vel_slow" ).toDouble() ;
        lineEdit_alt_vel_spec->setText ( QString::number ( alt_vel ) );
        alt_vel=alt_vel/ ( CTS_alt*1000 );
        sprintf ( cmd,"I1722=%f",alt_vel);
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    }
}


void DirectAzalt::on_lineEdit_alt_vel_spec_returnPressed()
{
    //	emit alt_vel_spec ( lineEdit_alt_vel_spec->text().toFloat() );
    char cmd[255];
    QString str;
    double altVelocityCmd= lineEdit_alt_vel_spec->text().toFloat();
    alt_vel = altVelocityCmd/ ( CTS_alt*1000 );
    sprintf ( cmd,"I1722=%f",alt_vel );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}

void DirectAzalt::on_pushButton_az_go_clicked()
{
    on_lineEdit_az_vel_spec_returnPressed();
    char cmd[255];
    QString str;
    //setPIDparam_Az_going();

    az_target =  lineEdit_az_targetD->text().toFloat() *3600+lineEdit_az_targetM->text().toFloat() *60+lineEdit_az_targetS->text().toFloat() ;

    if(flagtcs.Az_SoftLimit_P&&(az_target>s_Az.PosReal_Degree*3600))
        return;
    if(flagtcs.Az_SoftLimit_N&&(az_target<s_Az.PosReal_Degree*3600))
        return;


    az_target=az_target /CTS_az;
    sprintf ( cmd,"#21J=%f",az_target );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );




}


void DirectAzalt::on_pushButton_alt_go_clicked()
{
    on_lineEdit_alt_vel_spec_returnPressed();
    char cmd[255];
    QString str;
    setPIDparam_Alt_going();

    alt_target= lineEdit_alt_targetD->text().toFloat() *3600+lineEdit_alt_targetM->text().toFloat() *60+lineEdit_alt_targetS->text().toFloat() ;

    if(flagtcs.Alt_SoftLimit_P&&(alt_target>s_Alt.PosReal_Degree*3600))
        return;
    if(flagtcs.Alt_SoftLimit_N&&(alt_target<s_Alt.PosReal_Degree*3600))
        return;

    alt_target =  alt_target/CTS_alt;
    sprintf ( cmd,"#17J=%f",alt_target );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );


    //   	if((alt_target/3600<0)||(alt_target/3600)>90)
    // 	{
    // 		QMessageBox::warning ( this,
    // 		                      tr ( "Warn" ),
    // 		                       tr ( "Altitude target beyonds range!" ),
    // 		                       QMessageBox::Yes );
    // 	}
    //         else
    // 	{
    //
    // 		sprintf ( cmd,"#2J=%f#3J=%f",alt_target,alt_target );
    // 		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    // 	}



    // 	bool AmplifierStatus=true;
    // 	char StatusCUID_MA[100]="2.9.0.1.1.20061123.123456.4";
    // 	char ExeStatus[50]="START";
    // 	char RV[10]="1";
    // 	char Info[10]="test";
    // 	char bExeStatus[10]="true";
    // 	QString s_temp;
    // 	//QString DataBack="<pos=100>";
    // 	double data=180.123456789123456789;
    // //QString temp1=QString::number ( data,'f',8 );
    // 	QString DataBack=QString ( "<pos=%1>" ).arg ( data,0,'f',8 );
    // 	//    QString DataBack;
    // 	// DataBack.append (" <pos=" );
    // 	//  DataBack.append (temp1);
    // 	//DataBack.append (">" );
    // 	MakeString ( StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    // 	qDebug() <<s_temp;

}



void DirectAzalt::on_pushButton_alt_park_clicked()
{
    double alt_ParkTarget;
    QString str;

    //alt_ParkTarget= SMySetting::instance_p()->value ( "az_alt/alt_park" ).toDouble();
    /*alt_target= lineEdit_alt_targetD->text().toFloat() *3600+lineEdit_alt_targetM->text().toFloat() *60+lineEdit_alt_targetS->text().toFloat() ;
    if(alt_target<1)
    {
        if(s_Alt.PosReal_Degree>40)alt_ParkTarget= 28./3600.;
    else alt_ParkTarget= 180+28./3600.;
    }
    else if(alt_target<360)alt_ParkTarget= 180+28./3600.;
    else alt_ParkTarget= 28./3600.;*/
    alt_ParkTarget=90;
    on_radioButton_alt_vel_fast_toggled ( true );
    on_lineEdit_alt_vel_spec_returnPressed();
    //setPIDparam_Alt_going();
    ShowAltTarget ( alt_ParkTarget );//show park target
    on_pushButton_alt_go_clicked();
}

void DirectAzalt::ShowAltTarget ( double AltTarget ) //show Alt target:degree to (degree,minute,second)
{
    double ALT_d=0,ALT_m=0;
    double ALT_s=0;
    char s[255];

    ALT_d=int ( AltTarget );//ALT target
    ALT_m=int ( ( AltTarget-ALT_d ) *60 );
    ALT_s= ( ( AltTarget-ALT_d ) *60-ALT_m ) *60;//

    sprintf ( s,"%3d",int ( ALT_d ) );//
    lineEdit_alt_targetD->setText ( s ); //degree
    sprintf ( s,"%3d",int ( ALT_m ) );
    lineEdit_alt_targetM->setText ( s );//minute
    sprintf ( s,"%6.3f",ALT_s );
    lineEdit_alt_targetS->setText ( s );//second
}


void DirectAzalt::on_pushButton_alt_stop_clicked()
{

    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC ( "#17j/",str );
}



/*void DirectAzalt::on_pushButton_EmergencyStop_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC ( "#5k#6K",str );
        QString str;
 SQUMAC::instance_p()->QcommCmdPMAC ( EmergencyStop,str );
        pushButton_EmergencyStop->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);\n"
"font: 10pt \"Abyssinica SIL\";\n"
"font: 63 10pt \"URW Gothic L\";\n"
"font: oblique 12pt \"Abyssinica SIL\";\n"
"color: rgb(255, 255, 255);"));

        pushButton_EnableAllAmplifiers->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 170, 127);\n"
"font: 10pt \"Abyssinica SIL\";\n"
"font: 63 10pt \"URW Gothic L\";\n"
"font: oblique 12pt \"Abyssinica SIL\";\n"
" \n"
"color: rgb(0, 0, 0);"));

 QCoreApplication::processEvents();
 sleep(2);
        SQUMAC::instance_p()->QcommCmdPMAC ( EmergencyStopRelease,str );
 }*/


/*void DirectAzalt::on_pushButton_EnableAllAmplifiers_clicked()
{	
        QString str;
 SQUMAC::instance_p()->QcommCmdPMAC ( EnableAmplifiersCabinet,str );
        pushButton_EnableAllAmplifiers->setStyleSheet(QString::fromUtf8("background-color: rgb( 85, 170,0);\n"
"font: 10pt \"Abyssinica SIL\";\n"
"font: 63 10pt \"URW Gothic L\";\n"
"font: oblique 12pt \"Abyssinica SIL\";\n"
" \n"
"color: rgb(255, 255, 255);"));
        pushButton_EmergencyStop->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 255);\n"
"font: 10pt \"Abyssinica SIL\";\n"
"font: 63 10pt \"URW Gothic L\";\n"
"font: oblique 12pt \"Abyssinica SIL\";\n"
"color: rgb(0, 0, 0);"));
 QCoreApplication::processEvents();
 sleep(2);
        SQUMAC::instance_p()->QcommCmdPMAC ( EnableAmplifiersCabinetRelease,str );
}*/

/*
function :  Get Status From UMAC ,such as limit,home...
//input:
char cmd[];			//command that send out to UMAC
int n;     			//number of commands


output:

QStringList  Status		//Status of hardware
*/
void DirectAzalt::GetStatusFromUMAC ( char cmd[],int n,QStringList* Status )
{
    QString str;
    int it=0;
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    *Status= str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );

    for ( QStringList::iterator point = ( *Status ).begin(); point != ( *Status ).end();++it, ++point )
    {}
    if ( it<n )
    {
        //		UMAC_ETHERNET=false;
        return;
    }
    else
        UMAC_ETHERNET=true;
}


void DirectAzalt::CheckAmplifier()
{
    QString str;
    char cmd[255];
    //bool  Alt_Amp1=false, Alt_Amp2=false;

    sprintf ( cmd,"M2138M1738" );// M543M643
    QStringList  list;
    int n=2;
    GetStatusFromUMAC ( cmd,n, &list );

    ///////////amplifier 1     Az ////////////////////////////
    if ( !QString::compare ( list[0], "0" ) ) //amplifier 1，=1：ok
    {
        Az_AmplifierLabel1->setStyleSheet ( "background-color:rgb(0,170, 0)" );  // green,no error
        flagtcs.Az_Amplifier=true;
    }
    else
    {
        Az_AmplifierLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
        flagtcs.Az_Amplifier=false;
    }
    //qDebug()<<"CheckAmplifier()"<<list[0]<<"flagtcs.Az_Amplifier"<<flagtcs.Az_Amplifier;
    ///////////amplifier 1     Alt ////////////////////////////
    if ( !QString::compare ( list[1], "0" ) ) //amplifier 2，=1：ok
    {
        Alt_AmplifierLabel1->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
        flagtcs.Alt_Amplifier=true;//Alt_Amp1=true;
    }
    else
    {
        Alt_AmplifierLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
        flagtcs.Alt_Amplifier=false;//Alt_Amp1=false;
    }
    ///////////amplifier 2     Alt ////////////////////////////
    /*	if ( !QString::compare ( list[2], "1" ) ) //amplifier 2，=1：ok
 {
  Alt_AmplifierLabel2->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
  Alt_Amp2=true;
 }
 else
 {
  Alt_AmplifierLabel2->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
  Alt_Amp2=false;
 }
*/
    /*if ( Alt_Amp1 )
  flagtcs.Alt_Amplifier=true;
 else
        flagtcs.Alt_Amplifier=false;*/
}

void DirectAzalt::CheckLimit()
{
    QString str;
    char cmd[255];
    sprintf ( cmd,"M7018M7016M7014M7012" );//error1~error3, warning1~warning3
    QStringList  list;
    int n=4;
    GetStatusFromUMAC ( cmd,n, &list );

    /////////// LIM+   Az ////////////////////////////
    if ( !QString::compare ( list[0], "1" ) )
    {
        //label_az_hard_limitP->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
        writeLog->writeerror("AZ  LIMIT+","	AZ  LIMIT+");
    }
    else
        label_az_hard_limitP->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit

    //////////LIM- Az   ////////////////////////////
    if ( !QString::compare ( list[1], "1" ) )
    {
        //label_az_hard_limitM->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
        writeLog->writeerror("AZ  LIMIT-","	AZ  LIMIT-");
    }
    else
        label_az_hard_limitM->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
    /////////// LIM+   Alt ////////////////////////////
    if ( !QString::compare ( list[2], "1" ) )
    {
        //label_alt_hard_limitP->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
        writeLog->writeerror("ALT  LIMIT+","	ALT  LIMIT+");
    }
    else
        label_alt_hard_limitP->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit

    //////////LIM- Alt   //////////////////////////////
    if ( !QString::compare ( list[3], "1" ) )
    {
        //label_alt_hard_limitM->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
        writeLog->writeerror("ALT  LIMIT-","	ALT  LIMIT-");
    }
    else
        label_alt_hard_limitM->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit



}

void DirectAzalt::CheckError()
{
    //"M542M642"; //following error
    //"M541M641"; //warning
    QString str;
    char cmd[255];
    sprintf ( cmd,"M2142M1742M2141M1741" );
    QStringList  list;
    int n=4;
    GetStatusFromUMAC ( cmd,n, &list );

    ///////////error1    warning1   Az ////////////////////////////
    if ( !QString::compare ( list[0], "1" ) )
    {
        Az_ErrorLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
        writeLog->writeerror("AZ  error","	AZ motor   error");
        flagtcs.Az_Error=true;
    }
    else if ( !QString::compare ( list[2], "1" ) )
        Az_ErrorLabel1->setStyleSheet ( "background-color:rgb(255, 255, 0)" );// yellow color,warning
    else
    {
        Az_ErrorLabel1->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error and no warning
        flagtcs.Az_Error=false;
    }

    //////////error2    warning2  ALT :1# channel  ////////////////////////////
    if ( !QString::compare ( list[1], "1" ) )
    {
        Alt_ErrorLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
        writeLog->writeerror("ALT 1 error","	ALT motor 1# error");
        flagtcs.Alt_Error=true;
    }
    else if ( !QString::compare ( list[3], "1" ) )
        Alt_ErrorLabel1->setStyleSheet ( "background-color:rgb(255, 255, 0)" );// yellow color,warning
    else
    {
        Alt_ErrorLabel1->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error and no warning
        flagtcs.Alt_Error=false;
    }
    //////////error3    warning3  ALT :2# channel  ////////////////////////////
    /*	if ( !QString::compare ( list[2], "1" ) )
 {
  Alt_ErrorLabel2->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
   writeLog->writeerror("ALT 2 error","	ALT motor 2# error");
 }
 else if ( !QString::compare ( list[5], "1" ) )
  Alt_ErrorLabel2->setStyleSheet ( "background-color:rgb(255, 255, 0)" );// yellow color,warning
 else
  Alt_ErrorLabel2->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error and no warning

*/
}


void DirectAzalt::CheckLoopStatus()
{
    QString str;
    char  cmd[255];
    sprintf ( cmd,"M2138M1738" );//
    QStringList  list;
    int n=2;
    GetStatusFromUMAC ( cmd,n, &list );

    ///////////loop 1     Az ////////////////////////////
    if ( !QString::compare ( list[0], "0" ) ) //loop 1，=0：ok
    {
        Az_LoopLabel1->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
        flagtcs.Az_LoopStatus=true;
    }
    else
    {
        Az_LoopLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
        flagtcs.Az_LoopStatus=false;
    }
    ///////////loop 1     Alt ////////////////////////////
    if ( !QString::compare ( list[1], "0" ) ) //alt loop 1，=0：ok
    {
        Alt_LoopLabel1->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
        flagtcs.Alt_LoopStatus=true;
    }
    else
    {
        Alt_LoopLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
        flagtcs.Alt_LoopStatus=false;
    }
    ///////////loop2     Alt ////////////////////////////
    /*	if ( !QString::compare ( list[2], "0" ) ) //alt loop2，=0：ok
  Alt_LoopLabel2->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
 else
                Alt_LoopLabel2->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error*/

}

void DirectAzalt::CheckAzHoming()
{

    QString str;
    char  cmd[255];
    sprintf ( cmd,"M2145" );
    QStringList  list;
    int n=1;
    GetStatusFromUMAC ( cmd,n, &list );
    /*
    if ( !QString::compare ( list[0], "0" ) )
    {
        label_az_homed->setStyleSheet ( "background-color:rgb(170, 170, 170)" );  // green,
        label_az_home->setText ( "No Home" );
        //sprintf ( cmd,"P801=1" );//
        //SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
        flagtcs.Az_Homing=false;
        flagtcs.Az_Homed=false;
    }
    else if ( !QString::compare ( list[0], "1" ) )
 {
  label_az_homed->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,
  label_az_home->setText ( "Homed" );
        //sprintf ( cmd,"P801=1" );//
        //SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
        flagtcs.Az_Homing=false;
        flagtcs.Az_Homed=true;
 }
    else if ( !QString::compare ( list[0], "2" ) )
    {

        if(list[1].toInt()>=1 )
        {
            label_az_homed->setStyleSheet ( "background-color:rgb(255, 255, 0)" );  // yellow
            label_az_home->setText ( "Homing..."+ list[1]);
            az_hometime++;
        }
        else
        {
            label_az_homed->setStyleSheet ( "background-color:rgb(170, 170, 170)" );  // yellow
            label_az_home->setText ( "No Home");
        }
        if(az_hometime>300 )
        {
            label_az_homed->setStyleSheet ( "background-color:rgb(255, 0, 0)" );  // red
            label_az_home->setText ( "Time too long");
            flagtcs.Az_Homing=false;
            flagtcs.Az_Homed=false;
            OcsCom.flag_HomeErrorAz=true;
        }
    }
    else if ( !QString::compare ( list[0], "3" ) )
    {
        label_az_homed->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,
        label_az_home->setText ( "Start__"+ list[0]+"__"+list[1] );

    }
    else
    {
        label_az_homed->setStyleSheet ( "background-color:rgb(255, 0, 0)" );  // red
        label_az_home->setText ( "Error __"+ list[0]+"__"+ list[1] );
        //flagtcs.Az_Homing=false;
        flagtcs.Az_Homed=false;
        OcsCom.flag_HomeErrorAz=true;
    }

*/
    if ( !QString::compare ( list[0], "0" ) )
    {
        if(s_Az.Velocity > 0.01)
        {
            label_az_homed->setStyleSheet ( "background-color:rgb(170, 170, 170)" );
            label_az_home->setText ( "Homing" );
            flagtcs.Az_Homing=true;
            flagtcs.Az_Homed=false;

        }
        else
        {
            label_az_homed->setStyleSheet ( "background-color:rgb(255, 0, 0)" );
            label_az_home->setText ( "No Home" );
            flagtcs.Az_Homing=false;
            flagtcs.Az_Homed=false;
        }
    }
    if ( !QString::compare ( list[0], "1" ) )
    {
        label_az_homed->setStyleSheet ( "background-color:rgb(0, 170, 0)" );
        label_az_home->setText ( "Homed" );
        flagtcs.Az_Homing=false;
        flagtcs.Az_Homed=true;
    }
}

void DirectAzalt::CheckAltHoming()
{

    QString str;
    char  cmd[255];
    sprintf ( cmd,"M1745" );
    QStringList  list;
    int n=1;
    GetStatusFromUMAC ( cmd,n, &list );
    /*
    if ( !QString::compare ( list[0], "0" ) )
    {
        label_alt_homed->setStyleSheet ( "background-color:rgb(170, 170, 170)" );  // green,
        label_alt_home->setText ( "No Home" );
        flagtcs.Alt_Homing=false;
        flagtcs.Alt_Homed=false;
    }
    else if ( !QString::compare ( list[0], "1" ) )
 {
        label_alt_homed->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,
  label_alt_home->setText ( "Homed" );
        //sprintf ( cmd,"P807=1" );//
        //SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
        flagtcs.Alt_Homing=false;
        flagtcs.Alt_Homed=true;
 }
    else if ( !QString::compare ( list[0], "2" ) )
    {

        if(list[1].toInt()>=1)
        {
            label_alt_homed->setStyleSheet ( "background-color:rgb(255, 255, 0)" );  // yellow
            label_alt_home->setText ( "Homing..."+ list[1] );
            alt_hometime++;
        }
        else
        {
            label_alt_homed->setStyleSheet ( "background-color:rgb(170, 170, 170)" );  // yellow
            label_alt_home->setText ( "No Home");
        }
        if(alt_hometime>300 )
        {
            label_alt_homed->setStyleSheet ( "background-color:rgb(255, 0, 0)" );  // red
            label_alt_home->setText ( "Time too long");
           //else label_alt_home->setText ( "Loop opened");
            flagtcs.Alt_Homing=false;
            flagtcs.Alt_Homed=false;
            OcsCom.flag_HomeErrorAlt=true;
        }
    }
    else if ( !QString::compare ( list[0], "3" ) )
    {
        label_alt_homed->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,
        label_alt_home->setText ( "Start__"+ list[0]+"__"+list[1] );

    }
    else
    {
        label_alt_homed->setStyleSheet ( "background-color:rgb(255, 0, 0)" );  // red
        label_alt_home->setText ( "Error __"+ list[0]+"__"+list[1]);
        //flagtcs.Alt_Homing=false;
        flagtcs.Alt_Homed=false;
        OcsCom.flag_HomeErrorAlt=true;
    }
*/

    if ( !QString::compare ( list[0], "0" ) )
    {
        if(s_Alt.Velocity > 0.01)
        {
            label_alt_homed->setStyleSheet ( "background-color:rgb(170, 170, 170)" );
            label_alt_home->setText ( "Homing" );
            flagtcs.Alt_Homing=true;
            flagtcs.Alt_Homed=false;
        }
        else
        {
            label_alt_homed->setStyleSheet ( "background-color:rgb(255, 0, 0)" );
            label_alt_home->setText ( "No Home" );
            flagtcs.Alt_Homing=false;
            flagtcs.Alt_Homed=false;
        }

    }
    if ( !QString::compare ( list[0], "1" ) )
    {
        label_alt_homed->setStyleSheet ( "background-color:rgb(0, 170, 0)" );
        label_alt_home->setText ( "Homed" );
        flagtcs.Alt_Homing=false;
        flagtcs.Alt_Homed=true;
    }
}
void DirectAzalt::on_pushButton_alt_dirP_pressed() //positive direct
{
    QString str;
    on_lineEdit_alt_vel_spec_returnPressed();
    //setPIDparam_Alt_going();
    SQUMAC::instance_p()->QcommCmdPMAC ( "#17j+",str );

}

void DirectAzalt::on_pushButton_alt_dirP_released()
{
    on_pushButton_alt_stop_clicked();

}
void DirectAzalt::on_pushButton_alt_dirM_pressed() //negative direct
{
    QString str;
    on_lineEdit_alt_vel_spec_returnPressed();
    //setPIDparam_Alt_going();
    SQUMAC::instance_p()->QcommCmdPMAC ( "#17j-",str );

}
void DirectAzalt::on_pushButton_alt_dirM_released()
{
    on_pushButton_alt_stop_clicked();

}


void DirectAzalt::setPIDparam_Alt_going()
{
    //Alt
    //	on_lineEdit_alt_vel_spec_returnPressed();
    QString str;
    double altVelocityCmd= lineEdit_alt_vel_spec->text().toFloat();
    if ( fabs ( altVelocityCmd ) >500 )
    {
        SQUMAC::instance_p()->QcommCmdPMAC ( AltPointParam,str );//only ALT motor,speed: I222=50=750"/s
    }
    else
    {
        SQUMAC::instance_p()->QcommCmdPMAC ( AltTrackParam,str );//only ALT motor,speed: I222=50=750"/s
    }
}



void  DirectAzalt::setValidator()
{
    lineEdit_az_vel_spec->setValidator ( new MyDoubleValidator ( az_vel_spec_BOTTOM,az_vel_spec_TOP,az_vel_spec_decimals,this ) );
    lineEdit_alt_vel_spec->setValidator ( new MyDoubleValidator ( alt_vel_spec_BOTTOM,alt_vel_spec_TOP,alt_vel_spec_decimals,this ) );
    lineEdit_az_targetD->setValidator ( new MyDoubleValidator ( az_targetD_BOTTOM,az_targetD_TOP,az_targetD_decimals,this ) );
    lineEdit_az_targetM->setValidator ( new QIntValidator ( az_targetM_BOTTOM,az_targetM_TOP,this ) );
    lineEdit_az_targetS->setValidator ( new MyDoubleValidator ( az_targetS_BOTTOM,az_targetS_TOP,az_targetS_decimals,this ) );
    lineEdit_alt_targetD->setValidator ( new MyDoubleValidator ( alt_targetD_BOTTOM,alt_targetD_TOP,alt_targetD_decimals,this ) );
    lineEdit_alt_targetM->setValidator ( new QIntValidator ( alt_targetM_BOTTOM,alt_targetM_TOP,this ) );
    lineEdit_alt_targetS->setValidator ( new MyDoubleValidator ( alt_targetS_BOTTOM,alt_targetS_TOP,alt_targetS_decimals,this ) );
}




//////////////////////////////////////////////////////////////
/*//input:
char cmd[];		//command that send out to UMAC
int n;     		//number of commands
double Seconds_old;	//last seconds,for save
int* Pointer;		//line of table
int PointerLast	        //last 	Pointer
double cts:			encoder ratio
int num:		//matrix Data rows

output:
double PosCmd:			command  position;//
double PosReal_Degree:		real position,degree
double FolErrUMAC:		following error  ,calculated by UMAC

double Velocity:
double Seconds:                 time for save date
double Seconds_old:                 time for save date
int* Pointer:   		pointer of table line
char *DataStore:             	Data for save
double (*p)[5]:                 Data[Q][5]


*/

//double PosReal_Degree;	//real position,degree

void DirectAzalt::GetDataFromUMAC ( char cmd[],int n,double cts,double* PosReal_Degree,double* FolErrUMAC,double* Velocity,double Seconds_old,double* Seconds ,int PointerLast,int* Pointer,double ( *p ) [6],QString DataTime[],int num )
{
    QString str;
    double PosCmd;		//command  position
    double PosReal;		//real position,seconds
    double FolErrPC;	//following error  ,calculated by PC

    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );

    QStringList  list = str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
    int it=0;
    for ( QStringList::iterator point = list.begin(); point != list.end();++it, ++point )
    {}
    if ( it<n )
    {
        //UMAC_ETHERNET=false;
        return;
    };

    PosCmd=list[0].toDouble() *cts/3072.0;			// command, float value

    PosReal= ( ( list[1].toDouble() +list[2].toDouble() ) /3072.0 ) *cts; // arcsec of real position
    *PosReal_Degree=PosReal/3600.0;// degree

    *FolErrUMAC=list[3].toDouble() *cts/3072.0;	// following error  ,calculated by UMAC

    FolErrPC=PosCmd-PosReal;			//following error ,calculated by PC,arcsec

    *Velocity=list[4].toDouble() *cts*1000*2/3072.0;

    QDateTime dt=QDateTime::currentDateTime();
    QTime time=dt.time();
    QString dtstr;
    dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );
    int msecond=time.msec();
    *Seconds=time.second() *1000+msecond;//ms

    if ( fabs ( *Seconds-Seconds_old ) >=90&&*Pointer<= ( num-1 ) ) //every 100ms
    {
        * ( * ( p+*Pointer ) +0 ) =PosCmd;	//Data[Pointer][0]
        * ( * ( p+*Pointer ) +1 ) =PosReal;	//Data[Pointer][1]
        * ( * ( p+*Pointer ) +2 ) =*FolErrUMAC;	//Data[Pointer][2]
        * ( * ( p+*Pointer ) +3 ) =FolErrPC;	//Data[Pointer][3]

        DataTime[*Pointer]=dtstr;
        *Pointer=PointerLast+1;
        //*Seconds_old=*Seconds;
    }
}

/*
output:Az.azSeconds,Global Variable
 Az.azPointer,Global Variable
input: Az.azPosReal_Degree ,Global Variable
 Az.azSeconds,Global Variable
 Az.azPointer,Global Variable
*/
void DirectAzalt::GetAzPosition()
{
    char  cmd[255];
    sprintf ( cmd,"m2161m2162m2164m2165m2174" );
    int n=5;
    double azFolErrUMAC=0;
    double azVelocity;
    int Az_d,Az_m;
    double Az_s;

    GetDataFromUMAC ( cmd,n,
                      CTS_az,
                      &s_Az.PosReal_Degree,
                      &azFolErrUMAC,
                      &azVelocity,
                      s_Az.Seconds,
                      &s_Az.Seconds,
                      s_Az.Pointer,
                      &s_Az.Pointer,
                      s_Az.Data,
                      s_Az.DataTime,
                      N );
    s_Az.Velocity=azVelocity;
    s_Az.error=azFolErrUMAC;
    //show data
    D2DMS ( s_Az.PosReal_Degree,&Az_d,&Az_m,&Az_s );

    lineEdit_az_posD->setText ( QString::number ( Az_d ) );
    lineEdit_az_posM->setText ( QString::number ( Az_m ) );
    lineEdit_az_posS->setText ( QString::number ( Az_s,'f',2 ) );
    lineEdit_az_poserr->setText ( QString::number ( azFolErrUMAC,'f',2 ) );
    lineEdit_az_vel->setText ( QString::number ( azVelocity,'f',2 ) );


    //           QString  Data;
    //           long offset=0;
    //           int data_length=4;
    //
    //         SQUMAC::instance_p()->QgetDataDPRAM( Data, offset, data_length);
    //         qDebug()<<Data;

}

/*
output:Alt.altSeconds,Global Variable
 Alt.altPointer,Global Variable
input: Alt.altPosReal_Degree ,Global Variable
 Alt.altSeconds,Global Variable
 Alt.altPointer,Global Variable
*/
void DirectAzalt::GetAltPosition()
{
    char  cmd[255];
    sprintf ( cmd,"m1761m1762m1764m1765m1774" );
    int n=5;//number of cmd,eg.m261m262m264m265m274
    double altFolErrUMAC=0;
    double altVelocity;
    int Alt_d,Alt_m;
    double Alt_s;

    GetDataFromUMAC ( cmd,n,
                      CTS_alt,
                      &s_Alt.PosReal_Degree,
                      &altFolErrUMAC,
                      &altVelocity,
                      s_Alt.Seconds,
                      &s_Alt.Seconds,
                      s_Alt.Pointer,
                      &s_Alt.Pointer,
                      s_Alt.Data,
                      s_Alt.DataTime,
                      N );
    s_Alt.Velocity=altVelocity;
    s_Alt.error=altFolErrUMAC;
    //show data
    D2DMS ( s_Alt.PosReal_Degree, &Alt_d, &Alt_m, &Alt_s );

    lineEdit_alt_posD->setText ( QString::number ( Alt_d ) );
    lineEdit_alt_posM->setText ( QString::number ( Alt_m ) );
    lineEdit_alt_posS->setText ( QString::number ( Alt_s,'f',2 ) );
    lineEdit_alt_poserr->setText ( QString::number ( altFolErrUMAC,'f',2 ) );
    lineEdit_alt_vel->setText ( QString::number ( altVelocity,'f',2 ) );

    //by zxj-----
    datacount++;
    qDebug() << QString::number(datacount) << " " << lineEdit_alt_vel->text() << " " << lineEdit_alt_poserr->text();

    dataFile = new QFile(mFilename);
    dataFile->open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append);
    QTextStream out(dataFile);
    out << QString::number(datacount) << " " << lineEdit_alt_vel->text() << " " << lineEdit_alt_poserr->text() << "\r\n";
    dataFile->flush();
    dataFile->close();
}

void  DirectAzalt::GetAzReadHead()
{
    QString str;
    char cmd[255];
    sprintf ( cmd,"M901M1001M1101M1201M1103" );
    QStringList  list;
    int n=5;
    GetStatusFromUMAC ( cmd,n, &list );

    int head1=list[0].toInt();
    int head2=list[1].toInt();
    int head3=list[2].toInt();
    int head4=list[3].toInt();
    //int head5=list[4].toInt();
    if(head1>=8388608)head1-=16777216;
    if(head2>=8388608)head2-=16777216;
    if(head3>=8388608)head3-=16777216;
    if(head4>=8388608)head4-=16777216;
    int minhead=head1;
    if(minhead>head2)minhead=head2;
    if(minhead>head3)minhead=head3;
    if(minhead>head4)minhead=head4;
    int maxhead=head1;
    if(maxhead<head2)maxhead=head2;
    if(maxhead<head3)maxhead=head3;
    if(maxhead<head4)maxhead=head4;

    str=QString("%1__%2__%3__%4__").arg(head1).arg(head2).arg(head3).arg(head4);
    str+=list[4];
    if(maxhead-minhead>16) str+="__bad";
    else str+="__ok";
    label_Az_ReadHead->setText ( str );
}

void  DirectAzalt::GetAltReadHead()
{
    QString str;
    char cmd[255];
    sprintf ( cmd,"M1301M1401M1501M1601M1403" );
    QStringList  list;
    int n=5;
    GetStatusFromUMAC ( cmd,n, &list );
    int head1=list[0].toInt();
    int head2=list[1].toInt();
    int head3=list[2].toInt();
    int head4=list[3].toInt();
    if(head1>=8388608)head1-=16777216;
    if(head2>=8388608)head2-=16777216;
    if(head3>=8388608)head3-=16777216;
    if(head4>=8388608)head4-=16777216;
    int minhead=head1;
    if(minhead>head2)minhead=head2;
    if(minhead>head3)minhead=head3;
    if(minhead>head4)minhead=head4;
    int maxhead=head1;
    if(maxhead<head2)maxhead=head2;
    if(maxhead<head3)maxhead=head3;
    if(maxhead<head4)maxhead=head4;

    str=QString("%1__%2__%3__%4__").arg(head1).arg(head2).arg(head3).arg(head4);
    str+=list[4];
    if(maxhead-minhead>16) str+="__bad";
    else str+="__ok";
    label_Alt_ReadHead->setText ( str );
}

void DirectAzalt::GetMotorCurrent()
{
    QString str,str330,str331;
    char cmd[255];
    bool ok;
    int i331,i330;

    sprintf ( cmd,"ms1,i330" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str330 );
    str330.remove(0,7);
    i330=str330.toInt(&ok,16);
    if(i330>=8388608)i330-=16777216;
    str330.clear();
    str330=QString("%1__").arg(i330*92.2/32768,0,'f',3);

    sprintf ( cmd,"ms1,i331" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str331 );
    str331.remove(0,7);
    i331=str331.toInt(&ok,16);
    if(i331>=8388608)i331-=16777216;
    str331.clear();
    str331=QString("%1").arg(i331*92.2/32768,0,'f',3);
    label_az_I->setText ( str330+str331 );

    sprintf ( cmd,"ms4,i330" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str330 );
    str330.remove(0,7);
    i330=str330.toInt(&ok,16);
    if(i330>=8388608)i330-=16777216;
    str330.clear();
    str330=QString("%1__").arg(i330*37.9/32768,0,'f',3);

    sprintf ( cmd,"ms4,i331" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str331 );
    str331.remove(0,7);
    i331=str331.toInt(&ok,16);//list[0].toInt();
    if(i331>=8388608)i331-=16777216;
    str331.clear();
    str331=QString("%1").arg(i331*37.9/32768,0,'f',3);
    label_alt_I->setText ( str330+str331 );
}

void DirectAzalt::D2DMS ( double Degree,int* Deg,int* Min,double* Sec ) //change degree vlaue as degrees+minute+seconds
{

    *Deg=int ( Degree );
    *Min=int ( ( Degree-*Deg ) *60 );
    *Sec= ( ( Degree-*Deg ) *60-*Min ) *60;

    if ( *Deg!=0 )
        *Min=abs ( *Min );
    if ( *Deg||*Min )
        *Sec=fabs ( *Sec );
}


void DirectAzalt::on_pushButton_alt_brake_clicked()
{
    QString str;
    char cmd[255];
    if( flagtcs.Alt_Brake )
    {
        sprintf ( cmd,"#9j+" );
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    }
    if( flagtcs.Alt_Release )
    {
        sprintf ( cmd,"#9j-" );
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    }
}


void DirectAzalt::CheckAltBrake()
{
    QString str;
    char  cmd[255];
    sprintf ( cmd,"M1721M1722" );
    QStringList  list;
    int n=2;
    GetStatusFromUMAC ( cmd,n, &list );

    if ( !QString::compare ( list[0], "1" ) )
    {
        label_alt_braked->setStyleSheet ( "background-color:rgb(0, 170, 0)" );
        label_alt_brake->setText ( "Release" );
        pushButton_alt_brake->setText ( "Brake" );
        flagtcs.Alt_Brake = false;
        flagtcs.Alt_Release = true;
    }

    if ( !QString::compare ( list[1], "1" ) )
    {

        //label_alt_braked->setStyleSheet ( "background-color:rgb(255, 0, 0)" );
        label_alt_braked->setStyleSheet ( "background-color:rgb(0, 170, 0)" );
        label_alt_brake->setText ( "Brake" );
        pushButton_alt_brake->setText ( "Release" );
        flagtcs.Alt_Brake = true;
        flagtcs.Alt_Release = false;
    }

    if ( !QString::compare ( list[0], "0" ) && !QString::compare ( list[1], "0" ))
    {
        //label_alt_braked->setStyleSheet ( "background-color:rgb(255, 255, 0)" );
        label_alt_braked->setStyleSheet ( "background-color:rgb(0, 170, 0)" );
        label_alt_brake->setText ( "running..." );
        flagtcs.Alt_Brake = false;
        flagtcs.Alt_Release = false;
    }
}


