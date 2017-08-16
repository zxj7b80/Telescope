//
// C++ Interface: UMAC
//
// Description:

//
//

#ifndef UMAC_H
#define UMAC_H
#include <tr1/cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <QString>


typedef struct Int48
{
    int one;    //32-bit
    int two;  //32-bit
} INT48,*PINT48;

typedef struct tagEthernetINT48
{
    uint8_t   RequestType;
    uint8_t   Request;
    uint16_t  wValue;
    uint16_t  wIndex;
    uint16_t  wLength;
    INT48   bData[175];
    //char      bData[1492];  //change char
} ETHERNETINT48,*PETHERNETINT48;

typedef struct tagEthernetCmd
{
    uint8_t   RequestType;
    uint8_t   Request;
    uint16_t  wValue;
    uint16_t  wIndex;
    uint16_t  wLength;
    uint8_t   bData[2984];
    //char      bData[1492];  //change char
} ETHERNETCMD,*PETHERNETCMD;

typedef struct tagEthernetInt
{
    uint8_t   RequestType;
    uint8_t   Request;
    uint16_t  wValue;
    uint16_t  wIndex;
    uint16_t  wLength;
    int   bData[1492];
    //char      bData[1492];  //change char
} ETHERNETINT,*PETHERNETINT;


typedef struct tagEthernetFloat
{
    uint8_t   RequestType;
    uint8_t   Request;
    uint16_t  wValue;
    uint16_t  wIndex;
    uint16_t  wLength;
    float   bData[1492];
    //char      bData[1492];  //change char
} ETHERNETFLOAT,*PETHERNETFLOAT;

#define ETHERNETCMDSIZE 8
#define MAXDATA 1400 //one byte for '\0' sometimes.I have no ideas...

#define FOURMAXDATA  350
#define DOUBLEMAXDATA 175
#define RECVMAXDATA 192  // buffer overflew 

#define VR_UPLOAD              0xC0 //send something to the host.
#define VR_DOWNLOAD            0x40 //send something to the device

#define CTRL_A                 Ox01
#define CTRL_B                 Ox02
#define CTRL_C                 Ox03
#define CTRL_D                 Ox04
#define CTRL_E                 Ox05
#define CTRL_F                 Ox06
#define CTRL_G                 Ox07
#define CTRL_H                 Ox08
#define CTRL_I                 Ox09
#define CTRL_J                 Ox0A
#define CTRL_K                 Ox0B
#define CTRL_L                 Ox0C
#define CTRL_M                 Ox0D
#define CTRL_N                 Ox0E
#define CTRL_O                 Ox0F
#define CTRL_P                 Ox10
#define CTRL_Q                 Ox11
#define CTRL_R                 Ox12
#define CTRL_S                 Ox13
#define CTRL_T                 Ox14
#define CTRL_U                 Ox15
#define CTRL_V                 Ox16
#define CTRL_W                 Ox17
#define CTRL_X                 Ox18
#define CTRL_Y                 Ox19
#define CTRL_Z                 Ox1A

#define VR_PMAC_SENDLINE       0xB0 //send
#define VR_PMAC_GETLINE        0xB1 //obsolete.use GETBUFFER please.
#define VR_PMAC_FLUSH          0xB3 //NOT used now
#define VR_PMAC_GETMEM         0xB4 //get datas from DPRAM
#define VR_PMAC_SETMEM         0xB5 //set datas to DPRAM
#define VR_PMAC_SETBIT         0xBA //Not used now.
#define VR_PMAC_SETBITS        0xBB //Not used now.
#define VR_PMAC_PORT           0xBE //to insure the sent command was received.Not implement yet.
#define VR_PMAC_GETRESPONSE    0xBF //send common command to PMAC and get response,
#define VR_PMAC_READREADY      0xC2 //check if there is any data available.
#define VR_CTRL_RESPONSE       0xC4 //send control command to PMAC and get response.
#define VR_PMAC_GETBUFFER      0xC5 //get all data available inside the PMAC.
#define VR_PMAC_WRITEBUFFER    0xC6 //send muti-line characters to PMAC.
#define VR_PMAC_WRITEERROR     0xC7 //this one I do not know...
#define VR_FWDOWNLOAD          0xCB //NOT used now.
#define VR_IPADDRESS           0xE0 //to change or retrival IP address.Not implement yet.

#define CONNECT_TIME_OUT         5  //timeout for connect,10 seconds
#define SEND_TIME_OUTS           1   //timeout for send,1 sec
#define RECV_TIME_OUTS           1   //timeout for recv,1 sec
#define SEND_TIME_OUTU           0   //
#define RECV_TIME_OUTU           0

#define ERROR -1
#define CHECK_DATA              //this is very important

class UMAC
{
public:
    UMAC ( const std::string& ip_address,const unsigned int port=1025 );//
    virtual ~UMAC();

private:
    int sock;
    struct sockaddr_in client;
    int connectDevice();
    int setTimeoutForSendRecv();
    int isDataAvailable();
    int getDataAvailable();
    int cleanDataAvailable();
    INT48 changeType(const long long& sendData);
    long long changeType(const INT48& receiveData);

public:
    int commCmdPMAC ( const char cCommand[],QString& cResponse );
    int ctrlCmdPMAC ( const int cCommand,QString& cResponse );

    //int sendDataDPRAM ( const char cData[],uint16_t offset );
    int getDataDPRAM ( QString& data,long offset,int data_length );

    //The following if add &
    //  change
    /*float*/
    int sendDataDPRAM(const float fData [],uint16_t offset, int iDatalength);
    int getDataDPRAM (float * data, uint16_t offset,int length);
    /*int*/
    int sendDataDPRAM(const int iData[],uint16_t offset,int iDatalength);
    int getDataDPRAM (int* data, uint16_t offset,int length );
    /*char*/
    int sendDataDPRAM ( const char cData[],uint16_t offset );
    //int getDataDPRAM ( QString& data,uint16_t offset,int data_length );
    int getDataDPRAM ( QString& data,uint16_t offset,int data_length,int check = 0);  //    ok

    //int download_file ( char * file_path );

    void cleanclose();

    INT48 typeChange(const long long& sendData);
    long long typeChange(const INT48& receiveData);
    int getDataDPRAM(long long* data, uint16_t offset, int length);
    int getinformation(uint16_t offset,long long & followingError, int & servoCommand, int& servoStatus, int& generalStatus, long long& positionBias, int& filteredActuralVelocity, long long& masterPosition, long long& netActualPosition);
};
#endif
