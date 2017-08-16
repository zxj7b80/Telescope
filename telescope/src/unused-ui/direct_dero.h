//
// C++ Interface: direct_dero.h

#ifndef DIRECT_DERO_H
#define DIRECT_DERO_H
#include <QtGui>
#include "ui_direct_dero.h"
#include "writelog.h"
class MainWindow;
class DirectDero:public QScrollArea,private Ui::DirectDero
{
		Q_OBJECT
	public:
		DirectDero ( MainWindow * parent );
		~DirectDero();
	protected:
		virtual void closeEvent ( QCloseEvent *event );
	private:
		MainWindow *m_parent;
		QTimer *timer_DeroInfo;
                QTimer *timer_DeroCheckStatus;
		QTimer *timer_show;
		QTimer *timer_save;
		QTimer *timerDer_CheckCommandFromOCS;
		QTimer *timer_DeroreplyOCS;
		writelog *writeLog;

		void initUi();
		double c1_target;
		double c1_real;
		double c1_poserr;
		double c1_vel;
		double c1_acc;
		double c1_step;
		double n1_target;
		double n1_real;
		double n1_poserr;
		double n1_vel;
		double n1_acc;
		double n1_step;
		double n2_target;
		double n2_real;
		double n2_poserr;
		double n2_vel;
		double n2_acc;
		double n2_step;
                bool  flag_C1Homing;
		bool  flag_N2Homing;
		bool  flag_N1Homing;
		bool storeC1Data();
		bool storeN1Data();
		bool storeN2Data();
		//bool DataSave(double (*p)[5],QString FileName);
                bool  DataSave(double (*p)[6],QString DataTime[],QString FileName,int num);
		void  TcsCheckDer();
		void  PointByOCS_Dero();
		void  PointingByOCS_Dero();
		void  PointStopByOCS_Dero();
		void  MakeString(char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp);	
		void  SendToOCS(QString str);
		void  GetC1Position();
		void  GetN1Position();
		void  GetN2Position();
		void  CheckAmplifier();
		void  CheckError();
                void  CheckLoopStatus();
		void  CheckLimit();
		void  CheckSoftLimit();	
		void  CheckC1Homing();
		void  CheckN2Homing();
		void  CheckN1Homing();
                void  CheckBreak();
                void  setPIDparam_C1_going();
		void  setPIDparam_N1_going();
		void  setPIDparam_N2_going();
		void  GetDataFromUMAC(char cmd[],int n,double cts,double* PosReal_Degree,double* FolErrUMAC,double* Velocity,double Seconds_old,double* Seconds, int PointerLast,int* Pointer,double(*p)[6],QString DataTime[],int num);
                void  GetStatusFromUMAC ( char cmd[],int n,QStringList* Status);
		void  D2DMS(double Degree,int* Deg,int* Min,double* Sec);
                void  setValidator();
                void  SetVelcity( double vel,double cts );
		void  C1step2target(int direction,double realpos);
		void  ShowC1Target(double C1Target);
		void  N2step2target(int direction,double realpos);
           	void  ShowN2Target(double N2Target);
		void  N1step2target(int direction,double realpos);
           	void  ShowN1Target(double N1Target);
		struct StatusDero
		{
			bool  C1_Amplifier1;//C1 amplifier1 status:active (true)or not(false)
			bool  C1_Amplifier2;//C1 amplifier2 status:active (true)or not(false)
			bool  N1_Amplifier1;//N1 amplifier1 status:active (true)or not(false)
			bool  N1_Amplifier2;//N1 amplifier2 status:active (true)or not(false)
			bool  N2_Amplifier1;//N2 amplifier1 status:active (true)or not(false)
			bool  N2_Amplifier2;//N2 amplifier2 status:active (true)or not(false)


		}stausDero;



	private slots:
		void DeroInfo();
                void DeroCheckStatus();
		void saveData();
                void on_pushButton_c1_release_clicked();
                void on_pushButton_c1_break_clicked();
                void on_pushButton_n1_release_clicked();
                void on_pushButton_n1_break_clicked();
                void on_pushButton_n2_release_clicked();
                void on_pushButton_n2_break_clicked();
		void on_pushButton_c1_enable_clicked();
		void on_pushButton_c1_disable_clicked();
		void on_pushButton_c1_home_clicked();
		void on_pushButton_n1_enable_clicked();
		void on_pushButton_n1_disable_clicked();
		void on_pushButton_n1_home_clicked();
		void on_pushButton_n2_enable_clicked();
		void on_pushButton_n2_disable_clicked();
		void on_pushButton_n2_home_clicked();

		void on_lineEdit_c1_vel_spec_returnPressed();
// 		void on_lineEdit_c1_acc_spec_returnPressed();

		void on_lineEdit_n1_vel_spec_returnPressed();
// 		void on_lineEdit_n1_acc_spec_returnPressed();

		void on_lineEdit_n2_vel_spec_returnPressed();
//		void on_lineEdit_n2_acc_spec_returnPressed();

		void on_radioButton_c1_vel_fast_toggled ( bool on );
		void on_radioButton_c1_vel_mid_toggled ( bool on );
		void on_radioButton_c1_vel_slow_toggled ( bool on );
		//void on_radioButton_c1_acc_default_toggled ( bool on );

		void on_radioButton_n1_vel_fast_toggled ( bool on );
		void on_radioButton_n1_vel_mid_toggled ( bool on );
		void on_radioButton_n1_vel_slow_toggled ( bool on );
//		void on_radioButton_n1_acc_default_toggled ( bool on );

		void on_radioButton_n2_vel_fast_toggled ( bool on );
		void on_radioButton_n2_vel_mid_toggled ( bool on );
		void on_radioButton_n2_vel_slow_toggled ( bool on );
//		void on_radioButton_n2_acc_default_toggled ( bool on );

		void on_radioButton_c1_stepLong_toggled ( bool on );
		void on_radioButton_c1_stepMid_toggled ( bool on );
		void on_radioButton_c1_stepShort_toggled ( bool on );
		void on_radioButton_n2_stepLong_toggled ( bool on );
		void on_radioButton_n2_stepMid_toggled ( bool on );
		void on_radioButton_n2_stepShort_toggled ( bool on );
		void on_radioButton_n1_stepLong_toggled ( bool on );
		void on_radioButton_n1_stepMid_toggled ( bool on );
		void on_radioButton_n1_stepShort_toggled ( bool on );

		void on_pushButton_c1_go_clicked();
		void on_pushButton_n1_go_clicked();
		void on_pushButton_n2_go_clicked();

		void on_pushButton_c1_dirP_pressed();
		void on_pushButton_c1_dirP_released();
		void on_pushButton_c1_dirM_pressed();
		void on_pushButton_c1_dirM_released();

		void on_pushButton_n2_dirP_pressed();
		void on_pushButton_n2_dirP_released();
		void on_pushButton_n2_dirM_pressed();
		void on_pushButton_n2_dirM_released();

		void on_pushButton_n1_dirP_pressed();
		void on_pushButton_n1_dirP_released();
		void on_pushButton_n1_dirM_pressed();
		void on_pushButton_n1_dirM_released();

		void on_pushButton_c1_stop_clicked();
		void on_pushButton_n1_stop_clicked();
		void on_pushButton_n2_stop_clicked();


             	void on_pushButton_c1_stepP_clicked();
		void on_pushButton_c1_stepM_clicked();
             	void on_pushButton_n2_stepP_clicked();
		void on_pushButton_n2_stepM_clicked();
             	void on_pushButton_n1_stepP_clicked();
		void on_pushButton_n1_stepM_clicked();
		void on_pushButton_c1_park_clicked();
 		void on_pushButton_n2_park_clicked();
		void on_pushButton_n1_park_clicked();
		void CheckCommandFromOCS();
		void DeroreplyOCS();
};
#endif