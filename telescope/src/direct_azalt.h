//
// C++ Interface: direct_azalt.h
//

#ifndef DIRECT_AZALT_H
#define DIRECT_AZALT_H
#include <QtGui>
#include "ui_direct_azalt.h"
#include <QTcpServer>
#include "writelog.h"
#include <QFile>

class MainWindow;
class QUMAC;

class CSendMessageTCS :	public QWidget
        //   public Ui_TestEthernet
{
    Q_OBJECT
public:
    CSendMessageTCS ( char * );

private :
    void send_message ( char * );
    QTcpSocket * clientsocket;
    char c_sentstr[1000];
};

class DirectAzalt:public QScrollArea,private Ui::DirectAzalt
{
    Q_OBJECT
public:
    DirectAzalt ( MainWindow *parent );
    virtual ~DirectAzalt();

    MainWindow *m_parent;
//    //by zxj-----
    QFile *dataFile;//if not, will flash.
    long long datacount;
    QString mFilename;

protected:
    virtual void closeEvent ( QCloseEvent *event );

private:
    //		QTimer *timer_Sim_az_simPT;
    // AZ replay OCS every 5 seconds
    QTimer *timer2_AZreplyOCS;					// ALT replay OCS every 5 seconds
    QTimer *timer2_ALTreplyOCS;					// ALT replay OCS every 5 seconds
    QTimer *timer_CheckCommandFromOCS;				// check command from OCS every  second
    QTimer *timer_AzAltCheckStatus;					// check Az/Alt status
    QTimer *timer_MountInfo ;					//refresh screen
    writelog *writeLog;
    QTcpSocket *ClientSocket;
    void setValidator();
    bool DataSave(double (*p)[6],QString DataTime[],QString FileName,int num);
    void MakeString(char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp);
    void CombineString ( char StatusCUID[100],char ExeStatus[50],char bExeStatus[10],QString* s_temp );
    void SendToOCS(QString str);
    void initUi();
    //		void connectSignals();
    void writeErrorToOCS ( QString );
    //		DirectAzaltThread *direct_azalt_thread;
    double az_target;
    double az_real;
    double az_poserr;
    double az_vel;
    double az_acc;
    double az_step;
    int    az_hometime;

    double alt_target;
    double alt_real;
    double alt_poserr;
    double alt_vel;
    double alt_acc;
    double alt_step;
    int    alt_hometime;

    QVector<bool> az_error;
    QVector<QString> az_error_info;
    QVector<bool> alt_error;
    QVector<QString> alt_error_info;
    //QVector<bool> alt2_error;
    //QVector<QString> alt2_error_info;

    void iniNetwork();
    void TcsCheckAz();
    void TcsCheckAlt();
    void GetAzPosition();
    void GetAltPosition();
    void GetAzReadHead();
    void GetAltReadHead();
    void GetMotorCurrent();

    void  setPIDparam_Az_going();
    void  setPIDparam_Alt_going();
    //		void  TcpClientSend ( QString s_temp );
    void  GetDataFromUMAC ( char cmd[],int n,double cts,double* PosReal_Degree,double* FolErrUMAC,double* Velocity,double Seconds_old,double* Seconds ,int  PointerLast,int* Pointer,double ( *p ) [6],QString DataTime[],int num);
    void  D2DMS ( double Degree,int* Deg,int* Min,double* Sec ); //change degree vlaue as degrees+minute+seconds
    void  AZstep2target(int direction,double realpos);
    void  ALTstep2target(int direction,double realpos);
    void  ShowAzTarget(double AzTarget);
    void  ShowAltTarget(double AltTarget);
    void  GetStatusFromUMAC ( char cmd[],int n,QStringList* Status);
    void  CheckTarget();
    void  CheckSoftLimit();

public :
    //		void cmdOCS ( QHash<QString,QString> );
    //		QTimer *timer_infoAzOCS;
    //		QTimer *timer_alt;
    QTimer *timer_save_data;
    QTimer *timer_gopoint;
    //		void  StartTcpServer();
    void  GetAzAltPositionByOCS_AZ();
    void  EnableByOCS_AZ();
    void  DisableByOCS_AZ();
    void  HomeByOCS_AZ();
    void  HomingByOCS_AZ();
    void  PointByOCS_AZ();
    void  PointStopByOCS_AZ();
    void  PointingByOCS_AZ();

    void  EnableByOCS_ALT();
    void  DisableByOCS_ALT();
    void  HomeByOCS_ALT();
    void  HomingByOCS_ALT();
    void  PointByOCS_ALT();
    void  PointStopByOCS_ALT();
    void  PointingByOCS_ALT();

    void UnlockByOCS_ALT();
    void LockByOCS_ALT();

private slots:
    void  on_pushButton_az_stepP_clicked();
    void  on_pushButton_az_stepM_clicked();
    void  on_pushButton_alt_stepP_clicked();
    void  on_pushButton_alt_stepM_clicked();
    void  on_radioButton_az_stepLong_toggled ( bool on );
    void  on_radioButton_az_stepMid_toggled ( bool on );
    void  on_radioButton_az_stepShort_toggled ( bool on );
    void  on_radioButton_alt_stepLong_toggled ( bool on );
    void  on_radioButton_alt_stepMid_toggled ( bool on );
    void  on_radioButton_alt_stepShort_toggled ( bool on );
    //void  on_pushButton_alt_phase_clicked();
    //void  on_pushButton_az_phase_clicked();
    //	         int readDataIntoBuffer(int maxSize);
    //		QByteArray readDataIntoBuffer ( int maxSize );
    //		void receiveMessage();
    //		void slotReadMessage();
    //void slotDisplayError(QAbstractSocket::SocketError socketError);
    void  AZreplyOCS() ;//// AZ replay OCS every 5 seconds ,timer2_AZreplyOCS
    void  ALTreplyOCS() ;//// ALT replay OCS every 5 seconds ,timer2_ALTreplyOCS
    void  CheckCommandFromOCS();
    void  saveData();

    //		void infoAzOCS();
    //		void infoAltOCS();
    //		void updateInterface ( QHash<QString,QString> );
    void showMountInfo();
    void CheckAmplifier();
    void CheckError();
    void CheckLimit();
    void CheckLoopStatus();
    void CheckAzHoming();
    void CheckAltHoming();
    bool storeAzData();
    bool storeAltData();
    // void GetAzPosition(QHash<QString,QString>);
    //		void GetAltPosition(QHash<QString,QString>);
    void show_MountData();
    void AzAltCheckStatus();		// clock
    void on_pushButton_az_enable_clicked();
    void on_pushButton_az_disable_clicked();
    void on_pushButton_az_home_clicked();

    void on_radioButton_az_vel_fast_toggled ( bool on );
    void on_radioButton_az_vel_mid_toggled ( bool on );
    void on_radioButton_az_vel_slow_toggled ( bool on );
    void on_lineEdit_az_vel_spec_returnPressed();

    void on_pushButton_az_go_clicked();
    void on_pushButton_az_stop_clicked();
    void on_pushButton_az_park_clicked();
    void on_pushButton_alt_park_clicked();

    void on_pushButton_alt_enable_clicked();
    void on_pushButton_alt_disable_clicked();
    void on_pushButton_alt_home_clicked();

    void on_radioButton_alt_vel_fast_toggled ( bool on );
    void on_radioButton_alt_vel_mid_toggled ( bool on );
    void on_radioButton_alt_vel_slow_toggled ( bool on );
    void on_lineEdit_alt_vel_spec_returnPressed();

    void on_pushButton_alt_go_clicked();
    void on_pushButton_alt_stop_clicked();
    //void on_pushButton_EmergencyStop_clicked();
    //void on_pushButton_EnableAllAmplifiers_clicked();

    void on_pushButton_az_dirP_pressed();
    void on_pushButton_az_dirP_released();
    void on_pushButton_az_dirM_pressed();
    void on_pushButton_az_dirM_released();
    void on_pushButton_alt_dirP_pressed();
    void on_pushButton_alt_dirP_released();
    void on_pushButton_alt_dirM_pressed();
    void on_pushButton_alt_dirM_released();


    void gopoint();

    void on_pushButton_alt_brake_clicked();
    void CheckAltBrake();

signals:
};
#endif
