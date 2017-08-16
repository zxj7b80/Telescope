
// C++ Implementation: tracking.CPP
//function: tracking star and calculate startable(simulation)

#include "main_window.h"
#include "tracking.h"
#include "signs.h"
//#include "track_thread.h"
#include "astro.h"
#include "circum.h"
#include "server.h"
#include <time.h>
#include <QDateTime>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include "sendthread.h"
#include <QSound>
#include<QtTest/QTest>
//#include "TimeAndCoordinate.h"
#include "ADefine.h"


#define		TRACKINGSTARTMODE_0	0
#define		TRACKINGSTARTMODE_1	1
#define		TRACKINGSTARTMODE_2	2
#define		TRACKINGSTARTMODE_4	4
#define		TRACKINGSTARTMODE_8	8
#define		TRACKINGSTARTMODE_9	9
#define		TRACKINGSTARTMODE_10	10
#define		TRACKINGSTARTMODE_12	12
#define		TRACKINGSTARTMODE_16	16
#define		TRACKINGSTARTMODE_17	17
#define		TRACKINGSTARTMODE_18	18
#define		TRACKINGSTARTMODE_20	20
#define		TRACKINGSTARTMODE_24	24
#define		TRACKINGSTARTMODE_25	25
#define		TRACKINGSTARTMODE_26	26
#define		TRACKINGSTARTMODE_28	28

#define		TRACKINGSTOPMODE_1	1
#define		TRACKINGSTOPMODE_2	2
#define		TRACKINGSTOPMODE_4	4
#define		TRACKINGSTOPMODE_8	8
#define		TRACKINGSTOPMODE_9	9
#define		TRACKINGSTOPMODE_10	10
#define		TRACKINGSTOPMODE_12	12
#define		TRACKINGSTOPMODE_16	16
#define		TRACKINGSTOPMODE_17	17
#define		TRACKINGSTOPMODE_18	18
#define		TRACKINGSTOPMODE_20	20
#define		TRACKINGSTOPMODE_24	24
#define		TRACKINGSTOPMODE_25	25
#define		TRACKINGSTOPMODE_26	26
#define		TRACKINGSTOPMODE_28	28
	
writelog *writeLog1;
extern int OcsPort;						//Port of OCS (TCP/IP)
extern QString OcsIP; 						//IP of OCS(TCP/IP)

int deltaLinesAz;
int deltaLinesAlt;
int deltaLinesDero;

int AzNumPerGroup=2;
int AzTrackPeriod=500;
int AltNumPerGroup=2;
int AltTrackPeriod=500;
int DeroNumPerGroup=2;
int DeroTrackPeriod=500;
bool AltChangePeriod=false;

double Az_dError[100],Alt_dError[100],Dero_dError[100];
int iErrorAz=0,iErrorAlt=0,iErrorDero=0;
int iErrorStartAz=0,iErrorStartAlt=0,iErrorStartDero=0;
bool flag_bRecordErrorAz=1,flag_bRecordErrorAlt=1,flag_bRecordErrorDero=1;
double AzTrackTarget[1000],AltTrackTarget[1000],DeroTrackTarget[1000];
double de1taErrAz,az_AvError_last;
double de1taErrAlt,alt_AvError_last;
double de1taErrDero,dero_AvError_last;

double az_AvError=0,alt_AvError=0,dero_AvError=0;

int time_AzpointEnd=0;
int time_AltpointEnd=0;
int time_DeropointEnd=0;

long No1=0;				//star table line1,base time
long NoX;                  		//stand for the time when look up table 
double Ms[10];
int iMs=0;
double MsLast=0;
struct timeval tpstart,tpend; 
float timeuse; 
//long AzLine=0;

double AzDataRMS_show[M][6];
double AltDataRMS_show[M][6];
double DeroDataRMS_show[M][6];

double  AzData_tracking[M][6];		//save data
QString AzDataTime_tracking[M];		//time of save data
double  AltData_tracking[M][6];		//save data
QString AltDataTime_tracking[M];	//time of save data
double  DeroData_tracking[M][6];	//save data
QString DeroDataTime_tracking[M];	//time of save data

//long long AzFolErrUMACTestBYTE;
long long TestBYTE[M];	 

CalculateThread* threadVector;					//calculate thread
double azPosReal_Degree;
double altPosReal_Degree;
double deroPosReal_Degree;
extern  double RA_h,RA_m,RA_s;					//RA of star
extern 	double DEC_d,DEC_m,DEC_s;				//Dec of star
extern  double Temperature,Pressure;			//mBa;
extern  int cal_flag;					//0:no calculate star,1:caculate start,2:calculate finished
extern  int TableNum;
double ra0,Dec0;

QTimer *timer1;							//show data
QTimer *timer1_correctError;
QTimer *timer1_saveTrackingData;
//bool flag_TrackstarAzAlt=false;					// flag of track az alt
QTimer *timer_TrackreplyOCS;					// Track replay OCS every 5 seconds



int SampleTime;
extern	DateTime DT[];						//save day and time structure
extern double AZ[],ALT[],FR[],HA[],DEC[];
///////////star////////////////
QString starTable[TableNumMAX];
//Dynamic1Dim  <QString> starTable;
//QVector  <QString> vec(1000000);

extern FlagTCS flagtcs;
int ALT_d=0,ALT_m=0;
double ALT_s=0;
//save
QString FileName_TrackData;

float secondsAz=0,secondsAz_old=0;				//save data per second
//int secondsAlt=0,secondsAlt_old=0;				//save alt data per second
//double AzData_tracking[M][5];					//save data
//int TrackSeconds=0,TrackSecondsOld=0;				//send data to UMAC per second
int AzTrackSeconds=0;						//send data to UMAC per second
int AltTrackSeconds=0;						//send data to UMAC per second
int DeroTrackSeconds=0;						//send data to UMAC per second
int ADC1TrackSeconds=0;
int ADC2TrackSeconds=0;
int TrackSeconds=0;
int AzTrackSeconds_last=0;						//send data to UMAC per second
int AzTrackSeconds_delta=10;						//send data to UMAC per second
int Az_DataTotal=9;
int StarAtNorth_Az=0;
bool StarAtNorth=false;
bool StarAtZenith=false;
bool DeroReverse=false;
double totalerror=0;
int OpticAxis_X;
int OpticAxis_Y;
double Pixel_Second;

int AzGroup=0;
int AzGroupLast=3;
int AltGroup=0;
int AltGroupLast=3;
int DeroGroup=0;
int DeroGroupLast=3;
int ADC1Group=0;
int ADC1GroupLast=3;
int ADC2Group=0;
int ADC2GroupLast=3;
int Alt2Adc[20][2];                    ///////////////////
int AltforAdc=0;

bool flag_TrackingSave=false;
int PtrAlt=0;							//save ALT file when PtrgAlt=M
int Ptr=0;							// save AZ file when Ptrg=M
// star guider
//extern double az_guidedata;
// extern double alt_guidedata;
// extern double dero_guidedata;
extern double azFolErr,altFolErr;
//tracking

double AZ_TrackTargetNew=0;					// tracking target(with offset)
double ALT_TrackTargetNew=0;					// tracking target(with offset)

extern int flag_line;
bool flag_firstPoint=true;
extern StarTarget target;
bool flag_bStartTrack=0;
long N_row_LAST=0;
long N_row=0;
//double alt_correct=0,az_correct=0,dero_correct=0 ;
double az_correct_last=0,alt_correct_last=0;
static int count = 0;
double azPosReal;
double  altPosReal;
int   tableEndTime=0;
// extern double az_guidedata;
// extern double alt_guidedata;
extern OcsCOM OcsCom;						//structure of comminication with OCS/TCS
//extern timer_t tt;
extern QString DataBackToOcs;
sAxis  AzTrack, AltTrack, DeroTrack,  ADC2Track, ADC1Track;
//sDero DeroTrack;
//sADC ADC2Track, ADC1Track;
extern sAxis  	s_Az,s_Alt,statusDero, statusM3,  statusM3Rot, statusADC1, statusADC2;
TrackStar 	trackstar;
FlagTrack 	flagtrack;
TrackStarToUmac  	aztrackstar, alttrackstar, derotrackstar, ADC1trackstar, ADC2trackstar;
MOTOR   Motor[9];
static bool 	save=false;

QReadWriteLock lock;

///////test time diffrence between pc and umac ////////////

double deltaUt=-0.45913;
int alt_p=0;
void GetUmacTime();
int trackdelay=3;
//////////////////////////////GetEndTime();


QString axis;
timer_t PosixT;
//void  handlerPosix ( int sig, siginfo_t * extra, void *cruft );
void  StartPosixTimer();
void  LookupTable_Posix();
void  GetAzAltPosition_Posix();
void  GetDeroPosition_Posix();
void  GetAzAltDeroPosition_Posix();
bool  AzToUMAC_Posix();
bool  AltToUMAC_Posix();
bool  DeroToUMAC_Posix();


void GetStartTime();
void GetEndTime();
//point
double az_model=0;
double alt_model=0;
double dero_model=0;
double c1_model=0;
double n2_model=0;
double n1_model=0;
double az_model0=0;
double alt_model0=0;
/*
//14/05/20
double AW0=9.12;
double AN0=3.46;
double ACEC0=4.41;
double CA0=45.29;
double CRX0=0;
double CRY0=0;
double IA0=-142.06;
double IE0=48.03;
double NPAE0=0;
double NRX0=82.47;
double NRY0=0;
double TX0=0;
*/
//14/07/11
double AW0=5.9741;
double AN0=4.3148;
double ACEC0=3.6945;
double CA0=80.8798;
double CRX0=0;
double CRY0=0;
double IA0=-97.3186;
double IE0=-35.5938;
double NPAE0=0;
double NRX0=0;
double NRY0=0;
double TF0=0;
//14/07/16
double AW=0;
double AN=0;
double ACEC=0;
double ACES=0;
double CA=0;
double CRX=0;
double CRY=0;
double ECEC=0;
double ECES=0;
double IA=0;
double IE=0;
double NPAE=0;
double NRX=0;
double NRY=0;
double TF=0;

void GetUmacTime()
{
    QString str;
    char  cmd[255];
    sprintf ( cmd,"m100" );

    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );

    QStringList  list = str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
    int it=0;
    for ( QStringList::iterator point = list.begin(); point != list.end();++it, ++point )
        {}
    //qDebug()<<it;
    //if ( it<3 ) return;

     //NowServoTimer= list[0].toDouble() ;
}

Tracking::Tracking ( MainWindow *parent )
{
	m_parent = parent;
	setupUi ( this );
    show_info();
	timer_save_track_data = new QTimer;
	connect ( timer_save_track_data,SIGNAL ( timeout() ),this,SLOT ( saveTrackData() ) );
    timer_save_track_data->start ( TrackSaveClock );// TrackSaveClock
    //start 1000,  it stop after tracking,
    timer_ShowInfo= new QTimer ( this );//for showing information such as local time and star time
    timer_ShowInfo->connect ( timer_ShowInfo, SIGNAL ( timeout() ), this, SLOT ( show_info() ) );
    timer_ShowInfo->start ( ShowInfoClock );
     //start 3000
    timer_CheckStatus=new QTimer ( this );
    timer_CheckStatus->connect ( timer_CheckStatus, SIGNAL ( timeout() ), this, SLOT ( CheckStatus() ) );
    timer_CheckStatus->start ( TrackCheckStatusClock );

    timer_CalAndShowRms = new QTimer;
	connect ( timer_CalAndShowRms,SIGNAL ( timeout() ),this,SLOT (CalAndShowRms() ) );
    //timer_CalAndShowRms->start (CalAndShowRmsClock );

    //start 500 at button track clicked, stop at button stop clicked
    timer_show_track_info = new QTimer;
	connect ( timer_show_track_info,SIGNAL ( timeout() ),this,SLOT ( showTrackInfo() ) );

    //start 5000 at  track star started by ocs, stop at star table end or program end
	timer_TrackreplyOCS=new QTimer ( this );
	timer_TrackreplyOCS->connect ( timer_TrackreplyOCS, SIGNAL ( timeout() ), this, SLOT ( TrackreplyOCS() ) );

    // next three timer start 100 at pointing, stop at tracking
    timer_CheckPointingORTrackingAz=new QTimer ( this );
    timer_CheckPointingORTrackingAz->connect ( timer_CheckPointingORTrackingAz, SIGNAL ( timeout() ), this, SLOT ( CheckPointingORTrackingAz() ) );

    timer_CheckPointingORTrackingAlt=new QTimer ( this );
	timer_CheckPointingORTrackingAlt->connect ( timer_CheckPointingORTrackingAlt, SIGNAL ( timeout() ), this, SLOT ( CheckPointingORTrackingAlt() ) );

    timer_CheckPointingORTrackingDero=new QTimer ( this );
	timer_CheckPointingORTrackingDero->connect ( timer_CheckPointingORTrackingDero, SIGNAL ( timeout() ), this, SLOT ( CheckPointingORTrackingDero() ) );

	flagtcs.flag_tracking=false;
	flagtcs.IsOpened_tracking=true;
	flagtcs.flag_AZIMUTH=true;
	flagtcs.flag_ALTITUDE=true;
    flagtcs.flag_DERO=false;
    flagtcs.flag_ADC1=false;
    flagtcs.flag_ADC2=false;
	trackstar.flagAzRange=true;
	trackstar.flagAltRange=true;
	trackstar.flagDeroRange=true;
    flagtcs.AxisEnabled=false;

	//ini star time
	GetStarTime_ini();
    SampleTime= SMySetting::instance_p()->value ( "misc/SampleTime" ).toInt();
    OpticAxis_X= SMySetting::instance_p()->value ( "misc/Optic_Axis_CCD_X" ).toInt();
    OpticAxis_Y= SMySetting::instance_p()->value ( "misc/Optic_Axis_CCD_Y" ).toInt();
    Pixel_Second=SMySetting::instance_p()->value ( "misc/CCD_Pixel_Second" ).toDouble();

	QString dtstr;
	dtstr= "00:00:00" ;
	label_EndTimer->setText ( dtstr );
    label_EndTimer->setStyleSheet ( "color:rgb(0, 200, 55)" );
	label_EndTimer->setFont ( QFont ( "Times", 14, QFont::Bold ) );

	setValidator();
    //IniDPRAM();
    readModel();
    //readADCtable();
    //on_checkBox_CorrectAuto_toggled(1);
    //on_checkBox_PointModel_toggled(1);
    flagtcs.AutoCorrect=true;
    flagtcs.PointModel=true;

/*
        double target=-359.123456;
	qDebug()<<target;
       
	int  i=target*1000000;
	qDebug()<<"i="<<i;


        qDebug()<<(QString::number(target, 'f', 6)); 
 	SendDouble2InttoDPRAM(M183_addr,target);*/
/*
 	 int iData[10];
// 	iData[0]=i;
 	int iDatalength=1;*/
//         SQUMAC::instance_p()->QsendDataDPRAM (iData,  M183_addr, iDatalength);
// 	qDebug()<<"iData write"<<iData[0];

// 	SQUMAC::instance_p()->QgetDataDPRAM (iData,  M183_addr, iDatalength);
// 	qDebug()<<"iData read"<<iData[0];

/*
	double tar=0;
        ReadDataIntFromDPRAM(M183_addr,&tar);
	qDebug()<<(QString::number(tar, 'f', 6)); ;*/

// 	QString fileName=SMySetting::instance_p()->value ( "misc/StarTable" ).toString();
// 	//QString fileName="/home/telescope/telescope/StarTable.txt";
// 	qDebug()<<fileName;

         

// for(long i=0;i<1000000;i++)
//     vec[i]="b";
// 
// qDebug()<<vec[999999]<<vec[0];

}

int DoubleToInt(double dValue)
{
if(dValue < 0.0)
return (int)(dValue-0.5);
else
return (int)( dValue+0.5);
return 0.0;
}


void  SendDouble2InttoDPRAM(uint16_t offset,double data)
{
        int iData[10];
	int iDatalength=1;
        qDebug()<<(QString::number(data, 'f', 6)); 
        iData[0]=DoubleToInt(data*1000000.0);

//	qDebug()<<"iData[0] sendout"<<iData[0];
    SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);
}


void ReadDataIntFromDPRAM(uint16_t offset,double *data)
{
        int iData[10];
	int iDatalength=1;
        SQUMAC::instance_p()->QgetDataDPRAM (iData,  offset, iDatalength);
	*data=double(iData[0])/1000000;

//	qDebug()<<"iData[0] read"<<iData[0];
}

Tracking::~Tracking()
{
	flagtcs.IsOpened_tracking=false;
    qDebug ( "Close tracking" );
//	timer_get_track_data->stop();
	timer_show_track_info->stop();
	timer_save_track_data->stop();

	timer_ShowInfo->stop();
	timer_TrackreplyOCS->stop();
	timer_CheckStatus->stop();
	timer_CalAndShowRms->stop();
	//timer_CheckPointingORTracking->stop();
//	timer_get_track_data->deleteLater();
	timer_show_track_info->deleteLater();
	timer_save_track_data->deleteLater();

	timer_ShowInfo->deleteLater();
	timer_TrackreplyOCS->deleteLater();
	timer_CheckStatus->deleteLater();
    timer_CalAndShowRms->deleteLater();
//timer_CheckPointingORTracking->deleteLater();
	if ( flagtcs.flag_tracking )
	{
// 		track_thread->exit();
// 		track_thread->wait();
// 		track_thread->deleteLater();
// 		timer_delete ( tt );
		timer_delete ( PosixT );
	}
 
	//QString str;
	
//   track_thread->exit();
// track_thread->wait();
//           //    sleep(4000);
//    track_thread->deleteLater();
}

void Tracking::closeEvent ( QCloseEvent *event )
{
	//parentWidget()->hide();
	//event->ignore();

//	if ( flag_TrackstarAzAlt )
//	{
    /*QMessageBox mb ( QMessageBox::Question,tr ( "Close and tracking will be stopped!" ),
	                 tr ( "Do you want to stop tracking?                 " ),
	                 QMessageBox::Yes |QMessageBox::No |QMessageBox::Cancel );
	if ( mb.exec() == QMessageBox::Yes )
	{
		//SMySetting::instance_p()->beginGroup ( "tracking_window" );
		//SMySetting::instance_p()->setValue ( "size",size() );
		//SMySetting::instance_p()->setValue ( "pos",pos() );
		//SMySetting::instance_p()->endGroup();
        on_pushButton_stop_clicked();
		qDebug() <<"stop tracking";
		return;
    }
    event->ignore();*/
//	}
}

void Tracking::show_info() 									//for showing information such as local time and star time
{
    //freshLocalTime();
	//get star time
	if ( !flagtcs.flag_tracking )
	{
                //CalLMST();
		GetStarTime();

		//qDebug()<<"satrtimer";
	}



    //GetAzPosition();
    //freshAzPosition(AzTrack.azPosReal_Degree,AzTrack.AzVelocity);
    //GetAltPosition();
    //freshAltPosition(AltTrack.altPosReal_Degree,AltTrack.AltVelocity);

    GetAzPosition_DPRAM();
    freshAzPosition(trackstar.AzPosRealForError,trackstar.AzVelocity);
    GetAltPosition_DPRAM();
    freshAltPosition(trackstar.AltPosRealForError,trackstar.AltVelocity);
    GetDeroPosition_DPRAM();
    freshDeroPosition(trackstar.DeroPosRealForError,trackstar.DeroVelocity);
    GetADC1Position_DPRAM();
    freshADC1Position(trackstar.ADC1PosRealForError,trackstar.ADC1Velocity);
    GetADC2Position_DPRAM();
    freshADC2Position(trackstar.ADC2PosRealForError,trackstar.ADC2Velocity);

    if ( cal_flag==2 ) 									//star table ready
    {
        LookupTargetNow();
        trackstar.AzTrackError= ( trackstar.AzTarget-trackstar.AzPosRealForError ) *3600;
        trackstar.AltTrackError= ( trackstar.AltTarget-trackstar.AltPosRealForError ) *3600;
        trackstar.DeroTrackError= ( trackstar.DeroTarget-trackstar.DeroPosRealForError ) *3600;
        freshTargetPosition();
        double A=trackstar.AzTarget*PI/180;
        double E=trackstar.AltTarget*PI/180;
        double azmodel0=AW*cos ( A ) *tan ( E ) +AN*sin ( A ) *tan ( E )-ACEC*cos(A)+IA
                +CA/cos ( E )-CRX*cos(A-E)/cos(E)+CRY*sin(A-E)/cos(E)
                +NPAE*tan(E) +NRX+NRY*tan ( E );
        double altmodel0= -AW*sin ( A ) +AN*cos ( A )-IE+TF*sin(PI/2-E)
                        +CRX*sin(A-E)+CRY*cos(A-E)
                        +NRX*sin( E )-NRY*cos ( E );
        char s[255];
        sprintf ( s,"%5.2f",azmodel0);
        label_Az_Guid->setText ( s );
        sprintf ( s,"%5.2f",altmodel0 );									//alt_correct,degree
        label_Alt_Guid->setText ( s );

    }

    //checkCommandFromOCS();
    //TargetBeyondRange();
    /*char s[255];
    sprintf ( s,"%5.2fs",trackstar.az_guidedata );
    label_Az_Guid->setText ( s );
    sprintf ( s,"%5.2fs",trackstar.alt_guidedata );									//alt_correct,degree
    label_Alt_Guid->setText ( s );
    sprintf ( s,"%5.2fs",trackstar.dero_guidedata );
    label_Dero_Guid->setText ( s );*/



}

void Tracking::freshLocalTime()
{
    /*QDateTime dt=QDateTime::currentDateTime();
	QTime time;
	time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "hh:mm:ss" );
	label_LocalTimer->setText ( dtstr );
	label_LocalTimer->setStyleSheet ( "color:rgb(255, 255, 255)" );
    label_LocalTimer->setFont ( QFont ( "Times", 14, QFont::Bold ) );*/
}

void  Tracking::freshTargetPosition()
{
    char s[255];
    int AZ_d=0,AZ_m=0, ALT_d=0,ALT_m=0,Dero_d=0,Dero_m=0;//,ADC_d=0,ADC_m=0;
    double AZ_s=0,ALT_s=0,Dero_s=0;//,ADC_s=0 ,tem=0;

    D2DMS (trackstar.AzTarget ,&AZ_d,&AZ_m,&AZ_s );
    sprintf ( s,"%d: %d: %5.2f",AZ_d,AZ_m,AZ_s );
    label_Az_Target->setText ( s );

    D2DMS (trackstar.AltTarget,&ALT_d,&ALT_m,&ALT_s );
    sprintf ( s,"%d: %d: %5.2f",ALT_d,ALT_m,ALT_s );
    label_Alt_Target->setText ( s );

    D2DMS ( trackstar.DeroTarget,&Dero_d,&Dero_m,&Dero_s );
    sprintf ( s,"%d: %d: %5.2f",Dero_d,Dero_m,Dero_s );
    label_Dero_Target->setText ( s );

    sprintf ( s, "%6.2f",trackstar.AzTrackError );
    //sprintf ( s, "%9.2fs",trackstar.AzTrackError );
    label_Az_RmsError->setText ( s );
    sprintf ( s, "%6.2f",trackstar.AltTrackError );
    //sprintf ( s, "%6.2f",trackstar.AltFolErrUMAC,trackstar.AltTrackError );
    label_Alt_RmsError->setText ( s );
    sprintf ( s, "%6.2f",trackstar.DeroTrackError );
    label_Dero_RmsError->setText ( s );
}

void Tracking::showTrackInfo()
{
    if ( flagtcs.flag_tracking )
        GetStarTime();

    //freshLocalTime();
	QHash<QString,QString> info_show;
	char s[255];
    int AZ_d=0,AZ_m=0, ALT_d=0,ALT_m=0,Dero_d=0,Dero_m=0,ADC_d=0,ADC_m=0;
    double AZ_s=0,ALT_s=0,Dero_s=0,ADC_s=0,tem=0;
    sprintf ( s,"%ld__%4.1f",aztrackstar.N_row, aztrackstar.deltaTime );
	label_track_line->setText ( s );								//the number of tracking star table
//	label_track_line->setStyleSheet ( "color:rgb(255, 255, 255)" );
    /////////////////////show tracking left time/////////////////////////////
    QDateTime dateTimenow;
    dateTimenow = QDateTime::currentDateTime();
    QTime timenow;
    timenow=dateTimenow.time();
    double utc_now=( timenow.hour() ) *3600+timenow.minute() *60.+timenow.second();
    int leftTime= tableEndTime-utc_now;
    //int leftTime= utc_now;
    if(leftTime<0)leftTime+=24*3600;
    int left_H= leftTime/3600;
    int left_m= (leftTime-left_H*3600)/60;
    int left_s= leftTime-left_H*3600-left_m*60;
    QString lts;
    lts=QString("%1 h %2 m %3 s").arg(left_H).arg(left_m).arg(left_s);
    label_EndTimer->setText ( lts );
    if(leftTime<15)label_EndTimer->setStyleSheet ( "color:rgb(200, 55, 0)" );
	/////////////////////show Az target/////////////////////////////
//	lock.lockForRead();
    tem=trackstar.AzTarget;
    D2DMS (tem ,&AZ_d,&AZ_m,&AZ_s );
//	lock.unlock();
    sprintf ( s,"%d: %d: %5.2f",AZ_d,AZ_m,AZ_s );
    label_Az_Target->setText ( s );

	D2DMS ( trackstar.AzPosRealForError,&AZ_d,&AZ_m,&AZ_s );
    sprintf ( s,"%d: %d: %5.2f",AZ_d,AZ_m,AZ_s );
    label_Az_Pos->setText ( s );

    sprintf ( s,"%6.3f__%6.3f",trackstar.AzVelocity,trackstar.AZ_TrackTargetSpeed );
    label_Az_Velocity->setText ( s );
    ///////////////////////////show ALT target///////////////////////////////////////////
    tem= trackstar.AltTarget;
    D2DMS (tem,&ALT_d,&ALT_m,&ALT_s );
    sprintf ( s,"%d: %d: %5.2f",ALT_d,ALT_m,ALT_s );
    label_Alt_Target->setText ( s );

	D2DMS ( trackstar.AltPosRealForError,&ALT_d,&ALT_m,&ALT_s );
    sprintf ( s,"%d: %d: %5.2f",ALT_d,ALT_m,ALT_s );
    label_Alt_Pos->setText ( s );

    sprintf ( s,"%6.3f__%6.3f",trackstar.AltVelocity,trackstar.ALT_TrackTargetSpeed );
    label_Alt_Velocity->setText ( s );
    ///////////////////////////show DERO Data///////////////////////////////////////////
    tem= trackstar.DeroTarget;
    D2DMS ( tem,&Dero_d,&Dero_m,&Dero_s );
    sprintf ( s,"%d: %d: %5.2f",Dero_d,Dero_m,Dero_s );
    label_Dero_Target->setText ( s );


	D2DMS ( trackstar.DeroPosRealForError,&Dero_d,&Dero_m,&Dero_s );
    sprintf ( s,"%d: %d: %5.2f",Dero_d,Dero_m,Dero_s );
    label_Dero_Pos->setText ( s );

    sprintf ( s,"%5.1f",trackstar.DeroVelocity );
    label_Dero_Velocity->setText ( s );
    ///////////////////////////show ADC1 Data///////////////////////////////////////////
    D2DMS ( trackstar.ADC2Target,&ADC_d,&ADC_m,&ADC_s );
    sprintf ( s,"%d: %d: %5.2f",ADC_d,ADC_m,ADC_s );
    label_ADC1_Target->setText ( s );
    label_ADC2_Target->setText ( s );
    D2DMS ( trackstar.ADC1PosRealForError,&ADC_d,&ADC_m,&ADC_s );
    sprintf ( s,"%d: %d: %5.2f",ADC_d,ADC_m,ADC_s );
    label_ADC1_Pos->setText ( s );

    sprintf ( s,"%5.1f",trackstar.ADC1Velocity );
    label_ADC1_Velocity->setText ( s );
    ///////////////////////////show ADC2 Data///////////////////////////////////////////
    //D2DMS ( trackstar.ADC1Target,&ADC_d,&ADC_m,&ADC_s );
    //sprintf ( s,"%d: %d: %5.2f",ADC_d,ADC_m,ADC_s );
    //label_ADC2_Target->setText ( s );

    D2DMS ( trackstar.ADC2PosRealForError,&ADC_d,&ADC_m,&ADC_s );
    sprintf ( s,"%d: %d: %5.2f",ADC_d,ADC_m,ADC_s );
    label_ADC2_Pos->setText ( s );

    sprintf ( s,"%5.1f",trackstar.ADC2Velocity );
    label_ADC2_Velocity->setText ( s );
    /////////////////show offset////////////////////////////////////////////////////////////
    sprintf ( s,"%5.2f",trackstar.az_correct_all );
    label_Az_Offset->setText ( s );
    sprintf ( s,"%5.2f",trackstar.alt_correct_all );
    label_Alt_Offset->setText ( s );
    sprintf ( s,"%5.2f",trackstar.dero_correct_all );
    label_Dero_Offset->setText ( s );
    sprintf ( s,"%5.2f",trackstar.ADC1_correct_all );
    label_ADC1_Offset->setText ( s );
    sprintf ( s,"%5.2f",trackstar.ADC2_correct_all );
    label_ADC2_Offset->setText ( s );
    /////////////////show guiding data/////////////////////////////////////////////////////
    /*sprintf ( s,"%5.1f",DeltaTime_Pc );
    label_Az_Guid->setText ( s );
    sprintf ( s,"%5.1f",DeltaTime_Umac );
    label_Alt_Guid->setText ( s );*/
    //sprintf ( s,"%5.1f",trackstar.az_guidedata );
    sprintf ( s,"%5.1f",az_model);//__%5.1f,az_model0);
    label_Az_Guid->setText ( s );
    //sprintf ( s,"%5.1f",trackstar.alt_guidedata );
    sprintf ( s,"%5.1f",alt_model);//,alt_model0);
    label_Alt_Guid->setText ( s );
    /*sprintf ( s,"%5.1f",trackstar.dero_guidedata );
    label_Dero_Guid->setText ( s );*/
    ///////////////////////////show tracking error///////////////////////////////////////////
    sprintf ( s, "%6.2f",trackstar.AzFolErrUMAC);
    label_Az_Error->setText ( s );
    sprintf ( s, "%6.2f",trackstar.AzTrackError );
    label_Az_RmsError->setText(s);

    sprintf ( s, "%6.2f",trackstar.AltFolErrUMAC);
    label_Alt_Error->setText ( s );
    sprintf ( s, "%6.2f",trackstar.AltTrackError );
    label_Alt_RmsError->setText(s);

    sprintf ( s, "%6.2f",trackstar.DeroFolErrUMAC );
    label_Dero_Error->setText ( s );
    sprintf ( s, "%6.2f",trackstar.DeroTrackError );
    label_Dero_RmsError->setText(s);

    sprintf ( s, "%6.1f",trackstar.ADC1FolErrUMAC);
    label_ADC1_Error->setText ( s );
    sprintf ( s, "%6.1f", (trackstar.ADC1PosRealForError-trackstar.ADC2PosRealForError)*3600);
    label_ADC1_RmsError->setText ( s );
    sprintf ( s, "%6.1f",trackstar.ADC2FolErrUMAC );
    label_ADC2_Error->setText ( s );

    sprintf ( s,"%5.2f",totalerror );
    label_Total_RmsError->setText(s);

    /*    P_alt=lineEdit_t_offset->text().toDouble();							//  "
	I_alt=lineEdit_Pressure->text().toDouble();
    D_alt=lineEdit_Temperature->text().toDouble();*/

	//Deroator
 

//	FreshDeroTrackingData ( trackstar.DeroTarget,trackstar.DeroTargetOffset,trackstar.DeroPosRealForError,trackstar.DeroTrackError,info_show );
    //FreshDeroTrackingData ( trackstar.DeroTarget,trackstar.DeroTargetOffset,trackstar.DeroPosRealForError,trackstar.DeroFolErrUMAC,info_show );
//	lock.lockForRead();
    //freshAzPosition(trackstar.AzPosRealForError,trackstar.AzVelocity);
    //freshAltPosition(trackstar.AltPosRealForError,trackstar.AltVelocity);
    //freshDeroPosition(trackstar.DeroPosRealForError, trackstar.DeroVelocity );
//	lock.unlock();	
	  			 
    //CheckTarget();
    //TargetBeyondRange();
    //checkCommandFromOCS();
    QString str;
    if ( flagtrack.StarTableEnd && flagtcs.flag_tracking)
	{
		writeLog->writeerror("Star table end","	Star table end");
        str="Star table end!";
		on_pushButton_stop_clicked();
        //flagtrack.StarTableEnd=false;
        /*QMessageBox msgBox ( QMessageBox::NoIcon,"Warning!","Lookup table end!" );

		msgBox.setText ( "Lookup table end!" );
		msgBox.setStyleSheet ( "QPushButton { background-color: rgb(255, 85, 0); color: rgb(85, 255, 0); }" );
        msgBox.exec();*/
		
	}
    if(trackstar.flagAzTracking && (trackstar.AzPosRealForError<-40 || trackstar.AzPosRealForError>400))
    {
        on_pushButton_stop_clicked();
        str="Az axis  beyonds  limit!" ;
    }
    if(trackstar.flagAltTracking && trackstar.AltPosRealForError<5 )
    {
        on_pushButton_stop_clicked();
        str="Alt axis beyonds  limit!" ;
    }
    if(trackstar.flagAzPointing) str+="Az is pointing...";
        //{label_Status->setText ( "Az pointing..." );  /* writeLog->writeregular("Az pointing","	Az is pointing");*/}
    else if(trackstar.flagAzTracking) str="Az is tracking...";
        //{label_AzStatus->setText ( "Az tracking..." );/* writeLog->writeregular("Az tracking"," Az is tracking");*/}
// 
    if(trackstar.flagAltPointing) str+="Alt is pointing...";
        //{label_AltStatus->setText ( "Alt pointing..." );/*writeLog->writeregular("Alt tracking","	Alt is pointing");*/}
    else if(trackstar.flagAltTracking) str+="Alt is tracking...";
        //{label_AltStatus->setText ( "Alt tracking..." ); /*writeLog->writeregular("Alt tracking"," Alt is tracking");*/}
// 
    if(trackstar.flagDeroPointing) str+="Dero is pointing...";
        //{label_DeroStatus->setText ( "Dero pointing..." );/*writeLog->writeregular("Dero pointing"," Dero is pointing");*/}
    else if(trackstar.flagDeroTracking) str+="Dero is tracking...";
        //{label_DeroStatus->setText ( "Dero tracking..." );/*writeLog->writeregular("Dero tracking"," Dero is tracking");*/}
    if(trackstar.flagADC1Pointing) str+="ADC1 is pointing...";
        //{label_DeroStatus->setText ( "Dero pointing..." );/*writeLog->writeregular("Dero pointing"," Dero is pointing");*/}
    else if(trackstar.flagADC1Tracking) str+="ADC1 is tracking...";
    if(trackstar.flagADC2Pointing) str+="ADC2 is pointing...";
        //{label_DeroStatus->setText ( "Dero pointing..." );/*writeLog->writeregular("Dero pointing"," Dero is pointing");*/}
    else if(trackstar.flagADC2Tracking) str+="ADC2 is tracking...";
    label_Status->setText ( str );
}


 


void Tracking::FreshDeroTrackingData ( double TrackTarget,double PosReal_Degree,double TrackError,QHash<QString,QString> info_show )
{
	//QHash<QString,QString> info_show;
	int DERO_d=0,DERO_m=0;
	double DERO_s=0;
	char s[255];
    D2DMS ( TrackTarget,&DERO_d,&DERO_m,&DERO_s );							//position in startable
    sprintf ( s,"%d° %d′ %5.2f″",DERO_d,DERO_m,DERO_s );
    label_Dero_Target->setText ( s );

	D2DMS ( PosReal_Degree,&DERO_d,&DERO_m,&DERO_s );
    sprintf ( s,"%d° %d′ %5.2f″",DERO_d,DERO_m,DERO_s );
    label_Dero_Pos->setText ( s );

    sprintf ( s, "%9.2f",TrackError );
	//sprintf ( s, "%9.2f",trackstar.DeroTrackError);								// track error
    label_Dero_Error->setText ( s );
//	trackstar.dero_offset=lineEdit_dero_offset->text().toDouble();					//dero offset
	m_parent->updateInterface ( info_show );
}

/*
void Tracking::AzAlt_enable()
{
	QString str;
	char cmd[255];
	sprintf ( cmd,CmdAzAltEnable );  								// ENABLE AZ  ALT
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}

void Tracking::AzAlt_disable()
{
	QString str;
	char cmd[255];
	sprintf ( cmd,CmdAzAltDisable);  								// DISABLE AZ ALT
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );

}*/


void Tracking::on_pushButton_stop_clicked()
{
    //QApplication::beep();
    //QSound::play("/home/rcz/Program/telescope/sound/base.mp3");

    //if(QSound::isAvailable ()) qDebug("ok there sound");
    //else  qDebug("sorry no sound");

	QString str;
    //SQUMAC::instance_p()->QcommCmdPMAC ( "DISABLE PLC2",str );
	timer_CalAndShowRms->stop ();
	if ( flagtcs.flag_AZIMUTH )
	{
		Az_stopTrack();
		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopAzMotion1,str );				//stop UMAC motion1
        SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopAz,str );
        //SQUMAC::instance_p()->QcommCmdPMAC ( CmdKillAzMotion1,str );					//stop go #1k
    }
	if ( flagtcs.flag_ALTITUDE )
	{
		Alt_stopTrack();
		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopAltMotion2,str );				//stop UMAC motion2
        SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopAlt,str );
        //SQUMAC::instance_p()->QcommCmdPMAC ( CmdKillAltMotion2,str );				//stop UMAC motion2
    }
    if ( flagtcs.flag_DERO )
	{
        Dero_stopTrack();
        SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopDeroMotion3,str );				//stop UMAC motion3
        SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopDero,str );
        //SQUMAC::instance_p()->QcommCmdPMAC ( CmdKillDeroMotion3,str );				//stop UMAC motion3  C1_1
    }

    if ( flagtcs.flag_ADC2 ||  flagtcs.flag_ADC1 )
    {
        ADC_stopTrack();
        SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopADC2,str );					//stop UMAC motion6 #6k#7k
    }
	if ( flagtcs.flag_tracking )
	{
	 	timer_show_track_info->stop();
		timer_ShowInfo->start ( ShowInfoClock );
        //timer_CheckPointingORTrackingAlt->stop();
        //timer_CheckPointingORTrackingAz->stop();
        //timer_CheckPointingORTrackingDero->stop();
// 		track_thread->exit();
// 		track_thread->wait();
// 		track_thread->deleteLater();
//		timer_delete ( tt );
//		flag_TrackstarAzAlt=false;
		timer_delete ( PosixT );

		flagtcs.flag_tracking=false;
		pushButton_track->setText ( "Track" );
		pushButton_track->setEnabled ( true );
	}

    trackstar.flagAzTracking=false;
    trackstar.flagAzPointing=false;
    trackstar.flagAltTracking=false;
    trackstar.flagAltPointing=false;
    trackstar.flagDeroTracking=false;
    trackstar.flagDeroPointing=false;
    trackstar.flagADC1Tracking=false;
    trackstar.flagADC1Pointing=false;
    trackstar.flagADC2Tracking=false;
    trackstar.flagADC2Pointing=false;
    str="stop!";
    label_Status->setText ( str );
}



void Tracking::IniTrack()
{
	QString str;
	SQUMAC::instance_p()->QcommCmdPMAC ( CmdClearFlagBit,str );				//Clear flag bit
	N_row_LAST=0;
	N_row=0;
    AltGroup=0;
    AzGroup=0;
	trackstar.flagAzRange =true;
	trackstar.flagAltRange=true;
 	trackstar.flagDeroRange=true;
	flagtrack.StarTableEnd=false;
	trackstar.flagAltCorrectPoint=true;
	trackstar.flagAzCorrectPoint=true;
	trackstar.flagDeroCorrectPoint=true;

    trackstar.flagAzTracking=false;
    trackstar.flagAzPointing=false;
    trackstar.flagAltTracking=false;
    trackstar.flagAltPointing=false;
    trackstar.flagDeroTracking=false;
    trackstar.flagDeroPointing=false;
    trackstar.flagADC1Tracking=false;
    trackstar.flagADC1Pointing=false;
    trackstar.flagADC2Tracking=false;
    trackstar.flagADC2Pointing=false;

    AzTrackSeconds=0;
	AltTrackSeconds=0;
	DeroTrackSeconds=0;
    ADC1TrackSeconds=0;
    ADC2TrackSeconds=0;
    TrackSeconds=0;
    AzTrackSeconds_last=0;
    AzTrackSeconds_delta=10;

	time_AzpointEnd=0;
	time_AltpointEnd=0;
	time_DeropointEnd=0;


    for(int z=0;z<10;z++)
    {
        aztrackstar.Target_Group1[z]=180*3600/CTS;
        aztrackstar.Target_Group2[z]=180*3600/CTS;
        alttrackstar.Target_Group1[z]=80*3600/CTS;
        alttrackstar.Target_Group2[z]=80*3600/CTS;
        derotrackstar.Target_Group1[z]=0;
        derotrackstar.Target_Group2[z]=0;
        aztrackstar.Vel_Group1[z]=0;
        aztrackstar.Vel_Group2[z]=0;
        alttrackstar.Vel_Group1[z]=0;
        alttrackstar.Vel_Group2[z]=0;
        derotrackstar.Vel_Group1[z]=0;
        derotrackstar.Vel_Group2[z]=0;
        trackstar.AzFolErrUMACTest[z]=0;
        trackstar.AltFolErrUMACTest[z]=0;
    }

	trackstar.az_offset=0;
	trackstar.az_offset_last=0;
    trackstar.az_offset_all=0;
    trackstar.az_guidedata=0;
	trackstar.az_guidedata_last=0;
    trackstar.az_guidedata_all=0;
    trackstar.az_correct=0;
    trackstar.az_correct_last=0;
    trackstar.az_correct_all=0;    
    trackstar.az_DataCount=0;

    trackstar.alt_offset=0;
    trackstar.alt_offset_last=0;
    trackstar.alt_offset_all=0;
    trackstar.alt_guidedata=0;
    trackstar.alt_guidedata_last=0;
    trackstar.alt_guidedata_all=0;
    trackstar.alt_correct=0;
    trackstar.alt_correct_last=0;
    trackstar.alt_correct_all=0;
    trackstar.alt_DataCount=0;


    trackstar.dero_offset=0;
    trackstar.dero_offset_last=0;
    trackstar.dero_offset_all=0;
    //trackstar.dero_guidedata=0;
    //trackstar.dero_guidedata_last=0;
    //trackstar.dero_guidedata_all=0;
    trackstar.dero_correct=0;
    trackstar.dero_correct_last=0;
    trackstar.dero_correct_all=0;

    trackstar.ADC1_offset=0;
    trackstar.ADC1_offset_last=0;
    trackstar.ADC1_offset_all=0;
    //trackstar.ADC1_guidedata=0;
    //trackstar.ADC1_guidedata_last=0;
    //trackstar.ADC1_guidedata_all=0;
    trackstar.ADC1_correct=0;
    trackstar.ADC1_correct_last=0;
    trackstar.ADC1_correct_all=0;

    trackstar.ADC2_offset=0;
    trackstar.ADC2_offset_last=0;
    trackstar.ADC2_offset_all=0;
    //trackstar.ADC2_guidedata=0;
    //trackstar.ADC2_guidedata_last=0;
    //trackstar.ADC2_guidedata_all=0;
    trackstar.ADC2_correct=0;
    trackstar.ADC2_correct_last=0;
    trackstar.ADC2_correct_all=0;

//	AzLine=0;
    derotrackstar.deltaTime=0;
    alttrackstar.deltaTime=0;
    aztrackstar.deltaTime=0;
    ADC1trackstar.deltaTime=0;
    ADC2trackstar.deltaTime=0;
    derotrackstar.flag_BeginTrack=false;
    alttrackstar.flag_BeginTrack=false;
    aztrackstar.flag_BeginTrack=false;
    ADC1trackstar.flag_BeginTrack=false;
    ADC2trackstar.flag_BeginTrack=false;
    //trackstar.AzCorrectLines=0;
    //trackstar.AltCorrectLines=0;
    //trackstar.DeroCorrectLines=0;

	iErrorStartAz=0;
	iErrorStartAlt=0;
	iErrorStartDero=0;

	flag_bRecordErrorAz=1;
	flag_bRecordErrorAlt=1;
	flag_bRecordErrorDero=1;
	iErrorAz=0;
	iErrorAlt=0;
	iErrorDero=0;

    az_AvError=0;
	alt_AvError=0;
	dero_AvError=0;
	az_AvError_last=az_AvError;
	alt_AvError_last=alt_AvError;
	dero_AvError_last=dero_AvError;

	trackstar.alt_point=0;
	for ( int ti=0; ti<100; ti++ )
	{
		Az_dError[ti]=0;
		Alt_dError[ti]=0;
		Dero_dError[ti]=0;
	}


    //QString str;
    char cmd[255];
    sprintf ( cmd,"M180=0M380=0M480=0M186=0M386=0M486=0");
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );

    /*GetUmacTime();
    QDateTime dt;
    dt=QDateTime::currentDateTime();
    //QDate date;
    QTime time;
    //date=dt.date();
    time=dt.time();
    StartUtc=(time.hour()*3600+time.minute()*60.+time.second())*1000+time.msec();
    StartServoTimer=NowServoTimer;*/
}

void Tracking::on_pushButton_track_clicked()
{
	QString str;

    str=lineEdit_deltaUT->text();
    deltaUt=str.toDouble();
    //qDebug()<<"aw="<<AW<<"  an="<<AN<<"  CA="<<CA<<"  IA="<<IA<<"  IE="<<IE<<"  ACEC="<<ACEC;
	IniTrack();
	if ( readStarTable() != true )
        {
		return;
        }

	LookupFirstPoint();
	if ( flagtrack.StarTableEnd )
	{
                label_Status->setText ( "Star table end!" );
		on_pushButton_stop_clicked();
		return;
	}
	showTrackInfo();
    //CheckTarget();

    /*if ( !trackstar.flagAzRange||!trackstar.flagAltRange||!trackstar.flagDeroRange ||trackstar.flagZenith)
	{
	//	showTrackInfo();
		return;
    }*/


	timer_ShowInfo->stop();
	timer_show_track_info->start ( ShowTrackInfoClock );
        pushButton_track->setText ( "Tracking..." );
	pushButton_track->setEnabled ( false );
 
    //AZ   Pointing
	if ( flagtcs.flag_AZIMUTH )
        {
            qDebug("*********************************Test Azi*********************************");
            PointAz();
            SetAzTrackingValueNo1();
            Az_beginPoint();
            trackstar.flagAzPointing=true;
	}
	//ALT   pointing
	if (flagtcs.flag_ALTITUDE )
        {
            qDebug("*********************************Test Alt*********************************");
            if(trackstar.AltTarget>5)
            {
                PointAlt();
                SetAltTrackingValueNo1();
                setPIDparam_Alt_point();
                Alt_beginPoint();
                trackstar.flagAltPointing=true;
                trackstar.flagAltRange=false;
                //label_Status->setText ( "Alt of target beyonds the range!" );
            }
            else trackstar.flagAltRange=true;
	}
    //derotator pointing
    if ( flagtcs.flag_DERO )
	{
        PointDero();
        Dero_beginPoint();
        trackstar.flagDeroPointing=true;
        //SetDeroTrackingValueNo1();
    }
    if ( flagtcs.flag_ADC1 || flagtcs.flag_ADC2 )
    {
        trackstar.flagADC1Tracking=true;
        PointADC();
    }

    //timer_CheckPointingORTrackingDero->start ( CheckPointingORTrackingClockDero );
    //timer_CheckPointingORTrackingAz->start ( CheckPointingORTrackingClockAz );
    //timer_CheckPointingORTrackingAlt->start ( CheckPointingORTrackingClockAlt);

 	StartTrackingThread();

	timer_CalAndShowRms->stop ();
	timer_CalAndShowRms->start (SampleTime*1000 );

}

void   Tracking::TargetBeyondRange()
{
	if ( !trackstar.flagAzRange )
        label_Status->setText ( "Az of target beyonds the range!" );
	if ( !trackstar.flagAltRange )
        label_Status->setText ( "Alt of target beyonds the range!" );
	if ( !trackstar.flagDeroRange )
        label_Status->setText ( "Dero of target beyonds the range!" );
	if ( trackstar.flagZenith)
        label_Status->setText ( "Target is too close to zenith!" );

}



void  SendFloatToDPRAM(uint16_t offset,float data)
{
    float fData[10];
    int iDatalength=1;
    fData[0]=data;
    SQUMAC::instance_p()->QsendDataDPRAM (fData,  offset, iDatalength);
}

void  SendIntToDPRAM(uint16_t offset,int data)
{
    int iData[10];
    int iDatalength=1;
    iData[0]=data;
    SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);
}

void  SendPointDataToDPRAM(uint16_t offset,int point_time,int pos,int vel_end)
{
    int iData[10];
    int iDatalength=3;
    iData[0]=point_time;
    iData[1]= pos;
    iData[2]= vel_end;
    SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);
}

void   Tracking::PointAz()
{
	QString str,strout;
	int delay_row;
 	QStringList  list;
	QByteArray ba;
    double temp1=fabs ( trackstar.AzTrackError );
    //qDebug()<<"trackstar.AzTrackError"<< trackstar.AzTrackError;
	StarTarget startarget;
    if(temp1 <= MAXA_Az/2) trackstar.AzPointTime= sqrt(temp1/3600)*2000+1000;
    else trackstar.AzPointTime= 12*1000+ (temp1- MAXA_Az/2)*1000/10000;
    //if(temp1 <= MAXA_Az) trackstar.AzPointTime= sqrt(temp1/7200)*2000;
    //else trackstar.AzPointTime= 10*1000+ (temp1- MAXA_Az)*1000/36000;

    trackstar.AzPointTime+=4000;//7000
    //trackstar.AzPointTime= 1000*temp1/12000.0 ;		                         //,PointTime:ms
    //if ( trackstar.AzPointTime<15000.0 )
    //	trackstar.AzPointTime=15000.0;

    //trackstar.AzPointTime=trackstar.AzPointTime+SoftwareDelayAz;

//	qDebug() <<"Az old target"<<trackstar.AzTarget;
	delay_row=int ( trackstar.AzPointTime*TargetNumPerSecond/1000 );
 //	lock.lockForWrite();
	trackstar.RowAzPoint=N_row+delay_row;
	time_AzpointEnd=trackstar.AzPointTime;		
				
    if ( trackstar.RowAzPoint>TableNum-1000 )
	{
		flagtrack.StarTableEnd=true;
        //qDebug()<<"PointAz  flagtrack.StarTableEnd=true"<<trackstar.RowAzPoint;
		return;
	}

	list= starTable[trackstar.RowAzPoint].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );        	//Get Az new target
	if ( list.size() >=3)
	{
        startarget.cAzimuth=list[2];
        //startarget.cAzimuth=list[3];
        trackstar.AzTarget=startarget.cAzimuth.toDouble();
        trackstar.AltTarget=list[3].toDouble();
	}
    double azpos=180;
    list= starTable[trackstar.RowAzPoint+TargetNumPerSecond].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );        	//Get Az new target
    if ( list.size() >=3)
    {
        azpos=list[2].toDouble();
        trackstar.AZ_TrackTargetSpeed=(azpos-trackstar.AzTarget)*3600;
    }
    else trackstar.AZ_TrackTargetSpeed=0;
    //SendPointDataToDPRAM(M582_addr,int(trackstar.AzPointTime-1000),int(trackstar.AzTarget*3600/CTS),
    //                     int(trackstar.AZ_TrackTargetSpeed/CTS));
    if(flagtcs.PointModel)
    {
        double A= trackstar.AzTarget*PI/180.0;
        double E= trackstar.AltTarget*PI/180.0;
        az_model=AW*cos ( A ) *tan ( E ) +AN*sin ( A ) *tan ( E )+ACES*sin(A)-ACEC*cos(A)
               +CA/cos ( E )-CRX*cos(A-E)/cos(E)+CRY*sin(A-E)/cos(E)+IA
                +NPAE*tan(E) +NRX+NRY*tan ( E );

        trackstar.AzTarget+= az_model/3600.0;
    }
    setPIDparam_Az_point();
    SendIntToDPRAM(M2182_addr,int(trackstar.AzPointTime-4000));
    SendIntToDPRAM(M2183_addr,int(trackstar.AzTarget*3600/CTS_az));
    //SendDouble2InttoDPRAM(M583_addr,trackstar.AzTarget);
	GetStartTime();
    qDebug() <<"Az firstPoint delay"<<trackstar.AzPointTime;
    qDebug() <<"Az new target"<<trackstar.AzTarget;
    qDebug() <<"Az N_row"<<N_row<<"pointRow"<<trackstar.RowAzPoint;

    aztrackstar.Vel_Group1[0]=trackstar.AZ_TrackTargetSpeed/CTS;
    aztrackstar.Vel_Group1[1]=trackstar.AZ_TrackTargetSpeed/CTS;
    aztrackstar.Vel_Group2[0]=trackstar.AZ_TrackTargetSpeed/CTS;
    aztrackstar.Vel_Group2[1]=trackstar.AZ_TrackTargetSpeed/CTS;
    aztrackstar.Target_Group1[0]=trackstar.AzTarget*3600/CTS;
    aztrackstar.Target_Group1[1]=aztrackstar.Target_Group1[0]+trackstar.AZ_TrackTargetSpeed/2/CTS;
    aztrackstar.Target_Group2[0]=aztrackstar.Target_Group1[1]+trackstar.AZ_TrackTargetSpeed/2/CTS;
    aztrackstar.Target_Group2[1]=aztrackstar.Target_Group2[0]+trackstar.AZ_TrackTargetSpeed/2/CTS;
}

void   Tracking::PointAlt()
{
    QString str,strout;
    int delay_row;
    QStringList  list;
    QByteArray ba;
    StarTarget startarget;
    //ALT point
    double temp1=fabs ( trackstar.AltTrackError );
    if(temp1 <= MAXA_Alt/2) trackstar.AltPointTime= sqrt(temp1/3600)*2000;
    else trackstar.AltPointTime= 8*1000+ (temp1- MAXA_Alt/2)*1000/8000;
    //if(temp1 <= MAXA_Alt) trackstar.AltPointTime= sqrt(temp1/7200)*2000;
    //else trackstar.AltPointTime= 5*1000+ (temp1- MAXA_Alt)*1000/18000;
    trackstar.AltPointTime+=6000;//7000;

    /*trackstar.AltPointTime= 1000*temp1/10800.0 ;							//3600"/s,PointTime:ms
    if ( trackstar.AltPointTime<10000.0 )
        trackstar.AltPointTime=10000.0;*/

    //qDebug() <<"old target"<<trackstar.AltTarget;
    //trackstar.AltPointTime=trackstar.AltPointTime+SoftwareDelayAlt;
    delay_row=int ( trackstar.AltPointTime*TargetNumPerSecond/1000 );
    //lock.lockForWrite();
    trackstar.RowAltPoint=N_row+delay_row;//+AltCorrectLines;//TrackPeriod*NumPerGroup;

    time_AltpointEnd=trackstar.AltPointTime;//+AltCorrectLines*TableLineTime;						//ms
    if ( trackstar.RowAltPoint>TableNum-1000 )
    {
        flagtrack.StarTableEnd=true;
        qDebug()<<"PointAlt  flagtrack.StarTableEnd=true"<<N_row;
        return;
    }

    list= starTable[trackstar.RowAltPoint].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );        	//Get Alt new target
    if ( list.size() >=4 )
    {
        startarget.cAltitude=list[3];
        trackstar.AltTarget=startarget.cAltitude.toDouble();
        trackstar.AzTarget=list[2].toDouble();
    }

    trackstar.flag_AltpointEnd=false;

    double altpos=70;
    list= starTable[trackstar.RowAltPoint+TargetNumPerSecond].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );        	//Get Az new target
    if ( list.size() >=4)
    {
        altpos=list[3].toDouble();
        trackstar.ALT_TrackTargetSpeed=(altpos-trackstar.AltTarget)*3600;
    }
    else trackstar.ALT_TrackTargetSpeed=0;

    //SendPointDataToDPRAM(M682_addr,int(trackstar.AltPointTime-1000),int(trackstar.AltTarget*3600/CTS),
    //                     int(trackstar.ALT_TrackTargetSpeed/CTS));
    //setPIDparam_Alt_point();
    if(flagtcs.PointModel)
    {
        double A= trackstar.AzTarget*PI/180.0;
        double E= trackstar.AltTarget*PI/180.0;
        alt_model= -AW*sin ( A ) +AN*cos ( A )-IE+TF*sin(PI/2-E)
                +CRX*sin(A-E)+CRY*cos(A-E)
                -ECEC*cos(E)-ECES*sin(E)+NRX*sin( E )-NRY*cos ( E );
        /*alt_model0= -AW0*sin ( A ) +AN0*cos ( A )-IE0+TF*sin(PI/2-E)
                +CRX0*sin(A-E)+CRY0*cos(A-E)
                +NRX0*sin( E )-NRY0*cos ( E );*/
        trackstar.AltTarget+= alt_model/3600.0;
    }
    SendIntToDPRAM(M1782_addr,int(trackstar.AltPointTime-3000));
    SendIntToDPRAM(M1783_addr,int(trackstar.AltTarget*3600/CTS_alt));
    //SendFloatToDPRAM(M682_addr,float(trackstar.AltPointTime));
    //SendDouble2InttoDPRAM(M683_addr,trackstar.AltTarget);

    qDebug() <<"Alt firstPoint delay ms"<<trackstar.AltPointTime;
    qDebug() <<"Alt new target"<<trackstar.AltTarget;
    qDebug() <<"Alt N_row"<<N_row<<"pointRow"<<trackstar.RowAltPoint;

    alttrackstar.Vel_Group1[0]    =trackstar.ALT_TrackTargetSpeed/CTS;
    alttrackstar.Vel_Group1[1]    =trackstar.ALT_TrackTargetSpeed/CTS;
    alttrackstar.Vel_Group2[0]    =trackstar.ALT_TrackTargetSpeed/CTS;
    alttrackstar.Vel_Group2[1]    =trackstar.ALT_TrackTargetSpeed/CTS;
    alttrackstar.Target_Group1[0] =trackstar.AltTarget*3600/CTS;
    alttrackstar.Target_Group1[1] =alttrackstar.Target_Group1[0]+trackstar.ALT_TrackTargetSpeed*0.5/CTS;
    alttrackstar.Target_Group2[0] =alttrackstar.Target_Group1[1]+trackstar.ALT_TrackTargetSpeed*0.5/CTS;
    alttrackstar.Target_Group2[1] =alttrackstar.Target_Group2[0]+trackstar.ALT_TrackTargetSpeed*0.5/CTS;
}



void   Tracking::PointDero()
{
	QString str,strout;
	int delay_row;
 	QStringList  list;
	QByteArray ba;
    StarTarget startarget;
    double temp1=fabs ( trackstar.DeroTrackError );
    if(temp1 <= MAXA_Dero) trackstar.DeroPointTime= sqrt(temp1/5400)*2000;
    else trackstar.DeroPointTime= 5*1000+ (temp1- MAXA_Dero/2)*1000/3600;
    trackstar.DeroPointTime+=5000;
    /*trackstar.DeroPointTime= 1000*temp1/7500.0 ;							//3600"/s,PointTime:ms
    if ( trackstar.DeroPointTime<10000.0 )
        trackstar.DeroPointTime=10000.0;*/

    //trackstar.DeroPointTime=trackstar.DeroPointTime+SoftwareDelayDero;
	time_DeropointEnd=trackstar.DeroPointTime;
	//qDebug()<<"time_DeropointEnd"<<time_DeropointEnd;
//	GetStartTime();
//	qDebug() <<"Dero old target"<<trackstar.DeroTarget;
	delay_row=int ( trackstar.DeroPointTime*TargetNumPerSecond/1000 );
    //lock.lockForWrite();
	trackstar.RowDeroPoint=N_row+delay_row;//+DeroCorrectLines;						//SoftwareDealyTime*TargetNumPerSecond

    if ( trackstar.RowDeroPoint>TableNum-100 )
	{
		flagtrack.StarTableEnd=true;
        //qDebug()<<"PointDero  flagtrack.StarTableEnd=true"<<trackstar.RowDeroPoint;
		return;
	}

	list= starTable[trackstar.RowDeroPoint].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );        	//Get Dero new target
    if ( list.size() >=4 )
	{
		startarget.cDero=list[4];
        trackstar.DeroTarget=(list[4].toDouble()-list[3].toDouble())/2+180;
        if(DeroReverse )
        {
            if(trackstar.DeroTarget>0)trackstar.DeroTarget-=90;
            else if(trackstar.DeroTarget<0)trackstar.DeroTarget+=90;
        }
        if(flagtcs.Dero_ALTHALF)
            trackstar.DeroTarget=(-list[3].toDouble())/2;
    }
    trackstar.flag_DeropointEnd=false;     //lock.unlock();

    double deropos;
    list= starTable[trackstar.RowDeroPoint+TargetNumPerSecond].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );        	//Get Az new target
    if ( list.size() >=4)
    {
        deropos=(list[4].toDouble()-list[3].toDouble())/2+180;
        if(DeroReverse)
        {
            if(deropos>0)deropos-=90;
            else if(deropos<0)deropos+=90;
        }
        if(flagtcs.Dero_ALTHALF)
            deropos=(-list[3].toDouble())/2;
        trackstar.Dero_TrackTargetSpeed=(deropos-trackstar.DeroTarget)*3600;
    }
    else trackstar.Dero_TrackTargetSpeed=0;

    //SendPointDataToDPRAM(M282_addr,int(trackstar.DeroPointTime-1000),int(trackstar.DeroTarget*3600/CTS_Dero),
     //                    int(trackstar.Dero_TrackTargetSpeed/CTS_Dero));
    SendIntToDPRAM(M282_addr,int(trackstar.DeroPointTime-2500));
    SendIntToDPRAM(M283_addr,int(trackstar.DeroTarget*3600/CTS_Dero));

    //qDebug() <<"firstPoint delay"<<trackstar.DeroPointTime<<"now N_row"<<N_row;
    //qDebug() <<"new target"<<trackstar.DeroTarget<<"pointRow"<<trackstar.RowDeroPoint;
    //qDebug() <<"Dero N_row"<<N_row<<"pointRow"<<trackstar.RowDeroPoint;
/*
    derotrackstar.Vel_Group1[0]=trackstar.Dero_TrackTargetSpeed/CTS_Dero;
    derotrackstar.Vel_Group1[1]=trackstar.Dero_TrackTargetSpeed/CTS_Dero;
    derotrackstar.Vel_Group2[0]=trackstar.Dero_TrackTargetSpeed/CTS_Dero;
    derotrackstar.Vel_Group2[1]=trackstar.Dero_TrackTargetSpeed/CTS_Dero;
    derotrackstar.Target_Group1[0]=trackstar.DeroTarget*3600/CTS;
    derotrackstar.Target_Group1[1]=derotrackstar.Target_Group1[0]+trackstar.Dero_TrackTargetSpeed/2/CTS_Dero;
    derotrackstar.Target_Group2[0]=derotrackstar.Target_Group1[1]+trackstar.Dero_TrackTargetSpeed/2/CTS_Dero;
    derotrackstar.Target_Group2[1]=derotrackstar.Target_Group2[0]+trackstar.Dero_TrackTargetSpeed/2/CTS_Dero;
*/
}

/*void  Tracking::PointADC(int altitude)
{
    if(altitude>90 || altitude<5)return;
    int i=1;
    int diff=altitude-Alt2Adc[i][0];
    if(altitude>15)
    {
        while(diff>0 && i<17)
        {
            i++;
            diff=altitude-Alt2Adc[i][0];
        }
    }
    //qDebug() <<"Adc point i="<<i<<"alt"<<Alt2Adc[i][0]<<"target"<<Alt2Adc[i][1];
    //AltforAdc=Alt2Adc[i][0];
    if(i<17)
    {
        //AltforAdc=Alt2Adc[i][0];
        qDebug() <<"Adc point i="<<i<<"alt"<<Alt2Adc[i][0]<<"target"<<Alt2Adc[i][1];
        char cmd[255];
        QString str;
        double adcTarget=(Alt2Adc[i-1][1]+(Alt2Adc[i][1]-Alt2Adc[i-1][1])*(altitude-Alt2Adc[i-1][0])/
                          (Alt2Adc[i][0]-Alt2Adc[i-1][0]))/2;
        trackstar.ADC1Target = adcTarget;
        trackstar.ADC2Target = adcTarget*3600 / CTS_ADC;
        //if ( flagtcs.flag_ADC1 )sprintf(cmd, "#3J=%7.1f", trackstar.ADC2Target);
        //else  if (  flagtcs.flag_ADC2 )sprintf(cmd, "#4J=%7.1f", trackstar.ADC2Target);
        //else   if ( flagtcs.flag_ADC1 && flagtcs.flag_ADC2 )
        sprintf(cmd, "#3J=%7.1f #4J=%7.1f", trackstar.ADC2Target,trackstar.ADC2Target);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
}*/
void  Tracking::PointADC()
{
    //if(altitude>90 || altitude<5)return;
    double Zen=(90-trackstar.AltTarget)/180*PI;
    QString tmp;
    tmp=lineEdit_Pressure->text();
    Pressure=tmp.toDouble();
    double P=Pressure*0.75006;//Pressure, from mbar to mmHg

    tmp=lineEdit_Temperature->text();
    Temperature=tmp.toDouble();
    double T=Temperature;//temperature,degree

    //tmp=lineEdit_RHumidity->text();
    //double E=tmp.toDouble();//relative hmunity

    //tmp=lineEdit_ADC2_Offset->text();
    //Zen=tmp.toDouble();// zenith

    double mm=P*(1+(1.049-0.0157*T)*P*0.000001)/(24.0294+0.087972*T);
    double p1=0.5761*tan(Zen/180*PI)+0.004844;
    /*double p2=E*0.0075008*0.000001*0.05968/(1+0.003661*T)
            *(-2.184*10000000*exp(-(T-44.64)*(T-44.64)/3.624/3.624)
              +2015*exp(-(T-158.7)*(T-158.7)/65.86/65.86));*/
    trackstar.ADC2Target=mm*p1;
    qDebug() <<"Zenith="<<Zen<<" p1="<<p1<<"  M="<<mm
            <<" adc=mm*(p1-p2)="<<trackstar.ADC2Target;
    char cmd[255];
    QString str;
    sprintf(cmd, "i322=%7.4f i422=%7.4f", 3.6/CTS_ADC,3.6/CTS_ADC);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    sprintf(cmd, "#3J=%7.4f #4J=%7.4f",
            (trackstar.ADC2Target*3600+trackstar.ADC1_correct_all)/ CTS_ADC,
            (trackstar.ADC2Target*3600+trackstar.ADC2_correct_all)/ CTS_ADC);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
 }
void    SetDeroTrackingValueNo1()
{
	int row=0;
	DeroGroup=1;
	row=trackstar.RowDeroPoint+LinesEvery100ms;
	qDebug()<<"row 1"<<row;
        DeroToUMAC(row);					//start M6000
	DeroGroup=DeroGroupLast;

	DeroGroup=2;
	row=trackstar.RowDeroPoint+101*LinesEvery100ms;
	qDebug()<<"row 2"<<row;
        DeroToUMAC(row);                                     //start M6050
	DeroGroup=DeroGroupLast;
	qDebug()<<"row3"<<row;
}

void    SetAzTrackingValueNo1()
{
    AzGroup=1;
    AzToUMAC_Posix();					//start M4000

	AzGroup=2;
    AzToUMAC_Posix();
	AzGroupLast=AzGroup;
	//AzGroup=AzGroupLast;
	qDebug()<<"SetAzTrackingValueNo1  AzGroup"<<AzGroup;
}


void    SetAltTrackingValueNo1()
{
	
    AltGroup=1;
    AltToUMAC_Posix();						//start M5000
    //AltGroupLast=AltGroup;
    AltGroup=2;
    AltToUMAC_Posix();
	AltGroupLast=AltGroup;
        qDebug()<<"SetAltTrackingValueNo1  AzGroup"<<AzGroup;
}

// void    SetDeroTrackingValueNo1()
// {
// 	DeroGroup=1;
//         DeroToUMAC(6000,trackstar.RowDeroPoint+LinesEvery100ms);					//start P6000
// 	DeroGroup=DeroGroupLast;
// 	DeroGroup=2;
//         DeroToUMAC(6010,trackstar.RowDeroPoint+101*LinesEvery100ms);                                     //start P6010
// 	DeroGroup=DeroGroupLast;
// }








// //tracking
// void   DeroToUMAC_Posix()
// {
// ///////////////////send data to UMAC/////////////////////////////////
// 	QString  s1_dero;
// 	QByteArray ba;
// 	QString str,str1;
// 	axis="DERO";
// 	int Tm=100;
// 	int StartP=6000;
// 	QStringList  list;
// 	int n=1;
// 	char cmd[255];
// 	sprintf ( cmd,"P484" );
// 	GetStatusFromUMAC ( cmd,n, &list );
// 	MakeStr ( axis,Tm,N_row,StartP,trackstar.dero_offset,trackstar.dero_correct,dero_model,trackstar.dero_guidedata,&s1_dero,&trackstar.DeroTargetOffset );
// 	if ( list[0]== "1" ) 										//check P484,p484=1?
// 		StartP=6000;
// 	else if ( list[0]== "2" ) 									//check P484=2?
// 		StartP=6010;
// 
// 	if ( ( list[0]== "1"||list[0]== "2" ) )
// 	{
// //		MakeStr ( axis,Tm,N_row,StartP,trackstar.dero_offset,dero_model,trackstar.dero_guidedata,&s1_dero,&trackstar.DeroTargetOffset );
//  
// 		ba= s1_dero.toLatin1();
// 		SQUMAC::instance_p()->QcommCmdPMAC ( ba,str1 );					//Dero 10 lines
// //	 	qDebug() <<"command"<<s1_dero;
// 		trackstar.flag_sendDero=false;							//don't send out data to UMAC
// 		SQUMAC::instance_p()->QcommCmdPMAC ( "P484=0",str1 );
// //	        qDebug()<<"tracking N_row"<<N_row;
// 	}
// 
// 
// }




// void   AzToUMAC_Posix()
// {
// ///////////////////send data to UMAC/////////////////////////////////
// 	QString  s1_az;
// 	QByteArray ba;
// 	QString str,str1;
// 	axis="AZ";
// 	int Tm=100;
// 	int StartP=4000;
// 	QStringList  list;
// 	int n=1;
// 	char cmd[255];
// 	sprintf ( cmd,"P184" );
// 	GetStatusFromUMAC ( cmd,n, &list );
// 
// 	MakeStr ( axis,Tm,N_row,StartP,trackstar.az_offset,trackstar.az_correct,az_model,trackstar.az_guidedata,&s1_az,&trackstar.AzTargetOffset );
// 
// 	if ( list[0]== "1" ) 										//check P184,p184=1?
// 		StartP=4000;
// 	else if ( list[0]== "2" ) 									//check P184=2?
// 		StartP=4010;
// 
// 	if ( ( list[0]== "1"||list[0]== "2" ) )
// 	{
// 	//	MakeStr ( axis,Tm,N_row,StartP,trackstar.az_offset,az_model,trackstar.az_guidedata,&s1_az,&trackstar.AzTargetOffset );
//  
// 		ba= s1_az.toLatin1();
// 		SQUMAC::instance_p()->QcommCmdPMAC ( ba,str1 );					//AZ 10 lines
// //	 	qDebug() <<"command"<<s1_az;
// 		trackstar.flag_sendAz=false;							//don't send out data to UMAC
// 		SQUMAC::instance_p()->QcommCmdPMAC ( "P184=0",str1 );
// //	        qDebug()<<"tracking N_row"<<N_row;
// 	}
// 
// 
// }


// void   DeroToUMAC(int StartP,int row)
// {
// ///////////////////send data to UMAC/////////////////////////////////
// 	QString  s1_dero;
// 	QByteArray ba;
// 	QString str,str1;
// 
// 	axis="DERO";
// 	int Tm=TrackPeriod;
//         for(int i=0;i<10;i++)
// 	{
// 
// 		trackstar.dero_correct[i]=0;
// 	}
// 	MakeStr ( axis,Tm,row,StartP,trackstar.dero_offset,trackstar.dero_correct,dero_model,trackstar.dero_guidedata,&s1_dero,&trackstar.DeroTargetOffset );
// 	ba= s1_dero.toLatin1();
// 	qDebug() <<"command"<<s1_dero;
// 	SQUMAC::instance_p()->QcommCmdPMAC ( ba,str1 );					//dero 10 lines
// 
// }

// void   AltToUMAC(int StartP,int row)
// {
// ///////////////////send data to UMAC/////////////////////////////////
// 	QString  s1_alt;
// 	QByteArray ba;
// //	QString sTMalt[10],sTargetAlt[10];
// 	QString str,str1;
// 
// 	axis="ALT";
// 	int Tm=TrackPeriod;
// 	MakeStr ( axis,Tm,row,StartP,trackstar.alt_offset,trackstar.alt_correct,alt_model,trackstar.alt_guidedata,&s1_alt,&trackstar.AltTargetOffset );
// 	ba= s1_alt.toLatin1();
// 	qDebug() <<"command"<<s1_alt;
// 	SQUMAC::instance_p()->QcommCmdPMAC ( ba,str1 );				//ALT 10 lines
// 
// }

//tracking
// void   AltToUMAC_Posix()
// {
// ///////////////////send data to UMAC/////////////////////////////////
// 	QString  s1_alt;
// 	QByteArray ba;
// 	QString str,str1;
// 	axis="ALT";
// 	int Tm=100;
// 	int StartP=5000;
// 	QStringList  list;
// 	int n=1;
// 	char cmd[255];
// 	sprintf ( cmd,"P384" );
// 	GetStatusFromUMAC ( cmd,n, &list );
// 
// 	//MakeStr ( axis,Tm,N_row,StartP,trackstar.alt_offset,alt_model,trackstar.alt_guidedata,&s1_alt,&trackstar.AltTargetOffset );
// 
// 	if ( list[0]== "1" ) 										//check P148,p148=1?
// 		StartP=5000;
// 	else if ( list[0]== "2" ) 									//check P384=2?
// 		StartP=5010;
// 
// 	if ( ( list[0]== "1"||list[0]== "2" ) )
// 	{
// 		MakeStr ( axis,Tm,N_row,StartP,trackstar.alt_offset,alt_model,trackstar.alt_guidedata,&s1_alt,&trackstar.AltTargetOffset );
//  
// 		ba= s1_alt.toLatin1();
// 		SQUMAC::instance_p()->QcommCmdPMAC ( ba,str1 );					//ALT 10 lines
// 	// 	qDebug() <<"command"<<s1_alt;
// 		trackstar.flag_sendAlt=false;							//don't send out data to UMAC
// 		SQUMAC::instance_p()->QcommCmdPMAC ( "P384=0",str1 );
// 	//        qDebug()<<"tracking N_row"<<N_row;
// 	}
// 
// 
// }

// void   Tracking::LookupFirstPoint() //
// {
// 	double utc2=0,tmp_time=0;
// 	double utc=0;
// 	int h,m;										  		//hour,miniute
// 	double second;												//second
// 	QStringList  list;
// 	StarTarget startarget;
// 	/////////////////////////////get UTC time/////////////////////////////////////
// 	QDateTime dt;
// 	dt=QDateTime::currentDateTime();
// 	QDate date;
// 	QTime time;
// 	date=dt.date();
// 	time=dt.time();
// 	utc= ( time.hour() ) *3600+time.minute() *60.+time.second() +time.msec() /1000.0;			//local time
// 
// 	
// 	for ( register int j=N_row_LAST;j< ( TableNum-20 );j++ ) 									//start from N_row_LAST
// 	{
// 		list= starTable[j].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
// 		if ( list.size() >=2 )
// 		{
// 			startarget.cDate=list[0];
// 			startarget.cTime=list[1];
// 		}
// 		tmp_time=startarget.cTime.toDouble();
// 		h=int ( tmp_time/10000 );
// 		m=int ( ( tmp_time-h*10000 ) /100 );
// 		second=tmp_time-h*10000-m*100;
// 		utc2=h*3600+m*60+second;
// 
// 		if ( fabs ( utc-utc2 ) <=0.01 )
// 		{
// 
// 			N_row=j;
// 			N_row_LAST=N_row;
// 
// 			if ( N_row< ( TableNumMAX-100 ) )
// 			{
// 				if ( list.size() >=5 )
// 				{
// 					startarget.cAzimuth=list[2];
// 					startarget.cAltitude=list[3];
// 					startarget.cDero=list[4];
// 				}
// //				lock.lockForWrite();
// 				trackstar.AzTarget=startarget.cAzimuth.toDouble();
// 				trackstar.AltTarget=startarget.cAltitude.toDouble();
// 				trackstar.DeroTarget=startarget.cDero.toDouble();
// 				CheckTarget();								//make sure targets are in range
// //				lock.unlock();
// 				GetAzAltPosition_Posix();						//AzPosRealForError  ,AltPosRealForError
// 				GetDeroPosition_Posix();						//get deroPosReal_Degree_forError
// 
// //				lock.lockForWrite();
// 				trackstar.AzTrackError= ( trackstar.AzTarget-trackstar.AzPosRealForError ) *3600;
// 				trackstar.AltTrackError= ( trackstar.AltTarget-trackstar.AltPosRealForError ) *3600;
// 				trackstar.DeroTrackError= ( trackstar.DeroTarget-trackstar.DeroPosRealForError ) *3600;
// //				lock.unlock();
// 			}
// 			else
// 				{qDebug() <<"LookupTable N_row number error"<<N_row;}
// 			//	double A=trackstar.AzTarget*3.1415926535897932384626433832795/180.0;	//MODEL
// 			//	double E=trackstar.AltTarget*3.1415926535897932384626433832795/180.0;
// 
// 
// 			///////////////////startable end?  //////////////////////////////////////////////////////////
// 			if ( N_row  > ( TableNum-1000 ) ||N_row==0 )
// 			{
// 				trackstar.StarTableEnd=true;
// 				return;
// 			}
// 			//break;
// 			return;
// 		}
// 
// 
// 	}
// 
// 
// 
// }

void Tracking::CheckTarget()								//make sure targets are in range
{
    /*if ( flagtcs.flag_AZIMUTH&&trackstar.flagAzTracking&&(trackstar.AzTarget<40||trackstar.AzTarget>130) )
        {trackstar.flagAzRange=false;on_pushButton_stop_clicked();
        label_Status->setText ( "Az of target beyonds the range!" );
        }
    else
        trackstar.flagAzRange=true;*/

	//if ( flagtcs.flag_ALTITUDE&&trackstar.flagAltTracking&&(trackstar.AltTarget<5||trackstar.AltTarget>90 ))
	if ( flagtcs.flag_ALTITUDE&&(trackstar.AltTarget<5||trackstar.AltTarget>90 ))
    {
        trackstar.flagAltRange=false;
        on_pushButton_stop_clicked();
        label_Status->setText ( "Alt of target beyonds the range!" );
    }
	else
		trackstar.flagAltRange=true;

/* 	if ( (flagtcs.flag_C1||  flagtcs.flag_N2||flagtcs.flag_N1)&&(trackstar.DeroTarget<-360||trackstar.DeroTarget>360)&&trackstar.flagDeroTracking )
        {trackstar.flagDeroRange=false;on_pushButton_stop_clicked();
        label_Status->setText ( "Dero of target beyonds the range!" );
    }
    else
        trackstar.flagDeroRange=true;*/


	if(trackstar.AltTarget>89.5&&(trackstar.AltTarget<90.5))
    {
        trackstar.flagZenith=true;
        on_pushButton_stop_clicked();
        label_Status->setText ( "Target is too close to zenith! " );
    }
	else
		trackstar.flagZenith=false;

}


void GetTableLine1()
{
	QStringList  list;
	double utc1=0;
	double tmp_time=0;
	int h,m;										  		//hour,miniute
	double second;	
	StarTarget startarget;	
	list= starTable[0].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	if ( list.size() >=2 )
	{
		startarget.cDate=list[0];
		startarget.cTime=list[1];
	}
	tmp_time=startarget.cTime.toDouble();
	h=int ( tmp_time/10000 );
	m=int ( ( tmp_time-h*10000 ) /100 );
	second=tmp_time-h*10000-m*100;
	utc1=h*3600+m*60+second;

	No1=long(utc1*1000/TableLineTime);
}


void   Tracking::LookupFirstPoint() //
{     										//second
	StarTarget startarget;
	QStringList  list;
	double utc=0;
    double haNow=0;
    /////////get No1//////
    GetTableLine1();
	/////////////////////////////get UTC time/////////////////////////////////////
	QDateTime dt;
	dt=QDateTime::currentDateTime();
	QDate date;
	QTime time;
	date=dt.date();
	time=dt.time();
    utc= ( time.hour() ) *3600+time.minute() *60.+time.second() +deltaUt +time.msec() /1000.0;						//local time
    NoX=long(utc*1000/TableLineTime);
    N_row=NoX-No1;
    if(N_row<0)N_row+=24*3600*1000/TableLineTime;
    if ( N_row  > ( TableNum-1000 ) )
	{
		flagtrack.StarTableEnd=true;
        qDebug()<<"LookupFirstPoint  flagtrack.StarTableEnd=true"
               <<"tablenum"<<TableNum<<"N now"<<N_row<<"No1"<<No1;
		return;
	}
	
	list= starTable[N_row].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	if ( list.size() >=5 )
	{
        startarget.cAzimuth=list[2];
        startarget.cAltitude=list[3];
		startarget.cDero=list[4];
	}

        
	//				lock.lockForWrite();
	trackstar.AzTarget=startarget.cAzimuth.toDouble();
	trackstar.AltTarget=startarget.cAltitude.toDouble();
    trackstar.DeroTarget=(startarget.cDero.toDouble()-trackstar.AltTarget)/2+180;
    haNow=list[5].toDouble();
    trackstar.ra=lst*15-haNow;
    trackstar.dec=list[6].toDouble();
    //qDebug()<<"ra"<<trackstar.ra<<"dec"<<trackstar.dec;
    StarAtNorth_Az=0;
    if(degrad(trackstar.dec)>=np.n_lat)
    {
        StarAtNorth=true;
        double AzTarget_End=0;
        list= starTable[TableNum-1000].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
        if ( list.size() >=5 )  AzTarget_End=list[2].toDouble();
        if (trackstar.AzTarget<90 && AzTarget_End> 270 ) StarAtNorth_Az=-1;
        /*{
            if(haNow<-170)StarAtNorth_Az=1;
            else StarAtNorth_Az=-1;
        }*/
        else if(trackstar.AzTarget>270 && AzTarget_End< 90 ) StarAtNorth_Az=1;
        /*{
            if(haNow>170)StarAtNorth_Az=-1;
            else StarAtNorth_Az=1;
        }*/
        if(fabs(list[5].toDouble())<90)DeroReverse=true;
        else DeroReverse=false;
        if(DeroReverse)
        {
            if(trackstar.DeroTarget>0)trackstar.DeroTarget-=90;
            else trackstar.DeroTarget+=90;
        }
    }
    else
    {
        StarAtNorth=false;
        StarAtNorth_Az=0;
    }
    if(flagtcs.Dero_ALTHALF)
        trackstar.DeroTarget=(-list[3].toDouble())/2;
    //if(fabs(degrad(trackstar.dec)-np.n_lat)<0.15)StarAtZenith=true;
    //else StarAtZenith=false;
    //CheckTarget();								//make sure targets are in range
    //lock.unlock();
	GetAzAltDeroPosition_Posix();						//AzPosRealForError  ,AltPosRealForError
	
	//				lock.lockForWrite();
	trackstar.AzTrackError= ( trackstar.AzTarget-trackstar.AzPosRealForError ) *3600;
	trackstar.AltTrackError= ( trackstar.AltTarget-trackstar.AltPosRealForError ) *3600;
	trackstar.DeroTrackError= ( trackstar.DeroTarget-trackstar.DeroPosRealForError ) *3600;
	//				lock.unlock();
	
	//	double A=trackstar.AzTarget*3.1415926535897932384626433832795/180.0;	//MODEL
	//	double E=trackstar.AltTarget*3.1415926535897932384626433832795/180.0;
	///////////////////startable end?  //////////////////////////////////////////////////////////
	N_row_LAST=N_row;
	return;

}



void   Tracking::LookupTargetNow()
{
                                            //second
    StarTarget startarget;
    QStringList  list;
    double utc=0;
    QDateTime dt;
    dt=QDateTime::currentDateTime();
    QDate date;
    QTime time;
    date=dt.date();
    time=dt.time();
    utc= ( time.hour() ) *3600+time.minute() *60.+time.second() +deltaUt +time.msec() /1000.0;						//local time
    QString lts;
    lts=QString("%1 h %2 m %3 s").arg(time.hour()).arg(time.minute()).arg(time.second());
    label_EndTimer->setText ( lts );
    NoX=long(utc*1000/TableLineTime);
    N_row=NoX-No1;
    if(N_row<0)N_row+=24*3600*1000/TableLineTime;
    if ( N_row  > ( TableNum-100 ) )
    {
        cal_flag=0;
        flagtrack.StarTableEnd=true;
        qDebug()<<"LookupTargetNow flagtrack.StarTableEnd=true"
               <<"tablenum"<<TableNum<<"N now"<<N_row<<"No1"<<No1;
        return;
    }

    list= starTable[N_row].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
    if ( list.size() >=5 )
    {
        startarget.cAzimuth=list[2];
        startarget.cAltitude=list[3];
        startarget.cDero=list[4];
    }


    //				lock.lockForWrite();
    trackstar.AzTarget=startarget.cAzimuth.toDouble();
    trackstar.AltTarget=startarget.cAltitude.toDouble();
    trackstar.DeroTarget=(startarget.cDero.toDouble()-trackstar.AltTarget)/2+180;
    trackstar.ra=lst*15-list[5].toDouble();
    trackstar.dec=list[6].toDouble();
    //qDebug()<<"ra"<<trackstar.ra<<"dec"<<trackstar.dec;

    if(degrad(trackstar.dec)>=np.n_lat)
    {
        StarAtNorth=true;

        if(fabs(list[5].toDouble())<90)DeroReverse=true;
        else DeroReverse=false;
        if(DeroReverse)
        {
            if(trackstar.DeroTarget>0)trackstar.DeroTarget-=90;
            else trackstar.DeroTarget+=90;
        }
    }
    else
    {
        StarAtNorth=false;
    }
    //StarAtNorth_Az=0;
    if(flagtcs.Dero_ALTHALF)
        trackstar.DeroTarget=(-list[3].toDouble())/2;
    return;
}



void Model()
{
		

}


 

bool  TrackDataForSave()
{
// 	QDateTime dt=QDateTime::currentDateTime();
    //QString dtstr;
// 	dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );
// 	QTime time=dt.time();

      //  double AzTrackError,AltTrackError,DeroTrackError;


/*
 	trackstar.AzTrackError= ( trackstar.AzTarget-trackstar.AzPosRealForError ) *3600;
	trackstar.AltTrackError= ( trackstar.AltTarget-trackstar.AltPosRealForError ) *3600;
	trackstar.DeroTrackError= ( trackstar.DeroTarget-trackstar.DeroPosRealForError ) *3600;*/ 

		if ( count <= M-1 )
		{
 		//	lock.lockForRead();
			AzData_tracking[count][0]=trackstar.AzPosRealForError;			//real position
            AzData_tracking[count][1]=trackstar.AzTarget;				//AZ target
//			AzData_tracking[count][1]=trackstar.AzFolErrUMACTest;
			AzData_tracking[count][2]=trackstar.AzTrackError;  			// AZ_TrackError
			AzData_tracking[count][3]=trackstar.AzFolErrUMAC;			//following error
			AzData_tracking[count][4]=trackstar.AzTargetUMAC;			//
            //AzData_tracking[count][5]=azPos;					//real position
            //AzDataTime_tracking[count]=dtstr;
			//[count]=AzFolErrUMACTestBYTE;

			AltData_tracking[count][0]=trackstar.AltPosRealForError;		//real position
            AltData_tracking[count][1]=trackstar.AltTarget;				//ALT_TrackTarget
			AltData_tracking[count][2]=trackstar.AltTrackError;			//ALT_TrackError
			AltData_tracking[count][3]=trackstar.AltFolErrUMAC; 			//following error
			AltData_tracking[count][4]=trackstar.AltTargetUMAC;
            //AltData_tracking[count][5]=altPos;					//real position
            //AltDataTime_tracking[count]=dtstr;

			DeroData_tracking[count][0]=trackstar.DeroPosRealForError;		//real position
            DeroData_tracking[count][1]=trackstar.DeroTarget;			//DERO_TrackTarget
			DeroData_tracking[count][2]=trackstar.DeroTrackError;				//DERO_TrackError
			DeroData_tracking[count][3]=trackstar.DeroFolErrUMAC; 		        //following error
			DeroData_tracking[count][4]=trackstar.DeroTargetUMAC;
            //DeroData_tracking[count][5]=deroPos;					//real position
            //DeroDataTime_tracking[count]=dtstr;
 		//	lock.unlock();
			count++;
			
		}
		return true;
}

/*
bool  TrackDataForSave()
{

	QDateTime dt=QDateTime::currentDateTime();
	QString dtstr;
	dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );
	QTime time=dt.time();
 

		if ( count <= M-1 )
		{
 		//	lock.lockForRead();
			AzData_tracking[count][0]=trackstar.AzPosRealForError;			//real position
			AzData_tracking[count][1]=trackstar.AzTarget;				//AZ target
			AzData_tracking[count][2]=trackstar.AzTrackError;  			// AZ_TrackError
			AzData_tracking[count][3]=trackstar.AzFolErrUMAC;			//following error
			AzData_tracking[count][4]=trackstar.az_guidedata;			//
			AzDataTime_tracking[count]=dtstr;

			AltData_tracking[count][0]=trackstar.AltPosRealForError;		//real position
			AltData_tracking[count][1]=trackstar.AltTarget;				//ALT_TrackTarget
			AltData_tracking[count][2]=trackstar.AltTrackError;			//ALT_TrackError
			AltData_tracking[count][3]=trackstar.AltFolErrUMAC; 			//following error
			AltData_tracking[count][4]=trackstar.alt_guidedata;
			AltDataTime_tracking[count]=dtstr;

			DeroData_tracking[count][0]=trackstar.DeroPosRealForError;		//real position
			DeroData_tracking[count][1]=trackstar.DeroTarget;			//ALT_TrackTarget
			DeroData_tracking[count][2]=trackstar.DeroTrackError;			//ALT_TrackError
			DeroData_tracking[count][3]=trackstar.DeroFolErrUMAC; 			//following error
			DeroData_tracking[count][4]=trackstar.dero_guidedata;
			DeroDataTime_tracking[count]=dtstr;
 		//	lock.unlock();
			count++;
			
		}

	 
	return true;
}



*/


void    AzCorrectPoint()								 
{
  	double ToffsetAz=0;
	double Wait=0;
        if(trackstar.AzTrackError>1000||az_AvError>1000)
              return;
        ToffsetAz=az_AvError/trackstar.AZ_TrackTargetSpeed;
//      	qDebug()<<"ToffsetAz"<<ToffsetAz;    	
        iErrorStartAz++;
	if(fabs(trackstar.AZ_TrackTargetSpeed)>100)
		Wait=400;
	else
	 	Wait=200;

	if(iErrorStartAz > Wait)
	{
		if(fabs(az_AvError-az_AvError_last)<30)
	        {
            //trackstar.AzCorrectLines=int(ToffsetAz*TargetNumPerSecond);

// 			float error=(ToffsetAz*TargetNumPerSecond-int(ToffsetAz*TargetNumPerSecond))*TableLineTime/1000.*trackstar.AZ_TrackTargetSpeed;
// 			float fData[10];
// 			int iDatalength=1;
//            		uint16_t offset=M180_addr;
//  			trackstar.az_point=trackstar.az_point+error;
//  			fData[0]=trackstar.az_point;
//    			SQUMAC::instance_p()->QsendDataDPRAM (fData,  offset, iDatalength);

			de1taErrAz=0;
			iErrorStartAz=-50;
			trackstar.flagAzCorrectPoint=false;

			if(fabs(trackstar.AZ_TrackTargetSpeed)>100)
                         	iErrorStartAz=-300;

			 qDebug()<<"flagAzCorrectPoint=false"<<"trackstar.flagAzTracking"<<trackstar.flagAzTracking;

       		}
      }
			az_AvError_last=az_AvError;


}

void    DeroCorrectPoint()								////every second
{
  	double ToffsetDero=0;
        if(trackstar.DeroTrackError>1000||dero_AvError>1000)
              return;
  	
        ToffsetDero=dero_AvError/trackstar.Dero_TrackTargetSpeed;
   //     qDebug()<<"Dero correct point"<<ToffsetDero;    	

        iErrorStartDero++;
	if(iErrorStartDero > 150)
	{


// 		trackstar.dero_point=dero_AvError/3600.0;
// 
//     		qDebug()<<"de1taErrDero"<<de1taErrDero<<dero_AvError<<trackstar.DeroTrackError;
// 	
// 		QString str;
// 		char cmd[255];
// 		sprintf ( cmd,"P480=%9.6f",trackstar.dero_point);
// 		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// 
// 		qDebug()<<"flagDeroCorrectPoint=false"<<trackstar.dero_point;
		if(fabs(dero_AvError-dero_AvError_last)<30)
		{
        //trackstar.DeroCorrectLines=int(ToffsetDero*TargetNumPerSecond);

// 		float error=(ToffsetDero*TargetNumPerSecond-int(ToffsetDero*TargetNumPerSecond))*TableLineTime/1000.*trackstar.DERO_TrackTargetSpeed;
// 		float fData[10];
// 		int iDatalength=1;
//           	uint16_t offset=M480_addr;
// 		trackstar.dero_point=trackstar.dero_point+int(ToffsetDero*TarNumPerSecond_UMAC);
// 		fData[0]=trackstar.dero_point;
//   		SQUMAC::instance_p()->QsendDataDPRAM (fData,  offset, iDatalength);
		}


               // if(fabs(dero_AvError)<10)
		//{
			trackstar.flagDeroCorrectPoint=false;
                 	de1taErrDero=0;
			iErrorStartDero=0;
		//}
		//else
		//{
		//	iErrorStartDero=10;	
		//}

        }
	
	dero_AvError_last=dero_AvError;

}

void    AltCorrectPoint()								////every second
{
	//double alt_AvError=0;
  	double ToffsetAlt=0;
        if(trackstar.AltTrackError>1000&&alt_AvError>1000)
              return;

        ToffsetAlt=alt_AvError/trackstar.ALT_TrackTargetSpeed;
      	//qDebug()<<"ToffsetAlt"<<ToffsetAlt;    	
	//qDebug()<<"iErrorStartAlt (AltCorrectPoint)"<<iErrorStartAlt;    	

        iErrorStartAlt++;
	if(iErrorStartAlt > 80)
	{

		if(fabs(alt_AvError-alt_AvError_last)<3)
		{
        //trackstar.AltCorrectLines=trackstar.AltCorrectLines+int(ToffsetAlt*TargetNumPerSecond);
		
// 		float error=(ToffsetAlt*TargetNumPerSecond-int(ToffsetAlt*TargetNumPerSecond))*TableLineTime/1000.*trackstar.ALT_TrackTargetSpeed;
// 		float fData[10];
// 		int iDatalength=1;
//           	uint16_t offset=M380_addr;
// 		trackstar.alt_point=trackstar.alt_point+int(ToffsetAlt*TarNumPerSecond_UMAC);
// 		fData[0]=trackstar.alt_point;
//   		SQUMAC::instance_p()->QsendDataDPRAM (fData,  offset, iDatalength);

		trackstar.flagAltCorrectPoint=false;
		de1taErrAlt=0;
		iErrorStartAlt=0;
		}
        }
		alt_AvError_last=alt_AvError;
}


void    DeroCorrect()
{
	QString str;
        //char cmd[255];
        //double Tim=TrackPeriod*10/1000;								//1s
	double CommandError=0;
  	double ToffsetDero=0;

	CommandError=-1*(trackstar.dero_guidedata);
       	if(trackstar.dero_guidedata!=trackstar.dero_guidedata_last)
      	{
		trackstar.dero_point=trackstar.dero_point+(trackstar.dero_guidedata-trackstar.dero_guidedata_last);
// 		sprintf ( cmd,"P480=%9.6f",trackstar.dero_point);
// 		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
		SendFloatToDPRAM(M480_addr,float(trackstar.dero_point));				//trackstar.dero_point: "
		trackstar.dero_guidedata_last=trackstar.dero_guidedata;
		de1taErrDero=0;
		iErrorStartDero=-100;

	}

	iErrorStartDero++;
	if(iErrorStartDero > 100)
	{
            ToffsetDero=dero_AvError/trackstar.Dero_TrackTargetSpeed;
        //trackstar.DeroCorrectLines=trackstar.DeroCorrectLines+int(ToffsetDero*TargetNumPerSecond);
		
// 		float error=(ToffsetDero*TargetNumPerSecond-int(ToffsetDero*TargetNumPerSecond))*TableLineTime/1000.*trackstar.DERO_TrackTargetSpeed;
// 		float fData[10];
// 		int iDatalength=1;
//           	uint16_t offset=M480_addr;
// 		trackstar.dero_point=trackstar.dero_point+int(ToffsetDero*TarNumPerSecond_UMAC);
// 		fData[0]=trackstar.dero_point;
//   		SQUMAC::instance_p()->QsendDataDPRAM (fData,  offset, iDatalength);

		de1taErrDero=0;
		iErrorStartDero=0;
//        

// 		if(ToffsetDero>0.01)
//                   ToffsetDero=0.01;
// 		if(ToffsetDero<-0.01)
//                   ToffsetDero=-0.01;
// 		qDebug()<<"ToffsetDero dero correct"<<ToffsetDero;  
 		 //qDebug()<<"ERROR dero correct"<<ToffsetDero;  

// //          	PID(CommandError,trackstar.DeroTrackError,P_dero,I_dero,D_dero,&dero_error_1,&ei_dero,&ed_dero,Tim, &de1taErrDero);	
//          	PID(CommandError,trackstar.DeroTrackError,P_alt,I_alt,D_alt,&dero_error_1,&ei_dero,&ed_dero,Tim, &de1taErrDero);	
//  	     	if(de1taErrDero>100) 
// 			de1taErrDero=100;
//                 if(de1taErrDero<=-100)
// 	        	de1taErrDero=-100;	 
//    		qDebug()<<"de1taErrDero"<<de1taErrDero<<dero_AvError<<trackstar.DeroTrackError;               
       	 	

//      
//        		for ( int i=0;i<10; i++ )
//        		{
//          		trackstar.dero_correct[i]=trackstar.dero_correct[9]+ ( i+1 ) *de1taErrDero/10;
// 			if ( trackstar.dero_correct[i]>3600 )
// 				trackstar.dero_correct[i]=3600;
// 			if ( trackstar.dero_correct[i]<-3600 )
// 				trackstar.dero_correct[i]=-3600;
// 		}


// 		if(fabs(trackstar.DERO_TrackTargetSpeed)>90)						//every 2 seconds
// 		    iErrorStartDero=80; 

        }


}  


void    AzCorrect()
{  
	QString str;
        //char cmd[255];
        //double Tim=TrackPeriod*10/1000;								//1s
	double CommandError=0;


// 	CommandError=-1*(trackstar.az_offset+trackstar.az_guidedata);
	CommandError=-1.0*(trackstar.az_guidedata);
       	if(trackstar.az_guidedata!=trackstar.az_guidedata_last)
      	{
		trackstar.az_point=trackstar.az_point+(trackstar.az_guidedata-trackstar.az_guidedata_last);
		SendFloatToDPRAM(M180_addr,float(trackstar.az_point));				//trackstar.az_point: "
		trackstar.az_guidedata_last=trackstar.az_guidedata;
		de1taErrAz=0;
		iErrorStartAz=-100;
	}

        iErrorStartAz++;
	if(iErrorStartAz<100)
		return;
 
		//PID ( CommandError,trackstar.AzTrackError,P_az,I_az,D_az,&az_error_1,&ei_az,&ed_az,Tim, &de1taErrAz );
 //		PID ( CommandError,trackstar.AzTrackError,P_alt,I_alt,D_alt,&az_error_1,&ei_az,&ed_az,Tim, &de1taErrAz );
// 		PID ( CommandError, az_AvError,P_alt,I_alt,D_alt,&az_error_1,&ei_az,&ed_az,Tim, &de1taErrAz );
// 		if ( de1taErrAz>100)
// 			de1taErrAz=100;
// 		if ( de1taErrAz<-100)
// 			de1taErrAz=-100;
//  	      	qDebug()<<"CommandError"<<CommandError;
//  	      	qDebug()<<"de1taErrAz"<<de1taErrAz<<az_AvError<<trackstar.AzTrackError;
// 		
// 		for ( int i=0;i<10; i++ )
//      		{
// 			trackstar.az_correct[i]=trackstar.az_correct[9]+ ( i+1 ) *de1taErrAz/10;
// 			if ( trackstar.az_correct[i]>3600 )
// 				trackstar.az_correct[i]=3600;
// 			if ( trackstar.az_correct[i]<-3600 )
// 				trackstar.az_correct[i]=-3600;
// 		}
		//de1taErrAz=0;

    	 
//  		if(fabs(trackstar.AZ_TrackTargetSpeed)>60)						//every 1 second
// 		{
// 
//   		//	if(fabs(az_AvError)>10)
// 	//		{
// 				ToffsetAz=az_AvError/trackstar.AZ_TrackTargetSpeed;
//  				//if(fabs(ToffsetAz)>0.1)
//                 	  	//	ToffsetAz=0.1*sgn(ToffsetAz);
// 	//		}
// //   			else if(fabs(az_AvError)>10&&fabs(az_AvError)<100)
// // 			{
// // 				if(fabs(az_AvError)>3.0)
// //         				ToffsetAz=3.0*sgn(az_AvError)/trackstar.AZ_TrackTargetSpeed;
// // 		 
// // 
// //  			}
// /*
//   			else if(fabs(az_AvError)>2&&fabs(az_AvError)<=10)
// 			{
// 
// 				ToffsetAz=2*sgn(az_AvError)/trackstar.AZ_TrackTargetSpeed;
// 			}
// 			else
// 			    ToffsetAz=az_AvError/trackstar.AZ_TrackTargetSpeed;*/
// 
//  				
// 
//  		}
       		
	
        /*if(fabs(trackstar.AZ_TrackTargetSpeed)<=60)
		{	
			ToffsetAz=az_AvError/trackstar.AZ_TrackTargetSpeed;
			if(ToffsetAz>0.01)
                	  	ToffsetAz=0.01;
			if(ToffsetAz<-0.01)
                  		ToffsetAz=-0.01;
 
		}
		else
			ToffsetAz=az_AvError/trackstar.AZ_TrackTargetSpeed;


                 if(fabs(az_AvError)<5)
		{
                    ToffsetAz=0.002*sgn(az_AvError);
		}
		if(fabs(az_AvError)<2)
		{
                    ToffsetAz=0.001*sgn(az_AvError);
		}

        trackstar.AzCorrectLines=trackstar.AzCorrectLines+int(ToffsetAz*TargetNumPerSecond);*/
// 		float error=(ToffsetAz*TargetNumPerSecond-int(ToffsetAz*TargetNumPerSecond))*TableLineTime/1000.*trackstar.AZ_TrackTargetSpeed;
// 		float fData[10];
// 		int iDatalength=1;
//          	uint16_t offset=M180_addr;
// 		trackstar.az_point=trackstar.az_point+error;
// 		fData[0]=trackstar.az_point;
//   		SQUMAC::instance_p()->QsendDataDPRAM (fData,  offset, iDatalength);

		iErrorStartAz=-100;
//		qDebug()<<"ToffsetAz az correct"<<ToffsetAz<<az_AvError<<int(ToffsetAz*TargetNumPerSecond);     
// 		for ( int i=0;i<10; i++ )
// 			//trackstar.az_correct[i]=trackstar.az_correct[9]+ ( i+1 ) *trackstar.AzCorrectLines/10;
// 			trackstar.az_correct[i]=trackstar.az_correct[9]+int(ToffsetAz*TargetNumPerSecond);
// 		qDebug()<<trackstar.az_correct[9];

 
// 		trackstar.az_point=trackstar.az_point+de1taErrAz/3600 ;
// 		sprintf ( cmd,"P180=%9.6f",trackstar.az_point);
// 		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// 		iErrorStartAz=0;
// 		qDebug()<<"trackstar.az_point"<<trackstar.az_point;
       


}

/*

void    DeroCorrect()
{
	QString str;
	char cmd[255];
	double Tim=TrackPeriod*10/1000;								//1s
	double CommandError=0;
  	double ToffsetDero=0;

	CommandError=-1*(trackstar.dero_guidedata);
       	if(trackstar.dero_guidedata!=trackstar.dero_guidedata_last)
      	{
		trackstar.dero_point=trackstar.dero_point+(trackstar.dero_guidedata-trackstar.dero_guidedata_last);
// 		sprintf ( cmd,"P480=%9.6f",trackstar.dero_point);
// 		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
		SendFloatToDPRAM(M480_addr,float(trackstar.dero_point));				//trackstar.dero_point: "
		trackstar.dero_guidedata_last=trackstar.dero_guidedata;
		de1taErrDero=0;
		iErrorStartDero=-100;

	}

	iErrorStartDero++;
	if(iErrorStartDero > 100)
	{
    		ToffsetDero=dero_AvError/trackstar.DERO_TrackTargetSpeed;

		if(fabs(trackstar.DERO_TrackTargetSpeed)<=60)
		{	
			ToffsetDero=dero_AvError/trackstar.DERO_TrackTargetSpeed;
			if(ToffsetDero>0.01)
                	  	ToffsetDero=0.01;
			if(ToffsetDero<-0.01)
                  		ToffsetDero=-0.01;
 
		}
		else
			ToffsetDero=dero_AvError/trackstar.DERO_TrackTargetSpeed;


                 if(fabs(dero_AvError)<5)
		{
                    ToffsetDero=0.002*sgn(dero_AvError);
		}
		if(fabs(dero_AvError)<2)
		{
                    ToffsetDero=0.001*sgn(dero_AvError);
		}

		trackstar.DeroCorrectLines=trackstar.DeroCorrectLines+int(ToffsetDero*TargetNumPerSecond);
		
// 		float error=(ToffsetDero*TargetNumPerSecond-int(ToffsetDero*TargetNumPerSecond))*TableLineTime/1000.*trackstar.DERO_TrackTargetSpeed;
// 		float fData[10];
// 		int iDatalength=1;
//           	uint16_t offset=M480_addr;
// 		trackstar.dero_point=trackstar.dero_point+error;
// 		fData[0]=trackstar.dero_point;
//   		SQUMAC::instance_p()->QsendDataDPRAM (fData,  offset, iDatalength);

		de1taErrDero=0;
		iErrorStartDero=0;
//        

// 		if(ToffsetDero>0.01)
//                   ToffsetDero=0.01;
// 		if(ToffsetDero<-0.01)
//                   ToffsetDero=-0.01;
// 		qDebug()<<"ToffsetDero dero correct"<<ToffsetDero;  
 		 //qDebug()<<"ERROR dero correct"<<ToffsetDero;  

// //          	PID(CommandError,trackstar.DeroTrackError,P_dero,I_dero,D_dero,&dero_error_1,&ei_dero,&ed_dero,Tim, &de1taErrDero);	
//          	PID(CommandError,trackstar.DeroTrackError,P_alt,I_alt,D_alt,&dero_error_1,&ei_dero,&ed_dero,Tim, &de1taErrDero);	
//  	     	if(de1taErrDero>100) 
// 			de1taErrDero=100;
//                 if(de1taErrDero<=-100)
// 	        	de1taErrDero=-100;	 
//    		qDebug()<<"de1taErrDero"<<de1taErrDero<<dero_AvError<<trackstar.DeroTrackError;               
       	 	

//      
//        		for ( int i=0;i<10; i++ )
//        		{
//          		trackstar.dero_correct[i]=trackstar.dero_correct[9]+ ( i+1 ) *de1taErrDero/10;
// 			if ( trackstar.dero_correct[i]>3600 )
// 				trackstar.dero_correct[i]=3600;
// 			if ( trackstar.dero_correct[i]<-3600 )
// 				trackstar.dero_correct[i]=-3600;
// 		}


// 		if(fabs(trackstar.DERO_TrackTargetSpeed)>90)						//every 2 seconds
// 		    iErrorStartDero=80; 

        }


}  
*/


void    AltCorrect()										//every second
{
//	double Tim=TrackPeriod*10/1000;								//1s
        double CommandError=0;
        double ToffsetAlt=0;
	QString str;
        //char cmd[255];

	CommandError=-1*(trackstar.alt_guidedata);
       	if(trackstar.alt_guidedata!=trackstar.alt_guidedata_last)
      	{
		trackstar.alt_point=trackstar.alt_point+(trackstar.alt_guidedata-trackstar.alt_guidedata_last);
		SendFloatToDPRAM(M380_addr,float(trackstar.alt_point));				//trackstar.alt_point: "
		trackstar.alt_guidedata_last=trackstar.alt_guidedata;

		de1taErrAlt=0;
		iErrorStartAlt=-100;
              
	}

       // if(fabs(alt_AvError)>1)
 	//return;
	//qDebug()<<"iErrorStartAlt (AltCorrect)"<<iErrorStartAlt;    	

        iErrorStartAlt++;
	if(iErrorStartAlt > 100)
	{
 
    		ToffsetAlt=alt_AvError/trackstar.ALT_TrackTargetSpeed;
//		qDebug()<<"ToffsetAlt alt correct"<<ToffsetAlt;     
	
		
// 		float error=(ToffsetAlt*TargetNumPerSecond-int(ToffsetAlt*TargetNumPerSecond))*TableLineTime/1000.*trackstar.ALT_TrackTargetSpeed;
// 		float fData[10];
// 		int iDatalength=1;
//           	uint16_t offset=M380_addr;
// 		trackstar.alt_point=trackstar.alt_point+error;
// 	        qDebug()<<"alt_point"<<trackstar.alt_point<<error;     
// 		fData[0]=trackstar.alt_point;
//        	       	SQUMAC::instance_p()->QsendDataDPRAM (fData,  offset, iDatalength);

		if(fabs(trackstar.ALT_TrackTargetSpeed)<=60)
		{	
			ToffsetAlt=alt_AvError/trackstar.ALT_TrackTargetSpeed;
			if(ToffsetAlt>0.01)
                	  	ToffsetAlt=0.01;
			if(ToffsetAlt<-0.01)
                  		ToffsetAlt=-0.01;
 
		}
		else
			ToffsetAlt=alt_AvError/trackstar.ALT_TrackTargetSpeed;


                 if(fabs(alt_AvError)<5)
		{
                    ToffsetAlt=0.002*sgn(alt_AvError);
		}
		if(fabs(alt_AvError)<2)
		{
                    ToffsetAlt=0.001*sgn(alt_AvError);
		}

    //trackstar.AltCorrectLines=trackstar.AltCorrectLines+int(ToffsetAlt*TargetNumPerSecond);





		de1taErrAlt=0;
		iErrorStartAlt=0;

 
 
	}

}




void PID(double target,double real,double Kp,double Ki,double Kd,double *error_1,double *ei,double *ed,double Time,double *output)
{
	 double error=-1*(target-real);
    	 *ei=*ei+error*Time;
	 if(*ei>10)
             *ei=10;

         *ed=(error-*error_1)/Time;
	 *output=Kp*error+Ki*(*ei)+Kd*(*ed); 
         *error_1=error; 

}





/*
void    AltCorrect()								////every second
{
	double Tim=TrackPeriod*10/1000;								//1s
	 				
       double CommandError=0;

         if(trackstar.AltTrackError>50)
             return;
        
         //PID(CommandError,trackstar.AltTrackError,P_alt,I_alt,D_alt,errLast,&error_1,ei_alt,ed_alt,Tim, &de1taErrAlt);
	PID(CommandError,trackstar.AltTrackError,P_alt,I_alt,D_alt,&alt_error_1,&ei_alt,&ed_alt,Tim, &de1taErrAlt);
// 	 error=-1*(CommandError-trackstar.AltTrackError);
//     	 ei=ei+error*Tim;
//          ed=(error-error_1)/Tim;
// 	 de1taErrAlt=P_alt*error+I_alt*ei+D_alt*ed; 
//          error_1=error; 

//         if(fabs(alt_AvError)>1)
// 	{
//           if(de1taErrAlt>=50.) 
// 		de1taErrAlt=50.;
//           if(de1taErrAlt<=-50.)
// 	        de1taErrAlt=-50.;
// 	}
//         else
//         {
// 	    if(de1taErrAlt>3) 
// 		de1taErrAlt=3;
//             if(de1taErrAlt<=-3)
// 	        de1taErrAlt=-3;
//         
//         }
	
  
        
	    if(de1taErrAlt>1) 
		de1taErrAlt=1;
            if(de1taErrAlt<=-1)
	        de1taErrAlt=-1;
        
         

//     qDebug()<<"de1taErrAlt"<<de1taErrAlt<<alt_AvError<<trackstar.AltTrackError;

      
        for ( int i=0;i<10; i++ )
       {
 	//trackstar.alt_correct[i]=trackstar.alt_correct[9]+ ( i+1 ) *de1taErrAlt/10*TrackPeriod/1000;
         trackstar.alt_correct[i]=trackstar.alt_correct[9]+ ( i+1 ) *de1taErrAlt/10;
	}
      
      

// 	iErrorStartAlt++;
// 	if ( iErrorStartAlt >=10 )
//         {
//  //         	de1taErrAlt=alt_AvError-alt_AvError_last;					//alt_AvError_last:100s before
// 		de1taErrAlt=alt_AvError-0;					//alt_AvError_last:100s before
// 		alt_AvError_last=alt_AvError;
// 		iErrorStartAlt=0;
// 		
//                 if(fabs(trackstar.AltTrackError)>fabs(alt_AvError))
//                 return;
// 		for ( int i=0;i<10; i++ )
// 		{
// 			if ( fabs ( alt_AvError ) >1&&fabs ( alt_AvError ) <20 )
// 				trackstar.alt_correct[i]=trackstar.alt_correct[9]+ ( i+1 ) *de1taErrAlt*TrackPeriod/1000;
// 			else if ( fabs ( alt_AvError ) >=0.1&&fabs ( alt_AvError ) <=1.0 )
// 				trackstar.alt_correct[i]=trackstar.alt_correct[9]+ ( i+1 ) *de1taErrAlt/5*TrackPeriod/1000;
// 			else if ( fabs ( alt_AvError ) <0.1 )
// 				trackstar.alt_correct[i]=trackstar.alt_correct[9]+ ( i+1 ) *de1taErrAlt/10*TrackPeriod/1000;
// 			//qDebug() << i<<trackstar.alt_correct[i];
// 		}
//  		qDebug()<<"de1taErrAlt"<<de1taErrAlt;
//          }
// 	
// 
// 	qDebug()<<"trackstar.alt_correct"<<trackstar.alt_correct<<"alt_AvError"<<alt_AvError;

}
*/









    



// void    DeroCorrectPoint()								////every second
// {
// 	double dero_AvError=0;
// 	double Tim=0.1;								//1s
// 	 				
//         double CommandError=0;
//         
//  
//          if(trackstar.DeroTrackError>500)
//               return;
// 
// 	Dero_dError[iErrorDero]=trackstar.DeroTrackError;
// 	iErrorDero++;
// 	if ( iErrorDero==10 ) iErrorDero=0;
// 
// 	for ( int ti=0; ti<10; ti++ )
// 	{
// 		dero_AvError+=Dero_dError[ti];
// 	}
// 	dero_AvError/=10;
// 
// 	if(fabs(dero_AvError)<0.3)
//         {
// 		trackstar.flagDeroCorrectPoint=false;
// 		dero_error_1=0;
// 		ei_dero=0;
// 		ed_dero=0;
// 		qDebug()<<"flagDeroCorrectPoint=false";
// 	}
// 
// 
//         
// 	PID(CommandError,trackstar.DeroTrackError,P_dero,I_dero,D_dero,&dero_error_1,&ei_dero,&ed_dero,Tim, &de1taErrDero);
//  
// 
//         if(fabs(dero_AvError)>1)
// 	{
//           if(de1taErrDero>=10.) 
// 		de1taErrDero=10.;
//           if(de1taErrDero<=-10.)
// 	        de1taErrDero=-10.;
// 	}
//         else
//         {
// 
//      	 if(de1taErrDero>3) 
// 		de1taErrDero=3;
//          if(de1taErrDero<=-3)
// 	        de1taErrDero=-3;
//         
//         }
// 	
//         
// 	trackstar.dero_point=trackstar.dero_point+de1taErrDero/3600.0;
// 
//     	qDebug()<<"de1taErrDero"<<de1taErrDero<<dero_AvError<<trackstar.DeroTrackError;
// 	
// 	QString str;
// 	char cmd[255];
// 	sprintf ( cmd,"P380=%9.6f",trackstar.dero_point);
// 	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// 
// }

// void    AzCorrectPoint()								////every second
// {
// 	double az_AvError=0;
// 	double Tim=0.1;								//1s
// 	 				
//         double CommandError=0;
//         
//  
//          if(trackstar.AzTrackError>100)
//               return;
// 
// 	Az_dError[iErrorAz]=trackstar.AzTrackError;
// 	iErrorAz++;
// 	if ( iErrorAz==10 ) iErrorAz=0;
// 
// 	for ( int ti=0; ti<10; ti++ )
// 	{
// 		az_AvError+=Az_dError[ti];
// 	}
// 	az_AvError/=10;
// 
// 	if(fabs(az_AvError)<0.3)
//         {
// 		trackstar.flagAzCorrectPoint=false;
// 		az_error_1=0;
// 		ei_az=0;
// 		ed_az=0;
// 		qDebug()<<"flagAzCorrectPoint=false";
// 	}
// 
// 
//         
// 	PID(CommandError,trackstar.AzTrackError,P_az,I_az,D_az,&az_error_1,&ei_az,&ed_az,Tim, &de1taErrAz);
//  
// 
//         if(fabs(az_AvError)>1)
// 	{
//           if(de1taErrAz>=10.) 
// 		de1taErrAz=10.;
//           if(de1taErrAz<=-10.)
// 	        de1taErrAz=-10.;
// 	}
//         else
//         {
// 
//      	 if(de1taErrAz>3) 
// 		de1taErrAz=3;
//          if(de1taErrAz<=-3)
// 	        de1taErrAz=-3;
//         
//         }
// 	
//         
// 	trackstar.az_point=trackstar.az_point+de1taErrAz/3600.0;
// 
//     	qDebug()<<"de1taErrAz"<<de1taErrAz<<az_AvError<<trackstar.AzTrackError;
// 	
// 	QString str;
// 	char cmd[255];
// 	sprintf ( cmd,"P380=%9.6f",trackstar.az_point);
// 	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// 
// }
// void    AltCorrectPoint()								////every second
// {
// 	double alt_AvError=0;
// 	double Tim=0.1;								//1s
// 	 				
//         double CommandError=0;
//         
//  
//          if(trackstar.AltTrackError>500)
//               return;
// 
// 	Alt_dError[iErrorAlt]=trackstar.AltTrackError;
// 	iErrorAlt++;
// 	if ( iErrorAlt==10 ) iErrorAlt=0;
// 
// 	for ( int ti=0; ti<10; ti++ )
// 	{
// 		alt_AvError+=Alt_dError[ti];
// 	}
// 	alt_AvError/=10;
// 
// // 	if(fabs(alt_AvError)<0.3)
// //         {
// // 		trackstar.flagAltCorrectPoint=false;
// // 		alt_error_1=0;
// // 		ei_alt=0;
// // 		ed_alt=0;
// // 		qDebug()<<"flagAltCorrectPoint=false";
// // 	}
// 
// 
//         iErrorStartAlt++;
// 	if(iErrorStartAlt > 50)
// 	{
//           	de1taErrAlt=alt_AvError;
// 		trackstar.flagAltCorrectPoint=false;
// 		trackstar.alt_point=trackstar.alt_point+de1taErrAlt/3600.0;
// 
//     		qDebug()<<"de1taErrAlt"<<de1taErrAlt<<alt_AvError<<trackstar.AltTrackError;
// 	
// 		QString str;
// 		char cmd[255];
// 		sprintf ( cmd,"P380=%9.6f",trackstar.alt_point);
// 		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// 
// 		qDebug()<<"flagAltCorrectPoint=false";
// 		de1taErrAlt=0;
//         }
//         
// // 	PID(CommandError,trackstar.AltTrackError,P_alt,I_alt,D_alt,&alt_error_1,&ei_alt,&ed_alt,Tim, &de1taErrAlt);
// //  
// // 
// //         if(fabs(alt_AvError)>1)
// // 	{
// //           if(de1taErrAlt>=10.) 
// // 		de1taErrAlt=10.;
// //           if(de1taErrAlt<=-10.)
// // 	        de1taErrAlt=-10.;
// // 	}
// //         else
// //         {
// // 
// //      	 if(de1taErrAlt>3) 
// // 		de1taErrAlt=3;
// //          if(de1taErrAlt<=-3)
// // 	        de1taErrAlt=-3;
// //         
// //         }
// 	
//         
// 
// 
// }








void Tracking::StartTrackingThread()
{
	QString str;

 // 	  	SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopAzMotion1,str );				//stop UMAC motion1 
 //	  	SQUMAC::instance_p()->QcommCmdPMAC ( CmdKillAzMotion1,str );				//stop UMAC motion1 	 
// 	 	 
 //		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopAltMotion2,str );				//stop UMAC motion2
// 	 	 
// 		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopC1Motion3,str );					//stop UMAC motion3
// 	 	 
 // 		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopN2Motion5,str );
// 	 	 
// 		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopN1Motion7,str );


     flagtcs.flag_tracking=true;
	//AZ   TRACKING
	if ( flagtcs.flag_AZIMUTH )
	{
   		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopAzMotion1,str );				//stop UMAC motion1
//   		sleep ( 2 );
		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStartAzMotion1,str );				// start UMAC motion1
        //flagtcs.flag_tracking=true;
//		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStartAzPointing,str );	
        //Az_beginPoint();
	}
	//ALT   TRACKING
	if ( flagtcs.flag_ALTITUDE )
	{
		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopAltMotion2,str );				//stop UMAC motion2
// 		sleep ( 1 );
		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStartAltMotion2,str );				// start UMAC motion2
        //flagtcs.flag_tracking=true;
	//	qDebug() <<"CmdStartAltMotion2"<<CmdStartAltMotion2;
//	        SQUMAC::instance_p()->QcommCmdPMAC ( CmdStartAltPointing,str );	
         //Alt_beginPoint();
	}
	//derotator tracking
    if ( flagtcs.flag_DERO )
	{
                SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopDeroMotion3,str );					//stop UMAC motion3
// 		sleep ( 1 );
                SQUMAC::instance_p()->QcommCmdPMAC ( CmdStartDeroMotion3,str );					//run UMAC motion3
        //flagtcs.flag_tracking=true;
	//	qDebug() <<"CmdStartC1Motion3"<<CmdStartC1Motion3;
	//	SQUMAC::instance_p()->QcommCmdPMAC ( CmdStartDeroPointing,str );
        //Dero_beginPoint();
	}
     /*   else if ( flagtcs.flag_ADC2 ||  flagtcs.flag_ADC1)
	{
                SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopADCMotion5,str );
// 		sleep ( 2 );
                SQUMAC::instance_p()->QcommCmdPMAC ( CmdStartADCMotion5,str );
		flagtcs.flag_tracking=true;
	//	SQUMAC::instance_p()->QcommCmdPMAC ( CmdStartDeroPointing,str );
		Dero_beginPoint();
    }*/
        /*else if ( flagtcs.flag_N1 )
	{
 		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStopN1Motion7,str );
// 		sleep ( 1 );
		SQUMAC::instance_p()->QcommCmdPMAC ( CmdStartN1Motion7,str );
		flagtcs.flag_tracking=true;
	//	SQUMAC::instance_p()->QcommCmdPMAC ( CmdStartDeroPointing,str );
		Dero_beginPoint();	
        }*/


	StartPosixTimer();
	/*
		//track_thread->start(QThread::TimeCriticalPriority);
		track_thread->start();*/
     

}
void Tracking::on_pushButton_enableAll_clicked()
{


//  	AzAlt_disable();
// 	AzAlt_enable();
// // 	readStarTable();
	QString str;
//	char cmd[255];

    if(!flagtcs.AxisEnabled)
    {
        if ( flagtcs.flag_AZIMUTH ) SQUMAC::instance_p()->QcommCmdPMAC ( CmdAzEnable,str );

        if ( flagtcs.flag_ALTITUDE ) SQUMAC::instance_p()->QcommCmdPMAC ( CmdAltEnable,str );

        if ( flagtcs.flag_DERO )  SQUMAC::instance_p()->QcommCmdPMAC ( CmdDeroEnable,str );

        if ( flagtcs.flag_ADC2 )  SQUMAC::instance_p()->QcommCmdPMAC ( CmdADC2Enable,str );

        if ( flagtcs.flag_ADC1 )  SQUMAC::instance_p()->QcommCmdPMAC ( CmdADC1Enable,str );

        pushButton_enableAll->setText ( "Disable" );
        flagtcs.AxisEnabled=true;
    }
    else
    {
        if ( flagtcs.flag_AZIMUTH ) SQUMAC::instance_p()->QcommCmdPMAC ( CmdKillAz,str );

        if ( flagtcs.flag_ALTITUDE ) SQUMAC::instance_p()->QcommCmdPMAC ( CmdKillAlt,str );

        if ( flagtcs.flag_DERO )  SQUMAC::instance_p()->QcommCmdPMAC ( CmdKillDero,str );

        if ( flagtcs.flag_ADC2 )  SQUMAC::instance_p()->QcommCmdPMAC ( CmdKillADC2,str );

        if ( flagtcs.flag_ADC1 )  SQUMAC::instance_p()->QcommCmdPMAC ( CmdKillADC1,str );

        pushButton_enableAll->setText ( "Enable" );
        flagtcs.AxisEnabled=false;
    }


}
void  setPIDparam_Az_tracking()
{
	//AZ
	QString str;
	SQUMAC::instance_p()->QcommCmdPMAC ( AzTrackParam,str );				 

}
void  setPIDparam_Az_point()
{
	//AZ
	QString str;
	SQUMAC::instance_p()->QcommCmdPMAC ( AzPointParam,str );				// with load and roller,  no filter,

}
void  setPIDparam_Alt_tracking(int m)
{
	//ALT
	QString str;
    if(m==0)SQUMAC::instance_p()->QcommCmdPMAC ( AltTrackParam,str );				//  tracking  ,with load,only ALT1
    else SQUMAC::instance_p()->QcommCmdPMAC ( AltTrackParam2,str );
    if(m==0)alt_p=0;
    else alt_p=1;
}
void  setPIDparam_Alt_point()
{
	//ALT
	QString str;
        SQUMAC::instance_p()->QcommCmdPMAC ( AltPointParam,str );				// UMAC parameters for going ,with load,only ALT1
        //SQUMAC::instance_p()->QcommCmdPMAC ( Alt2PointParam,str );				// ALT2 motor,speed: I322=50=750"/s
}

void  setPIDparam_C1_tracking()
{
	//C1
	QString str;
        SQUMAC::instance_p()->QcommCmdPMAC ( DeroTrackParam,str );
       // SQUMAC::instance_p()->QcommCmdPMAC ( C1GoParam_2,str );
}
void  setPIDparam_C1_pointing()
{ 
	QString str;
        SQUMAC::instance_p()->QcommCmdPMAC ( DeroPointParam,str );
        //SQUMAC::instance_p()->QcommCmdPMAC ( C1GoParamFast_2,str );
}

void  setPIDparam_N2_tracking()
{
	//C1
	QString str;
        SQUMAC::instance_p()->QcommCmdPMAC ( ADC2TrackParam,str );
        //SQUMAC::instance_p()->QcommCmdPMAC ( N2GoParam_2,str );
}
void  setPIDparam_N2_pointing()
{ 
	QString str;
        SQUMAC::instance_p()->QcommCmdPMAC ( ADC2PointParam,str );
        //SQUMAC::instance_p()->QcommCmdPMAC ( N2GoParamFast_2,str );
}


void  setPIDparam_N1_tracking()
{
	//C1
	QString str;
        SQUMAC::instance_p()->QcommCmdPMAC ( ADC1TrackParam,str );
        //SQUMAC::instance_p()->QcommCmdPMAC ( N1GoParam_2,str );
}
void  setPIDparam_N1_pointing()
{ 
	QString str;
        SQUMAC::instance_p()->QcommCmdPMAC ( ADC1PointParam,str );
        //SQUMAC::instance_p()->QcommCmdPMAC ( N1GoParamFast_2,str );
}

void Tracking::on_pushButton_save_clicked()
{

	if ( !save )
	{
		save=true;
		pushButton_save->setText ( "Loging" );
        //Save_label();
        iniLog();
		count=0;
	}
	else
	{
		save=false;
//		timer_save_track_data->stop();
		pushButton_save->setText ( "Log" );
	}

}






void Tracking::on_pushButton_AzOffset_clicked()
{
    double AltOffset, AzOffset, Aznew, Altnew, cosalt, deroalt,cosa,sina;

    AzOffset=lineEdit_az_offset->text().toDouble();
    //char s[255];
    if(fabs(AzOffset)>4000)
	{
		QMessageBox::warning ( this,
                         tr ( "Input illegal" ),
                         tr ( ("Input is limited -4000''~ +4000''") ),
		                       QMessageBox::Yes );
		return;
	}				 
    if(!flagtcs.OffsetPixelUnit)trackstar.az_offset= AzOffset;
    else
    {
        AltOffset=lineEdit_alt_offset->text().toDouble()-OpticAxis_Y;
        AzOffset-=OpticAxis_X;//05/17, get from long time star tracking
        //deroalt=degrad(trackstar.DeroPosRealForError*2+trackstar.AltPosRealForError);
        deroalt=degrad(90-trackstar.AltPosRealForError-0.845);
        cosa= cos(deroalt);
        sina= sin(deroalt);
        cosalt= cos(degrad(trackstar.AltPosRealForError));
        Aznew= AzOffset*cosa - AltOffset*sina;
        Altnew= AzOffset*sina + AltOffset*cosa;
        trackstar.az_offset= -Aznew*Pixel_Second/cosalt;
        trackstar.alt_offset= -Altnew*Pixel_Second;
        qDebug() <<"Alt pos:"<<trackstar.AltPosRealForError
                <<"deroalt"<<raddeg(deroalt);
        qDebug() <<"x offset:"<< AzOffset<<"x new:"<< Aznew
                   <<"y offset:"<< AltOffset<<"y new:"<< Altnew;
        qDebug() <<"Az_offset:"<<trackstar.az_offset<<"alt_offset:"<<trackstar.alt_offset;
        /*AltOffset=lineEdit_alt_offset->text().toDouble()-2003;
        AzOffset=lineEdit_az_offset->text().toDouble()-2226;//05/17, get from long time star tracking
        cosalt= cos(degrad(trackstar.AltPosRealForError));
        sinalt= sin(degrad(trackstar.AltPosRealForError));
        Aznew= AzOffset*cosalt + AltOffset*sinalt;
        Altnew= -AzOffset*sinalt + AltOffset*cosalt;
        trackstar.az_offset= -Aznew*Pixel_Second/cosalt;
        trackstar.alt_offset= -Altnew*Pixel_Second;*/
    }
    //trackstar.az_guidedata=trackstar.az_guidedata+AzOffset;
    //sprintf ( s,"%5.2f",trackstar.az_offset_all );
    //label_Az_Offset->setText ( s );

}
void Tracking::on_pushButton_AltOffset_clicked()
{
    double AltOffset, AzOffset, Aznew, Altnew, cosalt, deroalt,cosa,sina;
    AltOffset=lineEdit_alt_offset->text().toDouble();
    //char s[255];
    if(fabs(AltOffset)>4000)
	{
		QMessageBox::warning ( this,
		                 tr ( "Input illegal" ),
                         tr ( ("Input is limited -4000''~ +4000''") ),
		                       QMessageBox::Yes );
		return;
	}				 

    if(!flagtcs.OffsetPixelUnit)trackstar.alt_offset= AltOffset;
    else
    {
        AltOffset-=OpticAxis_Y;
        AzOffset=lineEdit_alt_offset->text().toDouble()-OpticAxis_X;//05/17, get from long time star tracking
        deroalt=degrad(90-trackstar.AltPosRealForError-0.845);//trackstar.DeroPosRealForError*2+
        cosa= cos(deroalt);
        sina= sin(deroalt);
        cosalt= cos(degrad(trackstar.AltPosRealForError));
        Aznew= AzOffset*cosa - AltOffset*sina;
        Altnew= AzOffset*sina + AltOffset*cosa;
        trackstar.az_offset= -Aznew*Pixel_Second/cosalt;
        trackstar.alt_offset= -Altnew*Pixel_Second;
        qDebug() <<"Alt pos:"<<trackstar.AltPosRealForError
                <<"deroalt"<<raddeg(deroalt);
        qDebug() <<"x offset:"<< AzOffset<<"x new:"<< Aznew
                   <<"y offset:"<< AltOffset<<"y new:"<< Altnew;
        qDebug() <<"Az_offset:"<<trackstar.az_offset<<"alt_offset:"<<trackstar.alt_offset;
        /*
        Aznew= -AzOffset*cosa + AltOffset*sina;
        Altnew= AzOffset*sina + AltOffset*cosa;
        trackstar.az_offset= -Aznew*Pixel_Second/cosalt;
        trackstar.alt_offset= -Altnew*Pixel_Second;*/
    }
    //trackstar.alt_guidedata=trackstar.alt_guidedata+AltOffset;
   // sprintf ( s,"%5.2f″",trackstar.alt_offset_all );
    //label_Alt_Offset->setText ( s );
}

void Tracking::on_pushButton_DeroOffset_clicked()
{
	double DeroOffset=lineEdit_dero_offset->text().toDouble();		
    //char s[255];
    if(fabs(DeroOffset)>4000)
	{
		QMessageBox::warning ( this,
		                 tr ( "Input illegal" ),
                         tr ( ("Input is limited -4000''~ +4000''") ),
		                       QMessageBox::Yes );
		return;
	}				 



    trackstar.dero_offset= DeroOffset;
    //trackstar.dero_guidedata=trackstar.dero_guidedata+DeroOffset;
    //sprintf ( s,"%5.2f″",trackstar.dero_offset_all );
    //label_Dero_Offset->setText ( s );
}

bool Tracking::getTrackData()
{

	QDateTime dt=QDateTime::currentDateTime();
	QString dtstr;
	dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );
	QTime time=dt.time();
	secondsAz=time.second() *1000+time.msec();
//	qDebug()<<secondsAz;
	if ( fabs ( secondsAz-secondsAz_old ) >=1000 ) 							//save data per 1 second
	{
		secondsAz_old=secondsAz;

		if ( count <= M-1 )
		{
//			lock.lockForRead();
			AzData_tracking[count][0]=trackstar.AzPosRealForError;			//real position
			AzData_tracking[count][1]=trackstar.AzTarget;				//AZ target
			AzData_tracking[count][2]=trackstar.AzTrackError;  			// AZ_TrackError
			AzData_tracking[count][3]=trackstar.AzFolErrUMAC;			//following error
			AzData_tracking[count][4]=trackstar.az_guidedata;			//
			AzDataTime_tracking[count]=dtstr;

			AltData_tracking[count][0]=trackstar.AltPosRealForError;		//real position
			AltData_tracking[count][1]=trackstar.AltTarget;				//ALT_TrackTarget
			AltData_tracking[count][2]=trackstar.AltTrackError;			//ALT_TrackError
			AltData_tracking[count][3]=trackstar.AltFolErrUMAC; 			//following error
			AltData_tracking[count][4]=trackstar.alt_guidedata;
			AltDataTime_tracking[count]=dtstr;

			DeroData_tracking[count][0]=trackstar.DeroPosRealForError;		//real position
			DeroData_tracking[count][1]=trackstar.DeroTarget;			 
			DeroData_tracking[count][2]=trackstar.DeroTrackError;			//ALT_TrackError
			DeroData_tracking[count][3]=trackstar.DeroFolErrUMAC; 			//following error
			DeroData_tracking[count][4]=trackstar.dero_guidedata;
			DeroDataTime_tracking[count]=dtstr;
//			lock.unlock();
			count++;
			return true;
		}

	}
	return true;
}



/*  function: save data to a file
//input:
 int num       //matrix Az.AzData rows,#define N 300
double ( *p ) [5]:   save data, Az.AzData[N][5] ,AltData[N][5]
QString DataTime[]:save time
QString FileName: file name

*/

bool Tracking::DataSave ( double ( *p ) [6],QString DataTime[],QString FileName,int num )
{
	char s_0[400] ;
	char s_1[60] ;
	char s_2[60];
	char s_3[60];
	char s_4[60];
//	int i,j;
	double t_end,t_start,t;

	QStringList  list1 = DataTime[num-1].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );

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
	if ( t<=0 )
		t=30;										//30	seconds
	////////////following error/////////////////////////////////////////
	double speed= ( * ( * ( p+num-1 ) +1 ) - * ( * ( p+0 ) +1 ) ) *3600 /t;			//Data[num-1][1]-Data[0][1]   get speed

// 	//get RMS of tracking error
// 	double Xi=0,Yi=0;
// 	for ( j=0;j<num;j++ ) 									//  num:   matrix Az.AzData rows
// 		Xi= * ( * ( p+j ) +2 ) +Xi;							//Data[j][2]
// 
// 	Xi=Xi/num;
// 
// 	for ( j=0;j<num;j++ )
// 		Yi= ( * ( * ( p+j ) +2 )-Xi ) * ( * ( * ( p+j ) +2 )-Xi ) +Yi;			//Data[j][2]
// 	if ( num>=1 )
// 		Yi=sqrt ( Yi/num );
// 	else
// 		Yi=0;
// 
// 	//PV
// 	double min,max;
// 	double PV;
// 	max=min=* ( * ( p+0 ) +2 );								//Data[0][2]
// 
// 	for ( j=0;j<num;j++ )
// 	{
// 		if ( max<=* ( * ( p+j ) +2 ) )     						//Data[j][2]
// 		{
// 			max=* ( * ( p+j ) +2 );             					//Data[j][2]
// 		}
// 		if ( min>* ( * ( p+j ) +2 ) )							//Data[j][2]
// 		{
// 			min=* ( * ( p+j ) +2 );							//Data[j][2]
// 
// 		}
// 	}
// 	PV=max-min;

        double Data[num];
	 int i=0;
      	for( i=0;i<num;i++)
        	Data[i]=* ( * ( p+i ) +2 ) ;

	double PvTrack,RmsTrack,maxTrack,minTrack;
	AnalyzeData( Data ,num,&PvTrack,&RmsTrack,&maxTrack,&minTrack);

      	for( i=0;i<num;i++)
        	Data[i]=* ( * ( p+i ) +3 ) ;

	double PvFol,RmsFol,maxFol,minFol;
	AnalyzeData( Data ,num,&PvFol,&RmsFol,&maxFol,&minFol);
/*


	//get RMS of tracking error
	double Xi_1=0,Yi_1=0;
	for ( j=0;j<num;j++ ) 									//  num:   matrix Az.AzData rows
		Xi_1= * ( * ( p+j ) +3 ) +Xi_1;							//Data[j][2]

	Xi_1=Xi_1/num;

	for ( j=0;j<num;j++ )
		Yi_1= ( * ( * ( p+j ) +3 )-Xi_1 ) * ( * ( * ( p+j ) +3 )-Xi_1 ) +Yi_1;			//Data[j][2]
	if ( num>=1 )
		Yi_1=sqrt ( Yi_1/num );
	else
		Yi_1=0;

	//PV
	double min_1,max_1;
	double PV_UMAC;
	max_1=min_1=* ( * ( p+0 ) +3 );								//Data[0][2]

	for ( j=0;j<num;j++ )
	{
		if ( max_1<=* ( * ( p+j ) +3 ) )     						//Data[j][2]
		{
			max_1=* ( * ( p+j ) +3 );             					//Data[j][2]
		}
		if ( min_1>* ( * ( p+j ) +3 ) )							//Data[j][2]
		{
			min_1=* ( * ( p+j ) +3 );							//Data[j][2]

		}
	}
	PV_UMAC=max_1-min_1;*/

	////////////// save file////////////////////
	QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "MM_dd_yyyy" );
	QString fileName =SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString() +FileName+dtstr+".txt";
	QFile file ( fileName );
	if ( !file.open ( QFile::Append|QFile::Truncate ) )
	{
		QMessageBox::warning ( this,
		                       tr ( "Warn" ),
		                       tr ( "open error!" ),
		                       QMessageBox::Yes );
		return false;
	}
	QTextStream out ( &file );

	for ( i=0;i<num;i++ )
	{
		out<<"\n";
		sprintf ( s_1," %11.7f",* ( * ( p+i ) +1 ) );				////TARGET（UMAC,"）     Data[i][0]
		sprintf ( s_2," %11.7f ",* ( * ( p+i ) +0 ) );				//real position,"          Data[i][1],PosReal_Degree_forError
		sprintf ( s_3," %8.4f",* ( * ( p+i ) +2 ) );				//track error,"  ,   Data[i][2]ALT_TrackTarget-altPosReal_Degree
		//sprintf ( s_4," %8.4f",* ( * ( p+i ) +3 ) );				//following error,"  ,  UMAC  Data[i][3]
		sprintf ( s_4," %11.7f",* ( * ( p+i ) +3 ) );				//real position
		sprintf ( s_0,"%s%s%s%s ",s_1,s_2,s_3,s_4 );
		QString string = QString ( QLatin1String ( s_0 ) ) + DataTime[i];
		out<<string;
	}

	sprintf ( s_1," V=  %6.4f''/s,",speed );
	sprintf ( s_2,"RMS=  %6.4f'' ",RmsTrack );
	sprintf ( s_3,"PV=  %6.4f'' ",PvTrack );
	sprintf ( s_0,"%s%s%s ",s_1,s_2,s_3 );
	out<<tr ( s_0 );
	sprintf ( s_1," max=  %6.4f'',",maxTrack );
	sprintf ( s_2," min=  %6.4f''",minTrack );
	out<<tr ( s_1 );
	out<<tr ( s_2 );

        sprintf ( s_2,"RMS_UMAC=  %6.4f'' ",RmsFol );
	sprintf ( s_3,"PV_UMAC=  %6.4f'' ",PvFol );
	sprintf ( s_0,"%s%s ",s_2,s_3 );
	out<<tr ( s_0 );
	sprintf ( s_1," max=  %6.4f'',",maxFol );
	sprintf ( s_2," min=  %6.4f''",minFol );
	out<<tr ( s_1 );
	out<<tr ( s_2);
	file.close();
	return true;

}


void Tracking::CalAndShowRms()
{
	int i=0;
    //int num=SampleTime*TrackSaveLinesPerSecond;
    double DataAz[M],DataAlt[M],DataDero[M];
    //char s[255];
    //double RmsAll=0;
 
	int n=count-SampleTime*TrackSaveLinesPerSecond;
//	qDebug()<<"SampleTime"<<SampleTime<<"n"<<n<<"count"<<count;
	if(n<0)
	{
		n=n+M;
		for(i=n;i<M;i++)
		{	
			DataAz[i-n]=AzData_tracking[i][3];
                	DataAlt[i-n]=AltData_tracking[i][3];
			DataDero[i-n]=DeroData_tracking[i][3];
			sleep(0);
			QCoreApplication::processEvents(); 
 			//qDebug()<<i-n<<DataAz[i-n];
		}
		for(i=0;i<count;i++)
		{
			DataAz[i+SampleTime*TrackSaveLinesPerSecond-count]=AzData_tracking[i][3];
			DataAlt[i+SampleTime*TrackSaveLinesPerSecond-count]=AltData_tracking[i][3];
			DataDero[i+SampleTime*TrackSaveLinesPerSecond-count]=DeroData_tracking[i][3];
			sleep(0);
			QCoreApplication::processEvents();
			//qDebug()<<i+SampleTime*TrackSaveLinesPerSecond-count<<DataAz[i+SampleTime*TrackSaveLinesPerSecond-count];
		}
         }
         else
      	{
		for(i=0;i<SampleTime*TrackSaveLinesPerSecond;i++)
           	{
			DataAz[i]=AzData_tracking[n+i][3];
			DataAlt[i]=AltData_tracking[n+i][3];
			DataDero[i]=DeroData_tracking[n+i][3];
			sleep(0);
			QCoreApplication::processEvents();
			//qDebug()<<i<<DataAz[i];
		}
	}
        


    /*double PvTrackAz,RmsTrackAz=0,maxTrackAz,minTrackAz;
	AnalyzeData( DataAz ,num,&PvTrackAz,&RmsTrackAz,&maxTrackAz,&minTrackAz);
	
    sprintf ( s,"%5.2f",RmsTrackAz );
    label_Az_RmsError->setText(s);
	
	double PvTrackAlt,RmsTrackAlt=0,maxTrackAlt,minTrackAlt;
	AnalyzeData( DataAlt ,num,&PvTrackAlt,&RmsTrackAlt,&maxTrackAlt,&minTrackAlt);
	if(!flagtcs.flag_ALTITUDE )
		RmsTrackAlt=0;
    sprintf ( s,"%5.2f",RmsTrackAlt );
    label_Alt_RmsError->setText(s);

	double PvTrackDero,RmsTrackDero=0,maxTrackDero,minTrackDero;
	AnalyzeData( DataDero ,num,&PvTrackDero,&RmsTrackDero,&maxTrackDero,&minTrackDero);
    sprintf ( s,"%5.2f",RmsTrackDero );
    label_Dero_RmsError->setText(s);

    RmsAll=sqrt(RmsTrackAz*RmsTrackAz*cos(trackstar.AltTarget*PI/180)*cos(trackstar.AltTarget*PI/180)+RmsTrackAlt*RmsTrackAlt);
    sprintf ( s,"%5.2f",RmsAll );
    label_Total_RmsError->setText(s);*/
 
}


void Tracking::AnalyzeData(double data[],int num,double *pv,double *rms, double *max,double *min)
{	//get RMS of tracking error
	double Xi=0,Yi=0;
        int j=0;
	for(j=0;j<num;j++)
	{
		Xi=data[j]+Xi;
	}

	Xi=Xi/num;
	
	for(j=0;j<num;j++)
	{
		Yi=(data[j]-Xi)*(data[j]-Xi)+Yi;
	}
	Yi=sqrt(Yi/num);
 	*rms=Yi;
	 
	*max=*min=data[0];

	for(j=0;j<num;j++)  
	{
		if(*max<=data[j])
		{
			*max=data[j];
		}
		if(*min>data[j])
		{
			*min=data[j];
			
		} 	
	}
	*pv=*max-*min;
}




bool Tracking::DataSave_UMAC ( double ( *p ) [6],QString DataTime[],QString FileName,int num )
{
	char s_0[400] ;
	char s_1[100] ;
	char s_2[100];
	char s_3[100];
	char s_4[100];
	double t_end,t_start,t;

	QStringList  list1 = DataTime[num-1].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );

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
	if ( t<=0 )
		t=30;										//30	seconds
	////////////following error/////////////////////////////////////////
	double speed= ( * ( * ( p+num-1 ) +1 ) - * ( * ( p+0 ) +1 ) ) *3600 /t;			//Data[num-1][1]-Data[0][1]   get speed


        double Data[num];
	 int i=0;
  
     	for( i=0;i<num;i++)
        	Data[i]=* ( * ( p+i ) +3 ) ;

	double PvFol,RmsFol,maxFol,minFol;
	AnalyzeData( Data ,num,&PvFol,&RmsFol,&maxFol,&minFol);

	////////////// save file////////////////////
	QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "MM_dd_yyyy" );
	QString fileName =SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString() +FileName+dtstr+".txt";
	QFile file ( fileName );
	if ( !file.open ( QFile::Append|QFile::Truncate ) )
	{
		QMessageBox::warning ( this,
		                       tr ( "Warn" ),
		                       tr ( "open error!" ),
		                       QMessageBox::Yes );
		return false;
	}
	QTextStream out ( &file );

	for ( i=0;i<num;i++ )
	{
		out<<"\n";
		sprintf ( s_1," %10.7f ",* ( * ( p+i ) +4 ) );				////TARGET（UMAC,"）     Data[i][0]
	
		sprintf ( s_2," %10.7f ",* ( * ( p+i ) +0 ) );				//real position,"          Data[i][1],PosReal_Degree_forError
//		sprintf ( s_3," %8.4f",* ( * ( p+i ) +2 ) );				//track error,"  ,   Data[i][2]ALT_TrackTarget-altPosReal_Degree
		sprintf ( s_4," %10.4f ",* ( * ( p+i ) +3 ) );				//follow error

		sprintf ( s_0,"%s%s%s ",s_1,s_2,s_4 );
		QString string = QString ( QLatin1String ( s_0 ) ) + DataTime[i];
		out<<string;
	}

	sprintf ( s_1," V=  %6.4f''/s,",speed );
	sprintf ( s_2,"RMS=  %6.4f'' ",RmsFol );
	sprintf ( s_3,"PV=  %6.4f'' ",PvFol );
	sprintf ( s_0,"%s%s%s ",s_1,s_2,s_3 );
	out<<tr ( s_0 );
	sprintf ( s_1," max=  %6.4f'',",maxFol );
	sprintf ( s_2," min=  %6.4f''",minFol );
	out<<tr ( s_1 );
	out<<tr ( s_2 );
      
	file.close();
	return true;

}



bool Tracking::CalculateTime (QString DataTime[],int num,double *t )
{
    double t_end,t_start;

	QStringList  list1 = DataTime[num-1].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );

	int it=0;
	for ( QStringList::iterator point = list1.begin(); point != list1.end();++it, ++point )
		{}
	it=0;
	QStringList  list_time;
	if(list1.size()>=2)
           list_time=  list1[1].split ( ":" );
	for ( QStringList::iterator point = list_time.begin(); point != list_time.end();++it, ++point )
		{}

	if(list_time.size()>=3)
		t_end=list_time[0].toDouble() *3600+list_time[1].toDouble() *60+list_time[2].toDouble();
    else return false;
	//start time
	list1 = DataTime[0].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );

	it=0;
	for ( QStringList::iterator point = list1.begin(); point != list1.end();++it, ++point )
		{}
	it=0;
	if(list1.size()>=2)
	 	list_time =  list1[1].split ( ":" );
	for ( QStringList::iterator point = list_time.begin(); point != list_time.end();++it, ++point )
		{}
	if(list_time.size()>=3)
		t_start=list_time[0].toDouble() *3600+list_time[1].toDouble() *60+list_time[2].toDouble();
    else return false;
	*t=t_end-t_start;
	if ( *t<=0 )
		*t=M*TrackSaveClock/1000.;										//30	seconds

        return true;
}



bool Tracking::Save_label() 
{
    char s[400];
	QString FileName="/TrackRMS";
    QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "MM_dd_yyyy" );
	QString fileName =SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString() +FileName+dtstr+".txt";
    //FileName_TrackData=SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString() +"/TrackData"+dtstr+".txt";
	QFile file ( fileName );
	if ( !file.open ( QFile::Append|QFile::Truncate ) )
	{
        /*QMessageBox::warning ( this,
		                       tr ( "Warn" ),
		                       tr ( "open error!" ),
                               QMessageBox::Yes );*/
		return false;
	}
    QTextStream out ( &file );

        //if ( flagtcs.flag_DERO)
    sprintf(s,"\nAZ Target   Real position    Error     ALT Target  Real position  Error     Dero Target   Real position   Error     Time\n");
        //else if (flagtcs.flag_N2)
        //	sprintf(s,"\nAZ Target   Real position    Error     ALT Target  Real position  Error     N2 Target   Real position   Error     Time\n");
        //else if (flagtcs.flag_N1)
        //	sprintf(s,"\nAZ Target   Real position    Error     ALT Target  Real position  Error     N1 Target   Real position   Error     Time\n");
    out<<tr (s);
      
	file.close();

	return true;
		 	
}

bool Tracking:: iniLog()
{
    QDateTime dt=QDateTime::currentDateTime();
    QTime time=dt.time();
    QString dtstr;
    dtstr=dt.toString ( "MM_dd_yyyy" );
    FileName_TrackData=SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString() +"/TrackData"+dtstr+".txt";
    QString str;
    QFile file(FileName_TrackData);
    if ( !file.open ( QFile::Append|QFile::Truncate ) )
    {
        str="Can't open file: "+FileName_TrackData;
        label_Status->setText ( str );
        return false;
    }

    QTextStream out ( &file );
    char s[400];

    dtstr=dt.toString("hh:mm:ss.z");
    int Az_d,Az_m,Alt_d,Alt_m;
    double Az_s,Alt_s;
    D2DMS ( trackstar.ra,&Az_d,&Az_m,&Az_s );
    D2DMS ( trackstar.dec,&Alt_d,&Alt_m,&Alt_s );

    sprintf ( s,"Star RA=%2d:%2d:%5.2f DEC=%2d:%2d:%5.2f, at time",Az_d,Az_m,Az_s, Alt_d,Alt_m,Alt_s);
    out<<tr (s)<<dtstr<<"\n";
    //sprintf(s,"\nAz_Target   Az_real_position  Az_Error_in_PC  Az_Error_in_UMAC  Alt_Target  Alt_real_position  Alt_Error_in_PC  Alt_Error_in_UMAC  Dero_Target  Dero_real_position  Dero_Error_in_PC  Dero_Error_in_UMAC  ADC1_position   ADC2_position ADC_pos_diffrence Total_error\n");
    sprintf(s,"\nAz_Target   Az_real_position  Az_Error_in_PC  Az_Error_in_UMAC  Alt_Target  Alt_real_position  Alt_Error_in_PC  Alt_Error_in_UMAC\n");  //

    out<<tr (s)<<dtstr<<"\n";
    file.close();
    return true;
}

bool Tracking::DataSaveAll_UMAC ( double ( *pAz ) [6],double ( *pAlt ) [6],double ( *pDero ) [6],QString DataTime[],QString FileName,int num )
{
	char s_0Az[400];
	char s_0Alt[400];
	char s_0Dero[400];
	char s_1[100];
	char s_2[100];
	char s_3[100];
	char s_4[100];
	double t=M*TrackSaveClock/1000.;
	CalculateTime(DataTime,num,&t);
	////////////following error/////////////////////////////////////////
	double speedAz= ( * ( * ( pAz+num-1 ) +1 ) - * ( * ( pAz+0 ) +1 ) ) *3600 /t;			//Data[num-1][1]-Data[0][1]   get speed
	double speedAlt= ( * ( * ( pAlt+num-1 ) +1 ) - * ( * ( pAlt+0 ) +1 ) ) *3600 /t;			//Data[num-1][1]-Data[0][1]   get speed
	double speedDero= ( * ( * ( pDero+num-1 ) +1 ) - * ( * ( pDero+0 ) +1 ) ) *3600 /t;			//Data[num-1][1]-Data[0][1]   get speed


    double TrackingErrorAz[num],TrackingErrorAlt[num],TrackingErrorDero[num];
    int i=0;
  
    for( i=0;i<num;i++)
    {
        TrackingErrorAz[i]=* ( * ( pAz+i ) +3 ) ;
        TrackingErrorAlt[i]=* ( * ( pAlt+i ) +3 ) ;
        TrackingErrorDero[i]=* ( * ( pDero+i ) +3 ) ;
	}

	double PvFolAz,RmsFolAz,maxFolAz,minFolAz;
	AnalyzeData( TrackingErrorAz ,num,&PvFolAz,&RmsFolAz,&maxFolAz,&minFolAz);

	double PvFolAlt,RmsFolAlt,maxFolAlt,minFolAlt;
	AnalyzeData( TrackingErrorAlt ,num,&PvFolAlt,&RmsFolAlt,&maxFolAlt,&minFolAlt);
	double PvFolDero,RmsFolDero,maxFolDero,minFolDero;
	AnalyzeData( TrackingErrorDero ,num,&PvFolDero,&RmsFolDero,&maxFolDero,&minFolDero);

	////////////// save file////////////////////
    QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "MM_dd_yyyy" );
	QString fileName =SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString() +FileName+dtstr+".txt";
	QFile file ( fileName );
	if ( !file.open ( QFile::Append|QFile::Truncate ) )
	{
        QMessageBox::warning ( this,
		                       tr ( "Warn" ),
		                       tr ( "open error!" ),
		                       QMessageBox::Yes );
		return false;
	}
	QTextStream out ( &file );

	for ( i=0;i<num;i++ )
	{
		out<<"\n";
		sprintf ( s_1," %10.7f ",* ( * ( pAz+i ) +4 ) );				////TARGET（UMAC,"）     Data[i][0]
//		sprintf ( s_1," %016llx    ",TestBYTE[i] );				////TARGET（UMAC,"）     Data[i][0]

		sprintf ( s_2," %10.7f ",* ( * ( pAz+i ) +0 ) );				//real position,"          Data[i][1],PosReal_Degree_forError
		sprintf ( s_4," %10.4f ",* ( * ( pAz+i ) +3 ) );				//follow error
		sprintf ( s_0Az,"%s%s%s ",s_1,s_2,s_4 );

		sprintf ( s_1," %10.7f ",* ( * ( pAlt+i ) +4 ) );				////TARGET（UMAC,"）     Data[i][0]
		sprintf ( s_2," %10.7f ",* ( * ( pAlt+i ) +0 ) );				//real position,"          Data[i][1],PosReal_Degree_forError
		sprintf ( s_4," %10.4f ",* ( * ( pAlt+i ) +3 ) );				//follow error
		sprintf ( s_0Alt,"%s%s%s ",s_1,s_2,s_4 );

		sprintf ( s_1," %10.7f ",* ( * ( pDero+i ) +4 ) );				////TARGET（UMAC,"）     Data[i][0]
		sprintf ( s_2," %10.7f ",* ( * ( pDero+i ) +0 ) );				//real position,"          Data[i][1],PosReal_Degree_forError
		sprintf ( s_4," %10.4f ",* ( * ( pDero+i ) +3 ) );				//follow error
		sprintf ( s_0Dero,"%s%s%s ",s_1,s_2,s_4 );

		QString string = QString ( QLatin1String ( s_0Az ) )+ QString ( QLatin1String ( s_0Alt ) )+ QString ( QLatin1String ( s_0Dero ) ) + DataTime[i];
		out<<string;


	}

	sprintf ( s_1,"(AZ %6.4f''/s,",speedAz );
	sprintf ( s_2,"RMS:%6.4f'' ",RmsFolAz );
	sprintf ( s_3,"PV:%6.4f'' ",PvFolAz );
	sprintf ( s_0Az,"%s%s%s ",s_1,s_2,s_3 );
	out<<tr ( s_0Az );
	sprintf ( s_1,"[%6.4f'',",minFolAz );
	sprintf ( s_2,"%6.4f''])",maxFolAz );
	out<<tr ( s_1 );
	out<<tr ( s_2 );

	sprintf ( s_1,"(ALT %6.4f''/s,",speedAlt );
	sprintf ( s_2,"RMS:%6.4f'' ",RmsFolAlt );
	sprintf ( s_3,"PV:%6.4f'' ",PvFolAlt );
	sprintf ( s_0Alt,"%s%s%s ",s_1,s_2,s_3 );
	out<<tr ( s_0Alt );
	sprintf ( s_1,"[%6.4f'',",maxFolAlt );
	sprintf ( s_2,"%6.4f''])",minFolAlt );
	out<<tr ( s_1 );
	out<<tr ( s_2 );

	sprintf ( s_1,"(DERO %6.4f''/s,",speedDero );
	sprintf ( s_2,"RMS:%6.4f'' ",RmsFolDero );
	sprintf ( s_3,"PV:%6.4f'' ",PvFolDero );
	sprintf ( s_0Dero,"%s%s%s ",s_1,s_2,s_3 );
	out<<tr ( s_0Dero );
	sprintf ( s_1,"[%6.4f'',",maxFolDero);
	sprintf ( s_2,"%6.4f''])",minFolDero );
	out<<tr ( s_1 );
	out<<tr ( s_2 );

      
	file.close();
	return true;

}

bool Tracking::DataSaveAll_PC ( double ( *pAz ) [6],double ( *pAlt ) [6],double ( *pDero ) [6],QString DataTime[],QString FileName,int num )
{
	char s_0Az[400];
	char s_0Alt[400];
	char s_0Dero[400];
	char s_1[100];
	char s_2[100];
	char s_3[100];
	char s_4[100];
	double t=M*TrackSaveClock/1000.;
	CalculateTime(DataTime,num,&t);
	////////////following error/////////////////////////////////////////
	double speedAz= ( * ( * ( pAz+num-1 ) +1 ) - * ( * ( pAz+0 ) +1 ) ) *3600 /t;			//Data[num-1][1]-Data[0][1]   get speed
	double speedAlt= ( * ( * ( pAlt+num-1 ) +1 ) - * ( * ( pAlt+0 ) +1 ) ) *3600 /t;			//Data[num-1][1]-Data[0][1]   get speed
	double speedDero= ( * ( * ( pDero+num-1 ) +1 ) - * ( * ( pDero+0 ) +1 ) ) *3600 /t;			//Data[num-1][1]-Data[0][1]   get speed


        double TrackingErrorAz[num],TrackingErrorAlt[num],TrackingErrorDero[num];
	 int i=0;
  
     	for( i=0;i<num;i++)
        {
        	TrackingErrorAz[i]=* ( * ( pAz+i ) +2 ) ;
        	TrackingErrorAlt[i]=* ( * ( pAlt+i ) +2 ) ;
        	TrackingErrorDero[i]=* ( * ( pDero+i ) +2 ) ;
	}

	double PvFolAz,RmsFolAz,maxFolAz,minFolAz;
	AnalyzeData( TrackingErrorAz ,num,&PvFolAz,&RmsFolAz,&maxFolAz,&minFolAz);

	double PvFolAlt,RmsFolAlt,maxFolAlt,minFolAlt;
	AnalyzeData( TrackingErrorAlt ,num,&PvFolAlt,&RmsFolAlt,&maxFolAlt,&minFolAlt);
	double PvFolDero,RmsFolDero,maxFolDero,minFolDero;
	AnalyzeData( TrackingErrorDero ,num,&PvFolDero,&RmsFolDero,&maxFolDero,&minFolDero);

	////////////// save file////////////////////
	QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "MM_dd_yyyy" );
	QString fileName =SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString() +FileName+dtstr+".txt";
	QFile file ( fileName );
	if ( !file.open ( QFile::Append|QFile::Truncate ) )
	{
		QMessageBox::warning ( this,
		                       tr ( "Warn" ),
		                       tr ( "open error!" ),
		                       QMessageBox::Yes );
		return false;
	}
	QTextStream out ( &file );

	for ( i=0;i<num;i++ )
	{
		out<<"\n";
		sprintf ( s_1," %10.7f ",* ( * ( pAz+i ) +1 ) );				////TARGET（UMAC,"）     Data[i][0]
		sprintf ( s_2," %10.7f ",* ( * ( pAz+i ) +5 ) );				//real position,"          Data[i][1],PosReal_Degree_forError
		sprintf ( s_4," %10.4f ",* ( * ( pAz+i ) +2 ) );				//follow error
		sprintf ( s_0Az,"%s%s%s ",s_1,s_2,s_4 );

		sprintf ( s_1," %10.7f ",* ( * ( pAlt+i ) +1 ) );				////TARGET（UMAC,"）     Data[i][0]
		sprintf ( s_2," %10.7f ",* ( * ( pAlt+i ) +5 ) );				//real position,"          Data[i][1],PosReal_Degree_forError
		sprintf ( s_4," %10.4f ",* ( * ( pAlt+i ) +2 ) );				//follow error
		sprintf ( s_0Alt,"%s%s%s ",s_1,s_2,s_4 );

		sprintf ( s_1," %10.7f ",* ( * ( pDero+i ) +1 ) );				////TARGET（UMAC,"）     Data[i][0]
		sprintf ( s_2," %10.7f ",* ( * ( pDero+i ) +5 ) );				//real position,"          Data[i][1],PosReal_Degree_forError
		sprintf ( s_4," %10.4f ",* ( * ( pDero+i ) +2 ) );				//follow error
		sprintf ( s_0Dero,"%s%s%s ",s_1,s_2,s_4 );

		QString string = QString ( QLatin1String ( s_0Az ) )+ QString ( QLatin1String ( s_0Alt ) )+ QString ( QLatin1String ( s_0Dero ) ) + DataTime[i];
		out<<string;


	}

	sprintf ( s_1,"(AZ %6.4f''/s,",speedAz );
	sprintf ( s_2,"RMS:%6.4f'' ",RmsFolAz );
	sprintf ( s_3,"PV:%6.4f'' ",PvFolAz );
	sprintf ( s_0Az,"%s%s%s ",s_1,s_2,s_3 );
	out<<tr ( s_0Az );
	sprintf ( s_1,"[%6.4f'',",minFolAz );
	sprintf ( s_2,"%6.4f''])",maxFolAz );
	out<<tr ( s_1 );
	out<<tr ( s_2 );

	sprintf ( s_1,"(ALT %6.4f''/s,",speedAlt );
	sprintf ( s_2,"RMS:%6.4f'' ",RmsFolAlt );
	sprintf ( s_3,"PV:%6.4f'' ",PvFolAlt );
	sprintf ( s_0Alt,"%s%s%s ",s_1,s_2,s_3 );
	out<<tr ( s_0Alt );
	sprintf ( s_1,"[%6.4f'',",maxFolAlt );
	sprintf ( s_2,"%6.4f''])",minFolAlt );
	out<<tr ( s_1 );
	out<<tr ( s_2 );

	sprintf ( s_1,"(DERO %6.4f''/s,",speedDero );
	sprintf ( s_2,"RMS:%6.4f'' ",RmsFolDero );
	sprintf ( s_3,"PV:%6.4f'' ",PvFolDero );
	sprintf ( s_0Dero,"%s%s%s ",s_1,s_2,s_3 );
	out<<tr ( s_0Dero );
	sprintf ( s_1,"[%6.4f'',",maxFolDero);
	sprintf ( s_2,"%6.4f''])",minFolDero );
	out<<tr ( s_1 );
	out<<tr ( s_2 );

      
	file.close();
	return true;

}

/*
 input:	Az.AzDataTime,Global Variable
       	Az.AzData,Global Variable
*/
bool Tracking::storeAzTrackData()
{

	QString FileName="/AzTrackRMS";
	//DataSave ( AzData_tracking,AzDataTime_tracking,FileName,M );						//AzDatamm_dd_yy.txt
	DataSave_UMAC( AzData_tracking,AzDataTime_tracking,FileName,M );						//AzDatamm_dd_yy.txt
	return true;
}

bool Tracking::storeAltTrackData()
{
	//AltTrackRMSmm_dd_yy.txt
	
	QString FileName="/AltTrackRMS";
	//DataSave ( AltData_tracking,AltDataTime_tracking,FileName,M );
	DataSave_UMAC( AltData_tracking,AltDataTime_tracking,FileName,M );
	return true;
}

/*
bool Tracking::storeAzTrackData_PC()
{
	QString FileName="/AzTrackRMS_PC";
	DataSave ( AzData_tracking,AzDataTime_tracking,FileName,M );						//AzDatamm_dd_yy.txt
	return true;
}
bool Tracking::storeAltTrackData_PC()
{
	//AltTrackRMSmm_dd_yy.txt
	QString FileName="/AltTrackRMS_PC";
	DataSave ( AltData_tracking,AltDataTime_tracking,FileName,M );
	return true;
}


bool Tracking::storeDeroTrackData_PC()
{
        //DeroTrackRMSmmmm_dd_yy.txt
	QString FileName;
	if ( flagtcs.flag_C1 )
		FileName="/C1TrackRMS";
	if ( flagtcs.flag_N2 )
		FileName="/N2TrackRMS";
	if ( flagtcs.flag_N1 )
		FileName="/N1TrackRMS";

 	DataSave ( DeroData_tracking,DeroDataTime_tracking,FileName,M );
	return true;
}*/

bool Tracking::storeDeroTrackData()
{
	//**Datamm_dd_yy.txt
	QString FileName;
        //if ( flagtcs.flag_DERO )
                FileName="/DeroTrackRMS";
        /*if ( flagtcs.flag_N2 )
		FileName="/N2TrackRMS";
	if ( flagtcs.flag_N1 )
                FileName="/N1TrackRMS";*/

// 	DataSave ( DeroData_tracking,DeroDataTime_tracking,FileName,M );
	DataSave_UMAC( DeroData_tracking,DeroDataTime_tracking,FileName,M );
	return true;
}

bool Tracking::saveTrackData()
{
    /*String FileName;

    if ( count >M-1 )
	{
		count=0;
//		return storeTrackData();
        if(save)
		{
			FileName="/TrackRMS";
            DataSaveAll_UMAC(AzData_tracking,AltData_tracking,DeroData_tracking,DeroDataTime_tracking,FileName,M );
			FileName="/PCTrackRMS";
	  		DataSaveAll_PC(AzData_tracking,AltData_tracking,DeroData_tracking,DeroDataTime_tracking,FileName,M );
		}

    }*/
    char s[600];
    QString str;


    if(save)
    {
        QFile file(FileName_TrackData);
        if ( !file.open ( QFile::Append|QFile::Truncate ) )
        {
            str="Can't open file: "+FileName_TrackData;
            label_Status->setText ( str );
            return false;
        }
        QTextStream out ( &file );
        //sprintf ( s,"%5.2f",totalerror );
        /*sprintf ( s,"%10.7f\t %10.7f\t %10.7f\t %10.7f\t %7.3f\t %7.3f\t   %10.7f\t %10.7f\t %10.7f\t  %10.7f\t %7.3f\t %7.3f\t %5i\t\n",
                  trackstar.AzTarget, trackstar.AzPosRealForError,
                  trackstar.AZ_TrackTargetSpeed,trackstar.AzVelocity,
                  trackstar.AzTrackError,trackstar.AzFolErrUMAC,
                  trackstar.AltTarget, trackstar.AltPosRealForError,
                  trackstar.ALT_TrackTargetSpeed,trackstar.AltVelocity,
                  trackstar.AltTrackError,trackstar.AltFolErrUMAC,trackstar.AltCommand);*/
        sprintf ( s,"%10.7f\t %10.7f\t %7.3f\t %7.3f\t %10.7f\t %10.7f\t %7.3f\t %7.3f\t   %10.7f\t %10.7f\t %7.3f\t %7.3f\t  %10.7f\t %10.7f\t %7.3f\t %7.3f \n",
                  trackstar.AzTarget, trackstar.AzPosRealForError,
                  trackstar.AzTrackError,trackstar.AzFolErrUMAC,
                  trackstar.AltTarget, trackstar.AltPosRealForError,
                  trackstar.AltTrackError,trackstar.AltFolErrUMAC);
                  /*trackstar.DeroTarget, trackstar.DeroPosRealForError,
                  trackstar.DeroTrackError,trackstar.DeroFolErrUMAC,
                  trackstar.ADC1PosRealForError, trackstar.ADC2PosRealForError,
                  (trackstar.ADC1PosRealForError-trackstar.ADC2PosRealForError)*3600.0,totalerror);*/
        /*sprintf ( s,"%10.7f\t %10.7f\t %7.3f\t %7.3f\t %10.7f\t %10.7f\t %7.3f\t %7.3f\t   %10.7f\t %10.7f\t %7.3f\t %7.3f\t  %10.7f\t %10.7f\t %7.3f\t %7.3f \n",
                  trackstar.AzTarget, trackstar.AzPosRealForError,
                  trackstar.AzTrackError,trackstar.AzFolErrUMAC,
                  trackstar.AltTarget, trackstar.AltPosRealForError,
                  trackstar.AltTrackError,trackstar.AltFolErrUMAC,
                  trackstar.DeroTarget, trackstar.DeroPosRealForError,
                  trackstar.DeroTrackError,trackstar.DeroFolErrUMAC,
                  trackstar.ADC1PosRealForError, trackstar.ADC2PosRealForError,
                  (trackstar.ADC1PosRealForError-trackstar.ADC2PosRealForError)*3600.0,totalerror);*/
        out<<tr (s);
        file.close();
    }


// 	if(n>SampleRmsTime*TrackSaveLinesPerSecond)
// 	
// 	if(n>SampleRmsTime*TrackSaveLinesPerSecond)
// 	{
//         //	for(int i=0;i<SampleRmsTime*TrackSaveLinesPerSecond;i++)
// //			AzDataRMS_show[i]=AzData_tracking[i];
// 
// 	}
// 	if(count>SampleRmsTime)
// 	        CalAndShowRms();


	return true;
}

bool Tracking::storeTrackData()
{
	if(flagtcs.flag_AZIMUTH)
		storeAzTrackData();
	if(flagtcs.flag_ALTITUDE)
		storeAltTrackData();
        if(flagtcs.flag_DERO)
		storeDeroTrackData();
	return true;
}

void Tracking::setValidator()
{
//m_RA_Hour->setValidator(new MyDoubleValidator(RA_Hour_BOTTOM,RA_Hour_TOP,RA_Hour_decimals,this));
	m_RA_Hour->setValidator ( new QIntValidator ( RA_Hour_BOTTOM,RA_Hour_TOP,this ) );
	m_RA_Minute->setValidator ( new QIntValidator ( RA_Minute_BOTTOM,RA_Minute_TOP,this ) );
	m_RA_Second->setValidator ( new MyDoubleValidator ( RA_Second_BOTTOM,RA_Second_TOP,RA_Second_decimals,this ) );

	m_DEC_Degree->setValidator ( new QIntValidator ( DEC_Degree_BOTTOM,DEC_Degree_TOP,this ) );
	m_DEC_Minute->setValidator ( new QIntValidator ( DEC_Minute_BOTTOM,DEC_Minute_TOP,this ) );
	m_DEC_Second->setValidator ( new MyDoubleValidator ( DEC_Second_BOTTOM,DEC_Second_TOP,DEC_Second_decimals,this ) );

	lineEdit_az_offset->setValidator ( new MyDoubleValidator ( az_offset_BOTTOM,az_offset_TOP,az_offset_decimals,this ) );
	lineEdit_alt_offset->setValidator ( new MyDoubleValidator ( alt_offset_BOTTOM,alt_offset_TOP,alt_offset_decimals,this ) );
	lineEdit_dero_offset->setValidator ( new MyDoubleValidator ( dero_offset_BOTTOM,dero_offset_TOP,dero_offset_decimals,this ) );
    //lineEdit_t_offset->setValidator ( new MyDoubleValidator ( t_offset_BOTTOM,t_offset_TOP,t_offset_decimals,this ) );
	lineEdit_Pressure->setValidator ( new MyDoubleValidator ( Pressure_BOTTOM,Pressure_TOP,Pressure_decimals,this ) );
	lineEdit_Temperature->setValidator ( new MyDoubleValidator ( Temperature_BOTTOM,Temperature_TOP,Temperature_decimals,this ) );
}


void Tracking::calculate_thread_finished()
{
	pushButton_calculate->setText ( "Calculate" );
	saveStarTable ( AZ,ALT,FR,HA,DEC,DT );
	pushButton_calculate->setEnabled ( true );
	pushButton_track->setEnabled ( true );
//	calculate_thread->deleteLater();
}
void Tracking::on_pushButton_calculate_clicked()
{
   //label_Status->setText ( "Calculating data" );
    /*pushButton_calculate->setText ( "Calculating..." );
	pushButton_calculate->setEnabled ( false );
	pushButton_track->setEnabled ( false );
    pushButton_calculate->repaint();*/

    QString tmp;
	tmp=m_RA_Hour->text();
	RA_h=tmp.toDouble();

	tmp=m_RA_Minute->text();
	RA_m=tmp.toDouble();

	tmp=m_RA_Second->text();
	RA_s=tmp.toDouble();

	tmp=m_DEC_Degree->text();
	DEC_d=tmp.toDouble();

	tmp=m_DEC_Minute->text();
	DEC_m=tmp.toDouble();
	tmp=m_DEC_Second->text();
	DEC_s=tmp.toDouble();

	tmp=lineEdit_Pressure->text();
	Pressure=tmp.toDouble();

	tmp=lineEdit_Temperature->text();
	Temperature=tmp.toDouble();

    ra0= hrrad ( RA_h+RA_m/60.+RA_s/3600. );
    if ( DEC_d>0 ) dec_star=DEC_d+DEC_m/60.0+DEC_s/3600.;					//star declination  ,degree
    else if ( DEC_d==0 )
    {
        if ( DEC_m>=0 ) dec_star=DEC_m/60.0+DEC_s/3600.;
        else dec_star=DEC_m/60.0-DEC_s/3600.;
    }
    else dec_star=DEC_d-DEC_m/60.0-DEC_s/3600.;
    Dec0= degrad ( dec_star );

    np.n_pressure=Pressure;
    np.n_temp=Temperature;
    double Lat=0,Lng=0;
    Lng=SMySetting::instance_p()->value ( "misc/longitudeD" ).toDouble() + ( SMySetting::instance_p()->value ( "misc/longitudeM" ).toInt() ) /60.+ ( SMySetting::instance_p()->value ( "misc/longitudeS" ).toDouble() ) /3600.;
    Lat=SMySetting::instance_p()->value ( "misc/latitudeD" ).toDouble() +SMySetting::instance_p()->value ( "misc/latitudeM" ).toInt() /60.+SMySetting::instance_p()->value ( "misc/latitudeS" ).toDouble() /3600.;
    np.n_lat=degrad ( Lat );
    np.n_lng=degrad ( Lng );

    QDateTime Noww=QDateTime::currentDateTimeUtc();
    QDate datee= Noww.date();
    QTime timee= Noww.time();
    cal_mjd ( datee.month(),datee.day(), datee.year(),&mjdc );
    now_lst ( mjdc,np.n_lng,&lstc );							//once

    qDebug()<<"utc time year is"<< datee.year()<< "day is "<<datee.day()<<"hour is "<<timee.hour();//<<" min is "<<timee.minute<<"sec is"<<timee.second;
    qDebug()<<"mjd0 ="<< mjd0<< "mjdc = "<<mjdc;
    qDebug()<<"lst0 ="<< lst0<< "lstc = "<<lstc;
    cal_flag=1;
	QDateTime dt[TableNumMAX];
//        Dynamic1Dim  <QDateTime> dt;
    dt[0]=QDateTime::currentDateTime();

	QDate date[TableNumMAX];
	QTime time[TableNumMAX];
	//Dynamic1Dim  <QDate> date;
	//Dynamic1Dim  <QTime> time;

	date[0]=dt[0].date();
	time[0]=dt[0].time();

	DT[0].year=date[0].year();
	DT[0].month=date[0].month();
	DT[0].day=date[0].day();

    DT[0].hour=time[0].hour();
	DT[0].minute=time[0].minute();
	DT[0].second=time[0].second();
//	DT[0].msec=time[0].msec();
    DT[0].msec=000;
    //calculate_new ( &AZ[0],&ALT[0],&FR[0],&HA[0],&DEC[0],DT[0] );
    calculate  ( &AZ[0],&ALT[0],&FR[0],&HA[0],&DEC[0],DT[0] );
    qDebug()<<"local time day is "<<DT[0].day<<"hour is "<<DT[0].hour<<" min is "<<DT[0].minute<<"sec is"<<DT[0].second;
    //       <<"lst"<<lst;
    for ( long i=1;i<TableNum;i++ )
	{
		DT[i]=DT[i-1];
        DT[i].msec=DT[i-1].msec+100;							//every 0.1s get a star position
		while ( DT[i].msec>=1000 )
		{
			DT[i].msec=DT[i].msec-1000;
			DT[i].second=DT[i].second+1;
		}
		while ( DT[i].second>=60 )
		{
			DT[i].second=DT[i].second-60;
			DT[i].minute=DT[i].minute+1;
		}
		while ( DT[i].minute>=60 )
		{
			DT[i].minute=DT[i].minute-60;
			DT[i].hour=DT[i].hour+1;
		}
		while ( DT[i].hour>=24 )
		{
			DT[i].hour=DT[i].hour-24;
			DT[i].day=DT[i].day+1;
		}

        calculate (&AZ[i],&ALT[i],&FR[i],&HA[i],&DEC[i],DT[i] );
        //QCoreApplication::processEvents();
        //sleep(0);
    }
    //qDebug()<<"first utc "<<( DT[71999].hour-8 ) *3600+DT[71999].minute*60.+DT[71999].second+DT[71999].msec/1000.0
    //       <<"lst"<<lst;

    saveStarTable ( AZ,ALT,FR,HA,DEC,DT );
    if ( readStarTable() )
    {
        cal_flag=2;
        GetTableLine1();
        label_Status->setText ( "Tracking data ready" );
    }
     else
    {
        cal_flag=0;
        label_Status->setText ( "Something wrong in calculating tracking data." );
    }
}
void Tracking::calculate ( double *AZ_cal, double *ALT_cal,double *FR_cal,double *ha_cal,double *dec_cal,DateTime dt )
{
    ra=ra0;
    Dec=Dec0;
	*dec_cal=dec_star;									//declination
    //qDebug()<<"RA_h"<<RA_h<<"RA_m"<<RA_m<<"RA_s"<<RA_s<<"ra"<<RA_h+RA_m/60.+RA_s/3600.;
    //double ra0=ra;
    //double dec0=Dec;
    double utc= ( dt.hour-8 ) *3600+dt.minute*60.+dt.second+dt.msec/1000.0;			//change system time to real utc
    if(utc<0)utc+=24*3600;
    //cal_mjd ( dt.month,dt.day,dt.year,&mjd0 );
    deltaUT=deltat ( mjdc );
    mjd1=mjdc+ ( utc+deltaUT ) /86400.0;

    precess_p03 ( J2000,mjd1,&ra,&Dec );

	sunpos ( mjd1,&lsn,&rsn,NULL );

	nut_eq ( mjd1,&ra,&Dec );

	ab_eq ( mjd1,lsn,&ra,&Dec );
    //qDebug()<<"delta Ra="<<degrad(ra0-ra)*3600<<" delta Dec= "<<degrad(dec0-Dec)*3600;
    //qDebug()<<"lst0_1 "<<lst0<<"lst "<<lst;
    //now_lst ( mjd0,np.n_lng,&lstc );							//once
    lst = 1.002737909350795* ( utc ) /3600.0 +
          lstc+ radhr ( np.n_lng );								//every time
	range ( &lst, 24.0 );
    //qDebug()<<"delta Ra="<<radhr(ra0-ra)*3600<<"ra"<<radhr(ra);
	ha=hrrad ( lst )-ra;
	ha_star=ha*180/PI;
	*ha_cal=ha_star;

	hadec_aa ( np.n_lat,ha,Dec,&alt,&az );							//calculate alt az from ha dec
    refract ( np.n_pressure,np.n_temp,alt,&alt );
	aa_hadec ( np.n_lat,alt,az,&ha1,&dec1 );

    derotator(np.n_lat, ha1, dec1,alt,&fpa );
    //derotator25m ( np.n_lat, ha1, dec1,&fpa );
    //qDebug()<<"utc "<<utc<<"lst "<<lst<<"ha"<<radhr(ha)*3600<<"ha1"<<radhr(ha1)*3600;
	*AZ_cal=az*180/PI;
	*ALT_cal=alt*180/PI;
	*FR_cal=fpa*180/PI;
}
/*
void Tracking::calculate_new (double *AZ_cal,double *ALT_cal,double *FR_cal,double *ha_cal,double *dec_cal,DateTime dt )
{
    ra=ra0;
    Dec=Dec0;
    *dec_cal=dec_star;									//declination
    //qDebug()<<"RA_h"<<RA_h<<"RA_m"<<RA_m<<"RA_s"<<RA_s<<"ra"<<RA_h+RA_m/60.+RA_s/3600.;
    //double ra0=ra;
    //double dec0=Dec;
    double utc= ( dt.hour-8 ) *3600+dt.minute*60.+dt.second+dt.msec/1000.0;			//change system time to real utc
    if(utc<0)utc+=24*3600;

    deltaUT=deltat ( mjdc );
    qDebug()<<"deltaUT="<<deltaUT;

    ra=ra0;
    Dec=Dec0;
    mjd1=mjd0+ ( utc +deltaUT) /86400.0;

    precess_p03 ( J2000,mjd1,&ra,&Dec );

    sunpos ( mjd1,&lsn,&rsn,NULL );

    nut_eq ( mjd1,&ra,&Dec );

    ab_eq ( mjd1,lsn,&ra,&Dec );
    //qDebug()<<"delta Ra="<<raddeg(raold-ra)*3600<<" delta Dec= "<<raddeg(decold-Dec)*3600;
    //qDebug()<<"Ra="<<raddeg(ra)*3600<<" Dec= "<<raddeg(Dec)*3600;
    now_lst ( mjd0,np.n_lng,&lst0 );							//once
    lst = 1.002737909350795* ( utc ) /3600.0 +
          lst0+ radhr ( np.n_lng );								//every time
    range ( &lst, 24.0 );


    ha=hrrad ( lst )-ra;
    ha_star=ha*180/PI;
    *ha_cal=ha_star;

    hadec_aa ( np.n_lat,ha,Dec,&alt,&az );
    double alt3= alt; //for test
    double ra3=ra;
    refract ( np.n_pressure,np.n_temp,alt,&alt );
    qDebug()<<"refract="<<raddeg(alt-alt3)*3600;
    aa_hadec ( np.n_lat,alt,az,&ha1,&dec1 );
    //ma ( np.n_lat, ha1, dec1, &alt, &az, &fpa );
    derotator(np.n_lat, ha1, dec1,alt,&fpa );
    //derotator25m ( np.n_lat, ha1, dec1,&fpa );
   *AZ_cal=az*180/PI;
    *ALT_cal=alt*180/PI;
    *FR_cal=fpa*180/PI;

    double dhour = double(dt.hour-8) + double(dt.minute)/60 + double(dt.second)/3600 +
            double(dt.msec)/3600000;
   double  mjd_new = star.ModifiedJulianDay(dt.year, dt.month, dt.day, dhour);
   double  mjd0_new = star.ModifiedJulianDay(dt.year, dt.month, dt.day, 0);
    qDebug()<<"mjd0="<<mjd0<<" mjd1="<<mjd1<<" mjd_new="<<mjd_new<<" mjd0_new="<<mjd0_new;
   double lst_new= star.GetMeanLST(mjd_new, np.n_lng);
   double lst0_new= star.GetGMST0(mjd0_new);
   qDebug()<<"lst0="<<lst0*15<<"lst0-lst0_new="<<(lst0*15-raddeg(lst0_new))*3600;

    double ep = star.GetEpoch(dt.year, dt.month, dt.day, dhour);
    double ra8, dec8, az8, alt8, alt9, ref8, ha8;
    ra8=ra0;
    dec8=Dec0;
    star.Epoch2Actual(2000, ra8, dec8, ep, ra8, dec8);
    ha8= lst_new - ra8;//hrrad ( lst )-ra8;
    star.Eq2AltAzi(ha8, dec8, np.n_lat, az8, alt8);

    ref8 = star.RefractReal(alt8, np.n_pressure, np.n_temp);
    alt9 = alt8 + ref8;
    qDebug()<<"ha8_ha="<<( ha8-ha)* RtoG*3600<<"ra3_ra8="<<( ra3-ra8)* RtoG*3600<<"d_dec="<<raddeg(dec8-Dec)*3600
              <<"lst="<<lst*15<<"lst_-lst_new="<<(lst*15-raddeg(lst_new))*3600;//(lst*15 -lst_new* RtoG)*3600;
    qDebug()<<"d_az="<<(az8-az)* RtoG*3600<<"d_alt="<<raddeg(alt8-alt3)*3600
              <<"dr_alt="<<raddeg(alt9-alt)*3600;
}
*/
bool Tracking::readStarTable()
{
    TableNum=0;
	long long num=0;
	StarTarget startarget;
	QString fileName=SMySetting::instance_p()->value ( "misc/StarTable" ).toString();
	QFile file ( fileName );
	if ( !file.open ( QFile::ReadOnly ) )
	{
		QMessageBox msgBox ( QMessageBox::NoIcon,"Warning!","open error!" );
		msgBox.setText ( "open error!" );
		msgBox.setStyleSheet ( "QPushButton { background-color: rgb(255, 85, 0); color: rgb(85, 255, 0); }" );
		msgBox.exec();

		return false;
	}

	QTextStream stream ( &file );
	//MultiLineEdit1->setText( t.read() );
	QString line;
	stream.readLine();											//first line,no use
	 

	while ( !stream.atEnd() )
	{
		starTable[num]= stream.readLine();
		num++;
        TableNum++;
	}
    if(TableNum > 180000) TableNum=180000;
	file.close();

	QString LastLine;
	LastLine=starTable[num-1] ; 										//last line,last time,just for show
	//  qDebug() << LastLine<<num;
	QStringList  list;
	list = LastLine.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );					//blank space
    if ( list.size() <2 )
    {
        label_Status->setText ( "Star table error!" );
        /*QMessageBox msgBox ( QMessageBox::NoIcon,"Warning!","Star table error!" );
		msgBox.setText ( "table error!" );
		msgBox.setStyleSheet ( "QPushButton { background-color: rgb(255, 85, 0); color: rgb(85, 255, 0); }" );
        msgBox.exec();*/

		return false;
	}

	startarget.cDate=list[0];
	startarget.cTime=list[1];

	label_EndTimer->setText ( list[1] );
	QString h_E = list[1].mid ( 0, 2 );
	QString m_E = list[1].mid ( 2, 2 );
	QString s_E = list[1].mid ( 4, 2 );
    QString y=h_E+" h "+m_E+" m "+s_E+" s";

	label_EndTimer->setText ( y );
    label_EndTimer->setStyleSheet ( "color:rgb(0, 85, 0)" );
    //label_EndTimer->setFont ( QFont ( "Times", 14, QFont::Bold ) );

    tableEndTime=h_E.toInt()*3600+m_E.toInt()*60+s_E.toInt();
	return true;
}

void  Tracking::readADCtable()
{
    long num=0;

    QString fileName="/home/scope/program/telescope/altitudetoadc.txt";
    QFile file ( fileName );

    if ( !file.open ( QFile::ReadOnly ) )
    {
        /*QMessageBox msgBox ( QMessageBox::NoIcon,"Warning!","open error!" );
        msgBox.setText ( "open error!" );
        msgBox.setStyleSheet ( "QPushButton { background-color: rgb(255, 85, 0); color: rgb(85, 255, 0); }" );
        msgBox.exec();*/

        return;
    }

    QTextStream stream ( &file );
    //MultiLineEdit1->setText( t.read() );
    QString line;
    //stream.readLine();											//first line,no use
    QStringList  list;


    while ( !stream.atEnd() )
    {
        line= stream.readLine();
        list = line.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
        if ( list.size() >1 )
        {
            Alt2Adc[num][0]=list[0].toInt();
            Alt2Adc[num][1]=list[1].toInt();
        }
        num++;

    }
    if(num<20)
    {
        for(int i=num; i<20; i++)
        {
            Alt2Adc[i][0]=Alt2Adc[num-1][0];
            Alt2Adc[i][1]=Alt2Adc[num-1][1];
        }
    }
    //qDebug()<<"Alt2Adc[num][0]"<<Alt2Adc[1][0]<<Alt2Adc[1][1];
    file.close();
}

bool Tracking::readModel()
{
    /*double IA1=0;
    double IE1=0;
    double NPAE1=0;
    double CA1=0;
    double AW1=0;
    double AN1=0;
    double ACEC1=0;
    double TF1=0;*/

	long long num=0;
	QString ModelTable[100];
	QStringList  list;
	QString fileName=SMySetting::instance_p()->value ( "misc/ModelFile" ).toString();
    //fileName="/home/scope/telescope/model/songmodel.dat";
	QFile file ( fileName );
	if ( !file.open ( QFile::ReadOnly ) )
	{
		QMessageBox msgBox ( QMessageBox::NoIcon,"Warning!","open error!" );
		msgBox.setText ( "open error!" );
		msgBox.setStyleSheet ( "QPushButton { background-color: rgb(255, 85, 0); color: rgb(85, 255, 0); }" );
		msgBox.exec();

		return FALSE;
	}

	QTextStream stream ( &file );

	while ( !stream.atEnd() )
	{
		ModelTable[num]= stream.readLine();
		num++;

	}

	file.close();

	for ( int i=2;i< ( num-1 );i++ )
	{
		list = ModelTable[i].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );					//blank space
        //ParseTerm ( list,&IA,&IE,&NPAE,&CA,&AW,&AN,&NRX,&NRY );
        if( list[0]=="AW"||list[0]=="=AW" ) AW=list[1].toDouble();
        else if( list[0]=="AN"||list[0]=="=AN" ) AN=list[1].toDouble();
        else if( list[0]=="ACEC"||list[0]=="=ACEC" ) ACEC=list[1].toDouble();
        else if( list[0]=="ACES"||list[0]=="=ACES" ) ACES=list[1].toDouble();
        else if( list[0]=="CA"||list[0]=="=CA" ) CA=list[1].toDouble();
        else if( list[0]=="CRX"||list[0]=="=CRX" ) CRX=list[1].toDouble();
        else if( list[0]=="CRY"||list[0]=="=CRY" ) CRY=list[1].toDouble();
        else if( list[0]=="ECEC"||list[0]=="=ECEC" ) ECEC=list[1].toDouble();
        else if( list[0]=="ECES"||list[0]=="=ECES" ) ECES=list[1].toDouble();
        else if(list[0]=="IA"||list[0]=="=IA") IA=list[1].toDouble();
        else if( list[0]=="IE"||list[0]=="=IE" ) IE=list[1].toDouble();
        else if( list[0]=="NPAE"||list[0]=="=NPAE" ) NPAE=list[1].toDouble();
        else if( list[0]=="NRX"||list[0]=="=NRX" ) NRX=list[1].toDouble();
        else if( list[0]=="NRY"||list[0]=="=NRY" ) NRY=list[1].toDouble();
        else if( list[0]=="TF"||list[0]=="=TF" ) TF=list[1].toDouble();
	}

    qDebug()<<"AW="<<AW<<"AN="<<AN<<"ACEC="<<ACEC<<"ACES="<<ACES<<"CA="<<CA
              <<"CRX="<<CRX<<"CRY="<<CRY<<"ECEC="<<ECEC<<"ECES="<<ECES
           <<"IA="<<IA<<"IE="<<IE<<"NPAE="<<NPAE<<"NRX="<<NRX<<"NRY="<<NRY<<"TF="<<TF;
	return TRUE;
}

// bool Tracking::ParseTerm ( QStringList  list,double *IA,double *IE,double *NPAE,double *CA,double *AW,double *AN )
// {
// //         for(int i=0;i<list.size();i++)
// //         {   qDebug()<<list[i];
// //  	}
// 
// 	if ( list[0]=="IA"||list[0]=="=IA" )
// 	{
// 		if ( list.size() >=1 )
// 			{*IA=list[1].toDouble();	qDebug() <<"IA="<<*IA;}
// 		return TRUE;
// 	}
// 
// 	if ( list[0]=="IE"||list[0]=="=IE" )
// 	{
// 		if ( list.size() >=1 )
// 			{*IE=list[1].toDouble();	qDebug() <<"IE="<<*IE;}
// 		return TRUE;
// 	}
// 
// 	if ( list[0]=="NPAE"||list[0]=="=NPAE" )
// 	{
// 		if ( list.size() >=1 )
// 			{*NPAE=list[1].toDouble();	qDebug() <<"NPAE="<<*NPAE;}
// 		return TRUE;
// 	}
// 
// 	if ( list[0]=="CA"||list[0]=="=CA" )
// 	{
// 		if ( list.size() >=1 )
// 			{*CA=list[1].toDouble();	qDebug() <<"CA="<<*CA;}
// 		return TRUE;
// 	}
// 
// 	if ( list[0]=="AW"||list[0]=="=AW" )
// 	{
// 		if ( list.size() >=1 )
// 			{*AW=list[1].toDouble();	qDebug() <<"AW="<<*AW;}
// 		return TRUE;
// 	}
// 
// 	if ( list[0]=="AN"||list[0]=="=AN" )
// 	{
// 		if ( list.size() >=1 )
// 			{*AN=list[1].toDouble();	qDebug() <<"AN="<<*AN;}
// 		return TRUE;
// 	}
// 	return FALSE;
// }



bool Tracking::ParseTerm ( QStringList  list,double *IA,double *IE,double *NPAE,double *CA,double *AW,double *AN,double *NRX,double *NRY )
{
 
	if ( list[0]=="IA"||list[0]=="=IA" )
	{
		if ( list.size() >=1 )
            *IA=list[1].toDouble();//	qDebug() <<"IA="<<*IA;}
		return TRUE;
	}

	if ( list[0]=="IE"||list[0]=="=IE" )
	{
		if ( list.size() >=1 )
            *IE=list[1].toDouble();//	qDebug() <<"IE="<<*IE;}
		return TRUE;
	}

	if ( list[0]=="NPAE"||list[0]=="=NPAE" )
	{
		if ( list.size() >=1 )
            *NPAE=list[1].toDouble();	//qDebug() <<"NPAE="<<*NPAE;}
		return TRUE;
	}

	if ( list[0]=="CA"||list[0]=="=CA" )
	{
		if ( list.size() >=1 )
            *CA=list[1].toDouble();	//qDebug() <<"CA="<<*CA;}
		return TRUE;
	}

	if ( list[0]=="AW"||list[0]=="=AW" )
	{
		if ( list.size() >=1 )
            *AW=list[1].toDouble();	//qDebug() <<"AW="<<*AW;}
		return TRUE;
	}

	if ( list[0]=="AN"||list[0]=="=AN" )
	{
		if ( list.size() >=1 )
            *AN=list[1].toDouble();	//qDebug() <<"AN="<<*AN;}
		return TRUE;
	}

	if ( list[0]=="NRX"||list[0]=="=NRX" )
	{
		if ( list.size() >=1 )
            *NRX=list[1].toDouble();	//qDebug() <<"NRX="<<*NRX;}
		return TRUE;
	}

	if ( list[0]=="NRY"||list[0]=="=NRY" )
	{
		if ( list.size() >=1 )
            *NRY=list[1].toDouble();	//qDebug() <<"NRY="<<*NRY;}
		return TRUE;
	}

	return FALSE;
}



bool SaveDataModel()
{ 
	char s_0[400];
	char s_1[60];
	char s_2[60];
	char s_3[20];
	char s_4[20];
	char s_5[400];
	char s_6[60];
	char s_7[60];
	char s_8[20];
	char s_9[20];
    QString fileName="/home/telescope/telescope/model/DataModel.txt";
	QFile file ( fileName );
	if ( !file.open ( QFile::WriteOnly|QFile::Truncate ) )
	{
		return false;
	}

	QTextStream out ( &file );
	sprintf ( s_0,"  AZ           ALT         AZ+offset          ALT+offset           \n" );
	out<<s_0;
	
	sprintf ( s_1,"  %10.6f ",trackstar.AzTarget );
	sprintf ( s_2,"  %10.6f ",trackstar.AltTarget );
    sprintf ( s_3,"  %10.6f ",trackstar.AzTarget+ ( trackstar.az_correct_all+az_model ) /3600.0 );
    sprintf ( s_4,"  %10.6f ",trackstar.AltTarget+ (trackstar.alt_correct_all+alt_model ) /3600.0 );
	
	sprintf ( s_0,"%s%s%s%s\n",s_1,s_2,s_3,s_4 );
	out<<s_0;

	file.close();

//////////////////////////////////////////////////////////////////////////////////////////////
    fileName="/home/telescope/telescope/model/DataDetail.txt";;
	QFile file_DataDetail( fileName );
	if ( !file.open ( QFile::WriteOnly|QFile::Truncate ) )
	{
		return false;
	}
	QTextStream out_DataDetail ( &file_DataDetail );
	sprintf ( s_0,"  AZ           ALT         AZ+offset          ALT+offset           \n" );
	out_DataDetail<<s_0;
	
	double pointerror=sqrt ( trackstar.az_guidedata*trackstar.az_guidedata*cos ( trackstar.AltTarget*PI/180.0 ) *
	                           cos (  trackstar.AltTarget*PI/180.0 ) +trackstar.alt_guidedata*trackstar.alt_guidedata );
	sprintf ( s_1,"  %10.6f ",trackstar.ra*180.0/PI );//ra,degree
	sprintf ( s_2,"  %10.6f ",trackstar.dec*180.0/PI );//dec, degree
	sprintf ( s_3,"  %10.6f ",trackstar.ha_star );//ha,degree
	sprintf ( s_4,"  %10.6f ",trackstar.AzTarget );// degree
	sprintf ( s_5,"  %10.6f ",trackstar.AltTarget );// degree
	sprintf ( s_6,"  %6.3f ",trackstar.az_guidedata );//degree
	sprintf ( s_7,"  %6.3f ",trackstar.alt_guidedata );//degree
	sprintf ( s_8,"  %6.3f ",pointerror );//degree
    //sprintf ( s_9,"  %d ",   AzDataTime_tracking[0] );
    QByteArray ba= AzDataTime_tracking[0].toLatin1();
    //s_9 = ba.data();
    sprintf ( s_9,"  %s ",   ba.data() );

	sprintf ( s_0,"%s%s%s%s%s%s%s%s%s\n",s_1,s_2,s_3,s_4,s_5,s_6,s_7,s_8,s_9);
	out_DataDetail<<s_0;

	file_DataDetail.close();

}






bool  Tracking::saveStarTable ( double AZ[],double ALT[],double FR[],double HA[],double DEC[],DateTime dt[] )
{
	char* s_0=new char[400] ;
	char* s_1=new char[60] ;
	char* s_2=new char[60];
	char* s_3=new char[20];
	char* s_4=new char[20];
	char* s_5=new char[20];
	char* s_6=new char[20];
	char* s_7=new char[20];
//	long num=0;
	long i=0;
	double seconds=0;

//////////////save file////////////////////
	//QString fileName = QFileDialog::getOpenFileName ( this );//select file
	QString fileName=SMySetting::instance_p()->value ( "misc/StarTable" ).toString();
	//QString fileName="/home/telescope/telescope/StarTable.txt";
	qDebug()<<fileName;
	QFile file ( fileName );
	if ( !file.open ( QFile::WriteOnly|QFile::Truncate ) )
	{
		return false;
	}

	QTextStream out ( &file );
	sprintf ( s_0,"  yyyymmdd   hhmmss      AZ           ALT         Dero          HA          DEC \n" );
	out<<s_0;
	for ( i=0;i<TableNum;i++ )
	{
		sprintf ( s_1,"  %d%02d%02d",dt[i].year,dt[i].month,dt[i].day );				//yyyymmdd
		seconds=dt[i].second+float ( dt[i].msec/1000.0 );
		sprintf ( s_2,"  %02d%02d%05.2f",dt[i].hour,dt[i].minute,seconds );				//hhmmss
        sprintf ( s_3,"  %12.9f",AZ[i] );
        sprintf ( s_4,"  %12.9f",ALT[i] );
		sprintf ( s_5,"  %10.7f",HA[i] );
		sprintf ( s_6,"  %10.7f",DEC[i] );
        sprintf ( s_7,"  %12.9f",FR[i] );
		sprintf ( s_0,"%s%s%s%s%s%s%s\n",s_1,s_2,s_3,s_4,s_7,s_5,s_6 );
		out<<s_0;
	}
    //cal_flag=0;
	// qDebug()<<cal_flag;
	file.close();
	return true;
}



void Tracking::checkCommandFromOCS()
{
    if(OcsCom.flag_TcsEmergenStop_AzAlt)
    {
        on_pushButton_stop_clicked();
        OcsCom.flag_TcsEmergenStop_AzAlt=false;
        return;
    }
	if ( flagtcs.flag_TrackStarByOCS_MA ) 			//start tracking
	{
		TrackStarByOCS_MA();
		flagtcs.flag_TrackStarByOCS_MA = false;
	}

	if ( flagtcs.flag_TrackStarStopByOCS_MA ) 		//stop tracking
	{
		TrackingStopByOCS_MA();
 		flagtcs.flag_TrackStarStopByOCS_MA = false;
	}
	if ( flagtcs.flag_GetTrackingPosByOCS_MA ) 		//Get Tracking Pos
	{
		GetTrackingPosByOCS_MA();
 		flagtcs.flag_GetTrackingPosByOCS_MA = false;
	}

}
/*input:

         OcsCom.StatusCUID_MA   ,Global Variable
*/


/* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
   StatusCUID:return to OCS, new time
   ExeStatus:Start , Active , Done
	RV:    0:working normally;1:warning;-1:fault
       Info:   if no error:"OK"; or,imformation of error
 bExeStatus:  start working or finish working:"true";still working:false
 DataBack:	Additional data back to OCS,eg. positon

*/

void Tracking::MakeString ( char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp )
{
	QDateTime datetime;
	QString s_datetime;

	datetime = QDateTime::currentDateTime();
	s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );						//RelatedCmdCUID

	QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +s_datetime+QString ( "><ExeStatus=%1><RV=%2><Info=%3><bExeStatus=%4>" ).arg ( ExeStatus ).arg ( RV ).arg ( Info ).arg ( bExeStatus ) +DataBack;
	( *s_temp ).append ( str );
}

void Tracking::MakeStringNew ( char StatusCUID[100],char RV[10],char Info[20],char Info1[50],char Info2[50],char Info3[50],char Info4[50],char Info5[50],char Info6[50],char bExeStatus[10],QString DataBack,QString* s_temp )
{
	QDateTime datetime;
	QString s_datetime;

	datetime = QDateTime::currentDateTime();
	s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

	QString str=QString ( "<RelatedCmdCUID=%1.%2><StatusCUID=" ).arg ( StatusCUID ).arg( s_datetime ) +s_datetime+QString ( "><RV=%1><Info=%2>%3%4%5%6%7%8<bExeStatus=false>" ).arg ( RV ).arg ( Info ).arg ( Info1 ).arg ( Info2 ).arg ( Info3 ).arg ( Info4 ).arg ( Info5 ).arg ( Info6 ) +DataBack;
	( *s_temp ).append ( str );
}
//void Tracking::CombineString ( char StatusCUID[100],char ExeStatus[50],char bExeStatus[10],QString DataBack,QString* s_temp )
void Tracking::CombineString ( QString StatusCUID,char ExeStatus[50],char bExeStatus[10],QString DataBack,QString* s_temp )
{
	QDateTime datetime;
	QString s_datetime;

	datetime = QDateTime::currentDateTime();
	s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

	QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +s_datetime+QString ( "><ExeStatus=%1><bExeStatus=true>" ).arg ( ExeStatus ) +DataBack;
	( *s_temp ).append ( str );
}

void Tracking::CombineString ( QString StatusCUID,char ExeStatus[50],char bExeStatus[10],QString* s_temp )
{
	QDateTime datetime;
	QString s_datetime;

	datetime = QDateTime::currentDateTime();
	s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

	QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +s_datetime+QString ( "><ExeStatus=%1><bExeStatus=true>" ).arg ( ExeStatus );
	( *s_temp ).append ( str );
}

// void Tracking::CombineStringNew ( char StatusCUID[100],char RV[10],char Info[10],char Info1[10],char Info2[10],char Info3[10],char Info4[10],char Info5[10],char Info6[10],char bExeStatus[10],QString DataBack,QString* s_temp )
void Tracking::CombineStringNew ( QString StatusCUID,char RV[10],char Info[20],char Info1[50],char Info2[50],char Info3[50],char Info4[50],char Info5[50],char Info6[50],char bExeStatus[10],QString DataBack,QString* s_temp )
{
	QDateTime datetime;
	QString s_datetime;

	datetime = QDateTime::currentDateTime();
	s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

	QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +s_datetime+QString ( "><RV=%1><Info=%2>%3%4%5%6%7%8<bExeStatus=false>" ).arg ( RV ).arg ( Info ).arg ( Info1 ).arg ( Info2 ).arg ( Info3 ).arg ( Info4 ).arg ( Info5 ).arg ( Info6 ) +DataBack;
	( *s_temp ).append ( str );
}



void   Tracking::SendToOCS ( QString str )
{
    sendthread *send = new sendthread(m_parent,str,OcsIP,OcsPort);
    send->run();
}

void Tracking::TrackingStopByOCS_MA()
{
    QString s_temp,s_temp1,s_temp2;
	char ExeStatus[10]="";
	char RV[10]="";
	char Info[20];
    /*char Info1[50] =  "";
	char Info2[50] =  "";
	char Info3[50] =  "";
    char Info4[50] =  "";
    char Info5[50] =  "";
    char Info6[50] =  "";*/

    char bExeStatus[10]="true";
	QString DataBack="";
    DataBack =  QString ( "<Az=%1><Alt=%2><Dero=%3>" ).arg ( trackstar.AzPosRealForError,0,'f',8 ).arg ( trackstar.AltPosRealForError,0,'f',8 ).arg ( trackstar.DeroPosRealForError,0,'f',8 );

	flagtcs.flag_TrackStarStopByOCS_MA=false;
	flagtcs.flag_TrackStarByOCS_MA=false;
	flagtcs.flag_TrackingStarByOCS_MA=false;

    sprintf ( ExeStatus, "Start" );
    sprintf ( RV, "0" );
    CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( Info, "OK" );
    MakeString ( OcsCom.StatusCUID_Track,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);
    sprintf ( ExeStatus, "Done" );
    //CombineString ( OcsCom.TrackStartUid,ExeStatus,bExeStatus,&s_temp );
    MakeString ( OcsCom.StatusCUID_Track,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    SendToOCS ( s_temp2 );

	
	trackstar.flagDeroTracking=false;

    //if ( pushButton_stop->isEnabled() )
    //{
    on_pushButton_stop_clicked();
    //}
    /*sprintf ( ExeStatus, "Start" );
    CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
    s_temp="";

    QTime dieTime = QTime::currentTime().addMSecs(1000);
    while ( QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    if ( !flagtcs.Alt_LoopStatus && !flagtcs.Az_LoopStatus && !flagtcs.Dero_LoopStatus )
    {
        sprintf ( RV,"0" );
        sprintf ( Info,"OK" );
        sprintf ( Info1,"<Alt=true>" );
        sprintf ( Info2,"<Az=true>" );
        sprintf ( Info3,"<Dero=true>" );
    }
    if ( flagtcs.Alt_LoopStatus )
    {
        sprintf ( RV,"-1" );
        sprintf ( Info,"TrackStopFalse" );
        sprintf ( Info1,"<Alt=false>" );
    }
    if ( flagtcs.Az_LoopStatus )
    {
        sprintf ( RV, "-1" );
        sprintf ( Info, "TrackStopFalse" );
        sprintf ( Info2,"<Az=false>" );
    }
    if ( flagtcs.Dero_LoopStatus )
    {
        sprintf ( RV, "-1" );
        sprintf ( Info, "TrackStopFalse" );
        sprintf ( Info3,"<Dero=false>" );
    }

    CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
    s_temp="";

    sprintf ( ExeStatus, "Done" );
    CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );*/

    /*if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_1 )
	{	
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
			//qDebug() << "Track Stop";
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(5000);
		while(QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.ADC2_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
                        sprintf ( Info1,"<ADC2=true>" );
		}
		else
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info1,"<ADC2=false>" );
		}
		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		OcsCom.TrackStopMode = 0;	
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_2 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while(QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.ADC1_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
                        sprintf ( Info1,"<ADC1=true>" );
		}
		else
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info1,"<ADC1=false>" );
		}
		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";	

		OcsCom.TrackStopMode = 0;
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_4 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while(QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.Dero_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
                        sprintf ( Info1,"<Dero=true>" );
		}
		else
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info1,"<Dero=false>" );
		}
		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";	
		
		OcsCom.TrackStopMode = 0;
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_8 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while(QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

		if ( !flagtcs.Alt_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
			sprintf ( Info1,"<Alt=true>" );
		}
		else
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
			sprintf ( Info1,"<Alt=false>" );
		}
		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		OcsCom.TrackStopMode = 0;	
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_9 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while ( QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.Alt_LoopStatus && !flagtcs.ADC2_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
			sprintf ( Info1,"Alt=true" );
                        sprintf ( Info2,"<ADC2=true>" );
		}
                else if ( flagtcs.Alt_LoopStatus && !flagtcs.ADC2_LoopStatus )
		{
			sprintf ( RV,"-1" );
			sprintf ( Info,"TrackStopFalse" );
			sprintf ( Info1,"<Alt=false>" );
		}
                else if ( !flagtcs.Alt_LoopStatus && flagtcs.ADC2_LoopStatus )
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info2,"<ADC2=false>" );
		}
                else if ( flagtcs.Alt_LoopStatus && flagtcs.ADC2_LoopStatus )
                {
                        sprintf ( RV,"-1" );
                        sprintf ( Info,"TrackStopFalse" );
                        sprintf ( Info1,"<Alt=false>" );
                        sprintf ( Info2,"<ADC2=false>" );
                }


		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		OcsCom.TrackStopMode = 0;	
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_10 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while ( QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.Alt_LoopStatus && !flagtcs.ADC1_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
			sprintf ( Info1,"<Alt=true>" );
                        sprintf ( Info2,"<ADC1=true>" );
		}
                else if ( flagtcs.Alt_LoopStatus && !flagtcs.ADC1_LoopStatus )
		{
			sprintf ( RV,"-1" );
			sprintf ( Info,"TrackStopFalse" );
			sprintf ( Info1,"<Alt=false>" );
		}
                else if ( flagtcs.ADC1_LoopStatus && !flagtcs.Alt_LoopStatus )
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info2,"<ADC1=false>" );
		}
                else if ( flagtcs.Alt_LoopStatus && flagtcs.ADC1_LoopStatus )
                {
                        sprintf ( RV,"-1" );
                        sprintf ( Info,"TrackStopFalse" );
                        sprintf ( Info1,"<Alt=false>" );
                        sprintf ( Info2,"<ADC1=false>" );
                }

		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		OcsCom.TrackStopMode = 0;
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_12 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while ( QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.Alt_LoopStatus && !flagtcs.Dero_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
			sprintf ( Info1,"<Alt=true>" );
			sprintf ( Info2,"<C1=true>" );
		}
                else if ( flagtcs.Alt_LoopStatus && !flagtcs.Dero_LoopStatus)
		{
			sprintf ( RV,"-1" );
			sprintf ( Info,"TrackStopFalse" );
			sprintf ( Info1,"<Alt=false>" );
		}
                else if ( !flagtcs.Alt_LoopStatus && flagtcs.Dero_LoopStatus )
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info2,"<Dero=false>" );
		}
                else if ( flagtcs.Alt_LoopStatus && flagtcs.Dero_LoopStatus )
                {
                        sprintf ( RV,"-1" );
                        sprintf ( Info,"TrackStopFalse" );
                        sprintf ( Info1,"<Alt=false>" );
                        sprintf ( Info2,"<Dero=false>" );
                }


		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		OcsCom.TrackStopMode = 0;
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_16 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while(QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

		if ( !flagtcs.Az_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
                        sprintf ( Info1,"<Az=true>" );
		}
		else
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
			sprintf ( Info1,"<Az=false>" );
		}
		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";	
		
		OcsCom.TrackStopMode = 0;
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_17 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while ( QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.Az_LoopStatus && !flagtcs.ADC2_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
			sprintf ( Info1,"<Az=true>" );
                        sprintf ( Info2,"<ADC2=true>" );
		}
                else if ( flagtcs.Az_LoopStatus && !flagtcs.ADC2_LoopStatus  )
		{
			sprintf ( RV,"-1" );
			sprintf ( Info,"TrackStopFalse" );
			sprintf ( Info1,"<Az=false>" );
		}
                else if ( !flagtcs.Az_LoopStatus && flagtcs.ADC2_LoopStatus  )
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info2,"<ADC2=false>" );
		}
                else if ( flagtcs.Az_LoopStatus && flagtcs.ADC2_LoopStatus  )
                {
                        sprintf ( RV, "-1" );
                        sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info1,"<Az=false>" );
                        sprintf ( Info2,"<ADC2=false>" );
                }

		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		OcsCom.TrackStopMode = 0;
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_18 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while ( QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.Az_LoopStatus && !flagtcs.ADC1_LoopStatus )
                {
                        sprintf ( RV,"0" );
                        sprintf ( Info,"OK" );
                        sprintf ( Info1,"<Az=true>" );
                        sprintf ( Info2,"<ADC1=true>" );
                }
                else if ( flagtcs.Az_LoopStatus && !flagtcs.ADC1_LoopStatus  )
                {
                        sprintf ( RV,"-1" );
                        sprintf ( Info,"TrackStopFalse" );
                        sprintf ( Info1,"<Az=false>" );
                }
                else if ( !flagtcs.Az_LoopStatus && flagtcs.ADC1_LoopStatus  )
                {
                        sprintf ( RV, "-1" );
                        sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info2,"<ADC1=false>" );
                }
                else if ( flagtcs.Az_LoopStatus && flagtcs.ADC1_LoopStatus  )
                {
                        sprintf ( RV, "-1" );
                        sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info1,"<Az=false>" );
                        sprintf ( Info2,"<ADC1=false>" );
                }

		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		OcsCom.TrackStopMode = 0;
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_20 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while ( QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.Az_LoopStatus && !flagtcs.Dero_LoopStatus )
                {
                        sprintf ( RV,"0" );
                        sprintf ( Info,"OK" );
                        sprintf ( Info1,"<Az=true>" );
                        sprintf ( Info2,"<C1=true>" );
                }
                else if ( flagtcs.Az_LoopStatus && !flagtcs.Dero_LoopStatus)
                {
                        sprintf ( RV,"-1" );
                        sprintf ( Info,"TrackStopFalse" );
                        sprintf ( Info1,"<Az=false>" );
                }
                else if ( !flagtcs.Az_LoopStatus && flagtcs.Dero_LoopStatus )
                {
                        sprintf ( RV, "-1" );
                        sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info2,"<Dero=false>" );
                }
                else if ( flagtcs.Az_LoopStatus && flagtcs.Dero_LoopStatus )
                {
                        sprintf ( RV,"-1" );
                        sprintf ( Info,"TrackStopFalse" );
                        sprintf ( Info1,"<Az=false>" );
                        sprintf ( Info2,"<Dero=false>" );
                }

		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		OcsCom.TrackStopMode = 0;
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_24 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while ( QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

		if ( !flagtcs.Alt_LoopStatus && !flagtcs.Az_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
			sprintf ( Info1,"<Alt=true>" );
			sprintf ( Info2,"<Az=true>" );
		}
                else if ( flagtcs.Alt_LoopStatus && !flagtcs.Az_LoopStatus )
		{
			sprintf ( RV,"-1" );
			sprintf ( Info,"TrackStopFalse" );
			sprintf ( Info1,"<Alt=false>" );
		}
                else if (  !flagtcs.Alt_LoopStatus && flagtcs.Az_LoopStatus  )
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
			sprintf ( Info2,"<Az=false>" );
		}
                else if (  flagtcs.Alt_LoopStatus && flagtcs.Az_LoopStatus  )
                {
                        sprintf ( RV, "-1" );
                        sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info1,"<Alt=false>" );
                        sprintf ( Info2,"<Az=false>" );
                }

		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		OcsCom.TrackStopMode = 0;
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_25 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while ( QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.Alt_LoopStatus && !flagtcs.Az_LoopStatus && !flagtcs.ADC2_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
			sprintf ( Info1,"<Alt=true>" );
			sprintf ( Info2,"<Az=true>" );
			sprintf ( Info3,"<N2=true>" );
		}
		if ( flagtcs.Alt_LoopStatus )
		{
			sprintf ( RV,"-1" );
			sprintf ( Info,"TrackStopFalse" );
			sprintf ( Info1,"<Alt=false>" );
		}
		if ( flagtcs.Az_LoopStatus )
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
			sprintf ( Info2,"<Az=false>" );
		}
                if ( flagtcs.ADC2_LoopStatus )
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
			sprintf ( Info3,"<N2=false>" );
		}

		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		OcsCom.TrackStopMode = 0;
	}
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_26 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while ( QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.Alt_LoopStatus && !flagtcs.Az_LoopStatus && !flagtcs.ADC1_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
			sprintf ( Info1,"<Alt=true>" );
			sprintf ( Info2,"<Az=true>" );
			sprintf ( Info3,"<N1=true>" );
		}
		if ( flagtcs.Alt_LoopStatus )
		{
			sprintf ( RV,"-1" );
			sprintf ( Info,"TrackStopFalse" );
			sprintf ( Info1,"<Alt=false>" );
		}
		if ( flagtcs.Az_LoopStatus )
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
			sprintf ( Info2,"<Az=false>" );
		}
                if ( flagtcs.ADC1_LoopStatus )
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
			sprintf ( Info3,"<N1=false>" );
		}

		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		OcsCom.TrackStopMode = 0;
    }
	if( OcsCom.TrackStopMode == TRACKINGSTOPMODE_28 )
	{
		if ( pushButton_stop->isEnabled() )
		{
			on_pushButton_stop_clicked();
		}
		sprintf ( ExeStatus, "START" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		QTime dieTime = QTime::currentTime().addMSecs(1000);
		while ( QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}

                if ( !flagtcs.Alt_LoopStatus && !flagtcs.Az_LoopStatus && !flagtcs.Dero_LoopStatus )
		{
			sprintf ( RV,"0" );
			sprintf ( Info,"OK" );
			sprintf ( Info1,"<Alt=true>" );
			sprintf ( Info2,"<Az=true>" );
			sprintf ( Info3,"<C1=true>" );
		}
		if ( flagtcs.Alt_LoopStatus )
		{
			sprintf ( RV,"-1" );
			sprintf ( Info,"TrackStopFalse" );
			sprintf ( Info1,"<Alt=false>" );
		}
		if ( flagtcs.Az_LoopStatus )
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
			sprintf ( Info2,"<Az=false>" );
		}
                if ( flagtcs.Dero_LoopStatus )
		{
			sprintf ( RV, "-1" );
			sprintf ( Info, "TrackStopFalse" );
                        sprintf ( Info3,"<Dero=false>" );
		}

		CombineStringNew ( OcsCom.TrackStopUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";
	
		sprintf ( ExeStatus, "DONE" );
		CombineString ( OcsCom.TrackStopUid,ExeStatus,bExeStatus,DataBack,&s_temp );
		SendToOCS ( s_temp );
		s_temp="";

		OcsCom.TrackStopMode = 0;
    }*/
}


void Tracking::GetTrackingPosByOCS_MA()
{
    QString s_temp,s_temp1;
	char ExeStatus[50]="";
	char RV[10]="";
	char Info[20];

    char bExeStatus[10]="true";
	QString DataBack="";

    sprintf ( ExeStatus, "Start" );
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    //CombineString ( OcsCom.GetTrackPosUid,ExeStatus,bExeStatus,&s_temp );
    MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );

    //QString Mode = QString::number( OcsCom.TrackStartMode );
    DataBack =QString ( "<Az=%1><Alt=%2><Dero=%3>" ).arg ( trackstar.AzPosRealForError,0,'f',8 ).arg ( trackstar.AltPosRealForError,0,'f',8 ).arg ( trackstar.DeroPosRealForError,0,'f',8 );


    sprintf ( ExeStatus, "Done" );
    //CombineString ( OcsCom.GetTrackPosUid,ExeStatus,bExeStatus,&s_temp1 );
    MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );

}

void Tracking::TrackStarByOCS_MA()
{


    flagtcs.flag_TrackingStarByOCS_MA = true;

    QString s_temp;
    char ExeStatus[10]="";
    char bExeStatus[10]="true";
    char RV[10]="";
    char Info[20];
    QString DataBack="";

    sprintf ( ExeStatus, "Start" );
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    CombineString ( OcsCom.StatusCUID_Track,ExeStatus,bExeStatus,&s_temp );
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );

    if ( !timer_TrackreplyOCS->isActive () )
        timer_TrackreplyOCS->start ( TrackReplyClock );	//start check every 5 seconds

    on_pushButton_track_clicked();

    /*if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_0 )
	{
        on_checkBox_Az_toggled ( false );
        on_checkBox_Alt_toggled ( false );
        on_checkBox_Dero_toggled ( false );
        on_checkBox_ADC1_toggled ( false );
        on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_1 )
	{
         on_checkBox_Az_toggled ( false );
         on_checkBox_Alt_toggled ( false );
         on_checkBox_Dero_toggled ( false );
         on_checkBox_ADC1_toggled ( false );
         on_checkBox_ADC2_toggled ( true );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_2 )
	{
         on_checkBox_Az_toggled ( false );
         on_checkBox_Alt_toggled ( false );
         on_checkBox_Dero_toggled ( false );
         on_checkBox_ADC1_toggled ( true );
         on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_4 )
	{
         on_checkBox_Az_toggled ( false );
         on_checkBox_Alt_toggled ( false );
         on_checkBox_Dero_toggled ( true );
         on_checkBox_ADC1_toggled ( false );
         on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_8 )
	{
         on_checkBox_Az_toggled ( false );
         on_checkBox_Alt_toggled ( true );
         on_checkBox_Dero_toggled ( false );
         on_checkBox_ADC1_toggled ( false );
         on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_9 )
	{
         on_checkBox_Az_toggled ( false );
         on_checkBox_Alt_toggled ( true );
         on_checkBox_Dero_toggled ( false );
         on_checkBox_ADC1_toggled ( false );
         on_checkBox_ADC2_toggled ( true );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_10 )
	{
         on_checkBox_Az_toggled ( false );
         on_checkBox_Alt_toggled ( true );
         on_checkBox_Dero_toggled ( false );
         on_checkBox_ADC1_toggled ( true );
         on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_12 )
	{
         on_checkBox_Az_toggled ( false );
         on_checkBox_Alt_toggled ( true );
         on_checkBox_Dero_toggled ( true );
         on_checkBox_ADC1_toggled ( false );
         on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_16 )
	{
         on_checkBox_Az_toggled ( true );
         on_checkBox_Alt_toggled ( false );
         on_checkBox_Dero_toggled ( false );
         on_checkBox_ADC1_toggled ( false );
         on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_17 )
	{
         on_checkBox_Az_toggled ( true );
         on_checkBox_Alt_toggled ( false );
         on_checkBox_Dero_toggled ( false );
         on_checkBox_ADC1_toggled ( false );
         on_checkBox_ADC2_toggled ( true );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_18 )
	{
         on_checkBox_Az_toggled ( true );
         on_checkBox_Alt_toggled ( false );
         on_checkBox_Dero_toggled ( false );
         on_checkBox_ADC1_toggled ( true );
         on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_20 )
	{
         on_checkBox_Az_toggled ( true );
         on_checkBox_Alt_toggled ( false );
         on_checkBox_Dero_toggled ( true );
         on_checkBox_ADC1_toggled ( false );
         on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_24 )
	{
         on_checkBox_Az_toggled ( true );
         on_checkBox_Alt_toggled ( true );
         on_checkBox_Dero_toggled ( false );
         on_checkBox_ADC1_toggled ( false );
         on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_25 )
	{
         on_checkBox_Az_toggled ( true );
         on_checkBox_Alt_toggled ( true );
         on_checkBox_Dero_toggled ( false );
         on_checkBox_ADC1_toggled ( false );
         on_checkBox_ADC2_toggled ( true );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_26 )
	{
         on_checkBox_Az_toggled ( true );
         on_checkBox_Alt_toggled ( true );
         on_checkBox_Dero_toggled ( false );
         on_checkBox_ADC1_toggled ( true );
         on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
	}
	if( OcsCom.TrackStartMode == TRACKINGSTARTMODE_28 )
	{
         on_checkBox_Az_toggled ( true );
         on_checkBox_Alt_toggled ( true );
         on_checkBox_Dero_toggled ( true );
         on_checkBox_ADC1_toggled ( false );
         on_checkBox_ADC2_toggled ( false );

		OcsCom.TrackStartMode = 0;
    }*/



}
void Tracking::TrackreplyOCS() 									// reply OCS every 5 seconds,t
{
	if ( flagtcs.flag_TrackingStarByOCS_MA )
		TrackingStarByOCS_MA();

}
/*input:
         OcsCom.StatusCUID_MA   ,Global Variable
*/
void Tracking::TrackingStarByOCS_MA()
{
	flagtcs.flag_TrackStarByOCS_MA=false;
    //flagtcs.flag_TrackStarStopByOCS_MA=false;

    QString s_temp,s_temp1;
    char ExeStatus[10]="Active";
    char RV[10]="0";
    char Info[30];
    char bExeStatus[10]="";
	QString DataBack="";

    if( !trackstar.StarTableEnd && flagtcs.Az_Amplifier && flagtcs.Alt_Amplifier)// && flagtcs.Dero_Amplifier
	{
        if ( trackstar.flagAzPointing )
        DataBack =  QString ( "<Az=%1><Alt=%2><Dero=%3>" ).arg ( trackstar.AzTarget,0,'f',8 ).arg ( trackstar.AltPosRealForError,0,'f',8 ).arg ( trackstar.DeroPosRealForError,0,'f',8 );
        else
        DataBack =  QString ( "<Az=%1><Alt=%2><Dero=%3>" ).arg ( trackstar.AzPosRealForError,0,'f',8 ).arg ( trackstar.AltPosRealForError,0,'f',8 ).arg ( trackstar.DeroPosRealForError,0,'f',8 );

        sprintf ( Info,"OK" );
        //CombineStringNew ( OcsCom.TrackStartUid,RV,Info,Info1,Info2,Info3,Info4,Info5,Info6,bExeStatus,DataBack,&s_temp );
        MakeString ( OcsCom.StatusCUID_Track,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        SendToOCS ( s_temp );
    }

	else
	{
		if ( timer_TrackreplyOCS -> isActive () )
		{
            timer_TrackreplyOCS->stop();
		}

		on_pushButton_stop_clicked();

		flagtcs.flag_TrackingStarByOCS_MA = false;

        sprintf ( RV,"-1" );
        if(trackstar.StarTableEnd)sprintf ( Info,"star table end" );
        else if(!flagtcs.Az_Amplifier)sprintf ( Info,"Az amplifier error" );
        else if(!flagtcs.Alt_Amplifier)sprintf ( Info,"Alt amplifier error" );
        else sprintf ( Info,"unknown error" );
        MakeString ( OcsCom.StatusCUID_Track,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        SendToOCS ( s_temp );
        QTest::qWait(250);

        sprintf ( ExeStatus, "Done" );
        CombineString ( OcsCom.StatusCUID_Track,ExeStatus,bExeStatus,&s_temp1 );
       // MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        SendToOCS ( s_temp1 );

	}
}
void Tracking::GetStarTime_ini()
{
/////////////////////////////get local time/////////////////////////////////////

	time_t timep;
	struct tm *p;
	time ( &timep );
	p=gmtime ( &timep ); 										/*type: time_t, UTC */

    double utc= p->tm_hour*3600+  p->tm_min*60.+ p->tm_sec ;
    //qDebug() <<p->tm_hour<<p->tm_min<<p->tm_sec;
    //qDebug() <<p->tm_mon+1<<p->tm_mday<<p->tm_year+1900;
	cal_mjd ( p->tm_mon+1,p->tm_mday, p->tm_year+1900,&mjd0 );
    //qDebug() <<"mjd0="<<mjd0;
	double Lat=0,Lng=0;
	Lng=SMySetting::instance_p()->value ( "misc/longitudeD" ).toDouble() + ( SMySetting::instance_p()->value ( "misc/longitudeM" ).toInt() ) /60.+ ( SMySetting::instance_p()->value ( "misc/longitudeS" ).toDouble() ) /3600.;
	Lat=SMySetting::instance_p()->value ( "misc/latitudeD" ).toDouble() +SMySetting::instance_p()->value ( "misc/latitudeM" ).toInt() /60.+SMySetting::instance_p()->value ( "misc/latitudeS" ).toDouble() /3600.;
	np.n_lat=degrad ( Lat );
	np.n_lng=degrad ( Lng );
//  	np.n_lat=degrad ( 32+3./60.+58/3600. );								//Nanjing purple mountain P.M.O  latitude 32.061°
//	np.n_lng=degrad ( 118+49./60.+28./3600. );							//Nanjing  purple mountain  P.M.O longitude 118.79125°

	now_lst ( mjd0,np.n_lng,&lst0 );								//once
	lst = 1.002737909* ( utc ) /3600.0 + lst0+ radhr ( np.n_lng );					//every time
	range ( &lst, 24.0 );
    qDebug() <<"lat"<<Lat <<"lng"<<Lng<<"mjd0"<<mjd0<<"lst0"<<lst0;
}


void Tracking::GetStarTime()
{
////// ///////////////////////get local time/////////////////////////////////////

// 	time_t timep;
// 	struct tm *p;
// 	time ( &timep );
// 	p=gmtime ( &timep );							 			//get time_t   UTC
// 	double utc_= p->tm_hour*3600+  p->tm_min*60.+ p->tm_sec;

	QDateTime dateTime;
	dateTime = QDateTime::currentDateTime();
	dateTime = dateTime.toUTC();
	QTime time;
	time=dateTime.time();
	double utc_=0;
    utc_= ( time.hour() ) *3600+time.minute() *60.+time.second() +deltaUt +time.msec() /1000.0;				//utc time

	lst = 1.002737909* ( utc_ ) /3600.0 + lst0+ radhr ( np.n_lng );					//every time
	range ( &lst, 24.0 );
    //qDebug()<<"utc "<<utc_<<"lst"<<lst;
	int T_d=0,T_m=0;
	double T_s=0;
	T_d=int ( lst );
	T_m=int ( ( lst-T_d ) *60 );
	T_s= ( ( lst-T_d ) *60.-T_m ) *60. ;
 
	T_m=abs ( T_m );
	T_s=fabs ( T_s );

	char s[100];
	sprintf ( s,"%d h  %d m  %5.3f s",T_d,T_m,T_s );
	label_StarTimer->setText ( s );
	label_StarTimer->setStyleSheet ( "color:rgb(0, 85, 0)" );
    label_StarTimer->setFont ( QFont ( "Times", 14, QFont::Bold ) );


}


void Tracking::D2DMS ( double Degree,int* Deg,int* Min,double* Sec ) 					//change degree vlaue as degrees+minute+seconds
{

	*Deg=int ( Degree );
	*Min=int ( ( Degree-*Deg ) *60 );
	*Sec= ( ( Degree-*Deg ) *60-*Min ) *60;

	if ( *Deg!=0 )
		*Min=abs ( *Min );
	if ( *Deg||*Min )
		*Sec=fabs ( *Sec );
}
void  Tracking::D2HMS(double Degree,int* Hour,int* Min,double* Sec)					//change degree vlaue as hour+minute+seconds
{
	Degree=Degree/15.;
	*Hour=int ( Degree );
	*Min=int ( ( Degree-*Hour ) *60 );
	*Sec= ( ( Degree-*Hour ) *60-*Min ) *60;
	if ( *Hour!=0 )
		*Min=abs ( *Min );
	if ( *Hour||*Min )
		*Sec=fabs ( *Sec );
}



void  Tracking::CalLMST()
{/*
	QString dtstr;
	QDateTime dateTime;
	double ra, dec, epoctime, outra, outdec, lmstime, az, alt, rot, ha;
	int deg=0,min=0,hour=0;
	double sec=0;
	char s[100];
        //TimeAndCoordinate timecoordinate;
	//LOCAL time
	dateTime = QDateTime::currentDateTime();
	dtstr=dateTime.toString ( "yyyy-MM-dd hh:mm:ss" );
	// ui->lineEdit_main_localtime->setText(dtstr);
	
	//UTC time
	dateTime = dateTime.toUTC();
	dtstr=dateTime.toString ( "yyyy-MM-dd hh:mm:ss" );
	
	//MJD time
	double epochtime;
	epochtime = calculate_epoctime ( dateTime );
	dtstr = QString::number ( epochtime,'f',8 );
	//    ui->lineEdit_main_mjdtime->setText(dtstr);
	
	//LMST time
	double lmsttime;
	//double lmst_hms;
	lmsttime = calculate_lmstime ( dateTime );
	//dtstr = QString::number ( lmsttime,'f',8 );
  	D2HMS(lmsttime,&hour,&min,&sec);
	sprintf ( s,"%d h  %d m  %5.3f s",hour,min,sec );
	label_LMST->setText ( s );
	label_LMST->setStyleSheet ( "color:rgb(0, 85, 0)" );
	label_LMST->setFont ( QFont ( "Times", 10, QFont::Bold ) );
	
	//j2000 to current
        //timecoordinate.Epoch2Actual ( ra, dec, epoctime, outra, outdec );
	ha = lmsttime - ra;
	
	//raw RA/DEC
	double Azi=trackstar.AzPosRealForError;
	double Alt=trackstar.AltPosRealForError;
	double GLatitude=0,GLongitude=0;
	GLongitude=SMySetting::instance_p()->value ( "misc/longitudeD" ).toDouble() + ( SMySetting::instance_p()->value ( "misc/longitudeM" ).toInt() ) /60.+ ( SMySetting::instance_p()->value ( "misc/longitudeS" ).toDouble() ) /3600.;
	GLatitude=SMySetting::instance_p()->value ( "misc/latitudeD" ).toDouble() +SMySetting::instance_p()->value ( "misc/latitudeM" ).toInt();
	
	double RA_raw,DEC_raw;
        //timecoordinate.AltAzi2Eq ( Azi, Alt,  lmsttime, GLongitude , GLatitude , RA_raw,  DEC_raw );
	//dtstr = QString::number ( RA_raw,'f',8 );
	qDebug()<<"RA_raw"<<RA_raw;
        D2HMS(RA_raw,&hour,&min,&sec);
	sprintf ( s,"%d h  %d m  %5.3f s",hour,min,sec );
	label_RA_raw->setText ( s );
	label_RA_raw->setStyleSheet ( "color:rgb(0, 85, 0)" );
	label_RA_raw->setFont ( QFont ( "Times", 10, QFont::Bold ) );

	D2DMS ( DEC_raw,&deg,&min,&sec );
	sprintf ( s,"%d d  %d m  %5.3f s",deg,min,sec );
	label_DEC_raw->setText ( s );
	label_DEC_raw->setStyleSheet ( "color:rgb(0, 85, 0)" );
	label_DEC_raw->setFont ( QFont ( "Times", 10, QFont::Bold ) );


*/
 
}



double Tracking::calculate_epoctime(QDateTime datetime)
{
    int year,month,day,hour,minute,second,microsec;
    //TimeAndCoordinate timecoordinate;
    //UTC time
    datetime = datetime.toUTC();
    QDate date = datetime.date();
    QTime time = datetime.time();
    year = date.year();
    month = date.month();
    day = date.day();
    hour = time.hour();
    minute = time.minute();
    second = time.second();
    microsec = time.msec() ;
    //return timecoordinate.GetEpoch(year,month,day,hour,minute,second,microsec);
    return 0;
}

 





double Tracking::calculate_lmstime(QDateTime datetime)
{
    int year,month,day,hour,minute,second,microsec;
    //TimeAndCoordinate timecoordinate;
	double GLatitude=0,GLongitude=0;
	GLongitude=SMySetting::instance_p()->value ( "misc/longitudeD" ).toDouble() + ( SMySetting::instance_p()->value ( "misc/longitudeM" ).toInt() ) /60.+ ( SMySetting::instance_p()->value ( "misc/longitudeS" ).toDouble() ) /3600.;
	GLatitude=SMySetting::instance_p()->value ( "misc/latitudeD" ).toDouble() +SMySetting::instance_p()->value ( "misc/latitudeM" ).toInt() /60.+SMySetting::instance_p()->value ( "misc/latitudeS" ).toDouble() /3600.;
    //UTC time
    datetime = datetime.toUTC();
    QDate date = datetime.date();
    QTime time = datetime.time();
    year = date.year();
    month = date.month();
    day = date.day();
    hour = time.hour();
    minute = time.minute();
    second = time.second();
    microsec = time.msec() ;
   // return timecoordinate.GetLMST(GLongitude, GLatitude, year, month, day, hour, minute, second, microsec);
    return 0;
}


// void Tracking::CheckPointingORTracking()
// {
// 	QString str;
//  	char cmd[255];
//  	sprintf ( cmd,CmdCheckPointingORTracking );
// 	QStringList  list;
// 	int n=3;
// 	GetStatusFromUMAC (  cmd,n, &list );
// 	bool flagAzStopClock=false,flagAltStopClock=false,flagDeroStopClock=false;
// 	//qDebug()<<"timer_CheckPointingORTracking";
// ///////////      Az ////////////////////////////
// 	if ( list[0]== "1" ) 										//AZ  =1：pointing
// 	{
// 		//label_az_ampli_enabled->setStyleSheet ( "background-color:rgb(0,170, 0)" );  		// green,no error
// 		trackstar.flagAzPointing=true;
// 		trackstar.flagAzTracking=false;
// 	}
// 	else if ( list[0]== "2" ) 									//AZ  =2:tracking
// 	{
// 		//label_az_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 0, 0)" );		// red color,error
// 		trackstar.flagAzPointing=false;
// 		trackstar.flagAzTracking=true;
// 	}
// 	else
// 	{
// 		trackstar.flagAzPointing=false;
// 		trackstar.flagAzTracking=false;
// 	}
// ///////////    Alt ////////////////////////////
// 
// 	if ( list[1]== "1" ) 										//Alt  =1：pointing
// 	{
// 		//label_az_ampli_enabled->setStyleSheet ( "background-color:rgb(0,170, 0)" );  		// green,no error
// 		trackstar.flagAltPointing=true;
// 		trackstar.flagAltTracking=false;
// 	}
// 	else if ( list[1]== "2" ) 									//Alt  =2:tracking
// 	{
// 		//label_az_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 0, 0)" );		// red color,error
// 		trackstar.flagAltPointing=false;
// 		trackstar.flagAltTracking=true;
// 	}
// 	else
// 	{
// 		trackstar.flagAltPointing=false;
// 		trackstar.flagAltTracking=false;
// 	}
// ///////////    Dero ////////////////////////////
// 
// 	if ( list[2]== "1" ) 										//Dero  =1：pointing
// 	{
// 		//label_az_ampli_enabled->setStyleSheet ( "background-color:rgb(0,170, 0)" );  		// green,no error
// 		trackstar.flagDeroPointing=true;
// 		trackstar.flagDeroTracking=false;
// 	}
// 	else if ( list[2]== "2" ) 									//Dero  =2:tracking
// 	{
// 		//label_az_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 0, 0)" );		// red color,error
// 		trackstar.flagDeroPointing=false;
// 		trackstar.flagDeroTracking=true;
// 	}
// 	else
// 	{
// 		trackstar.flagDeroPointing=false;
// 		trackstar.flagDeroTracking=false;
// 	}
// 
// 	if ( flagtcs.flag_AZIMUTH )
// 	{
// 		if ( trackstar.flagAzTracking )
// 			{flagAzStopClock=true;setPIDparam_Az_tracking();}
// 	}
// 	else
// 		flagAzStopClock=true;
// 
// 	if ( flagtcs.flag_ALTITUDE )
// 	{
// 		if ( trackstar.flagAltTracking )
// 			{flagAltStopClock=true;setPIDparam_Alt_tracking();qDebug()<<"ALt tracking";}
// 	}
// 	else
// 		flagAltStopClock=true;
// 
// 
// 	if ( flagtcs.flag_C1||flagtcs.flag_N2||flagtcs.flag_N1 )
// 	{
// 		if ( trackstar.flagDeroTracking &&trackstar.flag_DeropointEnd)
// 			{flagDeroStopClock=true;setPIDparam_C1_tracking();setPIDparam_N1_tracking();}
// 	}
// 	else
// 		flagDeroStopClock=true;
// 
// 
// 
// 
// 	if ( flagAzStopClock&&flagAltStopClock&&flagDeroStopClock )
// 	{
// 		timer_CheckPointingORTracking->stop();
//                 qDebug() <<"timer_CheckPointingORTracking  Stop";
// 	}
// 
// 
// }
/*
void Tracking::CheckPointingORTracking()
{
	QString str;
 	char cmd[255];
 	sprintf ( cmd,CmdCheckPointingORTracking );
	QStringList  list;
	int n=3;
	GetStatusFromUMAC (  cmd,n, &list );
	bool flagAzStopClock=false,flagAltStopClock=false,flagDeroStopClock=false;
	//qDebug()<<"timer_CheckPointingORTracking";

	if ( flagtcs.flag_AZIMUTH )
	{
		///////////      Az ////////////////////////////
		if ( list[0]== "1" ) 										//AZ  =1：pointing
		{
			trackstar.flagAzPointing=true;
			trackstar.flagAzTracking=false;
		}
		else if ( list[0]== "2" ) 									//AZ  =2:tracking
		{
			trackstar.flagAzPointing=false;
			trackstar.flagAzTracking=true;
			flagAzStopClock=true;
			setPIDparam_Az_tracking();
		}
		else
		{
			trackstar.flagAzPointing=false;
			trackstar.flagAzTracking=false;
		}
	}
	else
		flagAzStopClock=true;

	if ( flagtcs.flag_ALTITUDE )
	{
		///////////    Alt ////////////////////////////

		if ( list[1]== "1" ) 										//Alt  =1：pointing
		{
			trackstar.flagAltPointing=true;
			trackstar.flagAltTracking=false;
		}
		else if ( list[1]== "2" ) 									//Alt  =2:tracking
		{
			trackstar.flagAltPointing=false;
			trackstar.flagAltTracking=true;
			flagAltStopClock=true;
			setPIDparam_Alt_tracking();
			qDebug()<<"ALt tracking";
		}
		else
		{
			trackstar.flagAltPointing=false;
			trackstar.flagAltTracking=false;
		}
	}
	else
		flagAltStopClock=true;

	///////////    Dero ////////////////////////////
	if ( flagtcs.flag_C1||flagtcs.flag_N2||flagtcs.flag_N1 )
	{
		if ( list[2]== "1" ) 										//Dero  =1：pointing
		{
			trackstar.flagDeroPointing=true;
			trackstar.flagDeroTracking=false;
		}
		else if ( list[2]== "2" ) 									//Dero  =2:tracking
		{
			trackstar.flagDeroPointing=false;
			trackstar.flagDeroTracking=true;
			flagDeroStopClock=true;
			if(flagtcs.flag_C1)
				setPIDparam_C1_tracking();
			if(flagtcs.flag_N1)
				setPIDparam_N1_tracking();
			if(flagtcs.flag_N2)
				setPIDparam_N2_tracking();
		}
		else
		{
			trackstar.flagDeroPointing=false;
			trackstar.flagDeroTracking=false;
		}
	}
	else
		flagDeroStopClock=true;

	if ( flagAzStopClock&&flagAltStopClock&&flagDeroStopClock )
	{
		timer_CheckPointingORTracking->stop();
                qDebug() <<"timer_CheckPointingORTracking  Stop";
	}
}*/

/*
void Tracking::CheckPointingORTrackingDero()
{
	QString str;
 	char cmd[255];
 	sprintf ( cmd,CmdCheckPointingORTrackingDero );
	QStringList  list;
	int n=1;
	GetStatusFromUMAC (  cmd,n, &list );
	bool flagDeroStopClock=false;
//	qDebug()<<"timer_CheckPointingORTracking";
 
/////////    Dero ////////////////////////////
	if ( flagtcs.flag_C1||flagtcs.flag_N2||flagtcs.flag_N1 )
	{
		if ( list[0]== "1" ) 										//Dero  =1：pointing
		{
			trackstar.flagDeroPointing=true;
			trackstar.flagDeroTracking=false;
		}
		else if ( list[0]== "2" ) 									//Dero  =2:tracking
		{
			trackstar.flagDeroPointing=false;
			trackstar.flagDeroTracking=true;
			flagDeroStopClock=true;
			if(flagtcs.flag_C1)
				setPIDparam_C1_tracking();
			if(flagtcs.flag_N1)
				setPIDparam_N1_tracking();
			if(flagtcs.flag_N2)
				setPIDparam_N2_tracking();

			timer_CheckPointingORTrackingDero->stop();
			qDebug() <<"Dero CheckPointingORTracking  Stop";
		}
		else
		{
			trackstar.flagDeroPointing=false;
			trackstar.flagDeroTracking=false;
		}
	}
	else
		{
			flagDeroStopClock=true;
			timer_CheckPointingORTrackingDero->stop();
		} 
 
}*/



void Tracking::CheckPointingORTrackingDero()
{
 
	bool flagDeroStopClock=false;
        float fData[10];
	int iDatalength=1;
        uint16_t offset=M481_addr;
        SQUMAC::instance_p()->QgetDataDPRAM (fData,  offset, iDatalength);
	float DeroPoint=fData[0];
/////////    Dero ////////////////////////////
        if ( flagtcs.flag_DERO||flagtcs.flag_ADC2||flagtcs.flag_ADC1 )
	{
		if ( DeroPoint== 1 ) 										//Dero  =1：pointing
		{
			trackstar.flagDeroPointing=true;
			trackstar.flagDeroTracking=false;
		}
		else if ( DeroPoint== 2 ) 									//Dero  =2:tracking
		{

			trackstar.flagDeroPointing=false;
			trackstar.flagDeroTracking=true;
			flagDeroStopClock=true;
                        if(flagtcs.flag_DERO)
				setPIDparam_C1_tracking();
                        if(flagtcs.flag_ADC1)
				setPIDparam_N1_tracking();
                        if(flagtcs.flag_ADC2)
				setPIDparam_N2_tracking();

			timer_CheckPointingORTrackingDero->stop();
			qDebug() <<"Dero CheckPointingORTracking  Stop";
		}
		else
		{
			trackstar.flagDeroPointing=false;
			trackstar.flagDeroTracking=false;
		}
	}
	else
		{
			flagDeroStopClock=true;
			timer_CheckPointingORTrackingDero->stop();
		} 
 
}
void Tracking::CheckPointingORTrackingAlt()
{
 
	bool flagAltStopClock=false;


        int iData[10];
	int iDatalength=1;
        uint16_t offset=M1781_addr;
        SQUMAC::instance_p()->QgetDataDPRAM (iData,  offset, iDatalength);
    int AltPoint=iData[0];

	if ( flagtcs.flag_ALTITUDE )
	{
		///////////      ALT ////////////////////////////
		if ( AltPoint== 1) 										// =1：pointing
		{
			trackstar.flagAltPointing=true;
			trackstar.flagAltTracking=false;
		}
		else if ( AltPoint== 2) 									//  =2:tracking
		{
			trackstar.flagAltPointing=false;
			trackstar.flagAltTracking=true;
			flagAltStopClock=true;
            setPIDparam_Alt_tracking(0);
			timer_CheckPointingORTrackingAlt->stop();
                        qDebug() <<"Alt  CheckPointingORTracking  Stop";
		}
		else
		{
			trackstar.flagAltPointing=false;
			trackstar.flagAltTracking=false;
		}
	}
	else
		{
			flagAltStopClock=true;
			timer_CheckPointingORTrackingAlt->stop();
		}
}
void Tracking::CheckPointingORTrackingAz()
{
//	QString str;
 //	char cmd[255];
 //	sprintf ( cmd,CmdCheckPointingORTrackingAz);
//	QStringList  list;
//	int n=1;
//	GetStatusFromUMAC (  cmd,n, &list );
	bool flagAzStopClock=false;


        /*int iData[10];
	int iDatalength=1;
        uint16_t offset=M2181_addr;
        SQUMAC::instance_p()->QgetDataDPRAM (iData,  offset, iDatalength);*/
    int AzPoint=CheckUmacFlag_Az(M2181_addr);



	if ( flagtcs.flag_AZIMUTH )
	{
		///////////      Az ////////////////////////////
		if ( AzPoint== 1) 									//AZ  =1：pointing
		{
            trackstar.flagAzPointing=true;
			trackstar.flagAzTracking=false;
		}
		else if ( AzPoint== 2) 									//AZ  =2:tracking
		{
			trackstar.flagAzPointing=false;
			trackstar.flagAzTracking=true;
			flagAzStopClock=true;
            //setPIDparam_Az_tracking();
			timer_CheckPointingORTrackingAz->stop();
            qDebug() <<"Az  CheckPointingORTracking  Stop";
		}
		else
		{
			trackstar.flagAzPointing=false;
			trackstar.flagAzTracking=false;
		}
	}
	else
		{
			flagAzStopClock=true;
			timer_CheckPointingORTrackingAz->stop();
		}
}
void Tracking::CheckStatus()
{
 	char cmd[255];
	sprintf ( cmd,CheckAZALTLIMIT );
 	CheckAzAltLimit(cmd);
    CheckAmplifier();
    CheckError();
    if(trackdelay<0)checkCommandFromOCS();
    else trackdelay--;

    /*if ( flagtcs.flag_DERO )
    {
        sprintf ( cmd,CheckDeroERROR );CheckDeroError ( cmd );
        sprintf ( cmd,CheckDeroAMPLIFIER );CheckDeroAmplifier ( cmd );
        sprintf ( cmd,CheckDeroLIMIT );CheckDeroLimit ( cmd );
    }
    if ( flagtcs.flag_ADC2 )
    {
        sprintf ( cmd,CheckADC2ERROR );CheckDeroError ( cmd );
        sprintf ( cmd,CheckADC2AMPLIFIER );CheckDeroAmplifier ( cmd );
    }
    if ( flagtcs.flag_ADC1 )
    {
        sprintf ( cmd,CheckADC1ERROR );CheckDeroError ( cmd );
        sprintf ( cmd,CheckADC1AMPLIFIER );CheckDeroAmplifier ( cmd );
                 //sprintf ( cmd,CheckADC1LIMIT );CheckDeroLimit ( cmd );

    }*/

}

/*
function :  Get Status From UMAC ,such as limit,home...
//input:
char cmd[];			//command that send out to UMAC
int n;     			//number of commands


output:

QStringList  Status		//Status of hardware
*/
void  GetStatusFromUMAC ( char cmd[],int n,QStringList* Status )
{
	QString str;
	int it=0;
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	*Status= str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );

	for ( QStringList::iterator point = ( *Status ).begin(); point != ( *Status ).end();++it, ++point )
		{}
	if ( it<n )
	{
		//UMAC_ETHERNET=false;
		return;
	}
	//else
		//UMAC_ETHERNET=true;
}




//Check LIMIT information of az alt
//M131M132M231M232
void Tracking::CheckAzAltLimit( char cmd[255] )
{
	QString str;
	QStringList  list;
	int n=4;
    GetStatusFromUMAC ( cmd,n, &list );
    bool data2ocs=false;
    char Info[40]="OK";
    char ID[20]="";

/////////// az LIM+    ////////////////////////////
    if ( !QString::compare ( list[0], "1" ))
	{
        if( !flagtcs.Az_Limit_P)
        {
            label_Status->setText ( "Az LIMIT+ is on." );
            flagtcs.Az_Limit_P=1;
            writeLog->writeerror("Az  LIMIT+","	Az LIMIT+");
            data2ocs=true;
            sprintf ( Info,"Az  LIMIT+" );
            sprintf ( ID,TCSAZLIMIT );
        }
	}
    else if (flagtcs.Az_Limit_P)flagtcs.Az_Limit_P=0;
// 		label_AzStatus->setText ( "" );
/////////// AZ LIM-   ////////////////////////////
    if ( !QString::compare ( list[1], "1" ) )
	{
        if(!flagtcs.Az_Limit_N)
        {
            label_Status->setText ( "Az LIMIT- is on." );
            flagtcs.Az_Limit_N=1;
            writeLog->writeerror("Az  LIMIT-","	Az LIMIT-");
            data2ocs=true;
            sprintf ( Info,"Az  LIMIT-" );
            sprintf ( ID,TCSAZLIMIT );
        }
	}
    else   if (flagtcs.Az_Limit_N)flagtcs.Az_Limit_N=0;
// 		label_AzStatus->setText ( "" );

/////////// alt LIM+    ////////////////////////////
    if ( !QString::compare ( list[2], "1" ) )
	{
        if( !flagtcs.Alt_Limit_P)
        {
            label_Status->setText ( "Alt LIMIT+ is on." );
            flagtcs.Alt_Limit_P=1;
            writeLog->writeerror("Alt  LIMIT+","	Alt LIMIT+");
            data2ocs=true;
            sprintf ( Info,"Alt  LIMIT+" );
            sprintf ( ID,TCSALTLIMIT );
        }
	}
    else  if (flagtcs.Alt_Limit_P) flagtcs.Alt_Limit_P=0;
// 		label_AltStatus->setText ( "" );
/////////// Alt LIM-   ////////////////////////////
    if ( !QString::compare ( list[3], "1" ))
	{
        if( !flagtcs.Alt_Limit_N)
        {
            label_Status->setText ( "Alt LIMIT- is on." );
            flagtcs.Alt_Limit_N=1;
            writeLog->writeerror("Alt  LIMIT-","	Alt LIMIT-");
            data2ocs=true;
            sprintf ( Info,"Alt  LIMIT-" );
            sprintf ( ID,TCSALTLIMIT );
        }
	}
    else  if (flagtcs.Alt_Limit_N) flagtcs.Alt_Limit_N=0;
    if(!data2ocs)return;
    on_pushButton_stop_clicked();

    QString s_temp;
    char ExeStatus[10]="Done";
    char RV[10]="-1";
    char bExeStatus[10]="";
    QString DataBack="";


    MakeString ( ID,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
}



void Tracking::CheckAzaltAmplifier()
{
	QString str;
	char cmd[255];
        sprintf ( cmd,"M2138M1738" );//M554M654
	QStringList  list;
        int n=2;
	GetStatusFromUMAC ( cmd,n, &list );

///////////amplifier 1     Az ////////////////////////////
    if ( !QString::compare ( list[0], "0" ) ) 							//amplifier 1，=1：ok
	{
		label_az_ampli_enabled->setStyleSheet ( "background-color:rgb(0,170, 0)" );  		// green,no error
		flagtcs.Az_Amplifier=true;
	}
	else
	{
		label_az_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 0, 0)" );		// red color,error
		flagtcs.Az_Amplifier=false;
	}
///////////amplifier 1     Alt ////////////////////////////
    if (  !QString::compare ( list[1], "0" ) )	//amplifier1#2#  ，=1：ok
	{
		label_alt_ampli_enabled->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  		// green,no error
		flagtcs.Alt_Amplifier=true;
	}
	else
	{
		label_alt_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 0, 0)" );		// red color,error
		flagtcs.Alt_Amplifier=false;
	}
}

void Tracking::CheckAmplifier()
{
    QString str;
    char cmd[255];
    //sprintf ( cmd,"M554M654M239M339M439" );
    sprintf ( cmd,"M2138M1738M239M339M439" );
    QStringList  list;
        int n=5;
    GetStatusFromUMAC ( cmd,n, &list );

///////////amplifier      Az ////////////////////////////
    if ( !QString::compare ( list[0], "0" )) 							//amplifier 1，=1：ok
    {
       //if(!flagtcs.Az_Amplifier)
        //{
            label_az_ampli_enabled->setStyleSheet ( "background-color:rgb(0,170, 0)" );  		// green,no error
            flagtcs.Az_Amplifier=true;
            flagtcs.Az_LoopStatus=true;
        //}
    }
    else
    {
       // if(flagtcs.Az_Amplifier)
        //{
            label_az_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 0, 0)" );		// red color,error
            flagtcs.Az_Amplifier=false;
            flagtcs.Az_LoopStatus=false;
        //}
    }
///////////amplifier      Alt ////////////////////////////
    if (  !QString::compare ( list[1], "0" ) )	//amplifier1#2#  ，=1：ok
    {
        //if(!flagtcs.Alt_Amplifier)
        //{
            label_alt_ampli_enabled->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  		// green,no error
            flagtcs.Alt_Amplifier=true;
            flagtcs.Alt_LoopStatus=true;
        //}
    }
    else
    {
        //if(flagtcs.Alt_Amplifier)
        //{
            label_alt_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 0, 0)" );		// red color,error
            flagtcs.Alt_Amplifier=false;
            flagtcs.Alt_LoopStatus=false;
        //}
    }
////////// Dero ////////
    if ( !QString::compare ( list[2], "1" ) ) 							//amplifier 1，=1：ok
    {
        //if(!flagtcs.Dero_Amplifier)
        //{
            label_dero_ampli_enabled->setStyleSheet ( "background-color:rgb(0,170, 0)" );  		// green,no error
            flagtcs.Dero_Amplifier=true;
        //}
    }
    else
    {
        //if(flagtcs.Dero_Amplifier)
        //{
            label_dero_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 0, 0)" );		// red color,error
            flagtcs.Dero_Amplifier=false;
        //}
    }
    ////////// ADC1 ////////
    if ( !QString::compare ( list[3], "1" ) ) 							//amplifier 1，=1：ok
    {
       // if(!flagtcs.ADC1_Amplifier)
        //{
            label_ADC1_ampli_enabled->setStyleSheet ( "background-color:rgb(0,170, 0)" );  		// green,no error
            flagtcs.ADC1_Amplifier=true;
        //}
    }
    else
    {
        //if(flagtcs.ADC1_Amplifier)
        //{
            label_ADC1_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 0, 0)" );		// red color,error
            flagtcs.ADC1_Amplifier=false;
        //}
    }
    ////////// ADC2 ////////
    if ( !QString::compare ( list[4], "1" )) 							//amplifier 1，=1：ok
    {
        //if( !flagtcs.ADC2_Amplifier)
        //{
            label_ADC2_ampli_enabled->setStyleSheet ( "background-color:rgb(0,170, 0)" );  		// green,no error
            flagtcs.ADC2_Amplifier=true;
        //}
    }
    else
    {
        //if(flagtcs.ADC2_Amplifier)
        //{
            label_ADC2_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 0, 0)" );		// red color,error
            flagtcs.ADC2_Amplifier=false;
        //}
    }
}


void Tracking::CheckError()
{
    QString str;
	char cmd[255];
    sprintf ( cmd,"M2141M2142M1741M1742M241M242M341M342M441M442" );//following error, warning
	QStringList  list;
    int n=10;
	GetStatusFromUMAC ( cmd,n, &list );

/////////// Az ////////////////////////////
	if ( !QString::compare ( list[0], "1" ) )
        label_az_error->setStyleSheet ( "background-color:rgb(255, 255, 0)" );			// red color,error
    else if ( !QString::compare ( list[1], "1" ) )
        label_az_error->setStyleSheet ( "background-color:rgb(255, 0, 0)" );			// yellow color,warning
	else
		label_az_error->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  			// green,no error and no warning

//////////  ALT ////////////////////////////
    if (  !QString::compare ( list[2], "1" ) )
        label_alt_error->setStyleSheet ( "background-color:rgb(255, 255, 0)" );			// red color,error
    else if ( !QString::compare ( list[3], "1" ) )
        label_alt_error->setStyleSheet ( "background-color:rgb(255, 0, 0)" );			// yellow color,warning
	else
        label_alt_error->setStyleSheet ( "background-color:rgb(0, 170, 0)" );
   /////////// Dero ////////////////////////////
    if ( !QString::compare ( list[4], "1" ) )
        label_dero_error->setStyleSheet ( "background-color:rgb(255, 255, 0)" );			// red color,error
    else if ( !QString::compare ( list[5], "1" ) )
        label_dero_error->setStyleSheet ( "background-color:rgb(255, 0, 0)" );			// yellow color,warning
    else
        label_dero_error->setStyleSheet ( "background-color:rgb(0, 170, 0)" );
    /////////// ADC1 ////////////////////////////
    if ( !QString::compare ( list[6], "1" ) )
        label_ADC1_error->setStyleSheet ( "background-color:rgb(255, 255, 0)" );			// red color,error
    else if ( !QString::compare ( list[7], "1" ) )
        label_ADC1_error->setStyleSheet ( "background-color:rgb(255, 0, 0)" );			// yellow color,warning
    else
        label_ADC1_error->setStyleSheet ( "background-color:rgb(0, 170, 0)" );
    /////////// ADC2 ////////////////////////////
    if ( !QString::compare ( list[8], "1" ) )
        label_ADC2_error->setStyleSheet ( "background-color:rgb(255, 255, 0)" );			// red color,error
    else if ( !QString::compare ( list[9], "1" ) )
        label_ADC2_error->setStyleSheet ( "background-color:rgb(255, 0, 0)" );			// yellow color,warning
    else
        label_ADC2_error->setStyleSheet ( "background-color:rgb(0, 170, 0)" );
}

void Tracking::CheckAzaltError()
{
        QString str;
    char cmd[255];
        sprintf ( cmd,CmdCheckAzaltError );//following error, warning
    QStringList  list;
        int n=4;
    GetStatusFromUMAC ( cmd,n, &list );

///////////error1    warning1   Az ////////////////////////////
    if ( !QString::compare ( list[0], "1" ) )
        label_az_error->setStyleSheet ( "background-color:rgb(255, 0, 0)" );			// red color,error
        else if ( !QString::compare ( list[2], "1" ) )
        label_az_error->setStyleSheet ( "background-color:rgb(255, 255, 0)" );			// yellow color,warning
    else
        label_az_error->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  			// green,no error and no warning

//////////error2    warning2  ALT ////////////////////////////
        if (  !QString::compare ( list[1], "1" ) )
        label_alt_error->setStyleSheet ( "background-color:rgb(255, 0, 0)" );			// red color,error
        else if ( !QString::compare ( list[3], "1" ) )
        label_alt_error->setStyleSheet ( "background-color:rgb(255, 255, 0)" );			// yellow color,warning
    else
        label_alt_error->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  			// green,no error and no warning

}


void Tracking::CheckDeroError ( char cmd[255] )
{
	QStringList  list;
        int n=2;
	GetStatusFromUMAC ( cmd,n, &list );

	/////////error    warning    :1#2#channel  ////////////////////////////
        if (  !QString::compare ( list[0], "1" ) )
		label_dero_error->setStyleSheet ( "background-color:rgb(255, 0, 0)" );			// red color,error
        else if (  !QString::compare ( list[1], "1" ) )
		label_dero_error->setStyleSheet ( "background-color:rgb(255, 255, 0)" );		// yellow color,warning
	else
		label_dero_error->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  		// green,no error and no warning

}


void Tracking::CheckDeroAmplifier ( char cmd[255] )
{
	QStringList  list;
        int n=1;
	GetStatusFromUMAC ( cmd,n, &list );

	/////////error    warning    :1#2#channel  ////////////////////////////
        if (  !QString::compare ( list[0], "1" ) )	//amplifier =1：ok
		label_dero_ampli_enabled->setStyleSheet ( "background-color:rgb(0, 170, 0)" ); 	 	// green, ok
	else
		label_dero_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 0, 0)" );		// red color,error
}


void Tracking::CheckDeroLimit( char cmd[255] )
{
	QString str;
	QStringList  list;
	int n=2;
	GetStatusFromUMAC ( cmd,n, &list );
/////////// LIM+    ////////////////////////////
	if ( !QString::compare ( list[0], "1" ) )
	{
        label_Status->setText ( "Dero LIMIT+" );
		writeLog->writeerror("Dero  LIMIT+","	Dero LIMIT+");
	}
// 	else
// 		label_DeroStatus->setText ( "" );
/////////// LIM-   ////////////////////////////
	if ( !QString::compare ( list[1], "1" ) )
	{
        label_Status->setText ( "Dero LIMIT-" );
		writeLog->writeerror("Dero  LIMIT-","	Dero LIMIT-");
	}
// 	else
// 		label_DeroStatus->setText ( "" );
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
void Tracking::GetDataFromUMAC ( char cmd[],int n,double cts,double* PosReal_Degree,double* FolErrUMAC,double* Velocity,double Seconds_old,double* Seconds ,int PointerLast,int* Pointer,double ( *p ) [6],QString DataTime[],int num )
{
	QString str;
	double PosCmd;											//command  position
	double PosReal;											//real position,seconds
	double FolErrPC;										//following error  ,calculated by PC

	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	QStringList  list = str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	int it=0;
	for ( QStringList::iterator point = list.begin(); point != list.end();++it, ++point )
		{}
	if ( it<n )
	{
//		UMAC_ETHERNET=false;
		return;
	};

	PosCmd=list[0].toDouble() *cts/3072.0;								// command, float value

	PosReal= ( ( list[1].toDouble() +list[2].toDouble() ) /3072.0 ) *cts; 				// arcsec of real position
	*PosReal_Degree=PosReal/3600.0;									// degree

	*FolErrUMAC=list[3].toDouble() *cts/3072.0;							// following error  ,calculated by UMAC
	FolErrPC=PosCmd-PosReal;									//following error ,calculated by PC,arcsec
	*Velocity=list[4].toDouble() *cts*1000*2/3072.0;						// change counts/ms to "/s

	QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );
	int msecond=time.msec();
	*Seconds=time.second() *1000+msecond;								//ms

	if ( fabs ( *Seconds-Seconds_old ) >=90&&*Pointer<= ( num-1 ) ) 				//every 100ms
	{
		* ( * ( p+*Pointer ) +0 ) =PosCmd;							//Data[Pointer][0]
		* ( * ( p+*Pointer ) +1 ) =PosReal;							//Data[Pointer][1]
		* ( * ( p+*Pointer ) +2 ) =*FolErrUMAC;							//Data[Pointer][2]
		* ( * ( p+*Pointer ) +3 ) =FolErrPC;							//Data[Pointer][3]


		DataTime[*Pointer]=dtstr;
		*Pointer=PointerLast+1;
		//*Seconds_old=*Seconds;
	}
}


/*


output:AzTrack.azSeconds,Global Variable
	AzTrack.azPointer,Global Variable
input: AzTrack.azPosReal_Degree ,Global Variable
	AzTrack.azSeconds,Global Variable
	AzTrack.azPointer,Global Variable

*/
// void Tracking::GetAzPosition()
// {
// 	char  cmd[255];
// 	sprintf ( cmd,CmdGetAzPosition );
// 	int n=5;
// 	double azFolErrUMAC=0;
//  
// 
// 
// 	GetDataFromUMAC ( cmd,n,CTS,&AzTrack.azPosReal_Degree,&azFolErrUMAC,&AzTrack.AzVelocity,AzTrack.azSeconds,&AzTrack.azSeconds,AzTrack.azPointer,&AzTrack.azPointer,AzTrack.AzData,AzTrack.AzDataTime,M );
//  
// }


void Tracking::GetAzPosition()
{
    SQUMAC::instance_p()->Qgetinformation ( Motor21_addr, Motor[5].followingError, Motor[5].servoCommand,
                                            Motor[5].servoStatus, Motor[5].generalStatus,
                                            Motor[5].positionBias, Motor[5].filteredActuralVelocity,
                                            Motor[5].masterPosition, Motor[5].netActualPosition );
	
        AzTrack.PosReal_Degree= ( Motor[5].netActualPosition /3072.0 ) *CTS_az/3600;		//degree
    trackstar.AzFolErrUMAC= ( Motor[5].followingError /3072.0 ) *CTS_az;
        AzTrack.Velocity=  Motor[5].filteredActuralVelocity *CTS_az*1000*2/3072.0;				// change counts/ms to "/s

}

void Tracking::freshAzPosition(double azPos,double azVel )
{
    char s[255];
    int Az_d,Az_m;
	double Az_s;
	D2DMS ( azPos,&Az_d,&Az_m,&Az_s );
    sprintf ( s,"%3d: %2d: %5.2f",Az_d,Az_m,Az_s );
    label_Az_Pos->setText ( s );

    sprintf ( s,"%5.1f",azVel );
    label_Az_Velocity->setText ( s );
}



/*
output:AltTrack.altSeconds,Global Variable
	AltTrack.altPointer,Global Variable
input: AltTrack.altPosReal_Degree ,Global Variable
	AltTrack.altSeconds,Global Variable
	AltTrack.altPointer,Global Variable
*/
// void Tracking::GetAltPosition()
// {
// 	char  cmd[255];
// 	sprintf ( cmd,CmdGetAltPosition );
// 	int n=5;									//number of cmd,eg.m261m262m264m265m274
// 	double altFolErrUMAC=0;
// 	 
// 
// 	GetDataFromUMAC ( cmd,n,CTS,&AltTrack.altPosReal_Degree,&altFolErrUMAC,&AltTrack.AltVelocity,AltTrack.altSeconds,&AltTrack.altSeconds,AltTrack.altPointer,&AltTrack.altPointer,AltTrack.AltData,AltTrack.AltDataTime,M );
//  
// }

void Tracking::GetAltPosition()
{
    SQUMAC::instance_p()->Qgetinformation ( Motor17_addr, Motor[6].followingError, Motor[6].servoCommand,
                                            Motor[6].servoStatus, Motor[6].generalStatus,
                                            Motor[6].positionBias, Motor[6].filteredActuralVelocity,
                                            Motor[6].masterPosition, Motor[6].netActualPosition );
	
    AltTrack.PosReal_Degree= ( Motor[6].netActualPosition /3072.0 ) *CTS_alt/3600;				//degree
    trackstar.AltFolErrUMAC= ( Motor[6].followingError /3072.0 ) *CTS_alt;
    AltTrack.Velocity=  Motor[6].filteredActuralVelocity *CTS_alt*1000*2/3072.0;				// change counts/ms to "/s

}
void Tracking::freshAltPosition(double altPos,double altVel )
{
    char s[255];
    int Alt_d,Alt_m;
	double Alt_s;
	D2DMS ( altPos,&Alt_d,&Alt_m,&Alt_s );
    sprintf ( s,"%2d: %2d: %5.2f",Alt_d,Alt_m,Alt_s );
    label_Alt_Pos->setText ( s );

    sprintf ( s,"%5.1f",altVel );
    label_Alt_Velocity->setText ( s );
}


/*
output:C1Track.c1Seconds,Global Variable
	C1Track.c1Pointer,Global Variable
input: C1Track.c1PosReal_Degree ,Global Variable
	C1Track.c1Seconds,Global Variable
	C1Track.c1Pointer,Global Variable
*/
// void Tracking::GetC1Position()
// {
// 	char  cmd[255];
// 	sprintf ( cmd,CmdGetC1Position );								//m474:C1 average velocity
// 	int n=5;
// 	double c1FolErrUMAC=0;
//  
// 
// 	GetDataFromUMAC ( cmd,n,CTS_C1,&C1Track.c1PosReal_Degree,&c1FolErrUMAC,&C1Track.C1Velocity,C1Track.c1Seconds,&C1Track.c1Seconds,C1Track.c1Pointer,&C1Track.c1Pointer,C1Track.C1Data,C1Track.C1DataTime,Q );
//  }


void Tracking::GetDeroPosition()
{
    SQUMAC::instance_p()->Qgetinformation ( Motor2_addr, Motor[2].followingError, Motor[2].servoCommand,
                                            Motor[2].servoStatus, Motor[2].generalStatus,
                                            Motor[2].positionBias, Motor[2].filteredActuralVelocity,
                                            Motor[2].masterPosition, Motor[2].netActualPosition );
	
        DeroTrack.PosReal_Degree= ( Motor[2].netActualPosition /3072.0 ) *CTS_Dero/3600;				//degree
        trackstar.DeroFolErrUMAC= ( Motor[2].followingError /3072.0 ) *CTS_Dero;
        DeroTrack.Velocity=  Motor[2].filteredActuralVelocity *CTS_Dero*1000*2/3072.0;				// change counts/ms to "/s

}

void Tracking::freshDeroPosition(double deroPos,double deroVel )
{
	int Dero_d,Dero_m;
	double Dero_s;
    char s[255];
    D2DMS ( deroPos,&Dero_d,&Dero_m,&Dero_s );
    sprintf ( s,"%3d: %2d: %5.2f",Dero_d,Dero_m,Dero_s );
    label_Dero_Pos->setText ( s );

    sprintf ( s,"%5.1f",deroVel );
    label_Dero_Velocity->setText ( s );
}

void Tracking::freshADC1Position(double ADCPos,double ADCVel )
{
    int ADC_d,ADC_m;
    double ADC_s;
    char s[255];
    D2DMS ( ADCPos,&ADC_d,&ADC_m,&ADC_s );
    sprintf ( s,"%3d: %2d: %5.2f",ADC_d,ADC_m,ADC_s );
    label_ADC1_Pos->setText ( s );

    sprintf ( s,"%5.1f",ADCVel );
    label_ADC1_Velocity->setText ( s );
}

void Tracking::freshADC2Position(double ADCPos,double ADCVel )
{
    int ADC_d,ADC_m;
    double ADC_s;
    char s[255];
    D2DMS ( ADCPos,&ADC_d,&ADC_m,&ADC_s );
    sprintf ( s,"%3d: %2d: %5.2f",ADC_d,ADC_m,ADC_s );
    label_ADC2_Pos->setText ( s );

    sprintf ( s,"%5.1f",ADCVel );
    label_ADC2_Velocity->setText ( s );
}

////////////////////////////   n1////////////////////////////////////
/*
 output: N1Track.n1PosReal_Degree ,Global Variable
	 N1Track.n1Seconds,Global Variable
	 N1Track.n1Pointer,Global Variable
input:	N1Track.N1Data,,Global Variable
     	N1Track.N1DataTime,Global Variable
	N1Track.n1Seconds,Global Variable
        N1Track.n1Pointer,Global Variable
*/
// void Tracking::GetN1Position()
// {
// 
// 	char  cmd[255];
// 	sprintf ( cmd,CmdGetN1Position );								//m874:N1 average velocity
// 	int n=5;
// 	double n1FolErrUMAC=0;
//  
// 	GetDataFromUMAC ( cmd,n,CTS_N1,&N1Track.n1PosReal_Degree,&n1FolErrUMAC,&N1Track.N1Velocity,N1Track.n1Seconds,&N1Track.n1Seconds,N1Track.n1Pointer,&N1Track.n1Pointer,N1Track.N1Data,N1Track.N1DataTime,Q );
// 
// }

void Tracking::GetN1Position()
{
    SQUMAC::instance_p()->Qgetinformation ( Motor7_addr, Motor[7].followingError, Motor[7].servoCommand, Motor[7].servoStatus, Motor[7].generalStatus, Motor[7].positionBias, Motor[7].filteredActuralVelocity, Motor[7].masterPosition, Motor[7].netActualPosition );
	
        ADC1Track.PosReal_Degree= ( Motor[7].netActualPosition /3072.0 ) *CTS_ADC/3600;				//degree
//	trackstar.DeroFolErrUMAC= ( Motor[7].followingError /3072.0 ) *CTS_N1;
        ADC1Track.Velocity=  Motor[7].filteredActuralVelocity *CTS_ADC*1000*2/3072.0;				// change counts/ms to "/s
}

////////////////////////////   n2////////////////////////////////////
/*
////output: 	N2Track.n2PosReal_Degree ,Global Variable
     		N2Track.n2Seconds,Global Variable
                N2Track.n2Pointer,Global Variable
    input:	N2Track.N2DataTime,Global Variable
       		N2Track.N2Data,Global Variable
                N2Track.n2Seconds,Global Variable
                N2Track.n2Pointer,Global Variable
*/
// void Tracking::GetN2Position()
// {
// 
// 	char  cmd[255];
// 	sprintf ( cmd,CmdGetN2Position );//m674:N2 average velocity
// 	int n=5;
// 	double n2FolErrUMAC=0;
// 	GetDataFromUMAC ( cmd,n,CTS_N2,&N2Track.n2PosReal_Degree,&n2FolErrUMAC,&N2Track.N2Velocity,N2Track.n2Seconds,&N2Track.n2Seconds,N2Track.n2Pointer,&N2Track.n2Pointer,N2Track.N2Data,N2Track.N2DataTime,Q );
// }

void Tracking::GetN2Position()
{
    SQUMAC::instance_p()->Qgetinformation ( Motor8_addr, Motor[5].followingError, Motor[5].servoCommand, Motor[5].servoStatus, Motor[5].generalStatus, Motor[5].positionBias, Motor[5].filteredActuralVelocity, Motor[5].masterPosition, Motor[5].netActualPosition );
	
        ADC2Track.PosReal_Degree= ( Motor[5].netActualPosition /3072.0 ) *CTS_ADC/3600;				//degree
//	trackstar.DeroFolErrUMAC= ( Motor[5].followingError /3072.0 ) *CTS_N2;
        ADC2Track.Velocity=  Motor[5].filteredActuralVelocity *CTS_ADC*1000*2/3072.0;				// change counts/ms to "/s
}




// void  Az_beginTrack()
// {
// 	QString str;
// 	char cmd[255];
// 	sprintf ( cmd,"P186=1" );  								
// 	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// }

void  Az_beginPoint()
{
        /*int iData[10];
	int iDatalength=1;
        iData[0]=1;
         uint16_t offset=M2181_addr;
        SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);*/
        SendIntToDPRAM(M2181_addr,1);
        qDebug() <<"Az Start point";
}

void  Az_beginTrack()
{
        /*int iData[10];
	int iDatalength=1;
        iData[0]=1;
         uint16_t offset=M2186_addr;
        SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);*/
        SendIntToDPRAM(M2186_addr,1);
        qDebug() <<"Az Start track";
}
void  Az_stopTrack()
{
       /*int iData[10];
	int iDatalength=1;
        iData[0]=2;
        uint16_t offset=M2186_addr;
        SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);*/
        SendIntToDPRAM(M2186_addr,2);
        qDebug() <<"Az Stop track";
}




// void  Az_stopTrack()
// {
// 	QString str;
// 	char cmd[255];
// 	sprintf ( cmd,"P186=2" );  								
// 	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// }
// void  Alt_beginTrack()
// {
// 	QString str;
// 	char cmd[255];
// 	sprintf ( cmd,"P386=1" );  								
// 	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// }
// void  Alt_stopTrack()
// {
// 	QString str;
// 	char cmd[255];
// 	sprintf ( cmd,"P386=2" );  								
// 	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// }
// void  Dero_beginTrack()
// {
// 	QString str;
// 	char cmd[255];
// 	sprintf ( cmd,"P486=1" );  								
// 	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// 	qDebug()<<" Dero_beginTrack,P486=1";
// }
// void  Dero_stopTrack()
// {
// 	QString str;
// 	char cmd[255];
// 	sprintf ( cmd,"P486=2" );  								
// 	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// }

void  IniDPRAM()
{

  	QString str;
 	char cmd[255]; 
    sprintf ( cmd,"I49=1I57=1I47=20I50=20M99=255" );
 	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    sprintf ( cmd,"M99=255" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    sprintf ( cmd,"I49=1" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    sprintf ( cmd,"I57=1" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    sprintf ( cmd,"I47=20" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    sprintf ( cmd,"I50=20" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}

void  Alt_beginPoint()
{
        /*int iData[10];
	int iDatalength=1;
        iData[0]=1;
         uint16_t offset=M1781_addr;
        SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);*/
        SendIntToDPRAM(M1781_addr,1);
        qDebug() <<"Alt Start point";
}
void  Alt_beginTrack()
{
        /*int iData[10];
	int iDatalength=1;
        iData[0]=1;
        uint16_t offset=M1786_addr;
        SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);*/
        SendIntToDPRAM(M1786_addr,1);
        qDebug() <<"Alt Start track";
}

void  Alt_stopTrack()
{
        /*int iData[10];
	int iDatalength=1;
        iData[0]=2;
        uint16_t offset=M1786_addr;
        SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);*/
        SendIntToDPRAM(M1786_addr,2);
        qDebug() <<"Alt Stop track";
}


void  Dero_beginPoint()
{
        /*int iData[10];
	int iDatalength=1;
        iData[0]=1;
         uint16_t offset=M281_addr;
        SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);*/
    SendIntToDPRAM(M281_addr,1);
    qDebug() <<"dero Start point";
}

void  Dero_beginTrack()
{
       /* int iData[10];
	int iDatalength=1;
        iData[0]=1;
         uint16_t offset=M286_addr;
        SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);*/
    SendIntToDPRAM(M286_addr,1);
    qDebug() <<"dero Start track";
}
void  Dero_stopTrack()
{
        /*int iData[10];
	int iDatalength=1;
        iData[0]=2;
        uint16_t offset=M286_addr;
        SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);*/
    SendIntToDPRAM(M286_addr,2);
    qDebug() <<"dero stop track";
}

void  ADC_stopTrack()
{
    trackstar.flagADC1Tracking=false;
    trackstar.flagADC2Tracking=false;
    char cmd[255];
    QString str;
    sprintf(cmd,"#3j/ #4j/");
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

///////////////////////////////////////////////////////////////////////////////////////////////
void    LookupTable_realtime() 							//look up table every 100ms
{ 
	double utc=0;
	QStringList  list,list3;
	StarTarget startarget;
 	QString cAzimuth,cAltitude,cDero;
	long ii;
	/////////////////////////////get UTC time/////////////////////////////////////
	QDateTime dt;
	dt=QDateTime::currentDateTime();
	QDate date;
	QTime time;
	date=dt.date();
	time=dt.time();
	utc= ( time.hour() ) *3600+time.minute() *60.+time.second() +time.msec() /1000.0;						//local time
        NoX=long(utc*1000/TableLineTime);
        N_row=NoX-No1;
        if(N_row<0)N_row+=24*3600*1000/TableLineTime;

     //  qDebug() <<"No1"<<No1<<"NoX"<<NoX<<N_row;
	if ( N_row  > ( TableNum-1000 ) )
	{
		flagtrack.StarTableEnd=true;
		qDebug()<<"LookupTable_realtime  flagtrack.StarTableEnd=true"<<trackstar.RowDeroPoint;
		return;
	}

	list= starTable[N_row].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	if ( list.size() >=5 )
	{
		startarget.cAzimuth=list[2];
		startarget.cAltitude=list[3];
		startarget.cDero=list[4];
	}
 
 
	//target velocity
	list3= starTable[N_row+TargetNumPerSecond].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );		//for calculate average speed
	if ( list3.size() >=5 )
	{
		cAzimuth=list3[2];
		cAltitude=list3[3];
		cDero=list3[4];
	}
 	double az_1,az_2,alt_1,alt_2,dero_1,dero_2;
	az_1=startarget.cAzimuth.toDouble();
	az_2= cAzimuth.toDouble();
	alt_1=startarget.cAltitude.toDouble();
	alt_2= cAltitude.toDouble();
	dero_1=startarget.cDero.toDouble();
	dero_2= cDero.toDouble();
	
	trackstar.AZ_TrackTargetSpeed= ( az_2-az_1 ) *3600/((TargetNumPerSecond*TableLineTime)/1000);
	trackstar.ALT_TrackTargetSpeed= ( alt_2-alt_1 ) *3600/((TargetNumPerSecond*TableLineTime)/1000);
    trackstar.Dero_TrackTargetSpeed= ( dero_2-dero_1 ) *3600/((TargetNumPerSecond*TableLineTime)/1000);
	trackstar.AzTarget=startarget.cAzimuth.toDouble();
	trackstar.AltTarget=startarget.cAltitude.toDouble();
	trackstar.DeroTarget=startarget.cDero.toDouble();

	///////////////////////////////////get 10 line for 1(or 2) second/////////////////////////////////////////////////////////
    
    int num=AzTrackPeriod/TableLineTime;

       for ( int i=0;i<num;i++ )
	{
                ii=N_row+i*LinesEvery100ms;
		list3= starTable[ii].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
		if ( list3.size() >=5 )
		{
			cAzimuth=list3[2];
			cAltitude=list3[3];
			cDero=list3[4];
		}
		
		AzTrackTarget[i]=cAzimuth.toDouble();								//AZ target
        AltTrackTarget[i]=cAltitude.toDouble();								//ALT target
		DeroTrackTarget[i]=cDero.toDouble();			         				//Dero target
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////
void    LookupTable_Posix() 											//look up table every second
{ 
	double utc=0;
	QStringList  list,list3;
	StarTarget startarget;
 	QString cAzimuth,cAltitude,cDero;
	long ii;
	/////////////////////////////get UTC time/////////////////////////////////////
	QDateTime dt;
	dt=QDateTime::currentDateTime();
	QDate date;
	QTime time;
	date=dt.date();
	time=dt.time();
    utc= ( time.hour() ) *3600+time.minute() *60.+time.second() +deltaUt +time.msec() /1000.0;						//local time
        NoX=long(utc*1000/TableLineTime);
        N_row=NoX-No1;
        if(N_row<0)N_row+=24*3600*1000/TableLineTime;

 //       qDebug() <<"No1"<<No1<<"NoX"<<NoX<<N_row;
	if ( N_row  > ( TableNum-1000 ) )
	{
		flagtrack.StarTableEnd=true;
		qDebug()<<"LookupTable_Posix  flagtrack.StarTableEnd=true"<<trackstar.RowDeroPoint;
        return;
	}

	list= starTable[N_row].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	if ( list.size() >=5 )
	{
		startarget.cAzimuth=list[2];
        startarget.cAltitude=list[3];
		startarget.cDero=list[4];
	}
	//target velocity
	        int nextSecond=100;               									//100lines*10ms
		list3= starTable[N_row+nextSecond].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );		//for calculate average speed
	
	if ( list3.size() >=5 )
	{
		cAzimuth=list3[2];
		cAltitude=list3[3];
		cDero=list3[4];
	}
        

	double az_1,az_2,alt_1,alt_2,dero_1,dero_2;
	az_1=startarget.cAzimuth.toDouble();
	az_2= cAzimuth.toDouble();
	alt_1=startarget.cAltitude.toDouble();
	alt_2= cAltitude.toDouble();
	dero_1=startarget.cDero.toDouble();
	dero_2= cDero.toDouble();
	
	trackstar.AZ_TrackTargetSpeed= ( az_2-az_1 ) *3600/((nextSecond*TableLineTime)/1000);
	trackstar.ALT_TrackTargetSpeed= ( alt_2-alt_1 ) *3600/((nextSecond*TableLineTime)/1000);
    trackstar.Dero_TrackTargetSpeed= ( dero_2-dero_1 ) *3600/((nextSecond*TableLineTime)/1000);
	trackstar.AzTarget=startarget.cAzimuth.toDouble();
	trackstar.AltTarget=startarget.cAltitude.toDouble();
	trackstar.DeroTarget=startarget.cDero.toDouble();
	
	///////////////////////////////////get 10 line for 1(or 2) second/////////////////////////////////////////////////////////
    
    int num=AzTrackPeriod/TableLineTime;

       for ( int i=0;i<num;i++ )
	{
                ii=N_row+i*LinesEvery100ms;
		list3= starTable[ii].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
		if ( list3.size() >=5 )
		{
			cAzimuth=list3[2];
			cAltitude=list3[3];
			cDero=list3[4];
		}
		
		AzTrackTarget[i]=cAzimuth.toDouble();								//AZ target
		AltTrackTarget[i]=cAltitude.toDouble();								//ALT target
		DeroTrackTarget[i]=cDero.toDouble();			         				//Dero target
	}


}

void    LookupTable_Dero(bool firstTime) 											//look up table every second
{ 
	double utc=0;
	QStringList  list,list3;
    //StarTarget startarget;
    //long ii;
	double dero_1;
	double dero_2;
	/////////////////////////////get UTC time/////////////////////////////////////
	QDateTime dt;
	dt=QDateTime::currentDateTime();
	QDate date;
	QTime time;
	date=dt.date();
	time=dt.time();
    utc= ( time.hour() ) *3600+time.minute() *60.+time.second() +deltaUt +time.msec() /1000.0;						//local time
    NoX=long(utc*1000/TableLineTime);
    N_row=NoX-No1;
    if(N_row<0)N_row+=24*3600*1000/TableLineTime;
     //   qDebug() <<"No1"<<No1<<"NoX"<<NoX<<N_row;
	if ( N_row  > ( TableNum-1000 ) )
	{
		flagtrack.StarTableEnd=true;
		qDebug()<<"LookupTable_Dero  flagtrack.StarTableEnd=true";
		return;
	}
    //qDebug()<<"LookupTable_Dero num is "<<N_row<<" utc is "<<utc;
	list= starTable[N_row].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	if ( list.size() >=5 )
	{
        //startarget.cDero=list[4];
        dero_1=(list[4].toDouble()-list[3].toDouble())/2+180;
        if(DeroReverse)
        {
            if(dero_1>0)dero_1-=90;
            else if(dero_1<0)dero_1+=90;
        }
        if(flagtcs.Dero_ALTHALF)
            dero_1=(-list[3].toDouble())/2;
    }
	//int num=TrackPeriod/TableLineTime;
    /*int num=TrackPeriod*NumPerGroup/1000.*TargetNumPerSecond/LinesEvery100ms;
       for ( int i=0;i<num;i++ )
	{
                ii=N_row+i*LinesEvery100ms;
		list= starTable[ii].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
		if ( list.size() >=5 )
		{
			startarget.cDero=list[4];
 
		}
		
		DeroTrackTarget[i]=startarget.cDero.toDouble();								//Dero target
    }*/

	//target velocity
    list3= starTable[N_row+20].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );		//for calculate average speed
	if ( list3.size() >=5 )
	{
        dero_2=(list3[4].toDouble()-list3[3].toDouble())/2+180;
        if(DeroReverse)
        {
            if(dero_2>0)dero_2-=90;
            else if(dero_2<0)dero_2+=90;
        }
        if(flagtcs.Dero_ALTHALF)
            dero_2=(-list[3].toDouble())/2;
	}
    trackstar.Dero_TrackTargetSpeedLast=trackstar.Dero_TrackTargetSpeed;
    trackstar.Dero_TrackTargetSpeed= ( dero_2-dero_1 ) *3600/2;
    if(firstTime)
    {
        derotrackstar.deltaTime=time.msec()%100;
        derotrackstar.N_row=N_row;
        derotrackstar.Target_Group2[DeroNumPerGroup-1]=trackstar.DeroPosRealForError*3600/CTS;
        trackstar.Dero_TrackTargetSpeedLast=0;
    }

    if(trackstar.flagAltTracking && flagtcs.Dero_ALTHALF) trackstar.DeroTarget= -alttrackstar.Target_Last/7200.;
    else trackstar.DeroTarget=dero_1+(trackstar.Dero_TrackTargetSpeedLast*0.001*(time.msec()%100))/3600.;
}

void  LookAzTarget(long row,double *azTarget)
{
	QStringList  list;
	StarTarget startarget;
	list= starTable[row].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	if ( list.size() >=5 )
		startarget.cAzimuth=list[2];

	*azTarget=startarget.cAzimuth.toDouble();
}


void    LookupTable_Az(bool firstTime) 											//look up table every second
{
    //int deltatime=0;
    double utc=0;
    QStringList  list,list3;
	StarTarget startarget;
    //long ii;
	double az_1,az_2;

	QDateTime dt;
	dt=QDateTime::currentDateTime();
	QDate date;
	QTime time;
	date=dt.date();
	time=dt.time();
    utc=  time.hour()  *3600+time.minute() *60.+time.second() +deltaUt +time.msec()/1000;						//local time
    NoX=utc*1000/TableLineTime;
    N_row=NoX-No1;
    if(N_row<0)N_row+=24*3600*1000/TableLineTime;
    if ( N_row  > ( TableNum-1000 ) )
	{
        flagtrack.StarTableEnd=true;
		qDebug()<<"LookupTable_Az  flagtrack.StarTableEnd=true"<<N_row;
		return;
	}
    //qDebug()<<"LookupTable_Az num is "<<N_row<<" utc is "<<utc;
	list= starTable[N_row].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	if ( list.size() >=5 )
        startarget.cAzimuth=list[2];//////////////
	
    az_1=startarget.cAzimuth.toDouble();
    if(az_1>270 && StarAtNorth_Az==-1)az_1-=360;
    else if(az_1<90 && StarAtNorth_Az==1)az_1+=360;

    /*if(abs(aztrackstar.N_row-N_row)>2)
    {
        //qDebug() <<"aztrackstar.N_row"<<aztrackstar.N_row
                //<<"N_row"<<N_row;
        aztrackstar.N_row=N_row;
    }*/

    /*qDebug() <<"lookup table, time.msec()"<<time.msec()
             <<"utc"<<utc<<"now row"<<N_row
             <<"aztrackstar.N_row "<<aztrackstar.N_row;*/
	//target velocity
    list3= starTable[N_row+20].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );		//for calculate average speed
    if ( list3.size() >=5 )startarget.cAzimuth=list3[2];////
        az_2=startarget.cAzimuth.toDouble();
	
    if(az_2>270 && StarAtNorth_Az==-1)az_2-=360;
    else if(az_2<90 && StarAtNorth_Az==1)az_2+=360;
    trackstar.AZ_TrackTargetSpeedLast=trackstar.AZ_TrackTargetSpeed;
    trackstar.AZ_TrackTargetSpeed= ( az_2-az_1 ) *3600/2;
    //qDebug() <<"utc"<<utc<<"now row"<<N_row<<"az_1"<<az_1<<"az_2"<<az_2
    //             <<"aztrack Speed"<<trackstar.AZ_TrackTargetSpeed;

    if(firstTime)
    {
        aztrackstar.N_row=N_row;
        aztrackstar.deltaTime=time.msec()%100;
        aztrackstar.Target_Group2[AzNumPerGroup-1]=trackstar.AzPosRealForError*3600/CTS;
        aztrackstar.Vel_Group2[AzNumPerGroup-1]=0;
        trackstar.AZ_TrackTargetSpeedLast=0;
        aztrackstar.Target_Last=trackstar.AzPosRealForError*3600;
        aztrackstar.Vel_Last=0;
    }
    //trackstar.AzTarget=az_1+ (trackstar.AZ_TrackTargetSpeed*0.001*aztrackstar.deltaTime)/3600.;
    trackstar.AzTarget=az_1+ (trackstar.AZ_TrackTargetSpeedLast*0.001*(time.msec()%100))/3600.;
}

void    LookupTable_Alt(bool firstTime) 					//look up table every second
{ 
    double utc=0;
	QStringList  list,list3;
	StarTarget startarget;
    //long ii;
	double alt_1,alt_2;
	/////////////////////////////get UTC time/////////////////////////////////////
	QDateTime dt;
	dt=QDateTime::currentDateTime();
	QDate date;
	QTime time;
	date=dt.date();
	time=dt.time();

    utc=  time.hour() *3600+time.minute() *60.+time.second() +deltaUt +time.msec() /1000.0;						//local time
    NoX=utc*1000/TableLineTime;
    N_row=NoX-No1;
    if(N_row<0)N_row+=24*3600*1000/TableLineTime;
    /*qDebug() <<"time.msec()"<<time.msec()
             <<"deltatime"<<alttrackstar.deltaTime
             <<"utc"<<utc<<"now number"<<N_row
             <<"send Number "<<alttrackstar.N_row;*/
	if ( N_row  > ( TableNum-1000 ) )
	{
		flagtrack.StarTableEnd=true;
		qDebug()<<"LookupTable_Alt  flagtrack.StarTableEnd=true"<<trackstar.RowDeroPoint;
		return;
	}
    //qDebug()<<"LookupTable_Alt num is "<<N_row<<" utc is "<<utc;
    list= starTable[N_row].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	if ( list.size() >=5 )
	{
		startarget.cAltitude=list[3];
	}
	alt_1=startarget.cAltitude.toDouble();


    //if(abs(alttrackstar.N_row-N_row)>5)alttrackstar.N_row=N_row;

    //target velocity
    list3= starTable[N_row+20].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );		//for calculate average speed
	
    if ( list3.size() >=5 )startarget.cAltitude=list3[3];
    alt_2= startarget.cAltitude.toDouble();
    trackstar.ALT_TrackTargetSpeedLast=trackstar.ALT_TrackTargetSpeed;
    trackstar.ALT_TrackTargetSpeed= ( alt_2-alt_1 ) *3600/2;

    if(firstTime)
    {
        alttrackstar.deltaTime= time.msec()%100;
        alttrackstar.N_row=N_row;
        alttrackstar.Target_Group2[1]=trackstar.AltPosRealForError*3600/CTS;
        trackstar.ALT_TrackTargetSpeedLast=0;
    }
    //trackstar.AltTarget=alt_1+ (trackstar.ALT_TrackTargetSpeed*0.001*alttrackstar.deltaTime)/3600.;
    trackstar.AltTarget=alt_1+ (trackstar.ALT_TrackTargetSpeedLast*0.001*(time.msec()%100))/3600.;
}
void  handle(union sigval v)
{


    //CheckUmacFlag();
//	GetAzAltDeroPosition_UMAC();
	//AZ
    //GetAzPosition_DPRAM();
    if ( flagtcs.flag_AZIMUTH ) azTrack();
	//ALT
    //GetAltPosition_DPRAM();
    if (flagtcs.flag_ALTITUDE && !trackstar.flagAltRange) altTrack();

	//DERO
    //GetDeroPosition_DPRAM();
    if ( flagtcs.flag_DERO ) deroTrack();
    if((TrackSeconds%5)==0)
    {
        //qDebug()<<" Az model = "<<az_model<<" Alt model = "<<alt_model;
        GetADC1Position_DPRAM();
        GetADC2Position_DPRAM();
        if ( flagtcs.flag_ADC1 || flagtcs.flag_ADC2 ) ADCTrack();
        if ( !flagtcs.flag_DERO )GetDeroPosition_DPRAM();
        /*if(trackstar.flagAltTracking&&trackstar.flag_AltpointEnd)
            CalAltAverageError();
        if(trackstar.flagAzTracking&&trackstar.flag_AzpointEnd)
            CalAzAverageError();
        if(trackstar.flagDeroTracking&&trackstar.flag_DeropointEnd)
            CalDeroAverageError();*/

        //TrackDataForSave ();
        if ( flagtcs.flag_AZIMUTH && flagtcs.flag_ALTITUDE)
        {
            totalerror=sqrt(trackstar.AltTrackError*trackstar.AltTrackError+trackstar.AzTrackError*trackstar.AzTrackError*
                       cos(degrad(trackstar.AltPosRealForError))*cos(degrad(trackstar.AltPosRealForError)));

        }

    }

	
    //QString dts;
    //double azPos,altPos,deroPos;
    //Cal ( &azPos,&altPos,&deroPos, &dts );

	int overrun=timer_getoverrun ( PosixT );
	if ( overrun!=0 )
		qDebug() <<"overrun time"<<overrun;

    /*if(TrackSeconds==4)
    {
        TrackSeconds=0;
    }
    else */TrackSeconds++;
	return;
}

void  azTrack()
{
    //long Az_row=N_row;
    int num= 4;

    if( !flagtcs.flag_AZIMUTH)
    return;

    if(AzTrackSeconds==0)
        GetAzPosition_DPRAM();
    //LookupTable_Az();
    if(trackstar.flagAzPointing)
    {
        time_AzpointEnd=time_AzpointEnd-PosixTClock/1000000;
        //qDebug()<<"Az AzpointEnd_time"<<time_AzpointEnd;

        if(time_AzpointEnd<50)
        {
            setPIDparam_Az_tracking();
            GetEndTime();
            qDebug() <<"Az Point  first Used Time:"<<timeuse;
            AzGroup=CheckUmacFlag_Az(M185_addr);
            if(AzGroup==1)
            {
                LookupTable_Az(1);
                AzGroupLast=3;
                AzMakeCommand(aztrackstar.N_row);
                if(AzToUMAC_Posix())
                {
                    Az_beginTrack();
                    trackstar.flagAzPointing=false;
                    trackstar.flagAzTracking=true;
                    AzTrackSeconds=0;
                    AzGroupLast=AzGroup;
                }
            }
        }
    }
    if(AzTrackSeconds==0 && trackstar.flagAzPointing)
    {
        trackstar.AzTrackError=(trackstar.AzTarget-trackstar.AzPosRealForError)*3600;

    }

    if(AzTrackSeconds==0 && trackstar.flagAzTracking)
    {
        LookupTable_Az();
        trackstar.AzTrackError =(trackstar.AzTarget-trackstar.AzPosRealForError)*3600
                               +trackstar.az_correct_all + az_model;
                                                           // +trackstar.az_correct_all
        trackstar.AzFolErrUMACTest[trackstar.az_DataCount]=trackstar.AzTrackError;

        if(trackstar.az_DataCount>=Az_DataTotal)
        {
            double terr=0;
            for ( int z=0;z<=Az_DataTotal; z++ )
            {
                terr+=trackstar.AzFolErrUMACTest[z];
                trackstar.AzFolErrUMACTest[z]=0;
            }
            trackstar.AzFolErrUMACTest[10]=terr/(Az_DataTotal+1);

            if(flagtcs.AutoCorrect && fabs(trackstar.AZ_TrackTargetSpeed)>0.1)
            {
                if(fabs(trackstar.AZ_TrackTargetSpeed)<50)
                {
                    terr= trackstar.AzFolErrUMACTest[10]*0.6/trackstar.AZ_TrackTargetSpeed*1000;
                    Az_DataTotal=9;
                }
                else if(fabs(trackstar.AZ_TrackTargetSpeed)<500)
                {
                    terr= trackstar.AzFolErrUMACTest[10]*0.7/trackstar.AZ_TrackTargetSpeed*1000;
                    Az_DataTotal=7;
                }
                else if(fabs(trackstar.AZ_TrackTargetSpeed)<1000)
                {
                    terr= trackstar.AzFolErrUMACTest[10]*0.8/trackstar.AZ_TrackTargetSpeed*1000;
                    Az_DataTotal=5;
                }
                else if(fabs(trackstar.AZ_TrackTargetSpeed)<1800)
                {
                    terr= trackstar.AzFolErrUMACTest[10]*0.85/trackstar.AZ_TrackTargetSpeed*1000;
                    Az_DataTotal=4;
                }
                else
                {
                    terr= trackstar.AzFolErrUMACTest[10]*0.9/trackstar.AZ_TrackTargetSpeed*1000;
                    Az_DataTotal=3;
                }
                if(fabs(trackstar.AZ_TrackTargetSpeed)<10000 && fabs(trackstar.AZ_TrackTargetSpeed)>0.1)
                {
                    if(terr>500)terr=500;
                    else if(terr<-500)terr=-500;
                    terr+=aztrackstar.deltaTime;
                    aztrackstar.N_row+=int(terr)/100;
                    aztrackstar.deltaTime=int(terr)%100;
                }
            }
            trackstar.az_DataCount=0;
        }
       else trackstar.az_DataCount++;
        /*qDebug()<<" Az Target "<<trackstar.AzTarget
                <<" err "<<trackstar.AzTrackError
                <<" err/total "<<trackstar.AzFolErrUMACTest[10]
                <<" Speed "<<trackstar.AZ_TrackTargetSpeed
               <<"N_row"<<aztrackstar.N_row
                <<"delta ms"<<aztrackstar.deltaTime
                <<"Az_DataTotal"<<Az_DataTotal;*/
    //	Az_row=aztrackstar.N_rowAz+deltaLinesAz+trackstar.AzCorrectLines+SoftwareDelayAz*TargetNumPerSecond/1000.;

        //Az_row=aztrackstar.N_row+SoftwareDelayAz*TargetNumPerSecond/1000.;
        //Az_row=Az_row+10;
        //AzGroup=CheckUmacFlag_Az(M2184_addr);
        //AzMakeCommand(Az_row);
        //AzToUMAC_Posix();
        //AzTrackSeconds=0;
        //AzGroupLast=AzGroup;
    }

    if(trackstar.flagAzTracking)
    {
        AzGroup=CheckUmacFlag_Az(M185_addr);
        /*if(AzGroup<4 && AzGroup!=AzGroupLast )
        {
            if(fabs(trackstar.AZ_TrackTargetSpeed)>1000)
            {
                AzTrackSeconds_delta=AzTrackSeconds-AzTrackSeconds_last;
                while(AzTrackSeconds_delta<10)AzTrackSeconds_delta+=10;
            }
            else AzTrackSeconds_delta=10;
            AzTrackSeconds_last=AzTrackSeconds;
        }*/
        if(AzGroup<4 && AzGroup!=AzGroupLast && AzTrackSeconds<=num && AzTrackSeconds>1)
        {
            if(AzGroup!=AzGroupLast)aztrackstar.N_row=aztrackstar.N_row+AzNumPerGroup*AzTrackPeriod/100;//+10
            AzMakeCommand(aztrackstar.N_row);
            AzToUMAC_Posix();
            //qDebug()<<"AzTrackSeconds "<<AzTrackSeconds
            //        <<" data to Az group "<<AzGroup<<" number "<< aztrackstar.N_row;
                     // <<"AzTrackSeconds_delta"<<AzTrackSeconds_delta;
            AzGroupLast=AzGroup;
        }
        double target;
        int iTarget=AzTrackSeconds/3;
        if( AzGroup==1 ||AzGroup==10)target=aztrackstar.Target_Group2[iTarget]*CTS/3600;
        else target=aztrackstar.Target_Group1[iTarget]*CTS/3600;
        /*if( AzGroup==1 ||AzGroup==10)trackstar.AzTargetOffset=(aztrackstar.Target_Group2[iTarget]-
                                           aztrackstar.Target_Group2[NumPerGroup-1])*CTS/3600;
        else trackstar.AzTargetOffset=(aztrackstar.Target_Group1[iTarget]-
                                 aztrackstar.Target_Group1[NumPerGroup-1])*CTS/3600;*/
       /* qDebug()<<"AzTrackSeconds "<<AzTrackSeconds
               <<" Az Group "<<AzGroup<<" last group "<<AzGroupLast
                <<" Target "<<target;*/
                //<<" Pos "<<trackstar.AzPosRealForError;
               // <<" A "<<int(target*36000)%36000
                //<<" P "<<int(trackstar.AzPosRealForError*36000)%36000
               // <<" last "<<int(trackstar.AzTargetOffset*36000)%36000;
    }

    if(AzTrackSeconds>=num) AzTrackSeconds=0;
    else AzTrackSeconds++;


//        if( trackstar.flag_AzpointEnd)
//         {
// 		LookAzTarget(Az_row+AzTrackSeconds*LinesEvery100ms,&trackstar.AzTargetOffset);
// 		//trackstar.AzTargetOffset= trackstar.AzTargetOffset+trackstar.az_correct[int(AzTrackSeconds/10)]/3600+trackstar.az_guidedata/3600;
// 		trackstar.AzTargetOffset= trackstar.AzTargetOffset+trackstar.az_guidedata/3600;
//
// 	}

//        if( trackstar.flagAzCorrectPoint&&trackstar.flag_AzpointEnd)
//    		AzCorrectPoint();
//        if(!trackstar.flagAzCorrectPoint&&trackstar.flagAzTracking)
//        {	AzCorrectTarget();
// 		AzCorrect();
// 	}
//
}


void  altTrack()
{
    int num= 4;
    //long Alt_row;

   	if( !flagtcs.flag_ALTITUDE)
		return;

    if(AltTrackSeconds>=num)
        GetAltPosition_DPRAM();
    if(trackstar.flagAltPointing)
    {

        time_AltpointEnd=time_AltpointEnd-PosixTClock/1000000;
             //qDebug()<<"Alt AltpointEnd_time"<<time_AltpointEnd<<" Alt Group "<<AltGroup;

        if(time_AltpointEnd<0)
        {
            setPIDparam_Alt_tracking(0);
            AltGroup=CheckUmacFlag_Alt(M186_addr);
            if(AltGroup==1)
            {
              LookupTable_Alt(1);

              AltMakeCommand(alttrackstar.N_row);
              if(AltToUMAC_Posix())
              {
                  setPIDparam_Alt_tracking(0);
                  Alt_beginTrack();
                  trackstar.flagAltPointing=false;
                  trackstar.flagAltTracking=true;
                  AltTrackSeconds=0;
                  AltGroupLast=AltGroup;
              }
            }
        }
    }

    if(AltTrackSeconds==0 && trackstar.flagAltPointing)
    {
        trackstar.AltTrackError= (trackstar.AltTarget-trackstar.AltPosRealForError)*3600;
    }

    if(AltTrackSeconds==0 && trackstar.flagAltTracking)
	{
        LookupTable_Alt();
        trackstar.AltTrackError=(trackstar.AltTarget-trackstar.AltPosRealForError)*3600
                                +trackstar.alt_correct_all+ alt_model;
        trackstar.AltFolErrUMACTest[trackstar.alt_DataCount]=trackstar.AltTrackError;

        if(trackstar.alt_DataCount>=4)
        {
            double terr=0;
            for ( int z=0;z<=4; z++ )
            {
                terr+=trackstar.AltFolErrUMACTest[z];
                trackstar.AltFolErrUMACTest[z]=0;
            }
            trackstar.AltFolErrUMACTest[10]=terr/5;

            if(flagtcs.AutoCorrect && fabs(trackstar.ALT_TrackTargetSpeed)>0.1)
            {

                terr= trackstar.AltFolErrUMACTest[10]*0.6/trackstar.ALT_TrackTargetSpeed*1000;
                if(terr>500)terr=500;
                else if(terr<-500) terr=-500;
                terr+=alttrackstar.deltaTime;
                alttrackstar.N_row+=int(terr)/100;
                alttrackstar.deltaTime=int(terr)%100;
            }
            trackstar.alt_DataCount=0;
        }
        else trackstar.alt_DataCount++;
       /*qDebug()<<" Alt Target "<<trackstar.AltTarget
                <<" err "<<trackstar.AltTrackError
                <<" err/total "<<trackstar.AltFolErrUMACTest[10]
                <<" Speed "<<trackstar.ALT_TrackTargetSpeed
               <<"N_row"<<alttrackstar.N_row
                <<"delta ms"<<alttrackstar.deltaTime;*/

        /*if(fabs(trackstar.ALT_TrackTargetSpeed)<0.02)
        {
            if(AltTrackPeriod!=2000 )
            {
                AltTrackPeriod=2000;
                AltChangePeriod=true;
            }
        }

        else if(fabs(trackstar.ALT_TrackTargetSpeed)<0.04)
        {
            if(AltTrackPeriod!=1000 )
            {
                AltTrackPeriod=1000;
                AltChangePeriod=true;
            }
        }
        else if(fabs(trackstar.ALT_TrackTargetSpeed)>0.04)
        {
            if( AltTrackPeriod!=500 )
            {
                AltTrackPeriod=500;
                AltChangePeriod=true;
            }
        }*/
    }


    if(trackstar.flagAltTracking)
    {
        /*if(fabs(trackstar.ALT_TrackTargetSpeed)<0.03 )
        {
            if(alt_p==0)setPIDparam_Alt_tracking(1);
        }
        else
        {
            if(alt_p==1)setPIDparam_Alt_tracking(0);
        }*/
        AltGroup=CheckUmacFlag_Alt(M186_addr);
        if(AltGroup<4 && AltGroup !=AltGroupLast && AltTrackSeconds<=num && AltTrackSeconds>1)
        {

            if(AltGroup !=AltGroupLast) alttrackstar.N_row=alttrackstar.N_row+AltTrackPeriod/50;//10
            AltMakeCommand(alttrackstar.N_row);
            AltToUMAC_Posix();
            //qDebug()<<" data to alt group "<<AltGroup<<" last group "<<AltGroupLast;
            //       <<"AltTrackSeconds "<<AltTrackSeconds<<" number "<< alttrackstar.N_row;
            AltGroupLast=AltGroup;
            /*if(AltChangePeriod)
            {
                AltChangePeriod=false;
                SendIntToDPRAM(M1785_addr,AltTrackPeriod);
            }*/
        }
        double target;
        int iTarget=AltTrackSeconds/3;
        if(AltGroup==1||AltGroup==10)target=alttrackstar.Target_Group2[iTarget]*CTS/3600;
        else target=alttrackstar.Target_Group1[iTarget]*CTS/3600;
       /*qDebug()<<"AltTrackSeconds "<<AltTrackSeconds
               <<" Alt Group "<<AltGroup<<" last group "<<AltGroupLast
                <<" Alt Target "<<target
                <<" Pos "<<trackstar.AltPosRealForError;*/
                //<<" last "<<int(trackstar.AltTargetOffset*36000)%36000;
    }
    if(AltTrackSeconds>=num) AltTrackSeconds=0;
    else AltTrackSeconds++;


  /*      if( trackstar.flagAltCorrectPoint&&trackstar.flagAltTracking) 
        AltCorrectPoint();
 
	 
       if(trackstar.flagAltTracking&&(!trackstar.flagAltCorrectPoint))
       {	AltCorrectTarget(); 
		AltCorrect();
	}*/ 

	
}



void  deroTrack()
{
    //long Dero_row;
    int num= 4;
        //int num=50;

    //if ( flagtcs.flag_DERO )  GetDeroPosition_DPRAM();
     /*if ( flagtcs.flag_ADC2 )  GetADC2Position_DPRAM();
    if ( flagtcs.flag_ADC1 )  GetADC1Position_DPRAM();*/

    if( !flagtcs.flag_DERO) return;
    if(DeroTrackSeconds>= num) GetDeroPosition_DPRAM();

    if(trackstar.flagDeroPointing)
    {

        time_DeropointEnd=time_DeropointEnd-PosixTClock/1000000;

        if(time_DeropointEnd<0)
        {

            DeroGroup=CheckUmacFlag_Dero(M182_addr);
            if(DeroGroup==1)
            {
                LookupTable_Dero(1);
                DeroMakeCommand(derotrackstar.N_row);
                if(DeroToUMAC_Posix())
                {
                    Dero_beginTrack();
                    trackstar.flagDeroPointing=false;
                    trackstar.flagDeroTracking=true;
                    DeroTrackSeconds=0;
                    DeroGroupLast=DeroGroup;
                }
             }
        }
    }

    if(DeroTrackSeconds==0 && trackstar.flagDeroPointing)
    {
        trackstar.DeroTrackError=(trackstar.DeroTarget-trackstar.DeroPosRealForError)*3600;
    }

    if(DeroTrackSeconds==0 && trackstar.flagDeroTracking)
	{
 
        LookupTable_Dero();
        trackstar.DeroTrackError=(trackstar.DeroTarget-trackstar.DeroPosRealForError)*3600;
        trackstar.DeroFolErrUMACTest[trackstar.dero_DataCount]=trackstar.DeroTrackError;

        if(trackstar.dero_DataCount>=4)
        {
            double terr=0;
            for ( int z=0;z<=4; z++ )
            {
                terr+=trackstar.DeroFolErrUMACTest[z];
                trackstar.DeroFolErrUMACTest[z]=0;
            }
            trackstar.DeroFolErrUMACTest[10]=terr/5;

            if(flagtcs.AutoCorrect &&  fabs(trackstar.Dero_TrackTargetSpeed)>0.8)
            {

                terr= trackstar.DeroFolErrUMACTest[10]*0.5/trackstar.Dero_TrackTargetSpeed*1000;
                if(terr>100)terr=100;
                else if(terr<-100)terr=-100;
                terr+=derotrackstar.deltaTime;
                derotrackstar.N_row+=int(terr)/100;
                derotrackstar.deltaTime=int(terr)%100;
            }
            trackstar.dero_DataCount=0;
        }
        else trackstar.dero_DataCount++;
        /*qDebug()<<"timenumber:"<<TrackSeconds<<" dero Target "<<trackstar.DeroTarget
                        <<" err "<<trackstar.DeroTrackError
                        <<" err/total "<<trackstar.DeroFolErrUMACTest[10]
                        <<" Speed "<<trackstar.Dero_TrackTargetSpeed
                       <<"N_row"<<derotrackstar.N_row
                        <<"delta ms"<<derotrackstar.deltaTime;*/
	}	

    if(trackstar.flagDeroTracking)
	{
        DeroGroup=CheckUmacFlag_Dero(M182_addr);
        //qDebug()<<" DeroGroup=(M182)= "<<DeroGroup<<" last group "<<DeroGroupLast;
        if(DeroGroup<4 && DeroGroup!=DeroGroupLast && DeroTrackSeconds<=num && DeroTrackSeconds>1)
        {
            if(DeroGroup!=DeroGroupLast) derotrackstar.N_row=derotrackstar.N_row+DeroTrackPeriod/50;
            DeroMakeCommand(derotrackstar.N_row);
            DeroToUMAC_Posix();
                //qDebug()<<" data to dero group "<<DeroGroup<<" last group "<<DeroGroupLast;
            DeroGroupLast=DeroGroup;
        }
        /*if(DeroGroup==1)
            trackstar.DeroTarget=(derotrackstar.Target_Group2[DeroTrackSeconds]-
                                  derotrackstar.Target_Group2[NumPerGroup])*CTS_Dero/3600;
        else
            trackstar.DeroTarget=(derotrackstar.Target_Group1[DeroTrackSeconds]-
                                  derotrackstar.Target_Group1[NumPerGroup])*CTS_Dero/3600;*/
    }

    if(DeroTrackSeconds>=num)DeroTrackSeconds=0;
    else DeroTrackSeconds++;



// 	if( trackstar.flagDeroCorrectPoint&&trackstar.flagDeroTracking) 
//    		DeroCorrectPoint();
//  
//          if(trackstar.flagDeroTracking&&!trackstar.flagDeroCorrectPoint)
//         	{ DeroCorrectTarget();DeroCorrect();} 

}

void  ADCTrack()
{
    double Zen=(90-trackstar.AltTarget)/180*PI;
    double P=Pressure*0.75006;//Pressure, from mbar to mmHg
    double T=Temperature;//temperature,degree

    double mm=P*(1+(1.049-0.0157*T)*P*0.000001)/(24.0294+0.087972*T);
    double p1=0.5761*tan(Zen/180*PI)+0.004844;
    /*double p2=E*0.0075008*0.000001*0.05968/(1+0.003661*T)
            *(-2.184*10000000*exp(-(T-44.64)*(T-44.64)/3.624/3.624)
              +2015*exp(-(T-158.7)*(T-158.7)/65.86/65.86));*/
    trackstar.ADC2Target=mm*p1;
    //qDebug() <<"Zenith="<<Zen<<" p1="<<p1<<"  M="<<mm
    //        <<" adc=mm*(p1-p2)="<<trackstar.ADC2Target;

    char cmd[255];
    QString str;
    double adc1err=(trackstar.ADC1PosRealForError-trackstar.ADC2Target)*3600;
    if(fabs(adc1err)<30)
    {
        sprintf(cmd, "i322=%7.4f", 0.01/CTS_ADC);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
    double adc2err=(trackstar.ADC2PosRealForError-trackstar.ADC2Target)*3600;
    if(fabs(adc2err)<30)
    {
        sprintf(cmd, "i422=%7.4f", 0.01/CTS_ADC);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
    if(fabs(adc1err)>3 || fabs(adc2err)>3)
    {
        sprintf(cmd, "#3J=%7.4f #4J=%7.4f",
            (trackstar.ADC2Target*3600+trackstar.ADC1_correct_all)/ CTS_ADC,
            (trackstar.ADC2Target*3600+trackstar.ADC2_correct_all)/ CTS_ADC);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
}

void CalAzAverageError()
{
///////////////////Az average error///////////////////////////
// 	if(fabs(trackstar.AzTrackError)>100)
// 	retu
    double err=0;
    if(iErrorAz<50)
        Az_dError[iErrorAz]=trackstar.AzTrackError;
    iErrorAz++;

    if ( iErrorAz==50)
    {
        for ( int ti=0; ti<50; ti++ )
        {
            err+=Az_dError[ti];
        }
        iErrorAz=0;
        az_AvError=err/50;
        //		qDebug() <<"Az AverageError"<<az_AvError;
    }

}

void CalDeroAverageError()
{
///////////////////Alt average error///////////////////////////
    double err=0;
    if(iErrorDero<50)
        Dero_dError[iErrorDero]=trackstar.DeroTrackError;
    iErrorDero++;

    if ( iErrorDero==50)
    {
        for ( int ti=0; ti<50; ti++ )
        {
            err+=Dero_dError[ti];
        }
        iErrorDero=0;
        dero_AvError=err/50;
        //qDebug() <<"Dero AverageError"<<dero_AvError;
    }

}


void  AzCorrectTarget()
{
    	int ErrN=0;
	double DeltaTarget=0;
      	ErrN=N_row-N_row_LAST;
        DeltaTarget=double(ErrN-(AzTrackPeriod/TableLineTime)*AzNumPerGroup)*TableLineTime/1000.0*trackstar.AZ_TrackTargetSpeed;
	DeltaTarget=DeltaTarget/3600.;								//degree
    	if(DeltaTarget!=0&&fabs(ErrN)<500)
     	{
               int num=AzTrackPeriod/TableLineTime;
	 	 for ( int i=0;i<num; i++ )
       		{
			AzTrackTarget[i]=AzTrackTarget[i]+(i+1)*DeltaTarget/num;
		}
	}
}

// void  AzCorrectTarget()
// {
//     	int ErrN=0;
// 	double DeltaTarget=0;
//       	ErrN=N_row-N_row_LAST;
//         DeltaTarget=double(ErrN-(TrackPeriod/TableLineTime)*NumPerGroup)*TableLineTime/1000.0*trackstar.AZ_TrackTargetSpeed;
// 	DeltaTarget=DeltaTarget/3600.;								//degree
//     	if(DeltaTarget!=0&&fabs(ErrN)<500)
//      	{
//                int num=TrackPeriod/TableLineTime;
// 		for ( int i=0;i<num; i++ )
// 			AzTrackTarget[i]=AzTrackTarget[i]+(i+1)*DeltaTarget/num;
// 		//trackstar.AzTarget=trackstar.AzTarget+DeltaTarget;
// 	 	 
// 
// 	}
//  
// }

void  AltCorrectTarget()
{
    	int ErrN=0;
	double DeltaTarget=0;
      	ErrN=N_row-N_row_LAST;
        DeltaTarget=double(ErrN-(DeroTrackPeriod/TableLineTime)*DeroNumPerGroup)*TableLineTime/1000.0*trackstar.ALT_TrackTargetSpeed;
	DeltaTarget=DeltaTarget/3600.;								//degree
    	if(DeltaTarget!=0&&fabs(ErrN)<500)
     	{
               int num=AltTrackPeriod/TableLineTime;
	 	 for ( int i=0;i<num; i++ )
       		{
 			//trackstar.alt_correct[i]=trackstar.alt_correct[9]+ ( i+1 ) *de1taErrAlt/10*TrackPeriod/1000;
         	//	trackstar.alt_correct[i]=trackstar.alt_correct[9]+ ( i+1 ) *de1taErrAlt/10;
            //AltTrackTarget[i]=AltTrackTarget[i]+(i+1)*DeltaTarget/num;
          		
		}

	}
 //  qDebug()<<ErrN<<N_row<<N_row_LAST<<DeltaTarget;
 
}
void  DeroCorrectTarget()
{
    	int ErrN=0;
	double DeltaTarget=0;
      	ErrN=N_row-N_row_LAST;
        DeltaTarget=double(ErrN-(DeroTrackPeriod/TableLineTime)*DeroNumPerGroup)*TableLineTime/1000.0*trackstar.Dero_TrackTargetSpeed;
	DeltaTarget=DeltaTarget/3600.;								//degree
    	if(DeltaTarget!=0&&fabs(ErrN)<500)
     	{
               int num=DeroTrackPeriod/TableLineTime;
	 	 for ( int i=0;i<num; i++ )
       		{
			DeroTrackTarget[i]=DeroTrackTarget[i]+(i+1)*DeltaTarget/num;
		}
	}
 
}

void Cal(double *azPos,double *altPos,double *deroPos,QString *dts)
{
	QDateTime dt=QDateTime::currentDateTime();
	QString dtstr;
	dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );
	QTime time=dt.time();
        double ms=time.msec();
        double utc=0;
       *dts=dtstr;
        long NoNew,N_new;
        utc= ( time.hour() ) *3600+time.minute() *60.+time.second() +time.msec() /1000.0;				//local time
        NoNew=long(utc*1000/TableLineTime);
        N_new=NoNew-No1;
  //     long N_old= N_row+TrackSeconds*TrackPeriod/NumPerGroup;
        long N_oldAz= aztrackstar.N_row+AzTrackSeconds*LinesEvery100ms;
    long N_oldAlt= alttrackstar.N_row+AltTrackSeconds*LinesEvery100ms;
    long N_oldDero= derotrackstar.N_row+DeroTrackSeconds*LinesEvery100ms;

    	double DeltaT_az,DeltaT_alt,DeltaT_dero, t1;
	t1=(ms-int(ms/10)*10)/1000;											//get 1~9 ms of time, 
	//DeltaT=double(NoNew-NoOld)*TableLineTime/1000.0+t1;
        DeltaT_az=double(N_new-N_oldAz-LinesEvery100ms)*TableLineTime/1000.0+t1;
        DeltaT_alt=double(N_new-N_oldAlt-LinesEvery100ms)*TableLineTime/1000.0+t1;
	DeltaT_dero=double(N_new-N_oldDero-LinesEvery100ms)*TableLineTime/1000.0+t1;

        if(DeltaT_az>2.0)
          DeltaT_az=0;
        if(DeltaT_alt>2.0)
          DeltaT_alt=0;
        if(DeltaT_dero>2.0)
          DeltaT_dero=0;

 	*azPos=trackstar.AzPosRealForError-trackstar.AZ_TrackTargetSpeed*DeltaT_az/3600;				//AZ_TrackTargetSpeed  "/S
  	*altPos=trackstar.AltPosRealForError-trackstar.ALT_TrackTargetSpeed*DeltaT_alt/3600;
    *deroPos=trackstar.DeroPosRealForError-trackstar.Dero_TrackTargetSpeed*DeltaT_dero/3600;

     //  qDebug()<<trackstar.ALT_TrackTargetSpeed<<t1<<DeltaT<<N_new<<N_old<<TrackSeconds<<N_row;
      // qDebug()<<dtstr;
}

void   GetStartTime()
{
	gettimeofday(&tpstart,NULL); 
}

void  GetEndTime()
{
	gettimeofday ( &tpend,NULL );
	timeuse=1000000* ( tpend.tv_sec-tpstart.tv_sec ) +
	        tpend.tv_usec-tpstart.tv_usec;
	timeuse/=1000000;
    //qDebug() <<"Used Time:"<<timeuse;

}


void AzMakeStr(int Az_row)
{
 	QByteArray ba;
	QString str,str1;
	axis="AZ";
	
	int StartP=UmacStartP1_Az;											//P4000    P4010;
	qDebug()<<"AzGroup"<<AzGroup<<"AzGroupLast"<<AzGroupLast;
 
	if(AzGroup==AzGroupLast||AzGroup==10||AzGroup==20)
      //CheckUmacFlag_Az();
       AzGroup=CheckUmacFlag_Az(M2184_addr);
	qDebug()<<"AzGroup"<<AzGroup<<"AzGroupLast"<<AzGroupLast;

        if(AzGroup==AzGroupLast||AzGroup==10||AzGroup==20)
	{
//		trackstar.flag_AzSendtoUMAC=false;
                return;
	}
           

	if ( AzGroup== 1 ) 										//check P148,p148=1?
		{StartP=UmacStartP1_Az;trackstar.flag_sendAz=true;}
	else if ( AzGroup== 2 ) 									//check P184=2?
		{StartP=UmacStartP2_Az;trackstar.flag_sendAz=true;}
        else
		{
			trackstar.flag_sendAz=false;
			return;
		}

	//qDebug() <<"tracking N_row"<<N_row<<"AzGroup"<<AzGroup;
	//double azOffset=trackstar.az_offset-trackstar.az_offset_last;
	
    MakeStr ( axis,AzTrackPeriod,Az_row,StartP,trackstar.az_offset,trackstar.az_correct,az_model,trackstar.az_guidedata,&trackstar.cCmdtoAz,&trackstar.AzTargetOffset );
 	if(flagtrack.StarTableEnd)
 	{
		trackstar.flag_sendAz=false;
		return;
	}
// 	trackstar.AzCorrectLines=trackstar.az_correct[9];
// 	for ( int i=0;i<10; i++ )
// 	{
// 		trackstar.az_correct[i]=trackstar.az_correct[9];
//               		 
//          }

	//trackstar.flag_AzSendtoUMAC=true;
}

void AzMakeCommand(int row)
{
    //QByteArray ba;
    //QString str,str1;
    //axis="AZ";
	
    //qDebug()<<"AzGroup"<<AzGroup<<"AzGroupLast"<<AzGroupLast;
 
    if(AzGroup==AzGroupLast||AzGroup==10||AzGroup==20)
	{
//		trackstar.flag_AzSendtoUMAC=false;
                return;
	}
    if(flagtrack.StarTableEnd)
    {
        //trackstar.flag_sendAz=false;
        return;
    }

    //trackstar.flag_sendAz=true;
    //MakeDPRAMCommand(axis,TrackPeriod,Az_row,az_model);
    QStringList  list;
    QString sTargetPos;
    long  LINE=row;

    for ( int i=0;i<3;i++ )
    {
        double tmp_pos=0;
        double A=0;
        double E=0;
        //double azmodel=0;
        if ( LINE  < ( TableNum-1000 ) )
        {
            LINE=LINE+AzTrackPeriod/100;//TrackPeriod/TableLineTime;						//TableLineTime=100ms

            list = starTable[LINE].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts ); //10line=1s,offset in adcance
            if ( list.size() >=5 )
            {

                sTargetPos=list[2];////////////////////////
                A=list[2].toDouble()*PI/180.0;
                E=list[3].toDouble()*PI/180.0;
                if(flagtcs.PointModel)
                {
                    az_model=AW*cos ( A ) *tan ( E ) +AN*sin ( A ) *tan ( E )+ACES*sin(A)-ACEC*cos(A)
                            +CA/cos ( E )-CRX*cos(A-E)/cos(E)+CRY*sin(A-E)/cos(E)+IA
                            +NPAE*tan(E) +NRX+NRY*tan ( E );
                    /*az_model0=AW0*cos ( A ) *tan ( E ) +AN0*sin ( A ) *tan ( E )-ACEC0*cos(A)+IA0
                            +CA0/cos ( E )-CRX0*cos(A-E)/cos(E)+CRY0*sin(A-E)/cos(E)
                            +NPAE0*tan(E) +NRX0+NRY0*tan ( E );*/

                }
                else  az_model=0;
                //qDebug() <<"Az model: "<<az_model<<" azmodel="<<azmodel;
            }
            else
                qDebug() <<"AzMakeCommandString list number error"<<list.size();
            tmp_pos=sTargetPos.toDouble();
            if(tmp_pos>270 && StarAtNorth_Az==-1)
            {
                tmp_pos-=360;
                if(tmp_pos<-40)flagtrack.StarTableEnd=true;
            }
            else if(tmp_pos<90 && StarAtNorth_Az==1)
            {
                tmp_pos+=360;
                if(tmp_pos>400)flagtrack.StarTableEnd=true;
            }
            //if(tmp_pos>270)
            if(AzGroup==1)aztrackstar.Target_Group1[i]=tmp_pos*3600 + az_model ;
            else aztrackstar.Target_Group2[i]=tmp_pos*3600 + az_model ;
        }
        else
        {
            qDebug() <<"AzMakeCommand()  ROW number close star table end error"<<LINE;
            flagtrack.StarTableEnd=true;
            return;
        }

    }
    double n_dis=0;
    double d_time=1.0;//NumPerGroup*TrackPeriod/1000.;
    double d_t=0.5;//TrackPeriod/1000.;
    double d_dis=0;
    double v_0=aztrackstar.Vel_Last;
    double v_1=trackstar.AZ_TrackTargetSpeed;
    double x_0=aztrackstar.Target_Last;
    double acc=3000;
    if(AzGroup==1)
    {
        //x_0=aztrackstar.Target_Group2[NumPerGroup-1]*CTS;
        n_dis=aztrackstar.Target_Group1[1]-x_0+trackstar.az_correct_all;
        //v_0=aztrackstar.Vel_Group2[NumPerGroup-1]*CTS;
        /*qDebug()<<"aztrackstar.Target_Group1"<<aztrackstar.Target_Group1[0]<<aztrackstar.Target_Group1[1]
              <<"speed"<<trackstar.AZ_TrackTargetSpeed<<"n_dis"<<n_dis;*/
    }
    else if(AzGroup==2)
    {
        //x_0=aztrackstar.Target_Group1[NumPerGroup-1]*CTS;
        n_dis=aztrackstar.Target_Group2[1]-x_0+trackstar.az_correct_all;
        //v_0=aztrackstar.Vel_Group1[NumPerGroup-1]*CTS;

      /* qDebug()<<"aztrackstar.Target_Group2"<<aztrackstar.Target_Group2[0]<<aztrackstar.Target_Group2[1]
              <<"speed"<<trackstar.AZ_TrackTargetSpeed<<"n_dis"<<n_dis;*/
    }
    if(fabs(v_0-trackstar.AZ_TrackTargetSpeedLast)>14400)v_0=trackstar.AZ_TrackTargetSpeedLast;
    int a_time=(v_1-v_0)/acc;
    d_dis=n_dis- (v_0+v_1)/2 * d_time;

    if(fabs(d_dis)>10000)//fabs(d_dis)>fabs(trackstar.AZ_TrackTargetSpeed) && fabs(trackstar.AZ_TrackTargetSpeed)>10)  || fabs(a_time)> d_time
    {

        if(a_time>d_time)
        {
            if(d_dis>=0)v_1=v_0+acc* d_time;
            else v_1=v_0+acc*0.25* d_time;
            //n_dis= (v_0+v_1)/2 * d_time;
        }
        else if(a_time<-d_time)
        {
            if(d_dis<=0)v_1=v_0-acc* d_time;
            else v_1=v_0-acc*0.25* d_time;
            //n_dis= (v_0+v_1)/2 * d_time;
        }
        else
        {
            double c_time=(-v_0+sqrt(v_0*v_0+fabs(acc*n_dis)))/acc;
            if(c_time>d_time )
            {
                if(d_dis>=0)v_1=v_0+acc* d_time;
                else v_1=v_0-acc* d_time;
                //n_dis= (v_0+v_1)/2 * d_time;
            }
            else
            {
                double v_2=(n_dis*2/d_time-v_0)/4.0;
                if(v_1*v_2>2)
                {
                    if(v_2>v_1*4) v_1*=4;
                    else v_1=v_2;
                }
                else if(v_1*v_2<-2)
                {
                    v_1=0;
                }
                else v_1=v_2;
            }
        }
        n_dis= (v_0+v_1)/2 * d_time;
        //qDebug() <<"Az makecomand :"<<d_dis<<" AzGroup "<<AzGroup
        //        <<"v_0"<<v_0<<"v_1"<<v_1;

        if(AzGroup==1)
        {
            aztrackstar.Vel_Group1[0]=v_0+(v_1- v_0)*d_t;
            aztrackstar.Target_Group1[0]=x_0+(v_0+aztrackstar.Vel_Group1[0])*d_t/2;

            aztrackstar.Vel_Group1[1]=v_1;
            aztrackstar.Target_Group1[1]=x_0+n_dis;
            qDebug()<<"x_0"<<x_0<<"n_dis"<<n_dis<<"v_0"<<v_0<<"v_1"<<v_1
                    <<"Target_Group1[0]"<<aztrackstar.Target_Group1[0]
                   <<"Target_Group1[1]"<<aztrackstar.Target_Group1[1]
                  <<"Vel_Group1[0]"<< aztrackstar.Vel_Group1[0]
                 <<"Vel_Group1[1]"<< aztrackstar.Vel_Group1[1];

        }
        else if(AzGroup==2)
        {
            aztrackstar.Vel_Group2[0]=v_0+(v_1- v_0)*d_t;
            aztrackstar.Target_Group2[0]=x_0+(v_0+aztrackstar.Vel_Group2[0])*d_t/2;

            aztrackstar.Vel_Group2[1]=v_1;
            aztrackstar.Target_Group2[1]=x_0+n_dis;
            qDebug()<<"x_0"<<x_0<<"n_dis"<<n_dis<<"v_0"<<v_0<<"v_1"<<v_1
                    <<"Target_Group2[0]"<<aztrackstar.Target_Group2[0]
                    <<"Target_Group2[1]"<<aztrackstar.Target_Group2[1]
                   <<"Vel_Group2[0]"<< aztrackstar.Vel_Group2[0]
                   <<"Vel_Group2[1]"<< aztrackstar.Vel_Group2[1];
        }
    }
    else
    {
        if(AzGroup==1)
        {
            aztrackstar.Vel_Group1[1]=( aztrackstar.Target_Group1[2]-
                                            aztrackstar.Target_Group1[0])*500/AzTrackPeriod;
            aztrackstar.Vel_Group1[0]=( aztrackstar.Target_Group1[1]+ trackstar.az_correct_all-
                                            x_0)*500/AzTrackPeriod;
            //if(AzGroupLast==3) aztrackstar.Vel_Group1[0]*=2;
            /*qDebug() <<"Az makecomand :normal"
                     <<"Vel_Group1[0]"<< aztrackstar.Vel_Group1[0]
                    <<"Vel_Group1[4]"<< aztrackstar.Vel_Group1[4];*/
        }
        else if(AzGroup==2)
        {
            aztrackstar.Vel_Group2[1]=( aztrackstar.Target_Group2[2]-
                                            aztrackstar.Target_Group2[0])*500/AzTrackPeriod;
            aztrackstar.Vel_Group2[0]=( aztrackstar.Target_Group2[1]+ trackstar.az_correct_all-
                                            x_0)*500/AzTrackPeriod;
           /* qDebug() <<"Az makecomand :normal"
                     <<"Vel_Group2[0]"<< aztrackstar.Vel_Group2[0]
                    <<"Vel_Group2[4]"<< aztrackstar.Vel_Group2[4];*/
        }
    }

    double tem_step=trackstar.AZ_TrackTargetSpeed;
    if(tem_step>0 && tem_step<60)tem_step= 60;
    else if(tem_step<0 && tem_step>-60)tem_step=-60;
    if(fabs(trackstar.az_guidedata) > 0.1 )
    {
        if(fabs(trackstar.az_guidedata) < fabs(tem_step))
        {
            trackstar.az_correct= trackstar.az_guidedata;
            //trackstar.az_guidedata_last=trackstar.az_guidedata;
            trackstar.az_guidedata_all+=trackstar.az_guidedata;
            trackstar.az_guidedata=0;
        }
        else if( (trackstar.az_guidedata>0 && tem_step>0) || (trackstar.az_guidedata<0 && tem_step<0)   )
        {
            trackstar.az_correct= tem_step;
            trackstar.az_guidedata_all+= tem_step;
            trackstar.az_guidedata-=tem_step;
        }
        else if( (trackstar.az_guidedata>0 && tem_step<0) || (trackstar.az_guidedata<0 && tem_step>0)   )
        {
            trackstar.az_correct= -tem_step;
            trackstar.az_guidedata_all+= -tem_step;
            trackstar.az_guidedata-= -tem_step;
        }
    }
    else if(fabs(trackstar.az_offset) > 0.1)
    {
        if( fabs(trackstar.az_offset) < fabs(tem_step))
        {
            trackstar.az_correct= trackstar.az_offset;
            trackstar.az_offset_all+=trackstar.az_offset;
            trackstar.az_offset=0;
        }
        else if( (trackstar.az_offset>0 && tem_step>0) || (trackstar.az_offset<0 && tem_step<0)   )
        {
            trackstar.az_correct= tem_step;
            trackstar.az_offset_all+= tem_step;
            trackstar.az_offset-=tem_step;
        }
        else if( (trackstar.az_offset>0 && tem_step<0) || (trackstar.az_offset<0 && tem_step>0)   )
        {
            trackstar.az_correct= -tem_step;
            trackstar.az_offset_all+= -tem_step;
            trackstar.az_offset-= -tem_step;
        }
    }
    for ( int i=0;i<2;i++ )
    {

        if(AzGroup==1)
        {
            aztrackstar.Target_Group1[i]  =( aztrackstar.Target_Group1[i]+ trackstar.AZ_TrackTargetSpeed*0.001*aztrackstar.deltaTime
                                          + trackstar.az_correct*(i+1)/2+ trackstar.az_correct_all)/CTS;
            //aztrackstar.Vel_Group1[i]/=CTS;
            aztrackstar.Vel_Group1[i]=(aztrackstar.Vel_Group1[i]+trackstar.az_correct)/CTS;
        }
        else
        {
            aztrackstar.Target_Group2[i]  =( aztrackstar.Target_Group2[i]+ trackstar.AZ_TrackTargetSpeed*0.001*aztrackstar.deltaTime
                                           + trackstar.az_correct*(i+1)/2+ trackstar.az_correct_all)/CTS;
            //aztrackstar.Vel_Group2[i]/=CTS;
            aztrackstar.Vel_Group2[i]=(aztrackstar.Vel_Group2[i]+trackstar.az_correct)/CTS;
        }
    }
    if(AzGroup==1)
    {
        aztrackstar.Target_Last=aztrackstar.Target_Group1[1]*CTS;
        aztrackstar.Vel_Last=aztrackstar.Vel_Group1[1]*CTS;
    }
    else
    {
        aztrackstar.Target_Last=aztrackstar.Target_Group2[1]*CTS;
        aztrackstar.Vel_Last=aztrackstar.Vel_Group2[1]*CTS;
    }
    trackstar.az_correct_all+=trackstar.az_correct;
    trackstar.az_correct=0;
// 	MakeStr ( axis,TrackPeriod,Az_row,StartP,trackstar.az_offset,trackstar.az_correct,az_model,trackstar.az_guidedata,&trackstar.cCmdtoAz,&trackstar.AzTargetOffset );

 
}
void AltMakeCommand(int row)
{
 	QByteArray ba;
	QString str,str1;
    //axis="ALT";
	
    //qDebug()<<"AltGroup"<<AltGroup<<"AltGroupLast"<<AltGroupLast;
    //if(AltGroup==AltGroupLast||AltGroup==10||AltGroup==20)
    //  CheckUmacFlag_Alt();
    //qDebug()<<"AltGroup"<<AltGroup<<"AltGroupLast"<<AltGroupLast;

    if(AltGroup==AltGroupLast||AltGroup==10||AltGroup==20)
	{
//		trackstar.flag_AltSendtoUMAC=false;
        return;
	}
    if(flagtrack.StarTableEnd)
    {
        //trackstar.flag_sendAlt=false;
        return;
    }

	 
    //trackstar.flag_sendAlt=true;
    //MakeDPRAMCommand(axis,TrackPeriod,Alt_row,alt_model);
	

    QStringList  list;
    QString sTargetPos;
    long  LINE=row;
    for ( int i=0;i<3;i++ )
    {
        double A=0;
        double E=0;
        if ( LINE  < ( TableNum-1000 ) )
        {
            LINE=LINE+AltTrackPeriod/100;						//TableLineTime=100ms

            list = starTable[LINE].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts ); //10line=1s,offset in adcance
            if ( list.size() >=5 )
            {

               sTargetPos=list[3];
               A=list[2].toDouble()*PI/180.0;
               E=list[3].toDouble()*PI/180.0;
               if(flagtcs.PointModel)
               {
                   alt_model= -AW*sin ( A ) +AN*cos ( A )-IE +TF*sin(PI/2-E)
                           +CRX*sin(A-E)+CRY*cos(A-E)
                           -ECEC*cos(E)-ECES*sin(E)+NRX*sin( E )-NRY*cos ( E );
                   /*alt_model0= -AW0*sin ( A ) +AN0*cos ( A )-IE0+TF*sin(PI/2-E)
                           +CRX0*sin(A-E)+CRY0*cos(A-E)
                           +NRX0*sin( E )-NRY0*cos ( E );*/
               }
               else  alt_model=0;
               //qDebug() <<"Alt model: "<<alt_model;
            }
            else
                qDebug() <<"AltMakeCommandString list number error"<<list.size();

            if( AltGroup==1)alttrackstar.Target_Group1[i]=sTargetPos.toDouble()*3600+alt_model;
            else alttrackstar.Target_Group2[i]=sTargetPos.toDouble()*3600+alt_model;
        }
        else
        {
            qDebug() <<"AltMakeCommand()  ROW number close star table end error"<<LINE;
            flagtrack.StarTableEnd=true;
            return;
        }

    }
    /*double tem=0.014;
    if(trackstar.ALT_TrackTargetSpeed<0.03 && trackstar.ALT_TrackTargetSpeed>-0.03)
    {

        if( AltGroup==1)
        {
            alttrackstar.Target_Group1[0]=alttrackstar.Target_Group2[1]*CTS+tem;
            alttrackstar.Target_Group1[1]=alttrackstar.Target_Group1[0]+tem;
            alttrackstar.Target_Group1[2]=alttrackstar.Target_Group1[1]+tem;
        }
        else
        {
            alttrackstar.Target_Group2[0]=alttrackstar.Target_Group1[1]*CTS+tem;
            alttrackstar.Target_Group2[1]=alttrackstar.Target_Group2[0]+tem;
            alttrackstar.Target_Group2[2]=alttrackstar.Target_Group2[1]+tem;
        }
    }
    else if(trackstar.ALT_TrackTargetSpeed>-0.035 && trackstar.ALT_TrackTargetSpeed<-0.03)
    {
        tem=0.0;
        if( AltGroup==1)
        {
            alttrackstar.Target_Group1[0]=alttrackstar.Target_Group2[1]*CTS-tem;
            alttrackstar.Target_Group1[1]=alttrackstar.Target_Group1[0]-tem;
            alttrackstar.Target_Group1[2]=alttrackstar.Target_Group1[1]-tem;
        }
        else
        {
            alttrackstar.Target_Group2[0]=alttrackstar.Target_Group1[1]*CTS-tem;
            alttrackstar.Target_Group2[1]=alttrackstar.Target_Group2[0]-tem;
            alttrackstar.Target_Group2[2]=alttrackstar.Target_Group2[1]-tem;
        }
    }*/
    //double tmp_val=0;
    if( AltGroup==1)
    {
        alttrackstar.Vel_Group1[1]=( alttrackstar.Target_Group1[2]-
                                        alttrackstar.Target_Group1[0])*500/AltTrackPeriod/CTS;
        alttrackstar.Vel_Group1[0]=( alttrackstar.Target_Group1[1]+ trackstar.alt_correct_all-
                                        alttrackstar.Target_Group2[1]*CTS)*500/AltTrackPeriod/CTS;
        //if(AzGroupLast==3) aztrackstar.Vel_Group1[0]*=2;
       /* qDebug() <<"Alt makecomand :"
                <<"Target_Group1[0]"<<QString("%1").arg(alttrackstar.Target_Group1[0], 0, 'g', 8)
                <<"Target_Group1[1]"<<QString("%1").arg(alttrackstar.Target_Group1[1], 0, 'g', 8)
                <<"Vel_Group1[0]"<< alttrackstar.Vel_Group1[0]*CTS
                 <<"Vel_Group1[1]"<< alttrackstar.Vel_Group1[1]*CTS;*/

    }
    else
    {
        alttrackstar.Vel_Group2[1]=( alttrackstar.Target_Group2[2]-
                                        alttrackstar.Target_Group2[0])*500/AltTrackPeriod/CTS;
        alttrackstar.Vel_Group2[0]=( alttrackstar.Target_Group2[1]+ trackstar.alt_correct_all-
                                        alttrackstar.Target_Group1[1]*CTS)*500/AltTrackPeriod/CTS;
        /*qDebug() <<"alt makecomand :"
                <<"Target_Group2[0]"<<QString("%1").arg(alttrackstar.Target_Group2[0], 0, 'g', 8)
                <<"Target_Group2[1]"<<QString("%1").arg(alttrackstar.Target_Group2[1], 0, 'g', 8)
                <<"Vel_Group2[0]"<< alttrackstar.Vel_Group2[0]*CTS
                <<"Vel_Group2[1]"<< alttrackstar.Vel_Group2[1]*CTS;*/
    }

    double tem_step=trackstar.ALT_TrackTargetSpeed;
    if(tem_step>0 && tem_step<60)tem_step=60;
    else if(tem_step<0 && tem_step>-60)tem_step=-60;
    if(fabs(trackstar.alt_guidedata) > 0.1 )
    {
        if(fabs(trackstar.alt_guidedata) < fabs(tem_step))
        {
            trackstar.alt_correct= trackstar.alt_guidedata;
            //trackstar.alt_guidedata_last=trackstar.alt_guidedata;
            trackstar.alt_guidedata_all+=trackstar.alt_guidedata;
            trackstar.alt_guidedata=0;
        }
        else if( (trackstar.alt_guidedata>0 && tem_step>0) || (trackstar.alt_guidedata<0 && tem_step<0)   )
        {
            trackstar.alt_correct= tem_step;
            trackstar.alt_guidedata_all+= tem_step;
            trackstar.alt_guidedata-=tem_step;
        }
        else if( (trackstar.alt_guidedata>0 && tem_step<0) || (trackstar.alt_guidedata<0 && tem_step>0)   )
        {
            trackstar.alt_correct= -tem_step;
            trackstar.alt_guidedata_all+= -tem_step;
            trackstar.alt_guidedata-= -tem_step;
        }
    }
    else if(fabs(trackstar.alt_offset) > 0.1)
    {
        if( fabs(trackstar.alt_offset) < fabs(tem_step))
        {
            trackstar.alt_correct= trackstar.alt_offset;
            trackstar.alt_offset_all+=trackstar.alt_offset;
            trackstar.alt_offset=0;
        }
        else if( (trackstar.alt_offset>0 && tem_step>0) || (trackstar.alt_offset<0 && tem_step<0)   )
        {
            trackstar.alt_correct= tem_step;
            trackstar.alt_offset_all+= tem_step;
            trackstar.alt_offset-=tem_step;
        }
        else if( (trackstar.alt_offset>0 && tem_step<0) || (trackstar.alt_offset<0 && tem_step>0)   )
        {
            trackstar.alt_correct= -tem_step;
            trackstar.alt_offset_all+= -tem_step;
            trackstar.alt_offset-= -tem_step;
        }
    }
    for ( int i=0;i<2;i++ )
    {
        if( AltGroup==1)
            alttrackstar.Target_Group1[i] =(alttrackstar.Target_Group1[i]+  trackstar.ALT_TrackTargetSpeed*0.001*alttrackstar.deltaTime
                                        + trackstar.alt_correct*(i+1)/2+trackstar.alt_correct_all)/CTS;
        else alttrackstar.Target_Group2[i] =(alttrackstar.Target_Group2[i]+  trackstar.ALT_TrackTargetSpeed*0.001*alttrackstar.deltaTime
                                             + trackstar.alt_correct*(i+1)/2+trackstar.alt_correct_all)/CTS;
    }
    trackstar.alt_correct_all+=trackstar.alt_correct;
    trackstar.alt_correct=0;



    if(AltGroup==1)
    {
        alttrackstar.Target_Last=alttrackstar.Target_Group1[1]*CTS;
        alttrackstar.Vel_Last=alttrackstar.Vel_Group1[1]*CTS;
    }
    else
    {
        alttrackstar.Target_Last=alttrackstar.Target_Group2[1]*CTS;
        alttrackstar.Vel_Last=alttrackstar.Vel_Group2[1]*CTS;
    }

}
void DeroMakeCommand(int row)
{
 	QByteArray ba;
	QString str,str1;
    //axis="DERO";
	

    //if(DeroGroup==DeroGroupLast||DeroGroup==10||DeroGroup==20)
    //  CheckUmacFlag_Dero();
//	qDebug()<<"DeroGroup"<<DeroGroup<<"DeroGroupLast"<<DeroGroupLast;

	if(DeroGroup==DeroGroupLast||DeroGroup==10||DeroGroup==20)
	{
//		trackstar.flag_DeroSendtoUMAC=false;
        return;
	}
    if(flagtrack.StarTableEnd)
    {
        //trackstar.flag_sendDero=false;
        return;
    }

    //trackstar.flag_sendDero=true;

    //MakeDPRAMCommand(axis,TrackPeriod,Dero_row,dero_model);


    QStringList  list;
    QString sTargetPos;
    long  LINE=row;
    for ( int i=0;i<3;i++ )
    {
        if ( LINE  < ( TableNum-1000 ) )
        {
            LINE=LINE+DeroTrackPeriod/100;						//TableLineTime=100ms

            list = starTable[LINE].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts ); //10line=1s,offset in adcance
            if ( list.size() >=5 )
            {

                sTargetPos=list[4];
            }
            else
                qDebug() <<"DeroMakeCommandString list number error"<<list.size();
            double deropos=(list[4].toDouble()-list[3].toDouble())/2+180;
            if(DeroReverse)
            {
                if(deropos>0)deropos-=90;
                else if(deropos<0)deropos+=90;
            }
            if(flagtcs.Dero_ALTHALF)
            {
                if(trackstar.flagAltTracking)deropos= -alttrackstar.Target_Last/7200.;
                else deropos=(-list[3].toDouble())/2;
            }
           if(DeroGroup==1) derotrackstar.Target_Group1[i]= deropos*3600;
           else  derotrackstar.Target_Group2[i]= deropos*3600;
                //derotrackstar.TargetCommandOffset[i]=sTargetPos.toDouble()+model;		//+guidestar  later
       }
        else
        {
            qDebug() <<"DeroMakeCommand()  ROW number close star table end error"<<LINE;
            flagtrack.StarTableEnd=true;
            return;
        }

    }
    //double tmp_val=trackstar.Dero_TrackTargetSpeed;
    if(DeroGroup==1)
    {
        derotrackstar.Vel_Group1[1]=( derotrackstar.Target_Group1[2]-
                                        derotrackstar.Target_Group1[0])*500/DeroTrackPeriod/CTS_Dero;
        derotrackstar.Vel_Group1[0]=( derotrackstar.Target_Group1[1]-
                                        derotrackstar.Target_Group2[1]*CTS_Dero)*500/DeroTrackPeriod/CTS_Dero;
        //if(DeroGroupLast==3) derotrackstar.Vel_Group1[0]*=2;
        /*qDebug() <<"Dero makecomand :"
                <<"Target_Group1[0]"<<QString("%1").arg(derotrackstar.Target_Group1[0], 0, 'g', 8)
                <<"Target_Group1[1]"<<QString("%1").arg(derotrackstar.Target_Group1[1], 0, 'g', 8)
                <<"Vel_Group1[0]"<< derotrackstar.Vel_Group1[0]*CTS_Dero
                 <<"Vel_Group1[1]"<< derotrackstar.Vel_Group1[1]*CTS_Dero;*/
    }
    else
    {
        derotrackstar.Vel_Group2[1]=( derotrackstar.Target_Group2[2]-
                                        derotrackstar.Target_Group2[0])*500/DeroTrackPeriod/CTS_Dero;
        derotrackstar.Vel_Group2[0]=( derotrackstar.Target_Group2[1]-
                                        derotrackstar.Target_Group1[1]*CTS_Dero)*500/DeroTrackPeriod/CTS_Dero;
        /*qDebug() <<"Dero makecomand :"
                <<"Target_Group2[0]"<<QString("%1").arg(derotrackstar.Target_Group2[0], 0, 'g', 8)
                <<"Target_Group2[1]"<<QString("%1").arg(derotrackstar.Target_Group2[1], 0, 'g', 8)
                <<"Vel_Group2[0]"<< derotrackstar.Vel_Group2[0]*CTS_Dero
                 <<"Vel_Group2[1]"<< derotrackstar.Vel_Group2[1]*CTS_Dero;*/
    }
    double tem_step=trackstar.Dero_TrackTargetSpeed;
    if(tem_step>0 && tem_step<100)tem_step=100;
    else if(tem_step<0 && tem_step>-100)tem_step=-100;
    if(fabs(trackstar.dero_guidedata) > 0.2 )
    {
        if(fabs(trackstar.dero_guidedata) < fabs(tem_step))
        {
            trackstar.dero_correct= trackstar.dero_guidedata;
            //trackstar.dero_guidedata_last=trackstar.dero_guidedata;
            trackstar.dero_guidedata_all+=trackstar.dero_guidedata;
            trackstar.dero_guidedata=0;
        }
        else if( (trackstar.dero_guidedata>0 && tem_step>0) || (trackstar.dero_guidedata<0 && tem_step<0)   )
        {
            trackstar.dero_correct= tem_step;
            trackstar.dero_guidedata_all+= tem_step;
            trackstar.dero_guidedata-=tem_step;
        }
        else if( (trackstar.dero_guidedata>0 && tem_step<0) || (trackstar.dero_guidedata<0 && tem_step>0)   )
        {
            trackstar.dero_correct= -tem_step;
            trackstar.dero_guidedata_all+= -tem_step;
            trackstar.dero_guidedata-= -tem_step;
        }
    }
    else if(fabs(trackstar.dero_offset) > 0.2)
    {
        if( fabs(trackstar.dero_offset) < fabs(tem_step))
        {
            trackstar.dero_correct= trackstar.dero_offset;
            trackstar.dero_offset_all+=trackstar.dero_offset;
            trackstar.dero_offset=0;
        }
        else if( (trackstar.dero_offset>0 && tem_step>0) || (trackstar.dero_offset<0 && tem_step<0)   )
        {
            trackstar.dero_correct= tem_step;
            trackstar.dero_offset_all+= tem_step;
            trackstar.dero_offset-=tem_step;
        }
        else if( (trackstar.dero_offset>0 && tem_step<0) || (trackstar.dero_offset<0 && tem_step>0)   )
        {
            trackstar.dero_correct= -tem_step;
            trackstar.dero_offset_all+= -tem_step;
            trackstar.dero_offset-= -tem_step;
        }
    }
    for ( int i=0;i<2;i++ )
    {
        if(DeroGroup==1)
            derotrackstar.Target_Group1[i]=(derotrackstar.Target_Group1[i]+  trackstar.Dero_TrackTargetSpeed*derotrackstar.deltaTime/1000
                                        + trackstar.dero_correct*(i+1)/2+trackstar.dero_correct_all)/CTS_Dero;
        else  derotrackstar.Target_Group2[i]=(derotrackstar.Target_Group2[i]+  trackstar.Dero_TrackTargetSpeed*derotrackstar.deltaTime/1000
                                        + trackstar.dero_correct*(i+1)/2+trackstar.dero_correct_all)/CTS_Dero;
    }
    trackstar.dero_correct_all+=trackstar.alt_correct;
    trackstar.dero_correct=0;
    if(DeroGroup==1)
    {
       derotrackstar.Target_Last= derotrackstar.Target_Group1[1]*CTS_Dero;
    }
    else derotrackstar.Target_Last= derotrackstar.Target_Group2[1]*CTS_Dero;
}

 
void MakeDPRAMCommand ( QString Axis,int Period,const int ROW,double model)
{ 
	QString tmp;

	QStringList  list;
    QString sTargetPos;
	StarTarget startarget;
    //double TargetCommand[NumPerGroup];
    long  LINE=ROW;
    //long L=ROW;


    /*list = starTable[LINE].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	startarget.cDate=list[0];
    startarget.cTime=list[1];*/
 		 
    /*int hour,minute,year,month,day;										  		//hour,miniute
    double second;												//second
    double tmp_date=0,tmp_time=0;
    tmp_time=startarget.cTime.toDouble();

	hour=int ( tmp_time/10000 );
  	minute=int ( ( tmp_time-hour*10000 ) /100 );
	second=tmp_time-hour*10000-minute*100;
  	tmp_date=startarget.cDate.toDouble();
    year=int(tmp_date/10000);
    month=int((tmp_date-year*10000)/100);
	day=int(tmp_date-year*10000-month*100);

	int iDatalength=6;

    if ( Axis=="AZ" )
	{
		aztrackstar.AzTime[0]=year;
		aztrackstar.AzTime[1]=month;
		aztrackstar.AzTime[2]=day;
		aztrackstar.AzTime[3]=hour;
		aztrackstar.AzTime[4]=minute;
		aztrackstar.AzTime[5]=second;
		SetTrackTm ( M185_addr,Tm );
		if ( AzGroup== 1 )
			{SQUMAC::instance_p()->QsendDataDPRAM ( aztrackstar.AzTime,  M188_addr, iDatalength ); }
		else if ( AzGroup== 2 )
			{SQUMAC::instance_p()->QsendDataDPRAM ( aztrackstar.AzTime,  M88_addr, iDatalength );}
	}
	else if ( Axis=="ALT" )
	{
		alttrackstar.AltTime[0]=year;
		alttrackstar.AltTime[1]=month;
		alttrackstar.AltTime[2]=day;
		alttrackstar.AltTime[3]=hour;
		alttrackstar.AltTime[4]=minute;
		alttrackstar.AltTime[5]=second;
      		SetTrackTm(M385_addr,Tm);
		if ( AltGroup== 1 )
			{SQUMAC::instance_p()->QsendDataDPRAM ( alttrackstar.AltTime,  M388_addr, iDatalength ); }
		else if ( AltGroup== 2 )
			{SQUMAC::instance_p()->QsendDataDPRAM ( alttrackstar.AltTime,  M288_addr, iDatalength );}

	}
	else if( Axis=="DERO" )
	{
		derotrackstar.DeroTime[0]=year;
		derotrackstar.DeroTime[1]=month;
		derotrackstar.DeroTime[2]=day;
		derotrackstar.DeroTime[3]=hour;
		derotrackstar.DeroTime[4]=minute;
		derotrackstar.DeroTime[5]=second;
		SetTrackTm ( M485_addr,Tm );
		if ( DeroGroup== 1 )
			{SQUMAC::instance_p()->QsendDataDPRAM ( derotrackstar.DeroTime,  M488_addr, iDatalength ); }
		else if ( DeroGroup== 2 )
			{SQUMAC::instance_p()->QsendDataDPRAM ( derotrackstar.DeroTime,  M588_addr, iDatalength );}
			
    }*/

  /*if(Axis=="AZ")
    {
        LINE=LINE+SoftwareDelayAz*TargetNumPerSecond/1000;//+(NumPerGroup*TrackPeriod)/1000*TargetNumPerSecond;
    //	LINE=LINE+(NumPerGroup*TrackPeriod)/1000*TargetNumPerSecond;
            qDebug()<<"LINE"<<LINE <<SoftwareDelayAz*TargetNumPerSecond/1000;
    }

	if(Axis=="ALT")
	 {
		 LINE=LINE+SoftwareDelayAlt*TargetNumPerSecond/1000;
         	qDebug()<<"LINE alt"<<LINE<<SoftwareDelayAlt*TargetNumPerSecond/1000;
	}
	if(Axis=="DERO")
	 {
		 LINE=LINE+SoftwareDelayDero*TargetNumPerSecond/1000;
         	qDebug()<<"LINE dero"<<LINE<<SoftwareDelayDero*TargetNumPerSecond/1000;
    }*/
    for ( int i=0;i<AzNumPerGroup;i++ )
	{
		if ( LINE  < ( TableNum-1000 ) )
		{
            LINE=LINE+Period/TableLineTime;						//TableLineTime=100ms

			list = starTable[LINE].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts ); //10line=1s,offset in adcance
			if ( list.size() >=5 )
			{
	
				if ( Axis=="AZ" )
					{sTargetPos=list[2];	}
				else if ( Axis=="ALT" )
					{sTargetPos=list[3];	}
				else if ( Axis=="DERO" )
					{sTargetPos=list[4];	}
			}
			else
				qDebug() <<"MakeCommandString list number error"<<list.size();

            if ( Axis=="AZ" )
			{
                aztrackstar.Target_Group1[i]=sTargetPos.toDouble()*3600;
                //aztrackstar.TargetCommandOffset[i]=sTargetPos.toDouble()+model;		//+guidestar  later
			}
			if ( Axis=="ALT" )
			{
                alttrackstar.Target_Group1[i]=sTargetPos.toDouble()*3600;
                //alttrackstar.TargetCommandOffset[i]=sTargetPos.toDouble()+model;		//+guidestar  later
			}
			if ( Axis=="DERO" )
			{
                derotrackstar.Target_Group1[i]=sTargetPos.toDouble()*3600/CTS_Dero;
                //derotrackstar.TargetCommandOffset[i]=sTargetPos.toDouble()+model;		//+guidestar  later
			}

		}
		else
		{
			qDebug() <<"MakeDPRAMCommand()  ROW number error"<<LINE;
			flagtrack.StarTableEnd=true;
			qDebug() <<"MakeDPRAMCommand()  flagtrack.StarTableEnd=true"<<LINE;
			return;
		}

	}
    if ( Axis=="AZ" )       aztrackstar.Target_Group1[AzNumPerGroup]=(aztrackstar.Target_Group1[1]-aztrackstar.Target_Group1[0])/CTS;
    else if ( Axis=="ALT" ) alttrackstar.Target_Group1[AltNumPerGroup]=(alttrackstar.Target_Group1[1]-alttrackstar.Target_Group1[0])/CTS;
    else if ( Axis=="DERO") derotrackstar.Target_Group1[DeroNumPerGroup]=(derotrackstar.Target_Group1[1]-derotrackstar.Target_Group1[0])/CTS_Dero;
    double tmp_val=0;
    if ( Axis=="AZ" )       tmp_val=(aztrackstar.Target_Group1[1]-aztrackstar.Target_Group1[0])*aztrackstar.deltaTime/Period;
    else if ( Axis=="ALT" ) tmp_val=(alttrackstar.Target_Group1[1]-alttrackstar.Target_Group1[0])*alttrackstar.deltaTime/Period;
    else if ( Axis=="DERO") tmp_val=(derotrackstar.Target_Group1[1]-derotrackstar.Target_Group1[0])*derotrackstar.deltaTime/Period;
    for ( int i=0;i<AzNumPerGroup;i++ )
    {
        if ( Axis=="AZ" )       aztrackstar.Target_Group1[i]  = (aztrackstar.Target_Group1[i]  + tmp_val + model)/CTS;
        else if ( Axis=="ALT" ) alttrackstar.Target_Group1[i] = ( alttrackstar.Target_Group1[i]+ tmp_val + model)/CTS;
        else if ( Axis=="DERO") derotrackstar.Target_Group1[i]= (derotrackstar.Target_Group1[i]+ tmp_val + model)/CTS_Dero;
    }
}



/*
void MakeDPRAMCommand ( QString Axis,int Tm,const int ROW,double model)
{ 
	QString tmp;
	double tmp_val;
	QStringList  list;
	QString sTargetPos;
	StarTarget startarget;
	double TargetCommand[NumPerGroup];
        int  LINE=ROW; 
	uint16_t offset;
	int iDatalength=1;
	float fData[10];
	int step=0;
	
    	if ( Axis=="AZ" )
	{
		SetTrackTm(M185_addr,Tm);
// 		SQUMAC::instance_p()->QgetDataDPRAM (fData,  M187_addr, iDatalength);
// 	        qDebug()<<"Az step"<<fData[0];
// 		step=int(fData[0]);
// 		if(step<=10&&step>0)
// 			step=10-step;
//                 else if(step>10&&step<=20)
// 			step=20-step;
//                 else
// 			step=0;
// 
//                qDebug()<<"Az delay"<<step;
//                LINE=LINE+(step+1)*TargetNumPerSecond-SoftwareDelayAz/1000.*TargetNumPerSecond;
	}
	else if ( Axis=="ALT" )
		SetTrackTm(M385_addr,Tm);
	else if( Axis=="DERO" )
	{
		SetTrackTm(M485_addr,Tm);
// 		SQUMAC::instance_p()->QgetDataDPRAM (fData,  M487_addr, iDatalength);
// 	        qDebug()<<"Dero step"<<fData[0];
// 		step=int(fData[0]);
// 		if(step<=10&&step>0)
// 			step=10-step;
//                 else if(step>10&&step<=20)
// 			step=20-step;
//                 else
// 			step=0;
// 
//                qDebug()<<"Dero  delay"<<step;
	      // LINE=LINE+(step+1)*TargetNumPerSecond-SoftwareDelayDero/1000.*TargetNumPerSecond;
} 

 
	for ( int i=0;i<NumPerGroup;i++ )
	{
		if ( LINE  < ( TableNum-1000 ) )
		{
			
			list = starTable[LINE].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts ); //10line=1s,offset in adcance
			if ( list.size() >=5 )
			{
				startarget.cDate=list[0];
				startarget.cTime=list[1];

				if ( Axis=="AZ" )
					{sTargetPos=list[2];	}
				else if ( Axis=="ALT" )
					{sTargetPos=list[3];	}
				else if ( Axis=="DERO" )
					{sTargetPos=list[4];	}
			}
			else
				qDebug() <<"MakeCommandString list number error"<<list.size();

			LINE=LINE+Tm/TableLineTime;						//TableLineTime=10ms
			if ( Axis=="AZ" )
			{
				aztrackstar.AzTargetCommand[i]=sTargetPos.toDouble()+model;
				aztrackstar.AzTargetCommandOffset[i]=sTargetPos.toDouble()+model;		//+guidestar  later
			}
			if ( Axis=="DERO" )
			{
				derotrackstar.DeroTargetCommand[i]=sTargetPos.toDouble()+model;
				derotrackstar.DeroTargetCommandOffset[i]=sTargetPos.toDouble()+model;		//+guidestar  later
			}

		}
		else
		{
			qDebug() <<"MakeDPRAMCommand()  ROW number error"<<LINE;
			flagtrack.StarTableEnd=true;
			qDebug() <<"MakeDPRAMCommand()  flagtrack.StarTableEnd=true"<<LINE;
			return;
		}

	}
}
*/



void  SendTrackingDataToDPRAM(uint16_t offset,double posData[],double velData[],int iDatalength)
{
    int iData[10];
	 

    for(int i=0;i<iDatalength;i++)
    {
        iData[i*4]=int(posData[i]);
        iData[i*4+1]=int((posData[i]-iData[i*4])*10000);
        iData[i*4+2]=int(velData[i]);
        iData[i*4+3]=int((velData[i]-iData[i*4+2])*10000);
    }


    //qDebug("pos:%10.3f",posData[0]);
    //qDebug()<<"Data[0]"<<iData[0]<<"Data[1]"<<iData[1];
    SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength*4);
}


bool   DeroToUMAC_Posix()
{
	if ( DeroGroup== 1 ) 										 
    {
        SendTrackingDataToDPRAM(M2000_DeroGroup1_StartAddr,derotrackstar.Target_Group1,derotrackstar.Vel_Group1,DeroNumPerGroup);
        /*qDebug()<<"time:"<<TrackSeconds<<",dero data to UMAC,group"<<DeroGroup
               <<"target"<<derotrackstar.Target_Group1[0]*CTS_Dero<<derotrackstar.Target_Group1[1]*CTS_Dero
              <<"velocity"<<derotrackstar.Vel_Group1[0]*CTS_Dero<<derotrackstar.Vel_Group1[1]*CTS_Dero;*/
    }
	else if ( DeroGroup== 2 ) 									 
    {
        SendTrackingDataToDPRAM(M2010_DeroGroup2_StartAddr,derotrackstar.Target_Group2,derotrackstar.Vel_Group2,DeroNumPerGroup);
        /*qDebug()<<"time:"<<TrackSeconds<<",dero data to UMAC,group"<<DeroGroup
               <<"target"<<derotrackstar.Target_Group2[0]*CTS_Dero<<derotrackstar.Target_Group2[1]*CTS_Dero
              <<"velocity"<<derotrackstar.Vel_Group2[0]*CTS_Dero<<derotrackstar.Vel_Group2[1]*CTS_Dero;*/
    }
    else
	{
        //trackstar.flag_sendDero=false;
        return false;
	}
	
    //trackstar.flag_sendDero=false;

    FlagPCOutData(M182_addr,DeroGroup*10);
    return true;
}
void   DeroToUMAC(int row)
{
///////////////////send data to UMAC/////////////////////////////////
	QString  s1_dero;
	QByteArray ba;
	QString str,str1;
	axis="DERO";
	qDebug()<<"row DeroToUMAC"<<row;
	dero_model=0;
    MakeDPRAMCommand(axis,DeroTrackPeriod,row,dero_model);
	if ( DeroGroup== 1 ) 										 
    {SendTrackingDataToDPRAM(M2000_DeroGroup1_StartAddr,derotrackstar.Target_Group1,derotrackstar.Vel_Group1,DeroNumPerGroup); }
	else if ( DeroGroup== 2 ) 									 
    {SendTrackingDataToDPRAM(M2010_DeroGroup2_StartAddr,derotrackstar.Target_Group2,derotrackstar.Vel_Group2,DeroNumPerGroup); }
        else
	{
		trackstar.flag_sendDero=false;
		return;
	}
    FlagPCOutData(M182_addr,DeroGroup*10);
}

bool   AzToUMAC_Posix()
{
///////////////////send data to UMAC/////////////////////////////////
    qDebug("///////////////////az send data to UMAC/////////////////////////////////");
	if ( AzGroup== 1 ) 										 
        {SendTrackingDataToDPRAM(M5000_AzGroup1_StartAddr,aztrackstar.Target_Group1,aztrackstar.Vel_Group1,AzNumPerGroup); }
	else if ( AzGroup== 2 ) 									 
        {SendTrackingDataToDPRAM(M5010_AzGroup2_StartAddr,aztrackstar.Target_Group2,aztrackstar.Vel_Group2,AzNumPerGroup); }
    else
	{
        //trackstar.flag_sendAz=false;
        return false;
	}

    QString S1="      ";
    QString S2="AzGroup";
    for(int i=0;i<AzNumPerGroup;i++)
    {
        if(AzGroup==1)S1=S1+QString::number(aztrackstar.Target_Group1[i],'f',10)+"    ";
        else S1=S1+QString::number(aztrackstar.Target_Group2[i],'f',10)+"    ";
    }

	S2=S2+QString::number(AzGroup ,10);
    writeLog1->writeregular(S1,S2);

    //trackstar.flag_sendAz=false;
    FlagPCOutData(M185_addr,AzGroup*10);
    return true;
}
void   AzToUMAC(int row)
{
///////////////////send data to UMAC/////////////////////////////////
	QString  s1_az,str;
	QByteArray ba;
	axis="AZ";
	 
	
    MakeDPRAMCommand(axis,AzTrackPeriod,row,az_model);
	if ( AzGroup== 1 ) 										 
    {SendTrackingDataToDPRAM(M5000_AzGroup1_StartAddr,aztrackstar.Target_Group1,aztrackstar.Vel_Group1,AzNumPerGroup); }
	else if ( AzGroup== 2 ) 									 
    {SendTrackingDataToDPRAM(M5010_AzGroup2_StartAddr,aztrackstar.Target_Group2,aztrackstar.Vel_Group2,AzNumPerGroup); }
    else
	{
        //trackstar.flag_sendAz=false;
		return;
	}

    //AzGroup=0;
    FlagPCOutData(M185_addr,AzGroup*10);
}



bool   AltToUMAC_Posix()
{
qDebug("///////////////////altsend data to UMAC/////////////////////////////////");
	if ( AltGroup== 1 ) 										 
    {SendTrackingDataToDPRAM(M6000_AltGroup1_StartAddr,alttrackstar.Target_Group1,alttrackstar.Vel_Group1,AltNumPerGroup); }
	else if ( AltGroup== 2 ) 									 
    {SendTrackingDataToDPRAM(M6010_AltGroup2_StartAddr,alttrackstar.Target_Group2,alttrackstar.Vel_Group2,AltNumPerGroup); }
    else
	{
        //trackstar.flag_sendAlt=false;
        return false;
	}
	
	trackstar.flag_sendAlt=false;
    FlagPCOutData(M186_addr,AltGroup*10);
    return true;
}
void   AltToUMAC(int row)
{
///////////////////send data to UMAC/////////////////////////////////
	axis="ALT";
    MakeDPRAMCommand(axis,AltTrackPeriod,row,alt_model);
	if ( AltGroup== 1 ) 										 
    {SendTrackingDataToDPRAM(M6000_AltGroup1_StartAddr,alttrackstar.Target_Group1,alttrackstar.Vel_Group1,5); }
	else if ( AltGroup== 2 ) 									 
    {SendTrackingDataToDPRAM(M6010_AltGroup2_StartAddr,alttrackstar.Target_Group2,alttrackstar.Vel_Group2,5); }
    else
	{
        //trackstar.flag_sendAlt=false;
		return;
	}
    //AltGroup=0;
    FlagPCOutData(M186_addr,AltGroup*10);
}

void AltMakeStr(int Alt_row)
{

	QByteArray ba;
	QString str,str1;
	axis="ALT";
	int StartP=UmacStartP1_Alt;											//P5000    P5010;
	
	if(AltGroup==AltGroupLast)
      AltGroup=CheckUmacFlag_Alt(M1784_addr);//CheckUmacFlag_Alt();
	qDebug()<<"AltGroup"<<AltGroup<<"AltGroupLast"<<AltGroupLast;

    if(AltGroup==AltGroupLast)
           return;

	if ( AltGroup== 1 ) 										 
		{StartP=UmacStartP1_Alt;trackstar.flag_sendAlt=true;}
	else if ( AltGroup== 2 ) 									 
		{StartP=UmacStartP2_Alt;trackstar.flag_sendAlt=true;}
        else
		{
			trackstar.flag_sendAlt=false;
			return;
		}

    // 	qDebug() <<"tracking N_row"<<N_row<<"AltGroup"<<AltGroup;


//	GetEndTime();
//	trackstar.alt_offset=trackstar.delta_altTargetDelay+trackstar.alt_offset+timeuse*trackstar.ALT_TrackTargetSpeed;
//	trackstar.alt_offset=trackstar.delta_altTargetDelay+trackstar.alt_offset;
	//int Alt_row=N_row+110;
     //   int Alt_row=N_row+deltaLinesAlt;
        MakeStr ( axis,AltTrackPeriod,Alt_row,StartP,trackstar.alt_offset,trackstar.alt_correct,alt_model,trackstar.alt_guidedata,&trackstar.cCmdtoAlt,&trackstar.AltTargetOffset );
	if(flagtrack.StarTableEnd)
 		trackstar.flag_sendAlt=false;
	  
}
void DeroMakeStr(int Dero_row)
{
	QByteArray ba;
	QString str,str1;
	axis="DERO";
	int StartP=UmacStartP1_Dero;											//P6000    P6010
        
        qDebug()<<"DeroGroup"<<DeroGroup<<"DeroGroupLast"<<DeroGroupLast;
 
	if(DeroGroup==DeroGroupLast)
      DeroGroup=CheckUmacFlag_Dero(M284_addr);//CheckUmacFlag_Dero();
        if(DeroGroup==DeroGroupLast)
        return;
        qDebug()<<"DeroGroup"<<DeroGroup<<"DeroGroupLast"<<DeroGroupLast;
 
	if ( DeroGroup== 1 ) 										//check P484,p484=1?
		{StartP=UmacStartP1_Dero;trackstar.flag_sendDero=true;}
	else if ( DeroGroup== 2 ) 									//check P484=2?
		{StartP=UmacStartP2_Dero;trackstar.flag_sendDero=true;}
        else
		{
			trackstar.flag_sendDero=false;
			return;
		}
	//dero_model=0;
    MakeStr ( axis,DeroTrackPeriod,Dero_row,StartP,trackstar.dero_offset,trackstar.dero_correct,dero_model,trackstar.dero_guidedata,&trackstar.cCmdtoDero,&trackstar.DeroTargetOffset );
	if(flagtrack.StarTableEnd)
 		trackstar.flag_sendDero=false;
}


void CheckUmacFlag()
{
    int iData[10];
    int iDatalength=6;
    uint16_t offset=M182_addr;
    SQUMAC::instance_p()->QgetDataDPRAM (iData,  offset, iDatalength);
    DeroGroup=iData[0];
    ADC1Group=iData[1];
    ADC2Group=iData[2];
    AzGroup=iData[3];
    AltGroup=iData[4];
}



// void CheckUmacFlag_Alt()
// {
//  ///////////////////send data to UMAC/////////////////////////////////
// 	QByteArray ba;
// 	QString str,str1;
// 	
// 	QStringList  list;
// 	int n=1;
// 	char cmd[255];
// 	sprintf ( cmd,"P384" );
// 	GetStatusFromUMAC ( cmd,n, &list );
//         AltGroup=list[0].toInt();
// // 	if ( list[0]== "1" ) 										//check P148,p148=1?
// // 		AltGroup=1;
// // 	else if ( list[0]== "2" ) 									//check P384=2?
// // 		AltGroup=2;
// }


// void CheckUmacFlag_Az()
// {
//  ///////////////////send data to UMAC/////////////////////////////////
// 	QByteArray ba;
// 	QString str,str1;
// 	
// 	QStringList  list;
// 	int n=1;
// 	char cmd[255];
// 	sprintf ( cmd,"P184" );
// 	GetStatusFromUMAC ( cmd,n, &list );
// 	AzGroup=list[0].toInt();
//  
// }


void CheckUmacStep(uint16_t offset,float *step)
{
        float fData[10];
	int iDatalength=1;
        SQUMAC::instance_p()->QgetDataDPRAM (fData,  offset, iDatalength);
	*step=fData[0];
}

int CheckUmacFlag_Az(uint16_t offset)
{
        int iData[10];
	int iDatalength=1;
       // uint16_t offset=M584_addr;
        SQUMAC::instance_p()->QgetDataDPRAM (iData,  offset, iDatalength);
    //AzGroup=fData[0];
        return (iData[0]);
}


int CheckUmacFlag_Alt(uint16_t offset)
{ 
        int iData[10];
	int iDatalength=1;
       // uint16_t offset=M684_addr;
        SQUMAC::instance_p()->QgetDataDPRAM (iData,  offset, iDatalength);
    //AltGroup=fData[0];
     return (iData[0]);
 
}
void  FlagPCOutData(uint16_t offset,int group)
{
        int iData[10];
	int iDatalength=1;
        iData[0]=group;
        SQUMAC::instance_p()->QsendDataDPRAM (iData,  offset, iDatalength);
}

int CheckUmacFlag_Dero(uint16_t offset)
{
        int iData[10];
	int iDatalength=1;
       // uint16_t offset=M284_addr;
        SQUMAC::instance_p()->QgetDataDPRAM (iData,  offset, iDatalength);
    //DeroGroup=fData[0];
    return (iData[0]);

}

 
// void CheckUmacFlag_Dero()
// {
//  ///////////////////send data to UMAC/////////////////////////////////
// 	QByteArray ba;
// 	QString str,str1;
// 	
// 	QStringList  list;
// 	int n=1;
// 	char cmd[255];
// 	sprintf ( cmd,"P484" );
// 	GetStatusFromUMAC ( cmd,n, &list );
// 	DeroGroup=list[0].toInt();;
// // 	if ( list[0]== "1" ) 										//check P484,p484=1?
// // 		DeroGroup=1;
// // 	else if ( list[0]== "2" ) 									//check P484=2?
// // 		DeroGroup=2;
// }

//tracking

// void   AltToUMAC_Posix()
// {
// 	//MakeStr ( axis,Tm,N_row,StartP,trackstar.alt_offset,alt_model,trackstar.alt_guidedata,&s1_alt,&trackstar.AltTargetOffset );
// 				//ALT 10 lines
// 	//qDebug() <<"command"<<ba;
// 	trackstar.flag_sendAlt=false;							//don't send out data to UMAC
// }


//tracking
// void   DeroToUMAC_Posix()
// {
// 	QByteArray ba;
// 	QString str1;
// 	ba= trackstar.cCmdtoDero.toLatin1();
// 	SQUMAC::instance_p()->QcommCmdPMAC ( ba,str1 );					//Dero 10 lines
// 	qDebug() <<"command"<<ba;
// 	trackstar.flag_sendDero=false;							//don't send out data to UMAC
// 	qDebug()<<"tracking N_row"<<N_row;
// 	 
// 
// 
// }


//int StartP:  ALT P5000
//Tm:time 100ms
//offset: "
//correct: degree
// void MakeStr ( QString Axis,int Tm,const int ROW,int StartP,double offset,double correct[10],double model,double guidedata,QString *s1,double *TrackTargetOffset )
// {
// 
// 	QString sTM;
// 	QString tmp;
// 	double tmp_val;
// 	QStringList  list;
// 	QString sTargetPos;
// 	// ROW=N_row_LAST;
// 	StarTarget startarget;
//         int  LINE=ROW; 
// 
// 	if ( Axis=="AZ" )
// 	{
//             if(AzGroup==1)
// 		sTM="P185="+QString::number ( Tm, 10 )+"P184=10";
//             if(AzGroup==2)
// 		sTM="P185="+QString::number ( Tm, 10 )+"P184=20";
// 
// 	}
// 
// 	else if ( Axis=="ALT" )
// 	{
//               	if(AltGroup==1)
// 			sTM="P385="+QString::number ( Tm, 10 )+"P384=10";
//                 if(AltGroup==2)
// 			sTM="P385="+QString::number ( Tm, 10 )+"P384=20";
//   
// 	}
// 	else if ( Axis=="DERO" )
// 		{
//                   if(DeroGroup==1)
//                       sTM="P485="+QString::number ( Tm, 10 )+"P484=10";
//                    if(DeroGroup==2)
//                        sTM="P485="+QString::number ( Tm, 10 )+"P484=20";
// 
//                    }
// 
// 	*s1=sTM;
// 	 int num=StartP;
// 
// 	
//         //int num=TrackPeriod/TableLineTime;
// 
// 	for ( int i=0;i<=9;i++ )
// 	{
// 		if ( LINE  < ( TableNum-1000 ) )
// 		{
// 			list = starTable[LINE].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts ); //10line=1s,offset in adcance
// 			if ( list.size() >=5 )
// 			{
// 				startarget.cDate=list[0];
// 				startarget.cTime=list[1];
// 
// 				if ( Axis=="AZ" )
// 					{sTargetPos=list[2];		sTM="P185="+QString::number ( Tm, 10 );}
// 				else if ( Axis=="ALT" )
// 					{sTargetPos=list[3];		sTM="P385="+QString::number ( Tm, 10 );}
// 				else if ( Axis=="DERO" )
// 					{sTargetPos=list[4];		sTM="P485="+QString::number ( Tm, 10 );}
// 			}
// 			else
// 				qDebug() <<"MakeCommandString list number error"<<list.size();
// 				LINE=LINE+Tm/TableLineTime;						//TableLineTime=10ms
// 		}
// 		else
// 			{
// 				qDebug() <<"MakeStr()  ROW number error"<<LINE;
// 				trackstar.StarTableEnd=true;
// 				return;
// 			}
// 
// 		tmp_val=sTargetPos.toDouble();
// 		//tmp_val=tmp_val+offset/3600.+model+guidedata/3600.+correct[i]/3600.;               //offset:" ,     guidedata:",correct:"
// 	//	tmp_val=tmp_val+model+guidedata/3600.+correct[i]/3600.;               		//offset:" ,     guidedata:",correct:"
// 		tmp_val=tmp_val+model+correct[i]/3600.;               				//offset:" ,     guidedata:",correct:"
// 		*TrackTargetOffset=tmp_val;					//degree
// 		tmp=QString::number ( double ( tmp_val ),'f',6 );
// 
// 		*s1=*s1+"P"+QString::number ( num, 10 ) +"="+tmp;
// 		num++;
// 	}
// 
// 
// 
// }

void  SetTrackTm(uint16_t offset,float Tm)
{
        float fData[10];
	int iDatalength=1;
       	fData[0]=Tm;
        SQUMAC::instance_p()->QsendDataDPRAM (fData,  offset, iDatalength);
}

void MakeStr ( QString Axis,int Tm,const int ROW,int StartP,double offset,double correct,double model,double guidedata,QString *s1,double *TrackTargetOffset )
{

	QString sTM;
	QString tmp;
	double tmp_val;
	QStringList  list;
	QString sTargetPos;
	// ROW=N_row_LAST;
	StarTarget startarget;
        int  LINE=ROW; 

	if ( Axis=="AZ" )
	{
//             if(AzGroup==1)
// 		sTM="P185="+QString::number ( Tm, 10 )+"P184=10";
//             if(AzGroup==2)
// 		sTM="P185="+QString::number ( Tm, 10 )+"P184=20";
//             if(AzGroup==1)
// 		sTM="P185="+QString::number ( Tm, 10 );
//             if(AzGroup==2)
// 		sTM="P185="+QString::number ( Tm, 10 );
		SetTrackTm(M185_addr,Tm);
	}

	else if ( Axis=="ALT" )
	{
              	if(AltGroup==1)
			sTM="P385="+QString::number ( Tm, 10 )+"P384=10";
                if(AltGroup==2)
			sTM="P385="+QString::number ( Tm, 10 )+"P384=20";
  
	}
	else if ( Axis=="DERO" )
		{
                  if(DeroGroup==1)
                      sTM="P485="+QString::number ( Tm, 10 )+"P484=10";
                   if(DeroGroup==2)
                       sTM="P485="+QString::number ( Tm, 10 )+"P484=20";

                   }

	*s1=sTM;
	 int num=StartP;

	
        //int num=TrackPeriod/TableLineTime;

	for ( int i=0;i<=9;i++ )
	{
		//LINE=LINE+(int)correct[i];		
		if ( LINE  < ( TableNum-1000 ) )
		{
			
			list = starTable[LINE].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts ); //10line=1s,offset in adcance
			if ( list.size() >=5 )
			{
				startarget.cDate=list[0];
				startarget.cTime=list[1];

				if ( Axis=="AZ" )
					{sTargetPos=list[2];	}
				else if ( Axis=="ALT" )
					{sTargetPos=list[3];	}
				else if ( Axis=="DERO" )
					{sTargetPos=list[4];	}
			}
			else
				qDebug() <<"MakeCommandString list number error"<<list.size();

				LINE=LINE+Tm/TableLineTime;						//TableLineTime=10ms
				//qDebug()<<"line"<<LINE<<correct[i];
		}
		else
			{
				qDebug() <<"MakeStr()  ROW number error"<<LINE;
				flagtrack.StarTableEnd=true;
				qDebug()<<"makeStr  flagtrack.StarTableEnd=true";
				return;
			}

		tmp_val=sTargetPos.toDouble();

		//tmp_val=tmp_val+offset/3600.+model+guidedata/3600.+correct[i]/3600.;               //offset:" ,     guidedata:",correct:"
	//	tmp_val=tmp_val+model+guidedata/3600.+correct[i]/3600.;               		//offset:" ,     guidedata:",correct:"
		//tmp_val=tmp_val+model+correct[i]/3600.;               				//offset:" ,     guidedata:",correct:"
                tmp_val=tmp_val+model;               				//offset:" ,     guidedata:",correct:"
		*TrackTargetOffset=tmp_val;					//degree
		tmp=QString::number ( double ( tmp_val ),'f',6 );

		*s1=*s1+"P"+QString::number ( num, 10 ) +"="+tmp;
		num++;
	}



}

/////////////////  posix timer ////////////////////////////
void  StartPosixTimer()
{
	//posix_timer
// 	sigset_t sigset;
// 	sigfillset ( &sigset );
// 	sigdelset ( &sigset, SIGRTMIN );
// 	sigprocmask ( SIG_SETMASK, &sigset, NULL );
// 
// 	struct sigaction sa;
// 	sigfillset ( &sa.sa_mask );
// 	sa.sa_flags = SA_SIGINFO;
// 	sa.sa_sigaction = handlerPosix;
// 	signal ( SIGALRM,test );
//  
// 	if ( sigaction ( SIGRTMIN, &sa, NULL ) < 0 )
// 	{
// 		perror ( "sigaction failed " );
// 		exit ( -1 );
// 	}
// 
// 	struct sigevent timer_event;
// 	struct itimerspec timer;
// 
// 	timer.it_interval.tv_sec = 0;									//struct timespec ,seconds and nanoseconds
// 	timer.it_interval.tv_nsec =100 * 1000 * 1000;						        //100 * 1000 * 1000;   100ms
// 	timer.it_value = timer.it_interval;
// 
// 	timer_event.sigev_notify = SIGEV_SIGNAL;
// 	timer_event.sigev_signo = SIGRTMIN;
// 	timer_event.sigev_value.sival_ptr = ( void * ) &PosixT;
// 
//    
// 	if ( timer_create ( CLOCK_REALTIME, &timer_event, &PosixT ) < 0 )				//create timer
// 	{
// 		perror ( "timer_create failed" );
// 		exit ( -1 );
// 	}
// 
// 	if ( timer_settime ( PosixT, 0, &timer, NULL ) < 0 ) //
// 	{
// 		perror ( "timer_settime failed" );
// 		exit ( -1 );
// 	}
   
 struct sigevent evp;
 struct itimerspec ts;
 //timer_t timer;
 int ret;

 memset   (&evp,   0,   sizeof   (evp));
 evp.sigev_value.sival_ptr = &PosixT;
 evp.sigev_notify = SIGEV_THREAD;
 evp.sigev_notify_function = handle;
 evp.sigev_value.sival_int = 3;   						//handle() parameter

 ret = timer_create(CLOCK_REALTIME, &evp, &PosixT);

 if( ret)
  perror("timer_create");

 ts.it_interval.tv_sec =0;
 ts.it_interval.tv_nsec =PosixTClock;					        //100 * 1000 * 1000;   100ms;
 ts.it_value.tv_sec = 0;
 ts.it_value.tv_nsec = 0;
 ts.it_value = ts.it_interval;
QDateTime dt=QDateTime::currentDateTime();
QTime time=dt.time();
double ms=time.msec();
ms=ms-int(ms/100)*100;

/*double  period=(50-ms)*1000*1000;						//around 50ms
if(period<0)
    period=period+100*1000*1000;*/
  double  period=(100-ms)*1000*1000;
 ts.it_value.tv_nsec=period;
 ret = timer_settime(PosixT, 0, &ts, NULL);
//ret = timer_settime(PosixT, TIMER_ABSTIME, &ts, NULL);
 if( ret )
  perror("timer_settime");
	//gettimeofday(&tpstart,NULL); 
 double tmp=period/(1000*1000);
 qDebug()<<tmp<<"tracking timer will start after "<<time.msec()<<"ms";

}









// struct timeval { 
// long tv_sec; /* seconds */ 
// long tv_usec; /* us*/ 
// }; 
 
//int gettimeofday(struct timeval *tv,struct timezone *tz); 
// void  handlerPosix ( int sig, siginfo_t * extra, void *cruft )
// {
//         struct timeval tpstart,tpend; 
// 	float timeuse; 
// 	gettimeofday(&tpstart,NULL); 
//  
// 	LookupTable_Posix();
// 	GetAzAltPosition_Posix();						//AzPosRealForError  ,AltPosRealForError
// 	GetDeroPosition_Posix();						//get deroPosReal_Degree_forError
// 	//TrackDataForSave();
// 
// 	lock.lockForWrite();
// 	trackstar.AzTrackError= ( trackstar.AzTarget-trackstar.AzPosRealForError ) *3600;
// 	trackstar.AltTrackError= ( trackstar.AltTarget-trackstar.AltPosRealForError ) *3600;
// 	trackstar.DeroTrackError= ( trackstar.DeroTarget-trackstar.DeroPosRealForError ) *3600;
// 	lock.unlock();
// 	if ( trackstar.flagAzTracking )
// 		AzToUMAC_Posix();
// 	if ( trackstar.flagAltTracking )
// 		AltToUMAC_Posix();
// 	if ( trackstar.flagDeroTracking )
// 		DeroToUMAC_Posix();
// 	
// 	gettimeofday ( &tpend,NULL );
// 	timeuse=1000000* ( tpend.tv_sec-tpstart.tv_sec ) +
// 	        tpend.tv_usec-tpstart.tv_usec;
// 	timeuse/=1000000;
// 	qDebug() <<"Used Time:"<<timeuse;
// 
// 	
// }



////get Az/Alt position:get AzPosRealForError  ,AltPosRealForError
void  GetAzAltPosition_Posix()
{
	QString str;
	char  cmd[255];

    sprintf ( cmd,"m2162m2164m2165m1762m1764m1765" );
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );

	QStringList  list = str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	int it=0;
	for ( QStringList::iterator point = list.begin(); point != list.end();++it, ++point )
		{}
	//qDebug()<<it;
	if ( it<6 )
	{

		return;
	};
	//lock.lockForWrite();
	if(list.size()>=6)
	{
		trackstar.AzPosRealForError = ( ( list[0].toDouble() +list[1].toDouble() ) /3072.0 ) *CTS/3600;		//degree
		trackstar.AltPosRealForError = ( ( list[3].toDouble() +list[4].toDouble() ) /3072.0 ) *CTS/3600;	//degree
		trackstar.AzFolErrUMAC= ( list[2].toDouble() /3072.0 ) *CTS;					 	//"
		trackstar.AltFolErrUMAC= ( list[5].toDouble() /3072.0 ) *CTS;						//"
	}
//	lock.unlock();	
 	
}



// ////get Az/Alt position:get AzPosRealForError  ,AltPosRealForError
// void  GetAzAltPosition_UMAC()
// {
// 	QString str;
// 	char  cmd[255];
// 
// 	sprintf ( cmd,"m161m162m164m165m261m262m264m265" );
// 	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// 
// 	QStringList  list = str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
// 	int it=0;
// 	for ( QStringList::iterator point = list.begin(); point != list.end();++it, ++point )
// 		{}
// 	//qDebug()<<it;
// 	if ( it<8 )
// 	{
// 
// 		return;
// 	};
// 	//lock.lockForWrite();
// 	if(list.size()>=8)
// 	{
// 		trackstar.AzTargetUMAC=list[0].toDouble() /3072.0*CTS/3600;	
// 		trackstar.AzPosRealForError = ( ( list[1].toDouble() +list[2].toDouble() ) /3072.0 ) *CTS/3600;		//degree
// 		trackstar.AltPosRealForError = ( ( list[5].toDouble() +list[6].toDouble() ) /3072.0 ) *CTS/3600;	//degree
// 		trackstar.AltTargetUMAC=list[4].toDouble() /3072.0*CTS/3600;
// 		trackstar.AzFolErrUMAC= ( list[3].toDouble() /3072.0 ) *CTS;					 	//"
// 		trackstar.AltFolErrUMAC= ( list[7].toDouble() /3072.0 ) *CTS;						//"
// 	}
// //	lock.unlock();	
//  	
// }



//get DeroPosRealForError
void  GetDeroPosition_Posix()
{
	QString str;
	char  cmd[255];
    if ( flagtcs.flag_DERO ) sprintf ( cmd,"m262m264m265" );
    /*else if ( flagtcs.flag_ADC1 ) sprintf ( cmd,"m362m364m365" );
    else if ( flagtcs.flag_ADC2 ) sprintf ( cmd,"m462m464m465" );*/

	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );

	QStringList  list = str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	int it=0;
	for ( QStringList::iterator point = list.begin(); point != list.end();++it, ++point )
		{}
	//qDebug()<<it;
    if ( it<3 ) return;

    if ( flagtcs.flag_DERO )
	{
//		lock.lockForWrite();
        trackstar.DeroPosRealForError = ( ( list[0].toDouble() +list[1].toDouble() ) /3072.0 ) *CTS_Dero/3600;		//degree
        trackstar.DeroFolErrUMAC= ( list[2].toDouble() /3072.0 ) *CTS_Dero;
//		lock.unlock();						//"
	}
    /*    else if ( flagtcs.flag_ADC2 )
	{
//		lock.lockForWrite();
                trackstar.DeroPosRealForError  = ( ( list[0].toDouble() +list[1].toDouble() ) /3072.0 ) *CTS_ADC/3600;		//degree
                trackstar.DeroFolErrUMAC= ( list[2].toDouble() /3072.0 ) *CTS_ADC;
//		lock.unlock();							//"
	}
        else if ( flagtcs.flag_ADC1 )
	{
//		lock.lockForWrite();
                trackstar.DeroPosRealForError  = ( ( list[0].toDouble() +list[1].toDouble() ) /3072.0 ) *CTS_ADC/3600;		//degree
                trackstar.DeroFolErrUMAC= ( list[2].toDouble() /3072.0 ) *CTS_ADC;
//		lock.unlock();							//"
    }*/
}





void  GetAzAltDeroPosition_Posix()
{
	QString str;
        //char  cmd[255];
    QString S1="m2162m2164m2165m2174m1762m1764m1765m1774";
	QString S2="";
	QString S0="";
    
    if ( flagtcs.flag_DERO ) S2="m262m264m265m274" ;
    /*else if ( flagtcs.flag_ADC2 ) S2="m362m364m365m374" ;
    else if ( flagtcs.flag_ADC1 ) S2="m462m464m465m474" ;*/

    S0=S1+S2;
    QByteArray ba;
	
	ba= S0.toLatin1();
	SQUMAC::instance_p()->QcommCmdPMAC ( ba,str);					//AZ 10 lines
 

	QStringList  list = str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	int it=0;
	for ( QStringList::iterator point = list.begin(); point != list.end();++it, ++point )
		{}
	//qDebug()<<it;
    if ( it<8 ) return;

	//lock.lockForWrite();
	if(list.size()>=8)
	{
		trackstar.AzPosRealForError = ( ( list[0].toDouble() +list[1].toDouble() ) /3072.0 ) *CTS/3600;		//degree
		trackstar.AltPosRealForError = ( ( list[4].toDouble() +list[5].toDouble() ) /3072.0 ) *CTS/3600;	//degree
		trackstar.AzFolErrUMAC= ( list[2].toDouble() /3072.0 ) *CTS;					 	//"
		trackstar.AltFolErrUMAC= ( list[6].toDouble() /3072.0 ) *CTS;						//"
		trackstar.AzVelocity= list[3].toDouble() *CTS*1000*2/3072.0;						// change counts/ms to "/s
       		trackstar.AltVelocity= list[7].toDouble() *CTS*1000*2/3072.0;						// change counts/ms to "/s
       
	}
//	lock.unlock();
    if(list.size()>=12)
	{
        if ( flagtcs.flag_DERO )
        {
//		lock.lockForWrite();
                trackstar.DeroPosRealForError = ( ( list[8].toDouble() +list[9].toDouble() ) /3072.0 ) *CTS_Dero/3600;		//degree
                trackstar.DeroFolErrUMAC= ( list[10].toDouble() /3072.0 ) *CTS_Dero;
                trackstar.DeroVelocity= list[11].toDouble() *CTS_Dero*1000*2/3072.0;						// change counts/ms to "/s
       

//		lock.unlock();						//"
        }
        /*else if ( flagtcs.flag_ADC2 )
        {
//		lock.lockForWrite();
                trackstar.DeroPosRealForError  = ( ( list[8].toDouble() +list[9].toDouble() ) /3072.0 ) *CTS_ADC/3600;		//degree
                trackstar.DeroFolErrUMAC= ( list[10].toDouble() /3072.0 ) *CTS_ADC;
                trackstar.DeroVelocity= list[11].toDouble() *CTS_ADC*1000*2/3072.0;						// change counts/ms to "/s
       
//		lock.unlock();							//"
        }
        else if ( flagtcs.flag_ADC1 )
        {
//		lock.lockForWrite();
                trackstar.DeroPosRealForError  = ( ( list[8].toDouble() +list[9].toDouble() ) /3072.0 ) *CTS_ADC/3600;		//degree
                trackstar.DeroFolErrUMAC= ( list[10].toDouble() /3072.0 ) *CTS_ADC;
                trackstar.DeroVelocity= list[11].toDouble() *CTS_ADC*1000*2/3072.0;						// change counts/ms to "/s
  
//		lock.unlock();							//"
        }*/

    }
	
 	
}


void IniUmacTime()
{
/////////////////////////////get local time/////////////////////////////////////
	QDateTime dt;
	dt=QDateTime::currentDateTime();
	QDate date;
	QTime time;
	date=dt.date();
	time=dt.time();
//	QString s1,sYear,sMonth,sDay,sHour,sMiniute,sSecond;
	QByteArray ba;
	QString str1;
	SQUMAC::instance_p()->QcommCmdPMAC (ClearAllTimeBuff,str1 );	
	sleep(0.1);
	double Sec = time.second()+time.msec() /1000.0;
	int Min =time.minute();
	int Hour =time.hour();
 	int Day =date.day();
 	int Month =date.month();
	int Year =date.year();
        //char cmd[255];
     

      	QString str=QString ("%1=%2%3=%4%5=%6%7=%8%9=%10%11=%12%13").arg (YearAtPwrUp).arg ( Year,0,10 ).arg (MonthAtPwrUp).arg ( Month,0,10 ).arg (DayAtPwrUp).arg ( Day,0,10 ).arg ( HourAtPwrUp ).arg (Hour,0,10).arg ( MinAtPwrUp ).arg (Min,0,10).arg ( SecAtPwrUp ).arg (Sec,0,'f',3).arg (FirstIteration);
	
 	qDebug()<<"time"<<str;

 	ba= str.toLatin1();
 	SQUMAC::instance_p()->QcommCmdPMAC ( ba,str1 );		
 	SQUMAC::instance_p()->QcommCmdPMAC (StartUMACTime,str1 );

}

void  GetAzPosition_DPRAM()
{
//    SQUMAC::instance_p()->Qgetinformation(Motor21_addr, Motor[5].followingError, Motor[5].servoCommand, Motor[5].servoStatus,
//                                          Motor[5].generalStatus, Motor[5].positionBias, Motor[5].filteredActuralVelocity,
//                                          Motor[5].masterPosition, Motor[5].netActualPosition);

//    trackstar.AzPosRealForError = ( Motor[5].netActualPosition /3072.0 ) *CTS_az/3600;				//degree
//    trackstar.AzFolErrUMAC= ( Motor[5].followingError /3072.0 ) *CTS_az;
//    trackstar.AzVelocity=  Motor[5].filteredActuralVelocity *CTS_az*1000*2/3072.0;				// change counts/ms to "/s
//    trackstar.AzCommand= Motor[5].servoCommand;
    trackstar.AzVelocity = s_Az.Velocity;
    trackstar.AzPosRealForError = s_Az.PosReal_Degree;
    trackstar.AzFolErrUMAC = s_Az.error;

}

void  GetAltPosition_DPRAM()
{
//    SQUMAC::instance_p()->Qgetinformation ( Motor17_addr, Motor[6].followingError, Motor[6].servoCommand, Motor[6].servoStatus, Motor[6].generalStatus, Motor[6].positionBias, Motor[6].filteredActuralVelocity, Motor[6].masterPosition, Motor[6].netActualPosition );
	
//    trackstar.AltPosRealForError= ( Motor[6].netActualPosition /3072.0 ) *CTS_alt/3600;				//degree
//    trackstar.AltFolErrUMAC= ( Motor[6].followingError /3072.0 ) *CTS_alt;
//    trackstar.AltVelocity=  Motor[6].filteredActuralVelocity *CTS_alt*1000*2/3072.0;				// change counts/ms to "/s
//    trackstar.AltCommand= Motor[6].servoCommand;
    trackstar.AltVelocity = s_Alt.Velocity;
    trackstar.AltPosRealForError = s_Alt.PosReal_Degree;
    trackstar.AltFolErrUMAC = s_Alt.error;
 }


void  GetDeroPosition_DPRAM()
{
    SQUMAC::instance_p()->Qgetinformation ( Motor2_addr, Motor[2].followingError, Motor[2].servoCommand, Motor[2].servoStatus, Motor[2].generalStatus, Motor[2].positionBias, Motor[2].filteredActuralVelocity, Motor[2].masterPosition, Motor[2].netActualPosition );
	
        trackstar.DeroPosRealForError = ( Motor[2].netActualPosition /3072.0 ) *CTS_Dero/3600;				//degree
        trackstar.DeroFolErrUMAC= ( Motor[2].followingError /3072.0 ) *CTS_Dero;
        trackstar.DeroVelocity=  Motor[2].filteredActuralVelocity *CTS_Dero*1000*2/3072.0;				// change counts/ms to "/s
	
}

void  GetADC2Position_DPRAM()
{
    SQUMAC::instance_p()->Qgetinformation ( Motor4_addr, Motor[4].followingError, Motor[4].servoCommand, Motor[4].servoStatus, Motor[4].generalStatus, Motor[4].positionBias, Motor[4].filteredActuralVelocity, Motor[4].masterPosition, Motor[4].netActualPosition );
	
        trackstar.ADC2PosRealForError= ( Motor[4].netActualPosition /3072.0 ) *CTS_ADC/3600;				//degree
        trackstar.ADC2FolErrUMAC= ( Motor[4].followingError /3072.0 ) *CTS_ADC;
        trackstar.ADC2Velocity=  Motor[4].filteredActuralVelocity *CTS_ADC*1000*2/3072.0;				// change counts/ms to "/s
 }

void  GetADC1Position_DPRAM()
{

    SQUMAC::instance_p()->Qgetinformation ( Motor3_addr, Motor[3].followingError, Motor[3].servoCommand, Motor[3].servoStatus, Motor[3].generalStatus, Motor[3].positionBias, Motor[3].filteredActuralVelocity, Motor[3].masterPosition, Motor[3].netActualPosition );
	
 	
        trackstar.ADC1PosRealForError= ( Motor[3].netActualPosition /3072.0 ) *CTS_ADC/3600;				//degree
        trackstar.ADC1FolErrUMAC= ( Motor[3].followingError /3072.0 ) *CTS_ADC;
        trackstar.ADC1Velocity=  Motor[3].filteredActuralVelocity *CTS_ADC*1000*2/3072.0;				// change counts/ms to "/s
  
}


////get Az/Alt position:get AzPosRealForError  ,AltPosRealForError
void  GetAzAltDeroPosition_UMAC()
{
	QString str;
        //char  cmd[255];
    QString S1="m2161m2162m2164m2165m2174m1761m1762m1764m1765m1774";
	QString S2="";
	QString S0="";
    
    if ( flagtcs.flag_DERO )
                {S2="m261m262m264m265m274" ;}
        /*else if ( flagtcs.flag_ADC1 )
                {S2="m361m362m364m365m374" ;}
        else if ( flagtcs.flag_ADC2 )
                {S2="m461m462m464m465m474" ;}*/

    S0=S1+S2;
    QByteArray ba;
	
	ba= S0.toLatin1();
	SQUMAC::instance_p()->QcommCmdPMAC ( ba,str);					//AZ 10 lines
 

	QStringList  list = str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	int it=0;
	for ( QStringList::iterator point = list.begin(); point != list.end();++it, ++point )
		{}
	//qDebug()<<it;
    if ( it<10 ) return;

	//lock.lockForWrite();
	if(list.size()>=10)
	{
		trackstar.AzPosRealForError = ( ( list[1].toDouble() +list[2].toDouble() ) /3072.0 ) *CTS/3600;		//degree
		trackstar.AltPosRealForError = ( ( list[6].toDouble() +list[7].toDouble() ) /3072.0 ) *CTS/3600;	//degree
		trackstar.AzFolErrUMAC= ( list[3].toDouble() /3072.0 ) *CTS;					 	//"
		trackstar.AltFolErrUMAC= ( list[8].toDouble() /3072.0 ) *CTS;						//"
		trackstar.AzVelocity= list[4].toDouble() *CTS*1000*2/3072.0;						// change counts/ms to "/s
        trackstar.AltVelocity= list[9].toDouble() *CTS*1000*2/3072.0;						// change counts/ms to "/s
        trackstar.AzTargetUMAC=list[0].toDouble() /3072.0*CTS/3600;
		trackstar.AltTargetUMAC=list[5].toDouble() /3072.0*CTS/3600;
	}
//	lock.unlock();
    if(list.size()>=15)
	{
        if ( flagtcs.flag_DERO )
        {
//		lock.lockForWrite();
                trackstar.DeroPosRealForError = ( ( list[11].toDouble() +list[12].toDouble() ) /3072.0 ) *CTS_Dero/3600;		//degree
                trackstar.DeroFolErrUMAC= ( list[13].toDouble() /3072.0 ) *CTS_Dero;
                trackstar.DeroVelocity= list[14].toDouble() *CTS_Dero*1000*2/3072.0;						// change counts/ms to "/s
                trackstar.DeroTargetUMAC=list[10].toDouble() /3072.0*CTS_Dero/3600;

//		lock.unlock();						//"
        }
       /* else if ( flagtcs.flag_ADC2 )
        {
//		lock.lockForWrite();
                trackstar.DeroPosRealForError  = ( ( list[11].toDouble() +list[12].toDouble() ) /3072.0 ) *CTS_ADC/3600;		//degree
                trackstar.DeroFolErrUMAC= ( list[13].toDouble() /3072.0 ) *CTS_ADC;
                trackstar.DeroVelocity= list[14].toDouble() *CTS_ADC*1000*2/3072.0;						// change counts/ms to "/s
                trackstar.DeroTargetUMAC=list[10].toDouble() /3072.0*CTS_ADC/3600;
//		lock.unlock();							//"
        }
        else if ( flagtcs.flag_ADC1 )
        {
//		lock.lockForWrite();
                trackstar.DeroPosRealForError  = ( ( list[11].toDouble() +list[12].toDouble() ) /3072.0 ) *CTS_ADC/3600;		//degree
                trackstar.DeroFolErrUMAC= ( list[13].toDouble() /3072.0 ) *CTS_ADC;
                trackstar.DeroVelocity= list[14].toDouble() *CTS_ADC*1000*2/3072.0;						// change counts/ms to "/s
                trackstar.DeroTargetUMAC=list[10].toDouble() /3072.0*CTS_ADC/3600;
//		lock.unlock();							//"
        }*/

    }
	
 	
}






void Tracking::on_checkBox_Az_toggled(bool checked)
{
    if ( checked )
    {
        flagtcs.flag_AZIMUTH=true;
        //qDebug()<<"az check on";
    }
    else
    {
        flagtcs.flag_AZIMUTH=false;
        if(trackstar.flagAzPointing || trackstar.flagAzTracking)
        {
            trackstar.flagAzTracking=false;
            trackstar.flagAzPointing=false;
            Az_stopTrack();
        }
    }

}

void Tracking::on_checkBox_Alt_toggled(bool checked)
{
    if ( checked )
    {
       flagtcs.flag_ALTITUDE=true;
    }
    else
    {
       flagtcs.flag_ALTITUDE=false;
       if(trackstar.flagAltPointing || trackstar.flagAltTracking)
       {
           trackstar.flagAltTracking=false;
           trackstar.flagAltPointing=false;
           Alt_stopTrack();
       }

    }

}

void Tracking::on_checkBox_Dero_toggled(bool checked)
{
    if ( checked )
    {
        flagtcs.flag_DERO=true;
                //flagtcs.flag_ADC1=false;
               // flagtcs.flag_ADC2=false;
    }
    else
    {

        label_dero_error->setStyleSheet ( "background-color:rgb(255, 251, 247)" );  				// grey
        label_dero_ampli_enabled->setStyleSheet ( "background-color:rgb(255, 251, 247)" );  			// grey

        flagtcs.flag_DERO=false;
        if(trackstar.flagDeroPointing || trackstar.flagDeroTracking)
        {
            trackstar.flagDeroTracking=false;
            trackstar.flagDeroPointing=false;
            Dero_stopTrack();
        }
    }

}

void Tracking::on_checkBox_ADC1_toggled(bool checked)
{
    if ( checked )
    {
        flagtcs.flag_ADC1=true;
    }
    else
    {

       flagtcs.flag_ADC1=false;
    }

}

void Tracking::on_checkBox_ADC2_toggled(bool checked)
{
    if ( checked )
    {
        flagtcs.flag_ADC2=true;
    }
    else
    {

       flagtcs.flag_ADC2=false;
    }

}


void Tracking::on_checkBox_CorrectAuto_toggled(bool checked)
{
    if ( checked )
    {
        flagtcs.AutoCorrect=true;
    }
    else
    {

       flagtcs.AutoCorrect=false;
    }


}

void Tracking::on_checkBox_PointModel_toggled(bool checked)
{
    if ( checked )
    {
        flagtcs.PointModel=true;
    }
    else
    {

       flagtcs.PointModel=false;
    }
}


void Tracking::on_pushButton_saveformodel_clicked()
{
    char s[400];
    QDateTime dt=QDateTime::currentDateTime();
    QTime time=dt.time();
    QString dtstr;
    dtstr=dt.toString ( "MM_dd_yyyy" );
    QString fileName =SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString() +"/ModelData"+dtstr+".txt";

    QFile file ( fileName );
    if ( !file.open ( QFile::Append|QFile::Truncate ) )
    {
        /*QMessageBox::warning ( this,
                               tr ( "Warn" ),
                               tr ( "open error!" ),
                               QMessageBox::Yes );*/
        return;
    }
    QTextStream out ( &file );

    sprintf ( s,"%10.7f\t %10.7f\t %10.7f\t %10.7f \n",
              trackstar.AzTarget,trackstar.AltTarget,
              trackstar.AzPosRealForError+trackstar.AzTrackError/3600.0,
              trackstar.AltPosRealForError+trackstar.AltTrackError/3600.0);
    out<<tr (s);

    file.close();

}

void Tracking::on_checkBox_Pixel_toggled(bool checked)
{
    if(checked)
    {
        flagtcs.OffsetPixelUnit=true;
        pushButton_AzOffset->setText ( "CCD+R" );
        pushButton_AltOffset->setText ( "CCD" );
    }
    else
    {
        flagtcs.OffsetPixelUnit=false;
        pushButton_AzOffset->setText ( "Input" );
        pushButton_AltOffset->setText ( "Input" );
    }
}

void Tracking::on_checkBox_ALTHALFDERO_toggled(bool checked)
{
    if(checked) flagtcs.Dero_ALTHALF=true;
    else flagtcs.Dero_ALTHALF=false;
}

void Tracking::on_pushButton_paste_clicked()
{
    QClipboard *board = QApplication::clipboard();
    QString str = board->text();
    QStringList liststar,listra,listdec;
    liststar = str.split(QRegExp("\\s+"));//QRegExp("\\W+")
    if(liststar.size()==2)
    {
        listra = liststar[0].split(":", QString::SkipEmptyParts);
        listdec = liststar[1].split(":", QString::SkipEmptyParts);
        if(listra.size()==3)
        {
            m_RA_Hour->setText(listra[0]);
            m_RA_Minute->setText(listra[1]);
            m_RA_Second->setText(listra[2]);
        }
        if(listdec.size()==3)
        {
            m_DEC_Degree->setText(listdec[0]);
            m_DEC_Minute->setText(listdec[1]);
            m_DEC_Second->setText(listdec[2]);
        }
        label_Status->setText ( "New target is ra="+liststar[0]+" and dec="+liststar[1] );
    }
    else label_Status->setText ( "Other content in clipboard. Space must be between RA and DEC string.");

   // QMessageBox::information(NULL, "From clipboard", str);

}

void Tracking::on_pushButton_ADC2_Offset_clicked()
{
    //PointADC();
    QString tmp;
    tmp=lineEdit_ADC2_Offset->text();
    trackstar.ADC2_correct_all+=tmp.toDouble();
    //system("/home/scope/gps/program/gpstime");
}

void Tracking::on_pushButton_ADC1Offset_clicked()
{
    QString tmp;
    tmp=lineEdit_ADC1_Offset->text();
    trackstar.ADC1_correct_all+=tmp.toDouble();
}


