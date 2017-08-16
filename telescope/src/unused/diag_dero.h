//diag_dero.h
#ifndef DIAG_DERO_H
#define DIAG_DERO_H
#include <QtGui>
#include "ui_diag_dero.h"
extern bool UMAC_ETHERNET;
class MainWindow;
class DiagDero:public QWidget,private Ui::DiagDero
{
		Q_OBJECT
	public:
		DiagDero ( MainWindow *parent );
		virtual ~DiagDero();
  		MainWindow *m_parent;
// 	protected:
// 		virtual void closeEvent ( QCloseEvent *event );
 	private:
		void iniTableWidget();
  		QTimer *timer_show_DeroDiag;
//		void GetDataFromUMAC_Dero(char cmd[],int n, double (*head)[4],double cts_single,double cts_all,double Seconds_old,double* Seconds,int PointerLast,int* Pointer,QString DataTime[],int num);
		void CommandToUMAC_Dero ( char cmd[],int n, double (*p)[3]);
//		void refreshDerotable ();
		void FreshDeroDiagTable (int row,int col,QString str);
//		bool storeC1Data();
		bool DataSave ( double ( *head ) [8],QString DataTime[],QString FileName,int num);
		bool Save(double head[3],double CalData[5],QString FileName,int PointerLast,int* Pointer,double	Head[][8],QString DataTime[]);
		void CalData(double head[3],double cts_single,double cts_all,double (*p)[5]);
  		QTimer *timer_save_DeroDiag;
// 		QTimer *timer_save_alt;
// 		bool flag_save;
// 		bool flag_save_alt;
// 		int count;
// 		int countAlt;
// 		QString path;//current dir
// 		double AzHeadData[NN][9];//save position data
// 		double AzHeadError[NN][9];//save position  error data
// 		double AltHeadData[NN][9];//save position data
// 		double AltHeadError[NN][9];//save position  error data
// 
// 		QString HeadDataTime[NN];
// 		QString HeadDataTime_alt[NN];
// 		double AZhead1_err,AZhead2_err,AZhead3_err,AZhead4_err,AZhead5_err,AZhead6_err,AZhead7_err,AZhead8_err;
// 		double AZhead1_pos,AZhead2_pos,AZhead3_pos,AZhead4_pos,AZhead5_pos,AZhead6_pos,AZhead7_pos,AZhead8_pos;
// 
// 		double ALThead1_err,ALThead2_err,ALThead3_err,ALThead4_err,ALThead5_err,ALThead6_err,ALThead7_err,ALThead8_err;
// 		double ALThead1_pos,ALThead2_pos,ALThead3_pos,ALThead4_pos,ALThead5_pos,ALThead6_pos,ALThead7_pos,ALThead8_pos;
// 
// 		double posAz_degree;
// 		double PosAz;
// 		double posAlt_degree;
// 		double PosAlt;
// 		bool SaveAzReadingHeadData();
// 		bool SaveAzReadingHeadError();
// 		bool SaveAltReadingHeadData();
// 		bool SaveAltReadingHeadError();
// 		bool SaveAzReadingHeadTitle();
// 		bool SaveAltReadingHeadTitle();
//		void initUi();
//		void connectDevice();
	private slots:
  		void ShowData();
  		void on_ClearPushBtn_clicked();
// 		void on_ClearPushBtn_alt_clicked();
// 		void on_SaveAzReadHeadPushBtn_clicked();
// 		void on_SaveAltReadHeadPushBtn_clicked();
// 		bool Save();
// 		bool Save_alt();
// 		void  ReadAltChannel();
};
#endif
