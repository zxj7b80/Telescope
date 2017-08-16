#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <math.h>
#include <stdio.h>
#include<QtTest/QTest>
#include "main_window.h"
#include "adc.h"
#include "ui_adc.h"
#include "define.h"
#include "signs.h"
//#include "qumac.h"
#include "server.h"
#include "sendthread.h"

#define deroreplyOCSClock				5000

extern FlagTCS  flagtcs;

extern OcsCOM OcsCom;
extern int OcsPort;						//Port of OCS (TCP/IP)
extern QString OcsIP; 						//IP of OCS(TCP/IP)
extern QString	DataBackToOcs;
int     ADCdelay=4;
sAxis  statusADC1, statusADC2;
//StatusADC statusADC1;
//StatusADC statusADC2;

ADC::ADC(MainWindow *parent) : ui(new Ui::ADC)
{
    m_parent = parent;
    ui->setupUi(this);

    //qDebug("Create adc!");
    flagtcs.IsOpened_adc = true;
    //connectDevice();
    getADC1Info();
    getADC2Info();
    timer_ADCInfo = new QTimer();
    connect(timer_ADCInfo, SIGNAL(timeout()), this, SLOT(showADCInfo()));
    timer_ADCInfo->start(TIMER_1000MS);

    timer_ADCCheckStatus = new QTimer();
    connect(timer_ADCCheckStatus, SIGNAL(timeout()), this, SLOT(adcCheckStatus()));
    timer_ADCCheckStatus->start(TIMER_1000MS);

    timer_save_data = new QTimer();
    connect(timer_save_data, SIGNAL(timeout()), this, SLOT(saveData()));
    timer_save_data->start(TIMER_1000MS);

    timer_replyOCS= new QTimer();
    connect(timer_save_data, SIGNAL(timeout()), this, SLOT(replyOCS()));
}

ADC::~ADC()
{
    delete ui;
    qDebug("Close adc");
    flagtcs.IsOpened_adc = false;
}

void ADC::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/*void ADC::connectDevice()
{
    SQUMAC::instance_p();
    qDebug("adc connect UMAC");
}
*/
bool ADC::getDataFromUMAC(char cmd[], int n, double cts, double *PosReal_Degree, double *FolErrUMAC, double *Velocity, double Seconds_old, double *Seconds, int PointerLast, int *Pointer, double (*p)[6], QString DataTime[], int num)
{
    QString str;
    double PosCmd;      //command  position
    double PosReal;     //real position,seconds
    double FolErrPC;    //following error  ,calculated by PC

    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    QStringList  list = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    int it = 0;
    for(QStringList::iterator point = list.begin(); point != list.end(); ++it, ++point){}
    if(it < n)
    {
        return FALSE;
    }

    PosCmd = list[0].toDouble() * cts / 3072.0;                             //command, float value
    PosReal = ((list[1].toDouble() + list[2].toDouble()) / 3072.0) * cts;   //arcsec of real position
    *PosReal_Degree = PosReal / 3600.0;                                     //degree of real position
    *FolErrUMAC = list[3].toDouble() * cts / 3072.0;                        //following error  ,calculated by UMAC
    FolErrPC = PosCmd - PosReal;                                            //following error ,calculated by PC,arcsec
    *Velocity = list[4].toDouble() * cts * 1000 * 2 / 3072.0;               //change counts/ms to "/s

    QDateTime dt = QDateTime::currentDateTime();
    QTime time = dt.time();
    QString dtstr;
    dtstr = dt.toString("yyyy-MM-dd hh:mm:ss.zzz");
    int msecond = time.msec();
    *Seconds = time.second() * 1000 + msecond;

    if(fabs(*Seconds - Seconds_old) >= 90 && *Pointer <= (num - 1))     //every 100ms
    {
        *(*(p + *Pointer) + 0) = PosCmd;                                //Data[Pointer][0]
        *(*(p + *Pointer) + 1) = PosReal;                               //Data[Pointer][1]
        *(*(p + *Pointer) + 2) = *FolErrUMAC;                           //Data[Pointer][2]
        *(*(p + *Pointer) + 3) = FolErrPC;                              //Data[Pointer][3]

        DataTime[*Pointer] = dtstr;
        *Pointer = PointerLast + 1;
        //*Seconds_old = *Seconds;
    }
    return TRUE;
}

bool ADC::getADC1Info()
{
    char cmd[255];
    sprintf(cmd, GET_ADC1_INFO);
    int n = 5;
    double AdcFolErrUMAC = 0;
    double AdcVelocity;
    int Adc_d,Adc_m;
    double Adc_s;

    getDataFromUMAC(cmd, n, CTS_ADC, &statusADC1.PosReal_Degree, &AdcFolErrUMAC,
                    &AdcVelocity, statusADC1.Seconds, &statusADC1.Seconds, statusADC1.Pointer,
                    &statusADC1.Pointer, statusADC1.Data, statusADC1.DataTime, N_NUMBER);

    D2DMS(statusADC1.PosReal_Degree, &Adc_d, &Adc_m, &Adc_s);
    ui->lineEdit_adc1_posD->setText(QString::number(Adc_d));
    ui->lineEdit_adc1_posM->setText(QString::number(Adc_m));
    ui->lineEdit_adc1_posS->setText(QString::number(Adc_s, 'f', 2));
    ui->lineEdit_adc1_error->setText(QString::number(AdcFolErrUMAC, 'f', 2));
    ui->lineEdit_adc1_vel->setText(QString::number(AdcVelocity, 'f', 2));
    return TRUE;
}

bool ADC::getADC2Info()
{
    char cmd[255];
    sprintf(cmd, GET_ADC2_INFO);
    int n = 5;
    double AdcFolErrUMAC = 0;
    double AdcVelocity;
    int Adc_d,Adc_m;
    double Adc_s;

    getDataFromUMAC(cmd, n, CTS_ADC, &statusADC2.PosReal_Degree, &AdcFolErrUMAC, &AdcVelocity,
                    statusADC2.Seconds, &statusADC2.Seconds, statusADC2.Pointer, &statusADC2.Pointer,
                    statusADC2.Data, statusADC2.DataTime, N_NUMBER);

    D2DMS(statusADC2.PosReal_Degree, &Adc_d, &Adc_m, &Adc_s);
    ui->lineEdit_adc2_posD->setText(QString::number(Adc_d));
    ui->lineEdit_adc2_posM->setText(QString::number(Adc_m));
    ui->lineEdit_adc2_posS->setText(QString::number(Adc_s, 'f', 2));
    ui->lineEdit_adc2_error->setText(QString::number(AdcFolErrUMAC, 'f', 2));
    ui->lineEdit_adc2_vel->setText(QString::number(AdcVelocity, 'f', 2));

    return TRUE;
}

bool ADC::D2DMS(double Degree, int* Deg, int* Min, double* Sec)
{
    *Deg = int(Degree);
    *Min = int((Degree - *Deg) * 60);
    *Sec = ((Degree - *Deg) *60 - *Min) *60;

    if(*Deg != 0)
        *Min = abs(*Min);
    if(*Deg || *Min)
        *Sec = fabs(*Sec);
    return TRUE;
}

bool ADC::showADCInfo()
{
    getADC1Info();
    getADC2Info();
    return TRUE;
}

void ADC::on_pushButton_adc1_enable_clicked()
{
    QString str;
    char cmd[255];
    sprintf(cmd, ENABLE_ADC1);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void ADC::on_pushButton_adc2_enable_clicked()
{
    QString str;
    char cmd[255];
    sprintf(cmd, ENABLE_ADC2);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void ADC::on_pushButton_adc1_disable_clicked()
{
    QString str;
    char cmd[255];
    sprintf(cmd, DISENABLE_ADC1);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void ADC::on_pushButton_adc2_disable_clicked()
{
    QString str;
    char cmd[255];
    sprintf(cmd, DISENABLE_ADC2);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void ADC::on_pushButton_adc1_home_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC (HOME_ADC1, str);
    flagtcs.ADC1_Homed = false;
    OcsCom.flag_HomeErrorADC1=false;
}

void ADC::on_pushButton_adc2_home_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC(HOME_ADC2, str);
    flagtcs.ADC2_Homed = false;
    OcsCom.flag_HomeErrorADC2=false;
}

void ADC::on_pushButton_adc1_go_clicked()
{
    on_lineEdit_adc1_vel_spec_returnPressed();
    char cmd[255];
    QString str;

    Adc1Target = ui->lineEdit_adc1_targetD->text().toFloat() * 3600 + ui->lineEdit_adc1_targetM->text().toFloat() * 60 + ui->lineEdit_adc1_targetS->text().toFloat();

//    if(flagtcs.Az_SoftLimit_P&&(az_target>Az.azPosReal_Degree*3600))
//            return;
//    if(flagtcs.Az_SoftLimit_N&&(az_target<Az.azPosReal_Degree*3600))
//            return;

    Adc1Target = Adc1Target / CTS_ADC;
    sprintf(cmd, SET_ADC1_POS, Adc1Target);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void ADC::on_pushButton_adc2_go_clicked()
{
    on_lineEdit_adc2_vel_spec_returnPressed();
    char cmd[255];
    QString str;

    Adc2Target = ui->lineEdit_adc2_targetD->text().toFloat() * 3600 + ui->lineEdit_adc2_targetM->text().toFloat() * 60 + ui->lineEdit_adc2_targetS->text().toFloat();

//    if(flagtcs.Az_SoftLimit_P&&(az_target>Az.azPosReal_Degree*3600))
//            return;
//    if(flagtcs.Az_SoftLimit_N&&(az_target<Az.azPosReal_Degree*3600))
//            return;

    Adc2Target = Adc2Target / CTS_ADC;
    sprintf(cmd, SET_ADC2_POS, Adc2Target);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void ADC::on_lineEdit_adc1_vel_spec_returnPressed()
{
    double Adc1VelocityCmd = 0;
    char cmd[255];
    QString str;
    Adc1VelocityCmd = ui->lineEdit_adc1_vel_spec->text().toFloat();
    Adc1Velocity = Adc1VelocityCmd / (CTS_ADC * 1000);
    sprintf(cmd, SET_ADC1_VEL, Adc1Velocity);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void ADC::on_lineEdit_adc2_vel_spec_returnPressed()
{
    double Adc2VelocityCmd = 0;
    char cmd[255];
    QString str;
    Adc2VelocityCmd = ui->lineEdit_adc2_vel_spec->text().toFloat();
    Adc2Velocity = Adc2VelocityCmd / (CTS_ADC * 1000);
    sprintf(cmd, SET_ADC2_VEL, Adc2Velocity);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void ADC::on_radioButton_adc1_vel_fast_toggled(bool checked)
{
    if(checked)
    {
        char cmd[255];
        QString str;
        //Adc1Velocity = SMySetting::instance_p()->value ( "ADC/adc_vel_fast" ).toDouble();
        Adc1Velocity = 3600;
        ui->lineEdit_adc1_vel_spec->setText(QString::number(Adc1Velocity));
        Adc1Velocity = Adc1Velocity / ( CTS_ADC * 1000);
        sprintf(cmd, SET_ADC1_VEL, Adc1Velocity);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
}

void ADC::on_radioButton_adc2_vel_fast_toggled(bool checked)
{
    if(checked)
    {
        char cmd[255];
        QString str;
        //Adc2Velocity = SMySetting::instance_p()->value ( "ADC/adc_vel_fast" ).toDouble();
        Adc2Velocity = 3600;
        ui->lineEdit_adc2_vel_spec->setText(QString::number(Adc2Velocity));
        Adc2Velocity = Adc2Velocity / ( CTS_ADC * 1000);
        sprintf(cmd, SET_ADC2_VEL, Adc2Velocity);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
}

void ADC::on_radioButton_adc1_vel_mid_toggled(bool checked)
{
    if(checked)
    {
        char cmd[255];
        QString str;
        //Adc1Velocity = SMySetting::instance_p()->value ( "ADC/adc_vel_mid" ).toDouble();
        Adc1Velocity = 600;
        ui->lineEdit_adc1_vel_spec->setText(QString::number(Adc1Velocity));
        Adc1Velocity = Adc1Velocity / ( CTS_ADC * 1000);
        sprintf(cmd, SET_ADC1_VEL, Adc1Velocity);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
}

void ADC::on_radioButton_adc2_vel_mid_toggled(bool checked)
{
    if(checked)
    {
        char cmd[255];
        QString str;
        //Adc2Velocity = SMySetting::instance_p()->value ( "ADC/adc_vel_mid" ).toDouble();
        Adc2Velocity = 600;
        ui->lineEdit_adc2_vel_spec->setText(QString::number(Adc2Velocity));
        Adc2Velocity = Adc2Velocity / ( CTS_ADC * 1000);
        sprintf(cmd, SET_ADC2_VEL, Adc2Velocity);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
}

void ADC::on_radioButton_adc1_vel_slow_toggled(bool checked)
{
    if(checked)
    {
        char cmd[255];
        QString str;
        //Adc1Velocity = SMySetting::instance_p()->value ( "ADC/adc_vel_slow" ).toDouble();
        Adc1Velocity = 15;
        ui->lineEdit_adc1_vel_spec->setText(QString::number(Adc1Velocity));
        Adc1Velocity = Adc1Velocity / ( CTS_ADC * 1000);
        sprintf(cmd, SET_ADC1_VEL, Adc1Velocity);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
}

void ADC::on_radioButton_adc2_vel_slow_toggled(bool checked)
{
    if(checked)
    {
        char cmd[255];
        QString str;
        //Adc2Velocity = SMySetting::instance_p()->value ( "ADC/adc_vel_slow" ).toDouble();
        Adc2Velocity = 15;
        ui->lineEdit_adc2_vel_spec->setText(QString::number(Adc2Velocity));
        Adc2Velocity = Adc2Velocity / ( CTS_ADC * 1000);
        sprintf(cmd, SET_ADC2_VEL, Adc2Velocity);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
}

void ADC::on_pushButton_adc1_stop_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC(SET_ADC1_STOP, str);
}

void ADC::on_pushButton_adc2_stop_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC(SET_ADC2_STOP, str);
}

void ADC::adcCheckStatus()
{
    checkADCHome();
    //checkADCSoftLimit();
    checkADCAmplifier();
    checkADCLoopStatus();
    checkADCError();
    if(ADCdelay<0)CheckCommandFromOCS();
    else ADCdelay--;
}

bool ADC::checkADCHome()
{
    char  cmd[255];
    sprintf(cmd, FLAG_CHECK_ADC_HOME);
    QStringList  list;
    int n = 4;
    getStatusFromUMAC(cmd, n, &list);

    if(!QString::compare(list[0], "1"))
    {
        ui->label_adc1_home->setStyleSheet("background-color:rgb(255, 255, 0)");    //yellow rgb(255, 255, 0)
        ui->label_adc1_home_mark->setText("Homing...");
        //flagtcs.ADC1_Homed = FALSE;
    }
    if(!QString::compare(list[1], "1"))
    {
        ui->label_adc1_home->setStyleSheet("background-color:rgb(0, 170, 0)");      //green rgb(0, 170, 0)
        ui->label_adc1_home_mark->setText("Homed");
        flagtcs.ADC1_Homed = TRUE;
    }
    if(!QString::compare(list[0], "0") && !QString::compare(list[1], "0"))
    {
        ui->label_adc1_home->setStyleSheet("background-color:rgb(255, 0, 0)");      //red rgb(255, 0, 0)
        ui->label_adc1_home_mark->setText ( "No Home" );
        //flagtcs.ADC1_Homed = FALSE;
        OcsCom.flag_HomeErrorADC1=true;
    }

    if(!QString::compare(list[2], "1"))
    {
        ui->label_adc2_home->setStyleSheet("background-color:rgb(255, 255, 0)");    //yellow rgb(255, 255, 0)
        ui->label_adc2_home_mark->setText("Homing...");
        //flagtcs.ADC2_Homed = FALSE;
    }
    if(!QString::compare(list[3], "1"))
    {
        ui->label_adc2_home->setStyleSheet("background-color:rgb(0, 170, 0)");      //green rgb(0, 170, 0)
        ui->label_adc2_home_mark->setText("Homed");
        flagtcs.ADC2_Homed = TRUE;
    }
    if(!QString::compare(list[2], "0") && !QString::compare(list[3], "0"))
    {
        ui->label_adc2_home->setStyleSheet("background-color:rgb(255, 0, 0)");      //red rgb(255, 0, 0)
        ui->label_adc2_home_mark->setText ( "No Home" );
        //flagtcs.ADC2_Homed = FALSE;
        OcsCom.flag_HomeErrorADC2=true;
    }

    return TRUE;
}

bool ADC::checkADCAmplifier()
{
    char cmd[255];
    sprintf(cmd, FLAG_CHECK_ADC_AMP);
    QStringList  list;
    int n = 2;
    getStatusFromUMAC(cmd,n, &list);

    if(!QString::compare(list[0], "1"))
    {
        ui->label_adc1_amplifier->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.ADC1_Amplifier = TRUE;
    }
    else
    {
        ui->label_adc1_amplifier->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.ADC1_Amplifier = FALSE;
    }
    if(!QString::compare(list[1], "1"))
    {
        ui->label_adc2_amplifier->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.ADC2_Amplifier = TRUE;
    }
    else
    {
        ui->label_adc2_amplifier->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.ADC2_Amplifier = FALSE;
    }

    return TRUE;
}

bool ADC::checkADCLoopStatus()
{
    char cmd[255];
    sprintf(cmd, FLAG_CHECK_ADC_LOOP);
    QStringList  list;
    int n = 2;
    getStatusFromUMAC(cmd,n, &list);

    if(!QString::compare(list[0], "0"))
    {
        ui->label_adc1_loop->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.ADC1_LoopStatus = TRUE;
    }
    else
    {
        ui->label_adc1_loop->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.ADC1_LoopStatus = FALSE;
    }
    if(!QString::compare(list[1], "0"))
    {
        ui->label_adc2_loop->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.ADC2_LoopStatus = TRUE;
    }
    else
    {
        ui->label_adc2_loop->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.ADC2_LoopStatus = FALSE;
    }

    return TRUE;
}

bool ADC::checkADCError()
{
    char cmd[255];
    sprintf(cmd, FLAG_CHECK_ADC_ERROR);
    QStringList  list;
    int n = 2;
    getStatusFromUMAC(cmd,n, &list);

    if(!QString::compare(list[0], "0"))
    {
        ui->label_adc1_error->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.ADC1_Error = TRUE;
    }
    else
    {
        ui->label_adc1_error->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.ADC1_Error = FALSE;
    }
    if(!QString::compare(list[1], "0"))
    {
        ui->label_adc2_error->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.ADC2_Error = TRUE;
    }
    else
    {
        ui->label_adc2_error->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.ADC2_Error = FALSE;
    }

    return TRUE;
}

bool ADC::getStatusFromUMAC(char cmd[], int n, QStringList *Status)
{
    QString str;
    int it = 0;
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    *Status = str.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    for(QStringList::iterator point = (*Status).begin(); point != (*Status).end(); ++it, ++point){}
    if(it < n)
    {
        return FALSE;
    }
    return TRUE;
}

void ADC::on_pushButton_adc_go_clicked()
{
    on_pushButton_adc1_go_clicked();
    on_pushButton_adc2_go_clicked();
}

void ADC::on_pushButton_adc_stop_clicked()
{
    on_pushButton_adc1_stop_clicked();
    on_pushButton_adc2_stop_clicked();
}

void ADC::on_pushButton_adc_park_clicked()
{
    double AdcVelocityCmd =3600;
    char cmd[255];
    QString str;

    Adc1Velocity = AdcVelocityCmd / (CTS_ADC * 1000);
    sprintf(cmd, SET_ADC2_VEL, Adc1Velocity);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    Adc1Target = 0;
    Adc1Target = Adc1Target / CTS_ADC;
    sprintf(cmd, SET_ADC1_POS, Adc1Target);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);

    Adc2Velocity = AdcVelocityCmd / (CTS_ADC * 1000);
    sprintf(cmd, SET_ADC2_VEL, Adc2Velocity);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    Adc2Target = 0;
    Adc2Target = Adc2Target / CTS_ADC;
    sprintf(cmd, SET_ADC2_POS, Adc2Target);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void ADC::saveData()
{
    if(!ui->radioButton_adc_save->isChecked())
        return;
    if(statusADC1.Pointer > 299)
    {
        storeADC1Data();
        statusADC1.Pointer = 0;
    }
    if(statusADC2.Pointer > 299)
    {
        storeADC2Data();
        statusADC2.Pointer = 0;
    }
}

bool ADC::storeADC1Data()
{
        QString FileName="/home/rcz/Program/telescope/data/ADC1_Data.txt";
        dataSave(statusADC1.Data, statusADC1.DataTime, FileName, N_NUMBER);
        return TRUE;
}

bool ADC::storeADC2Data()
{
        QString FileName="/home/rcz/Program/telescope/data/ADC2_Data.txt";
        dataSave(statusADC2.Data, statusADC2.DataTime, FileName, N_NUMBER);
        return TRUE;
}

bool ADC::dataSave(double (*p)[6], QString DataTime[], QString FileName, int num)
{
    char s_0[400] ;
    char s_1[60] ;
    char s_2[60];
    char s_3[60];
    char s_4[60];
    int i,j;
    double t_end,t_start,t;

    QStringList  list1 = DataTime[299].split(QRegExp("\\s+"), QString::SkipEmptyParts);

    int it = 0;
    for(QStringList::iterator point = list1.begin(); point != list1.end(); ++it, ++point){}
    it = 0;
    QStringList list_time = list1[1].split (":");
    for(QStringList::iterator point = list_time.begin(); point != list_time.end(); ++it, ++point){}
    t_end = list_time[0].toDouble() * 3600 + list_time[1].toDouble() * 60 + list_time[2].toDouble();
    list1 = DataTime[0].split(QRegExp("\\s+"), QString::SkipEmptyParts);

    it = 0;
    for(QStringList::iterator point = list1.begin(); point != list1.end(); ++it, ++point){}
    it=0;
    list_time =  list1[1].split (":");
    for(QStringList::iterator point = list_time.begin(); point != list_time.end(); ++it, ++point){}
    t_start = list_time[0].toDouble() * 3600 + list_time[1].toDouble() * 60 + list_time[2].toDouble();

    t = t_end - t_start;
    if(t < 0)
        t = 30;

    double speed = (*(*(p + num - 1) + 1) - *(*(p + 0) + 1)) / t;
    double Xi = 0,Yi = 0;
    for(j = 0; j < num; j++)
        Xi = *(*(p + j) + 2) + Xi;
    Xi = Xi / num;

    for(j = 0; j < num; j++)
        Yi = (*(*(p + j) + 2) - Xi) * (*(*(p + j) + 2) - Xi) + Yi;
    if(num >= 1)
        Yi = sqrt(Yi / num);
    else
        Yi = 0;

    double min, max;
    double PV;
    max = min = *(*(p + 0) + 2);

    for(j = 0; j < num; j++)
    {
        if(max <= *(*(p + j) + 2))
        {
            max = *(*(p + j) + 2);
        }
        if(min > *(*(p + j) + 2))
        {
            min = *(*(p + j) + 2);
        }
    }
    PV = max - min;

    QDateTime dt = QDateTime::currentDateTime();
    //QTime time = dt.time();
    QString dtstr;
    dtstr = dt.toString("MM_dd_yyyy");
    //QString fileName =SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString()+FileName+dtstr+".txt";
    QFile file(FileName);
    if(!file.open(QFile::Append | QFile::Truncate))
    {
        QMessageBox::warning(this, tr("Warn"), tr("open error!"), QMessageBox::Yes);
        //qDebug("ADC can't open file for save.");
        return FALSE;
    }
    QTextStream out(&file);

    for(i = 0; i < num; i++)
    {
        out << "\n";
        sprintf(s_1, "  %10.6f", *(*(p + i) + 0) / 3600);
        sprintf(s_2, "  %10.6f", *(*(p + i) + 1) / 3600);
        sprintf(s_3, "  %10.4f", *(*(p + i) + 3));
        sprintf(s_4, "  %10.4f", *(*(p + i) + 2));
        sprintf(s_0, "%s%s%s%s       ", s_1, s_2, s_3, s_4);
        QString string = QString(QLatin1String(s_0)) + DataTime[i];
        out << string;

    }
    out << "\n";
    sprintf(s_1, " V=  %6.3f''/s,", speed);
    sprintf(s_2, "RMS=  %6.4f'' ", Yi);
    sprintf(s_3, "PV=  %6.4f'' ", PV);
    sprintf(s_0, "%s%s%s ", s_1, s_2, s_3);
    out<<tr(s_0);
    sprintf(s_1, " max=  %6.4f'',", max);
    sprintf(s_2, " min=  %6.4f''", min);
    out << tr(s_1);
    out << tr(s_2);
    file.close();

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////
void  ADC::CheckCommandFromOCS()
{
    if(OcsCom.flag_TcsEmergenStop_ADC)
    {
        on_pushButton_adc1_stop_clicked();
        on_pushButton_adc2_stop_clicked();
        OcsCom.flag_TcsEmergenStop_ADC=false;
        OcsCom.flag_PointByOCS_ADC1=false;
        OcsCom.flag_PointStopByOCS_ADC1=false;
        OcsCom.flag_PointingByOCS_ADC1=false;
        OcsCom.flag_TcsHomingADC1=false;
        OcsCom.flag_PointByOCS_ADC2=false;
        OcsCom.flag_PointStopByOCS_ADC2=false;
        OcsCom.flag_PointingByOCS_ADC2=false;
        OcsCom.flag_TcsHomingADC2=false;
        return;
    }
    if (OcsCom.flag_TcsEnableADC1)
    {
        EnableByOCS_ADC1();
        OcsCom.flag_TcsEnableADC1=false;
    }
    else if (OcsCom.flag_TcsDisableADC1)
    {
        DisableByOCS_ADC1();
        OcsCom.flag_TcsDisableADC1=false;
    }
    else if ( OcsCom.flag_TcsCheckADC1 )
    {
        CheckByOCS_ADC1();
        OcsCom.flag_TcsCheckADC1=false;
    }
    else if ( OcsCom.flag_TcsHomeADC1 )
    {
        HomeByOCS_ADC1();
        OcsCom.flag_TcsHomeADC1=false;
        OcsCom.flag_HomeErrorADC1=false;
        OcsCom.flag_TcsHomingADC1=true;
    }
    else if ( OcsCom.flag_PointByOCS_ADC1 )
    {
        PointByOCS_ADC1();
        OcsCom.flag_PointByOCS_ADC1=false;
        OcsCom.flag_PointingByOCS_ADC1=true;
        OcsCom.flag_PointStopByOCS_ADC1=false;
    }
    else if ( OcsCom.flag_PointStopByOCS_ADC1 )
    {
        PointStopByOCS_ADC1();
        OcsCom.flag_PointByOCS_ADC1=false;
        OcsCom.flag_PointStopByOCS_ADC1=false;
        OcsCom.flag_PointingByOCS_ADC1=false;
        OcsCom.flag_TcsHomingADC1=false;
    }
////////////////////////////////////////////////
    if (OcsCom.flag_TcsEnableADC2)
    {
        EnableByOCS_ADC2();
        OcsCom.flag_TcsEnableADC2=false;
    }
    else if (OcsCom.flag_TcsDisableADC2)
    {
        DisableByOCS_ADC2();
        OcsCom.flag_TcsDisableADC2=false;
    }
    else if ( OcsCom.flag_TcsCheckADC2 )
    {
        CheckByOCS_ADC2();
        OcsCom.flag_TcsCheckADC2=false;
    }
    else if ( OcsCom.flag_TcsHomeADC2 )
    {
        HomeByOCS_ADC2();
        OcsCom.flag_TcsHomeADC2=false;
        OcsCom.flag_HomeErrorADC2=false;
        OcsCom.flag_TcsHomingADC2=true;
    }
    else if ( OcsCom.flag_PointByOCS_ADC2 )
    {
        PointByOCS_ADC2();
        OcsCom.flag_PointByOCS_ADC2=false;
        OcsCom.flag_PointingByOCS_ADC2=true;
        OcsCom.flag_PointStopByOCS_ADC2=false;
    }
    else if ( OcsCom.flag_PointStopByOCS_ADC2 )
    {
        PointStopByOCS_ADC2();
        OcsCom.flag_PointByOCS_ADC2=false;
        OcsCom.flag_PointStopByOCS_ADC2=false;
        OcsCom.flag_PointingByOCS_ADC2=false;
        OcsCom.flag_TcsHomingADC2=false;
    }
}

void  ADC::MakeString(char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp)
{
    QDateTime datetime;
    QString s_datetime;

    datetime = QDateTime::currentDateTime();
    s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

    QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +s_datetime+QString ( "><ExeStatus=%1><RV=%2><Info=%3><bExeStatus=%4>" ).arg ( ExeStatus ).arg ( RV ).arg ( Info ).arg ( bExeStatus ) +DataBack;
    ( *s_temp ).append ( str );
}

void  ADC::CombineString ( char StatusCUID[100],char ExeStatus[50],char bExeStatus[10],QString* s_temp )
{
    QDateTime datetime;
    QString s_datetime;

    datetime = QDateTime::currentDateTime();
    s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

    QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +
            s_datetime+QString ( "><ExeStatus=%1><bExeStatus=true>" ).arg ( ExeStatus );
    ( *s_temp ).append ( str );
}

void  ADC::SendToOCS(QString str)
{
    sendthread *send = new sendthread(m_parent,str,OcsIP,OcsPort);
    send->run();
}
////////////////////////////////////////////////////////////////////////
void  ADC::EnableByOCS_ADC1()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.ADC1_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    on_pushButton_adc1_enable_clicked();
    CombineString ( OcsCom.StatusCUID_ADC1,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    MakeString ( OcsCom.StatusCUID_ADC1,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_ADC1,ExeStatus,bExeStatus,&s_temp2 );
    SendToOCS ( s_temp2 );
}

void  ADC::DisableByOCS_ADC1()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.ADC1_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Fault" );

    on_pushButton_adc1_disable_clicked();
    CombineString ( OcsCom.StatusCUID_ADC1,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    MakeString ( OcsCom.StatusCUID_ADC1,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_ADC1,ExeStatus,bExeStatus,&s_temp2 );
    SendToOCS ( s_temp2 );
}

void  ADC::CheckByOCS_ADC1()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.ADC1_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    CombineString ( OcsCom.StatusCUID_ADC1,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    DataBack=QString ( "<Pos=%1>" ).arg ( statusADC1.PosReal_Degree,0,'f',8 );
    if(flagtcs.ADC1_Homed) DataBack+="<Homed=true>";
    else DataBack+="<Homed=false>";
    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );
    MakeString ( OcsCom.StatusCUID_ADC1,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_ADC1,ExeStatus,bExeStatus,&s_temp2 );
    SendToOCS ( s_temp2 );
}

void  ADC::PointByOCS_ADC1()
{
    int AZ_d=0,AZ_m=0;
    double AZ_s=0;
    char s[255];
    QDateTime datetime;
    QString s_datetime;

    on_radioButton_adc1_vel_fast_toggled(true);

    D2DMS ( statusADC1.point,&AZ_d,&AZ_m,&AZ_s );
    sprintf ( s,"%3d",int ( AZ_d ) );//target,
    ui->lineEdit_adc1_targetD->setText ( s );//lineEdit_dero_targetD
    sprintf ( s,"%3d",int ( AZ_m ) );
    ui->lineEdit_adc1_targetM->setText ( s );
    sprintf ( s,"%6.3f",AZ_s );
    ui->lineEdit_adc1_targetS->setText ( s );
    on_pushButton_adc1_go_clicked();

    if ( !timer_replyOCS->isActive () )
        timer_replyOCS->start ( deroreplyOCSClock );//start check every 5 seconds

    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
       StatusCUID:return to OCS, new time
       ExeStatus:Start , Active , Done
        RV:    0:working normally;1:warning;-1:fault
           Info:   if no error:"OK"; or,imformation of error
      bExeStatus:  start working or finish working:"true";still working:false
      DataBack:	Additional data back to OCS,eg. positon
    */
    QString s_temp;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.ADC1_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_ADC1Point,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
}

void  ADC::PointStopByOCS_ADC1()
{
    on_pushButton_adc1_stop_clicked();

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
    if ( flagtcs.ADC1_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    CombineString ( OcsCom.StatusCUID_ADC1,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    MakeString ( OcsCom.StatusCUID_ADC1,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_ADC1,ExeStatus,bExeStatus,&s_temp2 );
    SendToOCS ( s_temp2 );
}

void  ADC::PointingByOCS_ADC1()
{
    QString s_temp,s_temp2;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.ADC1_Amplifier )
        sprintf ( Info,"Pointing" );
    else
        sprintf ( Info,"Open Loop" );

    DataBack=QString ( "<Pos=%1>" ).arg ( statusADC1.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_ADC1Point,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
    if ( fabs ( statusADC1.PosReal_Degree-statusADC1.point ) <=0.01 ) 			//az reach target
    {
        QTest::qWait(250);
        OcsCom.flag_PointingByOCS_ADC1=false;
        //if ( timer_replyOCS->isActive () )
        //    timer_replyOCS->stop();

        sprintf ( ExeStatus,"Done" );
        sprintf ( bExeStatus,"true" );
        CombineString ( OcsCom.StatusCUID_ADC1Point,ExeStatus,bExeStatus,&s_temp2 );
        SendToOCS ( s_temp2 );
    }
}

void  ADC::HomeByOCS_ADC1()
{
    QString s_temp;
    char ExeStatus[50]="Start";
    //char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    //QString DataBack="";
    on_pushButton_adc1_home_clicked();

    sprintf ( Info,"OK" );

    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_ADC1Home,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );

    if ( !timer_replyOCS->isActive () )
        timer_replyOCS->start ( deroreplyOCSClock );
}

void  ADC::HomingByOCS_ADC1()
{
    QString s_temp,s_temp2;
    QString temp1;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.ADC1_Amplifier )
        sprintf ( Info,"Homing" );
    else
        sprintf ( Info,"Open Loop" );

    DataBack=QString ( "<Pos=%1>" ).arg ( statusADC1.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_ADC1Home,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
    if (flagtcs.ADC1_Homed ) 			//az reach target
    {
        //if ( timer_replyOCS->isActive () )
         //   timer_replyOCS->stop();
        QTest::qWait(250);
        OcsCom.flag_TcsHomingADC1=false;
        sprintf ( bExeStatus,"true" );
        sprintf ( ExeStatus,"Done" );
        CombineString ( OcsCom.StatusCUID_ADC1Home,ExeStatus,bExeStatus,&s_temp2 );
        SendToOCS ( s_temp2 );
    }
    if ( OcsCom.flag_HomeErrorADC1 ) 			//az reach target
    {
        QTest::qWait(250);
        OcsCom.flag_TcsHomingADC1=false;
        sprintf ( RV,"-1" );
        sprintf ( Info,"Homing Error" );
        DataBack=QString ( "<Pos=%1>" ).arg ( statusADC2.PosReal_Degree,0,'f',8 );
        MakeString ( OcsCom.StatusCUID_ADC1Home,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        SendToOCS ( s_temp );
        QTest::qWait(250);

        sprintf ( bExeStatus,"true" );
        sprintf ( ExeStatus,"Done" );
        CombineString ( OcsCom.StatusCUID_ADC1Home,ExeStatus,bExeStatus,&s_temp2 );
        SendToOCS ( s_temp2 );
    }

}
//////////////////////////////////////////////////////////////////////////////////
void  ADC::EnableByOCS_ADC2()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.ADC2_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    on_pushButton_adc2_enable_clicked();
    CombineString ( OcsCom.StatusCUID_ADC2,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    MakeString ( OcsCom.StatusCUID_ADC2,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_ADC2,ExeStatus,bExeStatus,&s_temp2 );
    SendToOCS ( s_temp2 );
}

void  ADC::DisableByOCS_ADC2()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.ADC2_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Fault" );

    on_pushButton_adc2_disable_clicked();
    CombineString ( OcsCom.StatusCUID_ADC2,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    MakeString ( OcsCom.StatusCUID_ADC2,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_ADC2,ExeStatus,bExeStatus,&s_temp2 );
    SendToOCS ( s_temp2 );
}

void  ADC::CheckByOCS_ADC2()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.ADC2_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    CombineString ( OcsCom.StatusCUID_ADC2,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    DataBack=QString ( "<Pos=%1>" ).arg ( statusADC2.PosReal_Degree,0,'f',8 );
    if(flagtcs.ADC2_Homed) DataBack+="<Homed=true>";
    else DataBack+="<Homed=false>";
    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );
    MakeString ( OcsCom.StatusCUID_ADC2,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_ADC2,ExeStatus,bExeStatus,&s_temp2 );
    SendToOCS ( s_temp2 );
}

void  ADC::PointByOCS_ADC2()
{
    int AZ_d=0,AZ_m=0;
    double AZ_s=0;
    char s[255];
    QDateTime datetime;
    QString s_datetime;

    on_radioButton_adc2_vel_fast_toggled(true);

    D2DMS ( statusADC2.point,&AZ_d,&AZ_m,&AZ_s );
    sprintf ( s,"%3d",int ( AZ_d ) );//target,
    ui->lineEdit_adc2_targetD->setText ( s );//lineEdit_dero_targetD
    sprintf ( s,"%3d",int ( AZ_m ) );
    ui->lineEdit_adc2_targetM->setText ( s );
    sprintf ( s,"%6.3f",AZ_s );
    ui->lineEdit_adc2_targetS->setText ( s );
    on_pushButton_adc2_go_clicked();

    if ( !timer_replyOCS->isActive () )
        timer_replyOCS->start ( deroreplyOCSClock );//start check every 5 seconds

    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
       StatusCUID:return to OCS, new time
       ExeStatus:Start , Active , Done
        RV:    0:working normally;1:warning;-1:fault
           Info:   if no error:"OK"; or,imformation of error
      bExeStatus:  start working or finish working:"true";still working:false
      DataBack:	Additional data back to OCS,eg. positon
    */
    QString s_temp;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.ADC2_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_ADC2Point,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
}

void  ADC::PointStopByOCS_ADC2()
{
    on_pushButton_adc2_stop_clicked();

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
    if ( flagtcs.ADC2_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    CombineString ( OcsCom.StatusCUID_ADC2,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    MakeString ( OcsCom.StatusCUID_ADC2,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_ADC2,ExeStatus,bExeStatus,&s_temp2 );
    SendToOCS ( s_temp2 );
}

void  ADC::PointingByOCS_ADC2()
{
    QString s_temp,s_temp2;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.ADC2_Amplifier )
        sprintf ( Info,"Pointing" );
    else
        sprintf ( Info,"Open Loop" );

    DataBack=QString ( "<Pos=%1>" ).arg ( statusADC2.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_ADC2Point,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
    if ( fabs ( statusADC2.PosReal_Degree-statusADC2.point ) <=0.01 ) 			//az reach target
    {
        QTest::qWait(250);
        OcsCom.flag_PointingByOCS_ADC2=false;
        //if ( timer_replyOCS->isActive () )
        //    timer_replyOCS->stop();

        sprintf ( ExeStatus,"Done" );
        sprintf ( bExeStatus,"true" );
        CombineString ( OcsCom.StatusCUID_ADC2Point,ExeStatus,bExeStatus,&s_temp2 );
        SendToOCS ( s_temp2 );
    }
}

void  ADC::HomeByOCS_ADC2()
{
    QString s_temp;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    on_pushButton_adc2_home_clicked();

    sprintf ( Info,"OK" );

    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_ADC2Home,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );

    if ( !timer_replyOCS->isActive () )
        timer_replyOCS->start ( deroreplyOCSClock );
}

void  ADC::HomingByOCS_ADC2()
{
    QString s_temp,s_temp2;
    QString temp1;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.ADC2_Amplifier )
        sprintf ( Info,"Homing" );
    else
        sprintf ( Info,"Open Loop" );

    DataBack=QString ( "<Pos=%1>" ).arg ( statusADC2.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_ADC2Home,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );

    if (flagtcs.ADC2_Homed ) 			//az reach target
    {
        //if ( timer_replyOCS->isActive () )
        //    timer_replyOCS->stop();
        QTest::qWait(250);
        OcsCom.flag_TcsHomingADC2=false;
        sprintf ( bExeStatus,"true" );
        sprintf ( ExeStatus,"Done" );
        CombineString ( OcsCom.StatusCUID_ADC2Home,ExeStatus,bExeStatus,&s_temp2 );
        SendToOCS ( s_temp2 );
    }
    if ( OcsCom.flag_HomeErrorADC2 ) 			//az reach target
    {
        OcsCom.flag_TcsHomingADC2=false;
        QTest::qWait(250);
        sprintf ( RV,"-1" );
        sprintf ( Info,"Homing Error" );
        DataBack=QString ( "<Pos=%1>" ).arg ( statusADC2.PosReal_Degree,0,'f',8 );
        MakeString ( OcsCom.StatusCUID_ADC2Home,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        SendToOCS ( s_temp );
        QTest::qWait(250);

        sprintf ( bExeStatus,"true" );
        sprintf ( ExeStatus,"Done" );
        CombineString ( OcsCom.StatusCUID_ADC2Home,ExeStatus,bExeStatus,&s_temp2 );
        SendToOCS ( s_temp2 );
    }
}

void ADC::replyOCS()
{
    if ( OcsCom.flag_PointingByOCS_ADC1 )  PointingByOCS_ADC1();
    else if ( OcsCom.flag_TcsHomingADC1 )  HomingByOCS_ADC1();

    if( OcsCom.flag_PointingByOCS_ADC2)     PointingByOCS_ADC2();
    else if ( OcsCom.flag_TcsHomingADC2)     HomingByOCS_ADC2();
}
