#ifndef DEROTATOR_H
#define DEROTATOR_H

#include <QDialog>

class MainWindow;

namespace Ui {
    class Derotator;
}

class Derotator : public QDialog
{
    Q_OBJECT

    public:
        Derotator(MainWindow *parent);
        ~Derotator();
        MainWindow *m_parent;

        bool getDataFromUMAC(char cmd[], int n, double cts, double* PosReal_Degree, double *FolErrUMAC, double *Velocity, double Seconds_old, double *Seconds, int PointerLast, int *Pointer, double (*p)[6], QString DataTime[], int num);
        bool getStatusFromUMAC(char cmd[], int n, QStringList* Status);
        bool D2DMS(double Degree, int* Deg, int* Min, double* Sec);
        bool getDeroInfo();
        bool getM3Info();
        bool checkDeroM3Home();
        //bool checkADCSoftLimit();
        bool checkDeroM3Amplifier();
        bool checkDeroM3LoopStatus();
        bool checkDeroError();
        bool checkM3Brake();
        bool checkM3RotPositon();
        bool checkM3Rot();
        bool checkM3TransSafePos();
        bool checkFiberSwitch();
        bool checkCover();
        bool checkDiskLock();
        bool dataSave(double(*p)[6], QString DataTime[], QString FileName, int num);
        void OpenCoverByOCS();
        void CloseCoverByOCS();
        void StopCoverByOCS();

    protected:
        void changeEvent(QEvent *e);

    private:
        Ui::Derotator *ui;

        QTimer *timer_DeroM3Info;
        QTimer *timer_DeroM3CheckStatus;
        QTimer *timer_save_data;
        QTimer *timer_DeroreplyOCS;
        double DeroTarget, M3Target;
        double DeroVelocity, M3Velocity, M3VelocityRot;

        //void connectDevice();
        void  CheckCommandFromOCS();
        void  MakeString(char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp);
        void  CombineString ( char StatusCUID[100],char ExeStatus[50],char bExeStatus[10],QString* s_temp );
        void  SendToOCS(QString str);
        void  GetStatusFromUMAC ( char cmd[],int n,QStringList* Status);
        void  GetDeroPositionByOCS();
        void  EnableByOCS_Dero();
        void  DisableByOCS_Dero();
        void  CheckByOCS_Dero();
        void  PointByOCS_Dero();
        void  PointStopByOCS_Dero();
        void  PointingByOCS_Dero();
        void  HomeByOCS_Dero();
        void  HomingByOCS_Dero();

        void  EnableByOCS_M3();
        void  DisableByOCS_M3();
        void  CheckByOCS_M3();
        void  PointByOCS_M3T();
        void  PointingByOCS_M3T();
        void  PointStopByOCS_M3T();
        void  HomeByOCS_M3();
        void  HomingByOCS_M3();
        void  LockByOCS_M3T();
        void  UnLockByOCS_M3T();

        void  PointByOCS_M3R();
        void  PointingByOCS_M3R();
        void  PointStopByOCS_M3R();

        void  PointByOCS_FL();
        void  PointingByOCS_FL();
        void  PointStopByOCS_FL();
        void OpenCoverReplyOCS();
        void CloseCoverReplyOCS();

    private slots:
        void replyOCS();
        bool showDeroM3Info();
        void deroM3CheckStatus();
        void saveData();
        bool storeDeroData();
        void on_pushButton_dero_enable_clicked();
        void on_pushButton_dero_home_clicked();
        void on_pushButton_dero_disable_clicked();
        void on_radioButton_dero_vel_fast_toggled(bool checked);
        void on_radioButton_dero_vel_mid_toggled(bool checked);
        void on_radioButton_dero_vel_slow_toggled(bool checked);
        void on_lineEdit_dero_vel_spec_returnPressed();
        void on_pushButton_dero_go_clicked();
        void on_pushButton_dero_stop_clicked();
        void on_pushButton_dero_park_clicked();
        void on_pushButton_m3_enable_clicked();
        void on_pushButton_m3_disable_clicked();
        void on_pushButton_m3_trans_home_clicked();
        void on_pushButton_m3_trans_unlock_clicked();
        void on_pushButton_m3_trans_go_clicked();
        void on_pushButton_m3_trans_stop_clicked();
        void on_pushButton_m3_rot_go_P_clicked();
        void on_pushButton_m3_rot_go_N_clicked();
        void on_pushButton_m3_rot_stop_clicked();
        void on_pushButton_m3_auto_Pos1_clicked();
        void on_pushButton_m3_auto_Pos2_clicked();
        void on_pushButton_fiber_Pos1_clicked();
        void on_pushButton_fiber_Pos2_clicked();
        void on_pushButton_COVER_LEFT_OPEN_clicked();
        void on_pushButton_COVER_RIGHT_OPEN_clicked();
        void on_pushButton_COVER_LEFT_CLOSE_clicked();
        void on_pushButton_COVER_RIGHT_CLOSE_clicked();
        void on_pushButton_COVER_STOP_clicked();
        void on_pushButton_COVER_OPEN_clicked();
        void on_pushButton_COVER_CLOSE_clicked();
        void on_pushButton_fiber_step_P_clicked();
        void on_pushButton_fiber_step_N_clicked();
        void on_pushButton_fiber_stop_clicked();
        void on_pushButton_fiber_brake_clicked();
        void on_pushButton_fiber_release_clicked();
        void on_pushButton_CCD_IN_clicked();
        void on_pushButton_CCD_OUT_clicked();
        void on_pushButton_HS_IN_clicked();
        void on_pushButton_HS_OUT_clicked();
};

#endif // DEROTATOR_H
