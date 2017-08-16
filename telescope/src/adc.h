#ifndef ADC_H
#define ADC_H

#include <QDialog>

class MainWindow;

namespace Ui {
    class ADC;
}

class ADC : public QDialog
{
    Q_OBJECT

public:
    ADC(MainWindow *parent);
    ~ADC();
    MainWindow *m_parent;

    bool getDataFromUMAC(char cmd[], int n, double cts, double* PosReal_Degree, double *FolErrUMAC, double *Velocity, double Seconds_old, double *Seconds, int PointerLast, int *Pointer, double (*p)[6], QString DataTime[], int num);
    bool getStatusFromUMAC(char cmd[], int n, QStringList* Status);
    bool D2DMS(double Degree, int* Deg, int* Min, double* Sec);
    bool getADC1Info();
    bool getADC2Info();
    bool checkADCHome();
    //bool checkADCSoftLimit();
    bool checkADCAmplifier();
    bool checkADCLoopStatus();
    bool checkADCError();
    bool dataSave(double(*p)[6], QString DataTime[], QString FileName, int num);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ADC *ui;

    QTimer *timer_ADCInfo;
    QTimer *timer_ADCCheckStatus;
    QTimer *timer_save_data;
    QTimer *timer_replyOCS;
    double Adc1Target, Adc2Target;
    double Adc1Velocity, Adc2Velocity;

    //void connectDevice();
    void  CheckCommandFromOCS();
    void MakeString(char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp);
    void  CombineString ( char StatusCUID[100],char ExeStatus[50],char bExeStatus[10],QString* s_temp );
    void  SendToOCS(QString str);

    void  EnableByOCS_ADC1();
    void  DisableByOCS_ADC1();
    void  CheckByOCS_ADC1();
    void  PointByOCS_ADC1();
    void  PointStopByOCS_ADC1();
    void  PointingByOCS_ADC1();
    void  HomeByOCS_ADC1();
    void  HomingByOCS_ADC1();

    void  EnableByOCS_ADC2();
    void  DisableByOCS_ADC2();
    void  CheckByOCS_ADC2();
    void  PointByOCS_ADC2();
    void  PointStopByOCS_ADC2();
    void  PointingByOCS_ADC2();
    void  HomeByOCS_ADC2();
    void  HomingByOCS_ADC2();
private slots:
    void replyOCS();
    bool showADCInfo();
    void adcCheckStatus();
    void saveData();
    bool storeADC1Data();
    bool storeADC2Data();

    void on_pushButton_adc1_enable_clicked();
    void on_pushButton_adc2_enable_clicked();
    void on_pushButton_adc1_disable_clicked();
    void on_pushButton_adc2_disable_clicked();
    void on_pushButton_adc1_home_clicked();
    void on_pushButton_adc2_home_clicked();
    void on_pushButton_adc1_go_clicked();
    void on_lineEdit_adc1_vel_spec_returnPressed();
    void on_lineEdit_adc2_vel_spec_returnPressed();
    void on_radioButton_adc1_vel_fast_toggled(bool checked);
    void on_radioButton_adc2_vel_fast_toggled(bool checked);
    void on_radioButton_adc1_vel_mid_toggled(bool checked);
    void on_radioButton_adc2_vel_mid_toggled(bool checked);
    void on_radioButton_adc1_vel_slow_toggled(bool checked);
    void on_radioButton_adc2_vel_slow_toggled(bool checked);
    void on_pushButton_adc2_go_clicked();
    void on_pushButton_adc1_stop_clicked();
    void on_pushButton_adc2_stop_clicked();
    void on_pushButton_adc_go_clicked();
    void on_pushButton_adc_stop_clicked();
    void on_pushButton_adc_park_clicked();
};

#endif // ADC_H
