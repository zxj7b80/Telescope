//
// C++ Interface: tracking.H
//

//
//
#ifndef TRACKING_H
#define TRACKING_H

#include <QtGui>
#include "ui_tracking.h"

#include "signs.h"
#include "astro.h"
#include "circum.h"
#include <time.h>
#include "writelog.h"
//#include  "createtracefile.h"
//#include "TimeAndCoordinate.h"
//#include "ATimeSpace.h"
//using namespace AstroUtil;
//#include "cstar.h"

void  GetTableLine1();
void  MakeStr ( QString Axis,int Tm,const int ROW,int StartP,double offset,double correct,double model,double guidedata,QString *s1,double *TrackTargetOffset );
void  SetAltTrackingValueNo1() ;
void  SetAzTrackingValueNo1();
void  SetDeroTrackingValueNo1();
void  AzToUMAC(int row);
void  AltToUMAC(int row);
void  DeroToUMAC(int row);

void  GetStatusFromUMAC ( char cmd[],int n,QStringList* Status );
void  SendFloatToDPRAM(uint16_t offset,float data);
void  SendIntToDPRAM(uint16_t offset,int data);
void  SendDouble2InttoDPRAM(uint16_t offset,double data);
void  SendPointDataToDPRAM(uint16_t offset,int point_time,int pos,int vel_end);
void  ReadDataIntFromDPRAM(uint16_t offset,double *data);
void  MakeDPRAMCommand ( QString Axis,int Period,const int ROW,double model);
void  AzMakeCommand(int row);
void  AltMakeCommand(int row);
void  DeroMakeCommand(int row);
void  Az_beginPoint();
void  Az_beginTrack();
void  Az_stopTrack();
void  Alt_beginPoint();
void  Alt_beginTrack();
void  Alt_stopTrack();
void  Dero_beginPoint();
void  Dero_beginTrack();
void  Dero_stopTrack();
void  ADC_stopTrack();
void  IniDPRAM();
bool  SaveDataModel();
void  IniUmacTime();
bool  TrackDataForSave();

void  AltMakeStr(int Alt_row);
void  AzMakeStr(int Az_row);
void  DeroMakeStr(int Dero_row);
void  CheckUmacStep(uint16_t offset,float *step);
int   CheckUmacFlag_Az(uint16_t offset);
int   CheckUmacFlag_Alt(uint16_t offset);
int   CheckUmacFlag_Dero(uint16_t offset);
void CheckUmacFlag();
void  Cal(double *azPos,double *altPos,double *deroPos,QString *dts);
void  AzCorrectTarget();
void  AltCorrectTarget();
void  DeroCorrectTarget();
void  AltCorrectPoint();	
void  GetAzAltDeroPosition_UMAC();
//void  GetAzAltDeroPosition_DPRAM();

void  GetAzPosition_DPRAM();
void  GetAltPosition_DPRAM();
void  GetDeroPosition_DPRAM();
void  GetADC2Position_DPRAM();
void  GetADC1Position_DPRAM();
void  PID(double target,double real,double Kp,double Ki,double Kd,double *error_1,double *ei,double *ed,double Time,double *output);
void  CalAzAverageError();
void  CalAltAverageError();
void  CalDeroAverageError();
void  LookupTable_realtime(); 	
void  LookupTable_Az(bool firstTime=0);
void  LookupTable_Alt(bool firstTime=0);
void  LookupTable_Dero(bool firstTime=0);
void  azTrack();
void  altTrack();
void  deroTrack();
void  ADCTrack();
void  FlagPCOutData(uint16_t offset,int group);
void  SetTrackTm(uint16_t offset,float Tm);
void  LookAzTarget(long row,double *azTarget);
class TrackThread;
class CalculateThread;
class MainWindow;

void  setPIDparam_Az_point();
void  setPIDparam_Alt_point();
void  setPIDparam_C1_tracking();
void  setPIDparam_C1_pointing();
void  setPIDparam_N2_tracking();
void  setPIDparam_N2_pointing();
void  setPIDparam_N1_tracking();
void  setPIDparam_N1_pointing();
void  setPIDparam_Alt_tracking(int m=0);
void  setPIDparam_Az_tracking();

class Tracking:public QScrollArea,private Ui::Tracking
{
		Q_OBJECT
	public:
		Tracking ( MainWindow *parent );
		virtual ~Tracking();
		MainWindow *m_parent;
		CalculateThread *calculate_thread;
		TrackThread *track_thread;
		QTimer *timer_get_track_data;					//get data for save
		QTimer *timer_save_track_data;					//save to file
		QTimer *timer_CheckPointingORTrackingDero;
		QTimer *timer_CheckPointingORTrackingAz;
		QTimer *timer_CheckPointingORTrackingAlt;
		QTimer *timer_show_track_info;
		QTimer *timer_CalAndShowRms;

		QTimer *timer_ShowInfo;						//for showing information such as local time and star time
		QTimer *timer_CheckStatus;					//CheckStatus
//		QTimer *timer_pointing;						//pointing
		writelog *writeLog;
	protected:
		virtual void closeEvent ( QCloseEvent *event );
	public:
		void initUi();
//		void connectSignals();
		bool readStarTable();
//		void cmdOCS ( QHash<QString,QString> );
        void  readADCtable();
		void  CheckTarget();
	private:
//                double lst,lst0,mjd0;
		Now np;
        void  setValidator();
		void  StartTrackingThread();
		void  GetAzPosition();
		void  GetAltPosition();
        void  GetDeroPosition();
		void  GetN2Position();
		void  GetN1Position();

		void  GetStarTime_ini();
		void  GetStarTime();

		void  checkCommandFromOCS();
		void  TrackStarByOCS_MA();
		void  TrackingStarByOCS_MA();
		void  TrackingStopByOCS_MA();
		void  GetTrackingPosByOCS_MA();
		bool  readModel();
//		bool  ParseTerm ( QStringList  list,double *IA,double *IE,double *NPAE,double *CA,double *AW,double *AN );
		bool  ParseTerm ( QStringList  list,double *IA,double *IE,double *NPAE,double *CA,double *AW,double *AN,double *NRX,double *NRY);
		bool  DataSave ( double ( *p ) [6],QString DataTime[],QString FileName,int num );
		bool  DataSave_UMAC ( double ( *p ) [6],QString DataTime[],QString FileName,int num );
		bool  DataSaveAll_PC ( double ( *pAz ) [6],double ( *pAlt ) [6],double ( *pDero ) [6],QString DataTime[],QString FileName,int num );
		bool  SaveAzTrackRMS();
		bool  SaveAltTrackRMS();
		bool  storeDeroTrackData();
		void  AnalyzeData(double data[],int num,double *pv,double *rms, double *max,double *min);

        void  calculate (double *AZ_cal, double *ALT_cal,double *FR_cal,
                         double *ha_cal,double *dec_cal,DateTime dt );
        //void  calculate_new (double *AZ_cal, double *ALT_cal,double *FR_cal,
        //                     double *ha_cal,double *dec_cal,DateTime dt );

		double ra,Dec;//ra and dec at epoch of date
		double ha,alt,az,fpa,ha1,dec1,ha_2,ha1_2,dec1_2,fpa2;		//hour angle,altitude and azimuth
//
        double  deltaUT,lst,lst0,lstc;
//                    Now np;

		double dec_star,ha_star;					//save,degree,hour,
		double utc2,lst2,alt2,az2;
        double mjd0,mjd1,mjdc;					//mjd0 modified julian date, counted from J1900.0
		double lsn,rsn,lsn2,rsn2;					//true geoc lng of sun,dist from sun to earth
        //CStar star;

		void  SendToOCS ( QString str );
		void  MakeString ( char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp );
        void  MakeStringNew ( char StatusCUID[100],char RV[10],char Info[20],char Info1[50],char Info2[50],char Info3[50],char Info4[50],char Info5[50],char Info6[50],char bExeStatus[10],QString DataBack,QString* s_temp );
// 		void  CombineString ( char StatusCUID[100],char ExeStatus[50],char bExeStatus[10],QString DataBack,QString* s_temp );
// 		void  CombineStringNew ( char StatusCUID[100],char RV[10],char Info[10],char Info1[10],char Info2[10],char Info3[10],char Info4[10],char Info5[10],char Info6[10],char bExeStatus[10],QString DataBack,QString* s_temp );
        void  CombineString ( QString StatusCUID,char ExeStatus[50],char bExeStatus[10],QString DataBack,QString* s_temp );
        void  CombineString ( QString StatusCUID,char ExeStatus[50],char bExeStatus[10],QString* s_temp );
        void  CombineStringNew ( QString StatusCUID,char RV[10],char Info[20],char Info1[50],char Info2[50],char Info3[50],char Info4[50],char Info5[50],char Info6[50],char bExeStatus[10],QString DataBack,QString* s_temp );
		void  CheckAzaltAmplifier();
		void  CheckAzaltError();
		void  CheckDeroError ( char cmd[255] );
		void  CheckDeroAmplifier ( char cmd[255] );
		void  CheckAzAltLimit( char cmd[255] );
		void  CheckDeroLimit( char cmd[255] );
        void  CheckAmplifier();
        void  CheckError();
        bool  storeAzTrackData();
		bool  storeAltTrackData();
		bool  CalculateTime (QString DataTime[],int num,double *t );
		//bool  DataSaveAll_UMAC ( double ( *p ) [5],QString DataTime[],QString FileName,int num );
		bool  DataSaveAll_UMAC ( double ( *pAz ) [6],double ( *pAlt ) [6],double ( *pDero ) [6],QString DataTime[],QString FileName,int num );
		bool  Save_label(); 
        bool  iniLog();
        void  GetDataFromUMAC ( char cmd[],int n,double cts,double* PosReal_Degree,double* FolErrUMAC,double* Velocity,double Seconds_old,double* Seconds ,int  PointerLast,int* Pointer,double ( *p ) [6],QString DataTime[],int num );
		void  D2DMS ( double Degree,int* Deg,int* Min,double* Sec ); //change degree vlaue as degrees+minute+seconds
		void  D2HMS(double Degree,int* Hour,int* Min,double* Sec);	
        void  FreshDeroTrackingData ( double TrackTarget,double PosReal_Degree,double TrackError,QHash<QString,QString> info_show );
		void  LookupFirstPoint();
        void  LookupTargetNow();
        void  PointAlt();
		void  PointAz();
		void  PointDero();
        void  PointADC();
		void  SetAltTrackingValue();
		void  IniTrack();
		void  TargetBeyondRange();
		void  freshLocalTime();
        void  freshTargetPosition();
        void  freshAzPosition(double azPos,double azVel );
		void  freshAltPosition(double altPos,double altVel );
        void  freshDeroPosition(double deroPos,double deroVel );
        void  freshADC1Position(double deroPos,double deroVel );
        void  freshADC2Position(double deroPos,double deroVel );
		
		void  CalLMST();
		double calculate_epoctime(QDateTime datetime);
		double calculate_lmstime(QDateTime datetime);
	private slots:
        void  showTrackInfo();						//showing tracking infomation
        void  show_info();						//for showing information such as local time and star time
        void  CheckStatus();						//for QTimer *timer_CheckStatus;//CheckStatus
        void  CheckPointingORTrackingAz();
        void  CheckPointingORTrackingAlt();
        void  CheckPointingORTrackingDero();
        void  on_pushButton_save_clicked();
        void  on_pushButton_track_clicked();
        void  on_pushButton_stop_clicked();
        void  on_pushButton_AzOffset_clicked();
        void  on_pushButton_AltOffset_clicked();
        void  on_pushButton_DeroOffset_clicked();
        void  on_pushButton_calculate_clicked();
        void  calculate_thread_finished();
		void  on_pushButton_enableAll_clicked();
        void  TrackreplyOCS();
        bool  saveStarTable ( double AZ[],double ALT[],double FR[],double HA[],double DEC[],DateTime dt[] );
        bool  getTrackData();
        bool  saveTrackData();
        bool  storeTrackData();

        void  CalAndShowRms();

        void  on_checkBox_Az_toggled(bool checked);
        void  on_checkBox_Alt_toggled(bool checked);
        void  on_checkBox_Dero_toggled(bool checked);
        void  on_checkBox_ADC1_toggled(bool checked);
        void  on_checkBox_ADC2_toggled(bool checked);
        void  on_pushButton_saveformodel_clicked();
        void  on_checkBox_CorrectAuto_toggled(bool checked);
        void  on_checkBox_PointModel_toggled(bool checked);
        void on_checkBox_Pixel_toggled(bool checked);
        void on_pushButton_paste_clicked();
        void on_pushButton_ADC2_Offset_clicked();
        void on_pushButton_ADC1Offset_clicked();
        void on_checkBox_ALTHALFDERO_toggled(bool checked);
};
#endif
