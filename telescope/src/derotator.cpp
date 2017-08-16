#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <math.h>
#include <stdio.h>
#include<QtTest/QTest>
#include "derotator.h"
#include "ui_derotator.h"
#include "main_window.h"
#include "define.h"
#include "signs.h"
//#include "qumac.h"
//#include "application.h"
#include "server.h"
#include "sendthread.h"
//#include "direct_azalt.h"

#define DeroreplyOCSClock				3000

extern FlagTCS  flagtcs;
sAxis  statusDero, statusM3,  statusM3Rot;
/*StatusDero      statusDero;
StatusM3        statusM3;
StatusM3        statusM3Rot;*/
extern FlagTCS flagtcs;
extern OcsCOM OcsCom;
extern int OcsPort;						//Port of OCS (TCP/IP)
extern QString OcsIP; 						//IP of OCS(TCP/IP)
extern QString LocalhostIP;
extern QString	DataBackToOcs;
double  M3_trans_pos=0;
double  Fiber_Light_pos=0;
int     derodelay=3;
int    M3_AutoPos_Index=0;

Derotator::Derotator(MainWindow *parent) : ui(new Ui::Derotator)
{
    m_parent = parent;
    ui->setupUi(this);
    showDeroM3Info();
    deroM3CheckStatus();

    flagtcs.IsOpened_dero = true;
    //connectDevice();

    timer_DeroM3Info = new QTimer();
    connect(timer_DeroM3Info, SIGNAL(timeout()), this, SLOT(showDeroM3Info()));
    timer_DeroM3Info->start(TIMER_1000MS);

    timer_DeroM3CheckStatus = new QTimer();
    connect(timer_DeroM3CheckStatus, SIGNAL(timeout()), this, SLOT(deroM3CheckStatus()));
    timer_DeroM3CheckStatus->start(TIMER_1000MS);

    timer_save_data = new QTimer();
    connect(timer_save_data, SIGNAL(timeout()), this, SLOT(saveData()));
    timer_save_data->start(TIMER_1000MS);

    timer_DeroreplyOCS=new QTimer ( this );
    timer_DeroreplyOCS->connect ( timer_DeroreplyOCS, SIGNAL ( timeout() ), this, SLOT ( replyOCS() ) );
    //timer_DeroreplyOCS->start ( DeroreplyOCSClock );
    ui->lineEdit_m3_trans_target->setValidator(new MyDoubleValidator(-40,40,3,this));
    M3_trans_pos =34;

    flagtcs.Mirror_Cover_stop = false;
    flagtcs.Mirror_Cover_open = false;
    flagtcs.Mirror_Cover_close = false;
    flagtcs.Mirror_Cover_right_close = false;
    flagtcs.Mirror_Cover_left_close = false;
    flagtcs.Mirror_Cover_right_open = false;
    flagtcs.Mirror_Cover_left_open = false;
}

Derotator::~Derotator()
{
    delete ui;
    qDebug("Close derotator");
    flagtcs.IsOpened_dero = false;
}

void Derotator::changeEvent(QEvent *e)
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

/*void Derotator::connectDevice()
{
    SQUMAC::instance_p();
    qDebug("Derotator connect UMAC");
}*/

bool Derotator::getDataFromUMAC(char cmd[], int n, double cts, double *PosReal_Degree,
                                double *FolErrUMAC, double *Velocity, double Seconds_old,
                                double *Seconds, int PointerLast, int *Pointer,
                                double (*p)[6], QString DataTime[], int num)
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

bool Derotator::getStatusFromUMAC(char cmd[], int n, QStringList *Status)
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

bool Derotator::D2DMS(double Degree, int* Deg, int* Min, double* Sec)
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

bool Derotator::getDeroInfo()
{
    char cmd[255];
    sprintf(cmd, GET_DERO_INFO);
    int n = 5;
    double DeroFolErrUMAC = 0;
    double DeroVelocity;
    int Dero_d,Dero_m;
    double Dero_s;

    getDataFromUMAC(cmd, n, CTS_Dero, &statusDero.PosReal_Degree, &DeroFolErrUMAC,
                    &DeroVelocity, statusDero.Seconds, &statusDero.Seconds,
                    statusDero.Pointer, &statusDero.Pointer,
                    statusDero.Data, statusDero.DataTime, N_NUMBER);

    D2DMS(statusDero.PosReal_Degree, &Dero_d, &Dero_m, &Dero_s);
    ui->lineEdit_dero_posD->setText(QString::number(Dero_d));
    ui->lineEdit_dero_posM->setText(QString::number(Dero_m));
    ui->lineEdit_dero_posS->setText(QString::number(Dero_s, 'f', 2));
    ui->lineEdit_dero_error->setText(QString::number(DeroFolErrUMAC, 'f', 2));
    ui->lineEdit_dero_vel->setText(QString::number(DeroVelocity, 'f', 2));
    return TRUE;
}

bool Derotator::getM3Info()
{
    char cmd[255], cmd2[255];
    sprintf(cmd, GET_M3_INFO);
    sprintf(cmd2, GET_M3_INFO_ROT);
    int n = 5;
    double M3FolErrUMAC = 0, M3FolErrUMACRot = 0;
    double M3Velocity, M3VelocityRot;
    int M3_d_rot, M3_m_rot;
    double M3_s_rot, M3_trans_velocity;

    getDataFromUMAC(cmd, n, CTS_M3_TRANS, &statusM3.PosReal_Degree, &M3FolErrUMAC, &M3Velocity,
                    statusM3.Seconds, &statusM3.Seconds, statusM3.Pointer, &statusM3.Pointer,
                    statusM3.Data, statusM3.DataTime, N_NUMBER);
    getDataFromUMAC(cmd2, n, CTS_M3_ROT, &statusM3Rot.PosReal_Degree, &M3FolErrUMACRot, &M3VelocityRot,
                    statusM3Rot.Seconds, &statusM3Rot.Seconds, statusM3Rot.Pointer, &statusM3Rot.Pointer,
                    statusM3Rot.Data, statusM3Rot.DataTime, N_NUMBER);

    //D2DMS(statusM3.M3PosReal_Degree, &M3_d, &M3_m, &M3_s);
    D2DMS(statusM3Rot.PosReal_Degree, &M3_d_rot, &M3_m_rot, &M3_s_rot);

    if(flagtcs.m3_trans_homed ||flagtcs.m3_trans_homing)
        M3_trans_pos = statusM3.PosReal_Degree * 3600.0 / CTS_M3_TRANS * CTS_M3_MM;

    M3_trans_velocity = M3Velocity / CTS_M3_TRANS * CTS_M3_MM;
    ui->lineEdit_m3_trans_pos->setText(QString::number(M3_trans_pos, 'f', 2));
    ui->lineEdit_m3_trans_vel->setText(QString::number(M3_trans_velocity, 'f', 2));
    //ui->lineEdit_m3_trans_posD->setText(QString::number(M3_d));
    //ui->lineEdit_m3_trans_posM->setText(QString::number(M3_m));
    //ui->lineEdit_m3_trans_posS->setText(QString::number(M3_s, 'f', 2));
    //ui->lineEdit_m3_trans_vel->setText(QString::number(M3Velocity, 'f', 2));

    ui->lineEdit_m3_rot_posD->setText(QString::number(M3_d_rot));
    ui->lineEdit_m3_rot_posM->setText(QString::number(M3_m_rot));
    ui->lineEdit_m3_rot_posS->setText(QString::number(M3_s_rot, 'f', 2));
    ui->lineEdit_m3_rot_vel->setText(QString::number(M3VelocityRot, 'f', 2));

    return TRUE;
}

bool Derotator::checkDeroM3Home()
{
    char  cmd[255];
    sprintf(cmd, FLAG_CHECK_DERO_M3_HOME);
    QStringList  list;
    int n = 4;
    getStatusFromUMAC(cmd, n, &list);

    if(!QString::compare(list[0], "1"))
    {
        ui->label_dero_home->setStyleSheet("background-color:rgb(255, 255, 0)");    //yellow rgb(255, 255, 0)
        ui->label_dero_home_mark->setText("Homing...");
        flagtcs.Dero_Homed = FALSE;
    }
    if(!QString::compare(list[1], "1"))
    {
        ui->label_dero_home->setStyleSheet("background-color:rgb(0, 170, 0)");      //green rgb(0, 170, 0)
        ui->label_dero_home_mark->setText("Homed");
        flagtcs.Dero_Homed = TRUE;
    }
    if(!QString::compare(list[0], "0") && !QString::compare(list[1], "0"))
    {
        ui->label_dero_home->setStyleSheet("background-color:rgb(255, 0, 0)");      //red rgb(255, 0, 0)
        ui->label_dero_home_mark->setText ( "No Home" );
        OcsCom.flag_HomeErrorDero=true;
        flagtcs.Dero_Homed = FALSE;
    }

    if(!QString::compare(list[2], "1"))
    {
        ui->label_m3_trans_home->setStyleSheet("background-color:rgb(255, 255, 0)");    //yellow rgb(255, 255, 0)
        ui->label_m3_trans_home_mark->setText("Homing...");
        flagtcs.m3_trans_homed = FALSE;
        flagtcs.m3_trans_homing = TRUE;
        ui->pushButton_m3_auto_Pos1->setEnabled ( false );
        ui->pushButton_m3_auto_Pos2->setEnabled ( false );
    }
    if(!QString::compare(list[3], "1"))
    {
        ui->label_m3_trans_home->setStyleSheet("background-color:rgb(0, 170, 0)");      //green rgb(0, 170, 0)
        ui->label_m3_trans_home_mark->setText("Homed");
        flagtcs.m3_trans_homed = TRUE;
        flagtcs.m3_trans_homing = false;
        if(!flagtcs.m3_rot_pos_p)ui->pushButton_m3_auto_Pos1->setEnabled ( true );
        else ui->pushButton_m3_auto_Pos1->setEnabled ( false );
        if(!flagtcs.m3_rot_pos_n)ui->pushButton_m3_auto_Pos2->setEnabled ( true );
        else ui->pushButton_m3_auto_Pos2->setEnabled ( false );
    }
    /*if(!QString::compare(list[2], "0") && !QString::compare(list[3], "0"))
    {
        ui->label_m3_trans_home->setStyleSheet("background-color:rgb(255, 0, 0)");      //red rgb(255, 0, 0)
        ui->label_m3_trans_home_mark->setText ( "No Home" );
        OcsCom.flag_HomeErrorM3=true;
        //flagtcs.m3_trans_home = FALSE;
        ui->pushButton_m3_auto_Pos1->setEnabled ( false );
        ui->pushButton_m3_auto_Pos2->setEnabled ( false );
    }*/

    return TRUE;
}

bool Derotator::checkDeroM3Amplifier()
{
    char cmd[255];
    sprintf(cmd, FLAG_CHECK_DERO_M3_AMP);
    QStringList  list;
    int n = 2;
    getStatusFromUMAC(cmd,n, &list);

    if(!QString::compare(list[0], "1"))
    {
        ui->label_dero_amplifier->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.Dero_Amplifier = TRUE;
    }
    else
    {
        ui->label_dero_amplifier->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.Dero_Amplifier = FALSE;
    }
    if(!QString::compare(list[1], "1"))
    {
        ui->label_m3_trans_amplifier->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.m3_trans_amp = TRUE;
    }
    else
    {
        ui->label_m3_trans_amplifier->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.m3_trans_amp = FALSE;
    }

    return TRUE;
}

bool Derotator::checkDeroM3LoopStatus()
{
    char cmd[255];
    sprintf(cmd, FLAG_CHECK_DERO_M3_LOOP);
    QStringList  list;
    int n = 2;
    getStatusFromUMAC(cmd,n, &list);

    if(!QString::compare(list[0], "0"))
    {
        ui->label_dero_loop->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.Dero_LoopStatus = TRUE;
    }
    else
    {
        ui->label_dero_loop->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.Dero_LoopStatus = FALSE;
    }
    if(!QString::compare(list[1], "0"))
    {
        ui->label_m3_trans_loop->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.m3_trans_loop_status = TRUE;
    }
    else
    {
        ui->label_m3_trans_loop->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.m3_trans_loop_status = FALSE;
    }

    return TRUE;
}

bool Derotator::checkDeroError()
{
    char cmd[255];
    sprintf(cmd, FLAG_CHECK_DERO_ERROR);
    QStringList  list;
    int n = 1;
    getStatusFromUMAC(cmd,n, &list);

    if(!QString::compare(list[0], "0"))
    {
        ui->label_dero_error->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.Dero_Error = TRUE;
    }
    else
    {
        ui->label_dero_error->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.Dero_Error = FALSE;
    }

    return TRUE;
}

bool Derotator::checkM3Brake()
{
    char cmd[255];
    sprintf(cmd, FLAG_CHECK_M3_BRAKE);
    QStringList  list;
    int n = 1;
    getStatusFromUMAC(cmd,n, &list);

    if(!QString::compare(list[0], "1"))
    {
        ui->label_m3_trans_brake->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.m3_trans_brake = TRUE;
        ui->pushButton_m3_trans_unlock->setText("Brake");
    }
    else
    {
        ui->label_m3_trans_brake->setStyleSheet("background-color:rgb(255, 0, 0)" );
        flagtcs.m3_trans_brake = FALSE;
        ui->pushButton_m3_trans_unlock->setText("Release");
    }

    return TRUE;
}

bool Derotator::checkM3TransSafePos()
{
    QString str;
    //double M3_trans_pos;
    //M3_trans_pos = statusM3.PosReal_Degree * 3600.0 / CTS_M3_TRANS * CTS_M3_MM;



    if(M3_trans_pos > 34)
    {
        ui->label_m3_rot_InPosition->setStyleSheet("background-color:rgb(0, 170, 0)" );
        flagtcs.flag_m3_rot_in_positon = true;
    }
    else if(M3_trans_pos > 2)
    {

        ui->label_m3_rot_InPosition->setStyleSheet("background-color:rgb(255, 0, 0)");
        flagtcs.flag_m3_rot_in_positon = false;
        ui->label_m3_trans_safe_pos->setStyleSheet("background-color:rgb(255,0, 0)");
        flagtcs.m3_trans_safe_pos = false;
    }
    else
    {
        ui->label_m3_rot_InPosition->setStyleSheet("background-color:rgb(255, 0, 0)");
        flagtcs.flag_m3_rot_in_positon = false;
        ui->label_m3_trans_safe_pos->setStyleSheet("background-color:rgb(0,170, 0)");
        flagtcs.m3_trans_safe_pos = true;
    }
    return TRUE;
}

bool Derotator::checkM3RotPositon()
{
    char cmd[255];
    sprintf(cmd, FLAG_CHECK_M3_ROT_POS);
    QStringList  list;
    int n = 3;
    getStatusFromUMAC(cmd,n, &list);

    if(!QString::compare(list[0], "0"))
    {
        ui->label_m3_rot_P->setStyleSheet("background-color:rgb(255,0, 0)");
        flagtcs.m3_rot_pos_p = FALSE;
        //ui->pushButton_m3_auto_Pos1->setEnabled ( true );
    }
    else  //position 1 tube
    {
        ui->label_m3_rot_P->setStyleSheet("background-color:rgb(0, 170, 0)" );
        flagtcs.m3_rot_pos_p = TRUE;
        flagtcs.m3_rot_pos_n = false;
        //ui->pushButton_m3_auto_Pos1->setEnabled ( false );
    }

    if(!QString::compare(list[1], "0"))
    {
        ui->label_m3_rot_N->setStyleSheet("background-color:rgb(255, 0, 0)");
        flagtcs.m3_rot_pos_n = FALSE;
        //ui->pushButton_m3_auto_Pos2->setEnabled ( true );
    }
    else  //position 2 hartman
    {
        ui->label_m3_rot_N->setStyleSheet("background-color:rgb(0, 170, 0)" );
        flagtcs.m3_rot_pos_n = TRUE;
        flagtcs.m3_rot_pos_p = false;
        //ui->pushButton_m3_auto_Pos2->setEnabled ( false );
    }

    return TRUE;
}

bool Derotator::checkM3Rot()
{
    char cmd[255];
    sprintf(cmd, FLAG_CHECK_M3_ROT);
    QStringList  list;
    int n = 1;
    getStatusFromUMAC(cmd,n, &list);

    if(!QString::compare(list[0], "0"))
    {
        flagtcs.m3_rot_pos = TRUE;
    }
    else
    {
        flagtcs.m3_rot_pos = FALSE;
    }
    return TRUE;
}

bool Derotator::checkFiberSwitch()
{
    char cmd[255];
    sprintf(cmd, FLAG_CHECK_FIBER_SWITCH);
    QStringList  list;
    int n = 1;
    getStatusFromUMAC(cmd,n, &list);

    if(!QString::compare(list[0], "1"))
    {
        ui->label_fiber_switch->setStyleSheet("background-color:rgb(0, 170, 0)");
        ui->label_fiber_switch_status->setText("Switch on");
        flagtcs.fiber_switch = TRUE;
    }
    else
    {
        ui->label_fiber_switch->setStyleSheet("background-color:rgb(255, 0, 0)");
        flagtcs.fiber_switch = FALSE;
        ui->label_fiber_switch_status->setText("Switch off");
    }
    return TRUE;
}

bool Derotator::dataSave(double (*p)[6], QString DataTime[], QString FileName, int num)
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
    if(!file.open(QFile::Append | QFile::WriteOnly))
    {
        QMessageBox::warning(this, tr("Warn"), tr("open error!"), QMessageBox::Yes);
        //qDebug("Dero can't open file for save.");
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

bool Derotator::showDeroM3Info()
{
    getDeroInfo();
    getM3Info();
    return TRUE;
}

void Derotator::deroM3CheckStatus()
{
    checkDeroM3Home();
    //checkADCSoftLimit();
    checkDeroM3Amplifier();
    checkDeroM3LoopStatus();
    checkDeroError();
    checkM3Brake();
    checkM3RotPositon();
    checkM3Rot();
    checkFiberSwitch();
    checkCover();
    checkM3TransSafePos();
    checkDiskLock();
    if(derodelay<0)CheckCommandFromOCS();
    else derodelay--;

    if(M3_AutoPos_Index==-1 && M3_trans_pos <= 1)on_pushButton_m3_auto_Pos1_clicked();
    else if(M3_AutoPos_Index==1 && M3_trans_pos <= 1)on_pushButton_m3_auto_Pos2_clicked();
    else if(M3_AutoPos_Index==-2 && M3_trans_pos >= 34)M3_AutoPos_Index=5;
    else if(M3_AutoPos_Index==2 && M3_trans_pos >= 34)M3_AutoPos_Index=5;
    else if(M3_AutoPos_Index>=5 && M3_AutoPos_Index<=40)M3_AutoPos_Index++;
    else if(M3_AutoPos_Index>40)
    {
        on_pushButton_m3_disable_clicked();
        M3_AutoPos_Index=0;
    }
}

void Derotator::saveData()
{
    if(!ui->radioButton__dero_save->isChecked())
        return;
    if(statusDero.Pointer > 299)
    {
        storeDeroData();
        statusDero.Pointer = 0;
    }
}

bool Derotator::storeDeroData()
{
    qDebug("123456789");
        QString FileName="/home/rcz/Program/telescope/data/Dero_Data.txt";
        dataSave(statusDero.Data, statusDero.DataTime, FileName, N_NUMBER);//N_NUMBER
        return TRUE;
}

void Derotator::on_pushButton_dero_enable_clicked()
{
    QString str;
    char cmd[255];
    sprintf(cmd, ENABLE_DERO);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void Derotator::on_pushButton_dero_home_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC (HOME_DERO, str);
    OcsCom.flag_HomeErrorDero=false;
    flagtcs.Dero_Homed = false;
}

void Derotator::on_pushButton_dero_disable_clicked()
{
    QString str;
    char cmd[255];
    sprintf(cmd, DISENABLE_DERO);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void Derotator::on_radioButton_dero_vel_fast_toggled(bool checked)
{
    if(checked)
    {
        char cmd[255];
        QString str;
        //Adc1Velocity = SMySetting::instance_p()->value ( "ADC/adc_vel_fast" ).toDouble();
        DeroVelocity = 3600;
        ui->lineEdit_dero_vel_spec->setText(QString::number(DeroVelocity));
        DeroVelocity = DeroVelocity / ( CTS_Dero * 1000);
        sprintf(cmd, SET_DERO_VEL, DeroVelocity);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
}

void Derotator::on_radioButton_dero_vel_mid_toggled(bool checked)
{
    if(checked)
    {
        char cmd[255];
        QString str;
        //Adc1Velocity = SMySetting::instance_p()->value ( "ADC/adc_vel_fast" ).toDouble();
        DeroVelocity = 600;
        ui->lineEdit_dero_vel_spec->setText(QString::number(DeroVelocity));
        DeroVelocity = DeroVelocity / ( CTS_Dero * 1000);
        sprintf(cmd, SET_DERO_VEL, DeroVelocity);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
}

void Derotator::on_radioButton_dero_vel_slow_toggled(bool checked)
{
    if(checked)
    {
        char cmd[255];
        QString str;
        //Adc1Velocity = SMySetting::instance_p()->value ( "ADC/adc_vel_fast" ).toDouble();
        DeroVelocity = 15;
        ui->lineEdit_dero_vel_spec->setText(QString::number(DeroVelocity));
        DeroVelocity = DeroVelocity / ( CTS_Dero * 1000);
        sprintf(cmd, SET_DERO_VEL, DeroVelocity);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }
}

void Derotator::on_lineEdit_dero_vel_spec_returnPressed()
{
    double DeroVelocityCmd = 0;
    char cmd[255];
    QString str;
    DeroVelocityCmd = ui->lineEdit_dero_vel_spec->text().toFloat();
    DeroVelocity = DeroVelocityCmd / (CTS_Dero * 1000);
    sprintf(cmd, SET_DERO_VEL, DeroVelocity);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void Derotator::on_pushButton_dero_go_clicked()
{
    on_lineEdit_dero_vel_spec_returnPressed();
    char cmd[255];
    QString str;

    DeroTarget = ui->lineEdit_dero_targetD->text().toFloat() * 3600 + ui->lineEdit_dero_targetM->text().toFloat() * 60 + ui->lineEdit_dero_targetS->text().toFloat();
    DeroTarget = DeroTarget / CTS_Dero;
    sprintf(cmd, SET_DERO_POS, DeroTarget);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void Derotator::on_pushButton_dero_stop_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC(SET_DERO_STOP, str);
}

void Derotator::on_pushButton_dero_park_clicked()
{
    double DeroVelocityCmd =3600;
    char cmd[255];
    QString str;

    DeroVelocity = DeroVelocityCmd / (CTS_Dero * 1000);
    sprintf(cmd, SET_DERO_VEL, DeroVelocity);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    DeroTarget = 0;
    DeroTarget = DeroTarget / CTS_Dero;
    sprintf(cmd, SET_DERO_POS, DeroTarget);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);//pushButton_paste
}

void Derotator::on_pushButton_m3_enable_clicked()
{
    QString str;
    char cmd[255];
    sprintf(cmd, ENABLE_M3);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void Derotator::on_pushButton_m3_disable_clicked()
{
    QString str;
    char cmd[255];
    M3_AutoPos_Index=0;
    sprintf(cmd, DISENABLE_M3);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void Derotator::on_pushButton_m3_trans_home_clicked()
{
    QString str;
    /*if(flagtcs.m3_trans_brake)
    {
            SQUMAC::instance_p()->QcommCmdPMAC (HOME_M3, str);
    }*/
    if(!flagtcs.m3_trans_brake)
    {
       SQUMAC::instance_p()->QcommCmdPMAC (UNLOCK_TRANS_M3, str);
    }
    SQUMAC::instance_p()->QcommCmdPMAC (HOME_M3, str);
    flagtcs.m3_trans_homed=false;
    OcsCom.flag_HomeErrorM3=false;
    /*else
    {
        QMessageBox mb(QMessageBox::Warning, tr("Warning!"), tr("M3 Translation motor is braked, Please check it!"), QMessageBox::Ok);
        mb.exec();
    }*/
}

void Derotator::on_pushButton_m3_trans_unlock_clicked()
{
    QString str;
    if(!flagtcs.m3_trans_brake)
    {
       SQUMAC::instance_p()->QcommCmdPMAC (UNLOCK_TRANS_M3, str);
    }
    else
    {
        SQUMAC::instance_p()->QcommCmdPMAC (LOCK_TRANS_M3, str);
    }
}

void Derotator::on_pushButton_m3_trans_go_clicked()
{
    char cmd[255];
    QString str;

    /*if(flagtcs.m3_trans_brake)
    {
        M3Velocity = ui->lineEdit_m3_set_trans_vel->text().toFloat();
        M3Velocity = M3Velocity / (CTS_M3_MM * 1000);
        sprintf(cmd, SET_M3_VEL, M3Velocity);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);

        //M3Target = ui->lineEdit_m3_trans_targetD->text().toFloat() * 3600 + ui->lineEdit_m3_trans_targetM->text().toFloat() * 60 + ui->lineEdit_m3_trans_targetS->text().toFloat();
        M3Target = ui->lineEdit_m3_trans_target->text().toFloat();
        M3Target = M3Target / CTS_M3_MM;
        sprintf(cmd, SET_M3_POS, M3Target);
        SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
    }*/
    if(!flagtcs.m3_trans_brake)
    {
       SQUMAC::instance_p()->QcommCmdPMAC (UNLOCK_TRANS_M3, str);
    }
    M3Velocity = ui->lineEdit_m3_set_trans_vel->text().toFloat();
    M3Velocity = M3Velocity / (CTS_M3_MM * 1000);
    sprintf(cmd, SET_M3_VEL, M3Velocity);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);

    //M3Target = ui->lineEdit_m3_trans_targetD->text().toFloat() * 3600 + ui->lineEdit_m3_trans_targetM->text().toFloat() * 60 + ui->lineEdit_m3_trans_targetS->text().toFloat();
    M3Target = ui->lineEdit_m3_trans_target->text().toFloat();
    M3Target = M3Target / CTS_M3_MM;
    sprintf(cmd, SET_M3_POS, M3Target);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);

}

void Derotator::on_pushButton_m3_trans_stop_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC(SET_M3_TRANS_STOP, str);
}

void Derotator::on_pushButton_m3_rot_go_P_clicked()
{
    QString str;
    char cmd[255];

    M3VelocityRot = ui->lineEdit_m3_set_rot_vel->text().toFloat();
    M3VelocityRot = M3VelocityRot / (CTS_M3_ROT * 1000);
    sprintf(cmd, SET_M3_VEL_ROT, M3VelocityRot);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);

    SQUMAC::instance_p()->QcommCmdPMAC(SET_M3_ROT_DIR_P, str);
    //SQUMAC::instance_p()->QcommCmdPMAC("P705=1", str);
}

void Derotator::on_pushButton_m3_rot_go_N_clicked()
{
    QString str;
    char cmd[255];

    M3VelocityRot = ui->lineEdit_m3_set_rot_vel->text().toFloat();
    M3VelocityRot = M3VelocityRot / (CTS_M3_ROT * 1000);
    sprintf(cmd, SET_M3_VEL_ROT, M3VelocityRot);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);

    SQUMAC::instance_p()->QcommCmdPMAC(SET_M3_ROT_DIR_N, str);
    //SQUMAC::instance_p()->QcommCmdPMAC("P705=2", str);
}

void Derotator::on_pushButton_m3_rot_stop_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC(SET_M3_ROT_STOP, str);
}

void Derotator::on_pushButton_m3_auto_Pos1_clicked()
{
    if(flagtcs.m3_rot_pos_p)return;
    QString str;
    //double M3_trans_pos;
    //M3_trans_pos = statusM3.PosReal_Degree * 3600.0 / CTS_M3_TRANS * CTS_M3_MM;
    if(!flagtcs.m3_trans_homed) return;
    if(M3_trans_pos <= 2)
    {
        on_pushButton_m3_enable_clicked();
        SQUMAC::instance_p()->QcommCmdPMAC(SET_M3_AUTO_1, str);//#7j:-95000,P705=1
        M3_AutoPos_Index=-2;
    }
    else
    {
        M3_AutoPos_Index=-1;
        ui->lineEdit_m3_trans_target->setText("0");
        on_pushButton_m3_trans_go_clicked();
    }
}

void Derotator::on_pushButton_m3_auto_Pos2_clicked()
{
    if(flagtcs.m3_rot_pos_n)return;
    QString str;
    //double M3_trans_pos;
    //M3_trans_pos = statusM3.PosReal_Degree * 3600.0 / CTS_M3_TRANS * CTS_M3_MM;
    if(!flagtcs.m3_trans_homed) return;
    if(M3_trans_pos <= 2)
    {
        on_pushButton_m3_enable_clicked();
        SQUMAC::instance_p()->QcommCmdPMAC(SET_M3_AUTO_2, str);//#7j:95000,P705=2
        M3_AutoPos_Index=2;
    }
    else
    {
        M3_AutoPos_Index=1;
        ui->lineEdit_m3_trans_target->setText("0");
        on_pushButton_m3_trans_go_clicked();
    }
}

void Derotator::on_pushButton_fiber_Pos1_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC(SET_FIBER_SWITCH_ON, str);
    SQUMAC::instance_p()->QcommCmdPMAC(SET_FIBER_DIR_P, str);
}

void Derotator::on_pushButton_fiber_Pos2_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC(SET_FIBER_SWITCH_OFF, str);
    SQUMAC::instance_p()->QcommCmdPMAC(SET_FIBER_DIR_N, str);
}
////////////////////////////////////////////////////
/* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
   StatusCUID:return to OCS, new time
   ExeStatus:Start , Active , Done
   RV:    0:working normally;1:warning;-1:fault
   Info:   if no error:"OK"; or,imformation of error
   bExeStatus:  start working or finish working:"true";still working:false
   DataBack:	Additional data back to OCS,eg. positon

*/

void Derotator::MakeString ( char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp )
{
    QDateTime datetime;
    QString s_datetime;

    datetime = QDateTime::currentDateTime();
    s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

    QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +s_datetime+QString ( "><ExeStatus=%1><RV=%2><Info=%3><bExeStatus=%4>" ).arg ( ExeStatus ).arg ( RV ).arg ( Info ).arg ( bExeStatus ) +DataBack;
    ( *s_temp ).append ( str );
}

void  Derotator::CombineString ( char StatusCUID[100],char ExeStatus[50],char bExeStatus[10],QString* s_temp )
{
    QDateTime datetime;
    QString s_datetime;

    datetime = QDateTime::currentDateTime();
    s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

    QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +s_datetime+QString ( "><ExeStatus=%1><bExeStatus=true>" ).arg ( ExeStatus );
    ( *s_temp ).append ( str );
}

void Derotator::SendToOCS(QString str)
{
    sendthread *send = new sendthread(m_parent,str,OcsIP,OcsPort);
    //sendthread *send = new sendthread(m_parent,str,LocalhostIP,OcsPort);
    send->run();

}

void Derotator::CheckCommandFromOCS()
{
    if(OcsCom.flag_TcsEmergenStop_M3)
    {
        on_pushButton_m3_rot_stop_clicked();
        on_pushButton_m3_trans_stop_clicked();
        on_pushButton_dero_stop_clicked();
        on_pushButton_COVER_STOP_clicked();
        OcsCom.flag_TcsEmergenStop_M3=false;
        OcsCom.flag_PointByOCS_Dero=false;
        OcsCom.flag_PointStopByOCS_Dero=false;
        OcsCom.flag_PointingByOCS_Dero=false;
        OcsCom.flag_PointByOCS_M3T=false;
        OcsCom.flag_PointStopByOCS_M3T=false;
        OcsCom.flag_PointingByOCS_M3T=false;
        OcsCom.flag_PointByOCS_M3R=false;
        OcsCom.flag_PointStopByOCS_M3R=false;
        OcsCom.flag_PointingByOCS_M3R=false;
        return;
    }
    /*if(OcsCom.flag_TcsGetDeroPosition)
    {
        OcsCom.flag_TcsGetDeroPosition=false;
        GetDeroPositionByOCS();
    }*/
    if (OcsCom.flag_TcsEnableDero)
    {
        EnableByOCS_Dero();
        OcsCom.flag_TcsEnableDero=false;
    }
    else if (OcsCom.flag_TcsDisableDero)
    {
        DisableByOCS_Dero();
        OcsCom.flag_TcsDisableDero=false;
    }
    else if ( OcsCom.flag_TcsCheckDero )
    {
        CheckByOCS_Dero();
        OcsCom.flag_TcsCheckDero=false;
    }
    else if ( OcsCom.flag_TcsHomeDero )
    {
        HomeByOCS_Dero();
        OcsCom.flag_TcsHomeDero=false;
        OcsCom.flag_HomeErrorDero=false;
        OcsCom.flag_TcsHomingDero=true;
    }
    else if ( OcsCom.flag_PointByOCS_Dero )
    {
        PointByOCS_Dero();
        OcsCom.flag_PointByOCS_Dero=false;
        OcsCom.flag_PointingByOCS_Dero=true;
        OcsCom.flag_PointStopByOCS_Dero=false;
    }
    else if ( OcsCom.flag_PointStopByOCS_Dero )
    {
        PointStopByOCS_Dero();
        OcsCom.flag_PointByOCS_Dero=false;
        OcsCom.flag_PointStopByOCS_Dero=false;
        OcsCom.flag_PointingByOCS_Dero=false;
    }
    else if (OcsCom.flag_TcsOpenCover)
    {
        OpenCoverByOCS();
        OcsCom.flag_TcsOpenCover=false;
    }
    else if (OcsCom.flag_TcsCloseCover)
    {
        CloseCoverByOCS();
        OcsCom.flag_TcsCloseCover=false;
    }
    else if (OcsCom.flag_TcsStopCover)
    {
        StopCoverByOCS();
        OcsCom.flag_TcsStopCover=false;
    }
///////////////////////////////////////////////////////////////
    if (OcsCom.flag_TcsEnableM3)
    {
        EnableByOCS_M3();
        OcsCom.flag_TcsEnableM3=false;
    }
    else if (OcsCom.flag_TcsDisableM3)
    {
        DisableByOCS_M3();
        OcsCom.flag_TcsDisableM3=false;
    }
    else if ( OcsCom.flag_TcsCheckM3 )
    {
        CheckByOCS_M3();
        OcsCom.flag_TcsCheckM3=false;
    }
    else if ( OcsCom.flag_TcsLockM3T )
    {
        LockByOCS_M3T();
        OcsCom.flag_TcsLockM3T=false;
    }
    else if ( OcsCom.flag_TcsUnlockM3T )
    {
        UnLockByOCS_M3T();
        OcsCom.flag_TcsUnlockM3T=false;
    }
    else if ( OcsCom.flag_TcsHomeM3 )
    {
        HomeByOCS_M3();
        OcsCom.flag_TcsHomeM3 =false;
        OcsCom.flag_HomeErrorM3=false;
        OcsCom.flag_TcsHomingM3=true;
    }
    else if ( OcsCom.flag_PointByOCS_M3T )
    {
        PointByOCS_M3T();
        OcsCom.flag_PointByOCS_M3T=false;
        OcsCom.flag_PointStopByOCS_M3T=false;
        OcsCom.flag_PointingByOCS_M3T=true;
    }
    else if ( OcsCom.flag_PointStopByOCS_M3T )
    {
        PointStopByOCS_M3T();
        OcsCom.flag_PointByOCS_M3T=false;
        OcsCom.flag_PointStopByOCS_M3T=false;
        OcsCom.flag_PointingByOCS_M3T=false;
    }
////////////////////////////////////////////
    if ( OcsCom.flag_PointByOCS_M3R ) 			//
    {
        PointByOCS_M3R();
        OcsCom.flag_PointByOCS_M3R=false;
        OcsCom.flag_PointingByOCS_M3R=true;					//pointing
        OcsCom.flag_PointStopByOCS_M3R=false;
    }
    else if ( OcsCom.flag_PointStopByOCS_M3R )
    {
        PointStopByOCS_M3R();
        OcsCom.flag_PointByOCS_M3R=false;
        OcsCom.flag_PointStopByOCS_M3R=false;
        OcsCom.flag_PointingByOCS_M3R=false;
    }
////////////////////////////////////////////////
    if(OcsCom.flag_PointByOCS_FL)
    {
        PointByOCS_FL();
        OcsCom.flag_PointByOCS_FL=false;
        OcsCom.flag_PointingByOCS_FL=true;
    }
    else if(OcsCom.flag_PointStopByOCS_FL)
    {
        PointStopByOCS_FL();
        OcsCom.flag_PointingByOCS_FL=false;
        OcsCom.flag_PointStopByOCS_FL=false;
    }
}
//OcsCom.flag_PointingByOCS_Dero,Global Variable
void Derotator::OpenCoverByOCS()
{
    QString s_temp,s_temp1,s_temp2,s_temp3;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[20];
    char bExeStatus[10]="true";
    QString DataBack="";

    CombineString ( OcsCom.StatusCUID_CoverOpen,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );

    on_pushButton_COVER_OPEN_clicked();

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "1" );
    sprintf ( Info," Cover openning" );
    MakeString ( OcsCom.StatusCUID_CoverOpen,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );

    if ( !timer_DeroreplyOCS->isActive () )
        timer_DeroreplyOCS->start ( DeroreplyOCSClock );//start check every 5 seconds
    flagtcs.Mirror_Cover_open = true;
}

void Derotator::CloseCoverByOCS()
{
    QString s_temp,s_temp1,s_temp2,s_temp3;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[20];
    char bExeStatus[10]="true";
    QString DataBack="";

    CombineString ( OcsCom.StatusCUID_CoverClose,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );

    on_pushButton_COVER_CLOSE_clicked();

    sprintf ( Info,"Cover closing" );
    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "1" );
    MakeString ( OcsCom.StatusCUID_CoverClose,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );

    if ( !timer_DeroreplyOCS->isActive () )
        timer_DeroreplyOCS->start ( DeroreplyOCSClock );//start check every 5 seconds
    flagtcs.Mirror_Cover_close = true;
}

void Derotator::StopCoverByOCS()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[20];
    char bExeStatus[10]="true";
    QString DataBack="";

    CombineString ( OcsCom.StatusCUID_CoverStop,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );

    on_pushButton_COVER_STOP_clicked();

    QTime dieTime;
    dieTime = QTime::currentTime().addMSecs(2000);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );

    sprintf ( RV, "0" );
    sprintf ( ExeStatus, "Active" );
    if ( flagtcs.Mirror_Cover_stop )
        sprintf ( Info,"Cover stoped" );
    else
        sprintf ( Info,"Cover stop fault" );
    MakeString ( OcsCom.StatusCUID_CoverStop,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );

    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_CoverStop,ExeStatus,bExeStatus,&s_temp2 );
    SendToOCS ( s_temp2 );
}

void Derotator::replyOCS() // replay OCS every 5 seconds,timer_replyOCS
{
    if ( OcsCom.flag_PointingByOCS_Dero )  PointingByOCS_Dero();
    else if ( OcsCom.flag_TcsHomingDero )  HomingByOCS_Dero();

    if( OcsCom.flag_PointingByOCS_M3T)     PointingByOCS_M3T();
    else if ( OcsCom.flag_TcsHomingM3)     HomingByOCS_M3();

    if( OcsCom.flag_PointingByOCS_M3R)     PointingByOCS_M3R();

    if( OcsCom.flag_PointingByOCS_FL)     PointingByOCS_FL();

    if( flagtcs.Mirror_Cover_open )
    {
        if( flagtcs.Mirror_Cover_left_open && flagtcs.Mirror_Cover_right_open )
        {
            flagtcs.Mirror_Cover_open = false;
            OpenCoverReplyOCS();
        }
    }
    if( flagtcs.Mirror_Cover_close )
    {
        if( flagtcs.Mirror_Cover_left_close && flagtcs.Mirror_Cover_right_close )
        {
            flagtcs.Mirror_Cover_close = false;
            CloseCoverReplyOCS();
        }
    }
}
void   Derotator::GetDeroPositionByOCS()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";


    CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );

    sprintf ( ExeStatus, "Active" );
    if ( flagtcs.Dero_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Fault" );
    DataBack=QString ( "<Dero=%1>" ).arg ( statusDero.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );

    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void  Derotator::EnableByOCS_Dero()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";

    CombineString ( OcsCom.StatusCUID_Dero,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);
    sprintf ( ExeStatus, "Active" );
    if ( flagtcs.Dero_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );
    DataBack=QString ( "<Dero=%1>" ).arg ( statusDero.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_Dero,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);
    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_Dero,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );

}

void   Derotator::DisableByOCS_Dero()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";

    CombineString ( OcsCom.StatusCUID_Dero,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);
    sprintf ( ExeStatus, "Active" );
    if ( flagtcs.Dero_Amplifier )
        sprintf ( Info,"OK Disable" );
    else
        sprintf ( Info,"Open Loop" );
    DataBack=QString ( "<Dero=%1>" ).arg ( statusDero.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_Dero,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);
    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_Dero,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );

}

void   Derotator::CheckByOCS_Dero()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.Dero_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    CombineString ( OcsCom.StatusCUID_Dero,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);
    sprintf ( ExeStatus, "Active" );    
    sprintf ( RV, "0" );
    sprintf ( Info, "OK" );
    DataBack=QString ( "<Pos=%1>" ).arg ( statusDero.PosReal_Degree,0,'f',8 );
    if(flagtcs.Dero_Homed) DataBack+="<Homed=true>";
    else DataBack+="<Homed=false>";
    MakeString ( OcsCom.StatusCUID_Dero,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);
    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_Dero,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );

}

void   Derotator::PointByOCS_Dero()
{
    int AZ_d=0,AZ_m=0;
    double AZ_s=0;
    char s[255];

    QString s_temp,s_temp1;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.Dero_Amplifier )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_DeroPoint,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    SendToOCS ( s_temp );

    if(fabs(statusDero.point)<=360)
    {
        D2DMS ( statusDero.point,&AZ_d,&AZ_m,&AZ_s );
        sprintf ( s,"%3d",int ( AZ_d ) );//target,
        ui->lineEdit_dero_targetD->setText ( s );//lineEdit_dero_targetD
        sprintf ( s,"%3d",int ( AZ_m ) );
        ui->lineEdit_dero_targetM->setText ( s );
        sprintf ( s,"%6.3f",AZ_s );
        ui->lineEdit_dero_targetS->setText ( s );
        on_radioButton_dero_vel_fast_toggled(true);
        on_pushButton_dero_go_clicked();
    }
    else
    {
        OcsCom.flag_PointingByOCS_Dero=false;
        sprintf ( RV,"-1" );
        sprintf ( ExeStatus,"Active" );
        sprintf ( Info,"Beyond Derotator range" );
        MakeString ( OcsCom.StatusCUID_DeroPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        SendToOCS ( s_temp );
        QTest::qWait(250);
        sprintf ( ExeStatus,"Done" );
        sprintf ( bExeStatus,"true" );
        CombineString ( OcsCom.StatusCUID_DeroPoint,ExeStatus,bExeStatus,&s_temp1);
        SendToOCS ( s_temp1 );
    }
    if ( !timer_DeroreplyOCS->isActive () )
        timer_DeroreplyOCS->start ( DeroreplyOCSClock );//start check every 5 seconds

}

void   Derotator::PointStopByOCS_Dero()
{
    on_pushButton_dero_stop_clicked();


    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.Dero_Amplifier )
        sprintf ( Info,"OK Stop" );
    else
        sprintf ( Info,"Open Loop" );

    CombineString ( OcsCom.StatusCUID_Dero,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( bExeStatus,"false" );
    sprintf ( ExeStatus, "Active" );
    DataBack=QString ( "<Pos=%1>" ).arg ( statusDero.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_Dero,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );

    QTest::qWait(250);
    sprintf(bExeStatus,"true");
    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_Dero,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void   Derotator::PointingByOCS_Dero()
{
    QString s_temp,s_temp2;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.Dero_Amplifier )
        sprintf ( Info,"Pointing" );
    else
        sprintf ( Info,"Open Loop" );

    DataBack=QString ( "<Pos=%1>" ).arg ( statusDero.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_DeroPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
    if ( fabs ( statusDero.PosReal_Degree-statusDero.point ) <=0.01 ) 			//az reach target
    {
        QTest::qWait(250);
        OcsCom.flag_PointingByOCS_Dero=false;
        //if ( timer_replyOCS->isActive () )
        //    timer_replyOCS->stop();

        sprintf ( ExeStatus,"Done" );
        sprintf ( bExeStatus,"true" );
        CombineString ( OcsCom.StatusCUID_DeroPoint,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp2 );
    }
}

void   Derotator::HomeByOCS_Dero()
{

    QString s_temp;
    char ExeStatus[50]="Start";
    //char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    //QString DataBack="";
    on_pushButton_dero_home_clicked();

    sprintf ( Info,"OK" );

    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_DeroHome,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    SendToOCS ( s_temp );

    if ( !timer_DeroreplyOCS->isActive () )
        timer_DeroreplyOCS->start ( DeroreplyOCSClock );

}

void   Derotator::HomingByOCS_Dero()
{
    QString s_temp,s_temp1;
    QString temp1;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[50];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.Dero_Amplifier )
        sprintf ( Info,"Homing" );
    else
        sprintf ( Info,"Open Loop" );

    DataBack=QString ( "<Pos=%1>" ).arg ( statusDero.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_DeroHome,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
    if (flagtcs.Dero_Homed )
    {
        QTest::qWait(250);
        OcsCom.flag_TcsHomingDero=false;
        sprintf ( bExeStatus,"true" );
        sprintf ( ExeStatus,"Done" );
        CombineString ( OcsCom.StatusCUID_DeroHome,ExeStatus,bExeStatus,&s_temp1 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp1 );
        return;
    }
    if ( OcsCom.flag_HomeErrorDero ) 			//az reach target
    {
        QTest::qWait(250);
        OcsCom.flag_TcsHomingDero=false;
        sprintf ( bExeStatus,"true" );
        sprintf ( Info,"Fault" );
        MakeString ( OcsCom.StatusCUID_DeroHome,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        SendToOCS ( s_temp );
        QTest::qWait(250);
        sprintf ( ExeStatus,"Done" );
        CombineString ( OcsCom.StatusCUID_DeroHome,ExeStatus,bExeStatus,&s_temp1 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp1 );
        return;
    }
}


//////////////////////////////////////////
void  Derotator::EnableByOCS_M3()
{
    on_pushButton_m3_enable_clicked();
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    //if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"OK" );
    //else
        //sprintf ( Info,"Open Loop" );
    CombineString ( OcsCom.StatusCUID_M3R,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);
    sprintf ( ExeStatus, "Active" );    
    //DataBack=QString ( "<M3T=%1>" ).arg ( statusM3.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_M3R,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);
    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_M3R,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void   Derotator::DisableByOCS_M3()
{
    on_pushButton_m3_disable_clicked();
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";

    CombineString ( OcsCom.StatusCUID_M3R,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);
    sprintf ( ExeStatus, "Active" );
    //if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"OK Disable" );
    //else
        //sprintf ( Info,"Open Loop" );
    //DataBack=QString ( "<M3T=%1>" ).arg ( statusM3.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_M3R,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);
    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_M3R,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void  Derotator::CheckByOCS_M3()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    //if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"OK" );
    //else
        //sprintf ( Info,"Open Loop" );

    CombineString ( OcsCom.StatusCUID_M3R,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);
    sprintf ( ExeStatus, "Active" );
    DataBack=QString ( "<Pos=%1>" ).arg ( statusM3.PosReal_Degree,0,'f',8 );
    if(flagtcs.m3_trans_homed)DataBack+="<Homed=true>";
    else DataBack+="<Homed=false>";
    MakeString ( OcsCom.StatusCUID_M3R,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);
    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_M3R,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void   Derotator::PointByOCS_M3R()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[100];
    char bExeStatus[10]="true";
    QString DataBack="";
    //if ( flagtcs.m3_ )
        sprintf ( Info,"OK" );
    //else
       // sprintf ( Info,"Fault" );

    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_M3RPoint,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    SendToOCS ( s_temp );

    if(!flagtcs.m3_trans_homed)
    {
        QTest::qWait(250);
        OcsCom.flag_PointingByOCS_M3R=false;
        sprintf ( RV,"-1" );
        sprintf ( ExeStatus,"Active" );
        sprintf ( bExeStatus,"false" );
        sprintf ( Info,"M3 linear motor not homed. First home it, then rotate M3 mirror.");
        DataBack=QString ( "<Pos=%1>" ).arg ( statusM3Rot.PosReal_Degree,0,'f',8 );
        MakeString ( OcsCom.StatusCUID_M3RPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
        SendToOCS ( s_temp1 );
        QTest::qWait(250);
        sprintf ( RV,"1" );
        sprintf ( ExeStatus,"Done" );
        sprintf ( bExeStatus,"true" );
        CombineString ( OcsCom.StatusCUID_M3RPoint,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp2 );
        return;
    }
    qDebug() <<"m3 statusM3.point is "<<statusM3Rot.point
            << "flagtcs.m3_rot_pos_n is "<<flagtcs.m3_rot_pos_n
            <<"flagtcs.m3_rot_pos_p is " <<flagtcs.m3_rot_pos_p;
    if(statusM3Rot.point<0.5 && !flagtcs.m3_rot_pos_n)
    {
        /*if(!flagtcs.m3_rot_pos_p)
        {
            QTest::qWait(250);
            OcsCom.flag_PointingByOCS_M3R=false;
            sprintf ( ExeStatus,"Active" );
            sprintf ( bExeStatus,"false" );
            DataBack=QString ( "<Pos=%1>" ).arg ( statusM3Rot.PosReal_Degree,0,'f',8 );
            MakeString ( OcsCom.StatusCUID_M3RPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
            SendToOCS ( s_temp1 );
            QTest::qWait(250);
            sprintf ( RV, "1");
            sprintf ( ExeStatus, "Done");
            CombineString ( OcsCom.StatusCUID_M3RPoint,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
            SendToOCS ( s_temp2 );
        }
        else*/ on_pushButton_m3_auto_Pos2_clicked();
    }
    else if(statusM3Rot.point>0.5 && !flagtcs.m3_rot_pos_p)
    {
        /*if(!flagtcs.m3_rot_pos_n)
        {
            QTest::qWait(250);
            OcsCom.flag_PointingByOCS_M3R=false;
            sprintf ( ExeStatus,"Active" );
            sprintf ( bExeStatus,"false" );
            DataBack=QString ( "<Pos=%1>" ).arg ( statusM3Rot.PosReal_Degree,0,'f',8 );
            MakeString ( OcsCom.StatusCUID_M3RPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
            SendToOCS ( s_temp1 );
            QTest::qWait(250);
            sprintf ( RV, "1");
            sprintf ( ExeStatus, "Done");
            CombineString ( OcsCom.StatusCUID_M3RPoint,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
            SendToOCS ( s_temp2 );
        }
        else*/ on_pushButton_m3_auto_Pos1_clicked();
    }
    else
    {
        QTest::qWait(250);
        OcsCom.flag_PointingByOCS_M3R=false;
        //sprintf ( RV, "1");
        sprintf ( ExeStatus,"Active" );
        sprintf ( bExeStatus,"false" );
        sprintf ( Info,"Already there" );
        DataBack=QString ( "<Pos=%1>" ).arg ( statusM3Rot.PosReal_Degree,0,'f',8 );
        MakeString ( OcsCom.StatusCUID_M3RPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
        SendToOCS ( s_temp1 );
        QTest::qWait(250);
        sprintf ( RV, "1");
        sprintf ( ExeStatus, "Done");
        CombineString ( OcsCom.StatusCUID_M3RPoint,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp2 );
    }
    if ( !timer_DeroreplyOCS->isActive () )
        timer_DeroreplyOCS->start ( DeroreplyOCSClock );//start check every 5 seconds

    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
       StatusCUID:return to OCS, new time
       ExeStatus:Start , Active , Done
        RV:    0:working normally;1:warning;-1:fault
           Info:   if no error:"OK"; or,imformation of error
     bExeStatus:  start working or finish working:"true";still working:false
      DataBack:	Additional data back to OCS,eg. positon
    */
}

void   Derotator::PointingByOCS_M3R()
{
    if(!OcsCom.flag_PointingByOCS_M3R) return;
    QString s_temp,s_temp1;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[100];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"Pointing" );
    else
        sprintf ( Info,"Open Loop" );

    DataBack=QString ( "<Pos=%1>" ).arg ( statusM3Rot.PosReal_Degree,0,'f',8 );


    if(M3_AutoPos_Index==1 || M3_AutoPos_Index==-1)
    {

        sprintf ( Info,"M3 linear motor move to outside position.");
    }
    else if(M3_AutoPos_Index==2 || M3_AutoPos_Index==-2)
    {

        if(M3_trans_pos<1)sprintf ( Info,"M3 mirror is rotating.");
        else sprintf ( Info,"M3 linear motor move to inside position.");
    }
    else if(M3_AutoPos_Index>4 && M3_AutoPos_Index<36)
        sprintf ( Info,"M3 linear motor will reach end position.");
    MakeString ( OcsCom.StatusCUID_M3RPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );

    if ( (statusM3Rot.point<0.5 && flagtcs.m3_rot_pos_n) || (statusM3Rot.point>0.5 && flagtcs.m3_rot_pos_p) ) 			//az reach target
    {
        QTest::qWait(250);
        OcsCom.flag_PointingByOCS_M3R=false;
        sprintf ( ExeStatus,"Done" );
        sprintf ( bExeStatus,"true" );
        CombineString ( OcsCom.StatusCUID_M3RPoint,ExeStatus,bExeStatus,&s_temp1 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp1 );
    }
}

void   Derotator::PointStopByOCS_M3R()
{
    on_pushButton_m3_rot_stop_clicked();

    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"OK Stop" );
    else
        sprintf ( Info,"Open Loop" );

    //MakeString ( OcsCom.StatusCUID_M3R,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_M3R,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( bExeStatus,"false" );
    sprintf ( ExeStatus, "Active" );
    DataBack=QString ( "<Pos=%1>" ).arg ( statusM3Rot.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_M3R,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);

    sprintf ( bExeStatus,"true" );
    sprintf ( ExeStatus,"Done" );
    CombineString ( OcsCom.StatusCUID_M3R,ExeStatus,bExeStatus,&s_temp2 );
    SendToOCS ( s_temp2 );
}


void   Derotator::PointByOCS_M3T()
{
    char s[255];
    QDateTime datetime;
    QString s_datetime;
    QString s_temp,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Fault" );

    MakeString ( OcsCom.StatusCUID_M3TPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );

    if(statusM3.point<0.5)
    {
        statusM3.point=0;
        sprintf ( s,"%6.3f",statusM3.point );//target,
        ui->lineEdit_m3_trans_target->setText ( s );//lineEdit_dero_targetD
        on_pushButton_m3_trans_go_clicked();
    }
    else if(statusM3.point>0.5)
    {
        statusM3.point=35;
        sprintf ( s,"%6.3f",statusM3.point );//target,
        ui->lineEdit_m3_trans_target->setText ( s );//lineEdit_dero_targetD
        on_pushButton_m3_trans_go_clicked();
    }
    else
    {
        OcsCom.flag_PointingByOCS_M3T=false;
        sprintf ( RV, "1");
        sprintf ( ExeStatus, "Done");
        sprintf ( Info,"Position Error" );
        s_temp="";
        //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        CombineString ( OcsCom.StatusCUID_M3TPoint,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp2 );
    }

    if ( !timer_DeroreplyOCS->isActive () )
        timer_DeroreplyOCS->start ( DeroreplyOCSClock );//start check every 5 seconds

    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
       StatusCUID:return to OCS, new time
       ExeStatus:Start , Active , Done
        RV:    0:working normally;1:warning;-1:fault
           Info:   if no error:"OK"; or,imformation of error
     bExeStatus:  start working or finish working:"true";still working:false
      DataBack:	Additional data back to OCS,eg. positon
    */

}

void   Derotator::PointingByOCS_M3T()
{
    QString s_temp;
    QString temp1;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"Pointing" );
    else
        sprintf ( Info,"Fault" );

        DataBack=QString ( "<Pos=%1>" ).arg ( M3_trans_pos,0,'f',8 );

    if ( fabs ( M3_trans_pos-statusM3.point ) <=0.01 ) 			// reach target
    {
        OcsCom.flag_PointingByOCS_M3T=false;
        //if ( timer_replyOCS->isActive () )
        //    timer_replyOCS->stop();

        sprintf ( ExeStatus,"Done" );
        sprintf ( bExeStatus,"true" );
    }

    MakeString ( OcsCom.StatusCUID_M3TPoint,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
}

void   Derotator::PointStopByOCS_M3T()
{
    on_pushButton_m3_trans_stop_clicked();

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
    //if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"OK" );
    //else
        //sprintf ( Info,"Open Loop" );
    CombineString ( OcsCom.StatusCUID_M3T,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);
    sprintf ( ExeStatus, "Active" );
    //DataBack=QString ( "<M3T=%1>" ).arg ( statusM3.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_M3T,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);
    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_M3T,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void   Derotator::LockByOCS_M3T()
{
    QString str;
    if(flagtcs.m3_trans_brake)
    {
       SQUMAC::instance_p()->QcommCmdPMAC (LOCK_TRANS_M3, str);
    }
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    //if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"Lock" );
    //else
        //sprintf ( Info,"Open Loop" );
    CombineString ( OcsCom.StatusCUID_M3T,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);
    sprintf ( ExeStatus, "Active" );
    //DataBack=QString ( "<Pos=%1>" ).arg ( statusM3.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_M3T,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);
    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_M3T,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void   Derotator::UnLockByOCS_M3T()
{
    QString str;
    if(!flagtcs.m3_trans_brake)
    {
       SQUMAC::instance_p()->QcommCmdPMAC (UNLOCK_TRANS_M3, str);
    }
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    //if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"Unlock" );
    //else
        //sprintf ( Info,"Open Loop" );
    CombineString ( OcsCom.StatusCUID_M3T,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);
    sprintf ( ExeStatus, "Active" );
    //DataBack=QString ( "<Pos=%1>" ).arg ( statusM3.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_M3T,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );
    QTest::qWait(250);
    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_M3T,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void   Derotator::PointByOCS_FL()
{
    //char s[255];
    QDateTime datetime;
    QString s_datetime;
    QString s_temp;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[100];
    char bExeStatus[10]="true";
    QString DataBack="";
    if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"OK" );
    else
        sprintf ( Info,"Open Loop" );

    MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );

    if(Fiber_Light_pos==0)on_pushButton_fiber_Pos1_clicked();
    else if(Fiber_Light_pos==1)on_pushButton_fiber_Pos2_clicked();
    else
    {
        OcsCom.flag_PointingByOCS_FL=false;
        sprintf ( RV, "1");
        sprintf ( ExeStatus, "Done");
        sprintf ( Info,"Position Error" );
        s_temp="";
        MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
        SendToOCS ( s_temp );
    }
    if ( !timer_DeroreplyOCS->isActive () )
        timer_DeroreplyOCS->start ( DeroreplyOCSClock );//start check every 5 seconds

    /* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
       StatusCUID:return to OCS, new time
       ExeStatus:Start , Active , Done
        RV:    0:working normally;1:warning;-1:fault
           Info:   if no error:"OK"; or,imformation of error
     bExeStatus:  start working or finish working:"true";still working:false
      DataBack:	Additional data back to OCS,eg. positon
    */

}

void   Derotator::PointingByOCS_FL()
{
    QString s_temp;
    QString s_temp1;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="false";
    QString DataBack="";
    //if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"Pointing" );
    //else
     //   sprintf ( Info,"Open Loop" );

    //DataBack=QString ( "<pos=%1>" ).arg ( statusM3Rot.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_FiberLight,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );
    if ( (Fiber_Light_pos>0 && flagtcs.fiber_switch) || (Fiber_Light_pos==0 && !flagtcs.fiber_switch) ) 			//az reach target
    {
        QTest::qWait(250);
        OcsCom.flag_PointingByOCS_FL=false;
        sprintf ( ExeStatus,"Done" );
        sprintf ( bExeStatus,"true" );
        CombineString ( OcsCom.StatusCUID_FiberLight,ExeStatus,bExeStatus,&s_temp1);
        SendToOCS ( s_temp1 );
    }   
}

void   Derotator::PointStopByOCS_FL()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    //if ( flagtcs.m3_trans_amp )
    sprintf ( Info,"OK Stop" );
    //else
    //sprintf ( Info,"Open Loop" );

    OcsCom.flag_PointingByOCS_FL=false;
    OcsCom.flag_PointStopByOCS_FL=false;

    CombineString ( OcsCom.StatusCUID_FiberLight,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );
    QTest::qWait(250);

    sprintf ( bExeStatus,"false" );
    sprintf ( ExeStatus, "Active" );
    //DataBack=QString ( "<Pos=%1>" ).arg ( statusDero.PosReal_Degree,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_FiberLight,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );

    QTest::qWait(250);
    sprintf(bExeStatus,"true");
    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_FiberLight,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void   Derotator::HomeByOCS_M3()
{
    QString s_temp;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";
    on_pushButton_m3_trans_home_clicked();

    sprintf ( Info,"OK" );

    MakeString ( OcsCom.StatusCUID_M3THome,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );

    if ( !timer_DeroreplyOCS->isActive () )
        timer_DeroreplyOCS->start ( DeroreplyOCSClock );

}

void   Derotator::HomingByOCS_M3()
{
    QString s_temp,s_temp1;
    char ExeStatus[50]="Active";
    char RV[10]="0";
    char Info[100];
    char bExeStatus[10]="false";
    QString DataBack="";
    if ( flagtcs.m3_trans_amp )
        sprintf ( Info,"Homing" );
    else
        sprintf ( Info,"Fault" );

    DataBack=QString ( "<Pos=%1>" ).arg ( M3_trans_pos,0,'f',8 );
    MakeString ( OcsCom.StatusCUID_M3THome,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    SendToOCS ( s_temp );

    if (flagtcs.m3_trans_homed ) 			//az reach target
    {
        //if ( timer_replyOCS->isActive () )
        //    timer_replyOCS->stop();
        OcsCom.flag_TcsHomingM3=false;
        sprintf ( bExeStatus,"true" );
        sprintf ( ExeStatus,"Done" );
        QTest::qWait(250);
        CombineString ( OcsCom.StatusCUID_M3THome,ExeStatus,bExeStatus,&s_temp1 );//OcsCom.TrackStopUid
        SendToOCS ( s_temp1 );
    }
    /*if ( OcsCom.flag_HomeErrorM3 )
    {
        OcsCom.flag_TcsHomingM3=false;
        sprintf ( RV,"-1" );
        sprintf ( bExeStatus,"true" );
        sprintf ( ExeStatus,"Done" );
        sprintf ( Info,"Can't find M3 linear motor zero position! Check and try again." );
    }*/

}

void Derotator::OpenCoverReplyOCS()
{
    QString s_temp2,s_temp3;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[20];
    char bExeStatus[10]="true";
    QString DataBack="";

    if ( flagtcs.Mirror_Cover_left_open && flagtcs.Mirror_Cover_right_open )
        sprintf ( Info,"Cover opened" );
    else
        sprintf ( Info,"Cover open fault" );

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    MakeString ( OcsCom.StatusCUID_CoverOpen,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    SendToOCS ( s_temp2 );

    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_CoverOpen,ExeStatus,bExeStatus,&s_temp3 );
    SendToOCS ( s_temp3 );
}

void Derotator::CloseCoverReplyOCS()
{
    QString s_temp2,s_temp3;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[20];
    char bExeStatus[10]="true";
    QString DataBack="";

    if ( flagtcs.Mirror_Cover_left_close && flagtcs.Mirror_Cover_right_close )
        sprintf ( Info,"Cover closed." );
    else
        sprintf ( Info,"Cover close fault" );

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "0" );
    MakeString ( OcsCom.StatusCUID_CoverClose,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    SendToOCS ( s_temp2 );

    sprintf(ExeStatus,"Done");
    CombineString ( OcsCom.StatusCUID_CoverClose,ExeStatus,bExeStatus,&s_temp3 );
    SendToOCS ( s_temp3 );
}

void Derotator::on_pushButton_COVER_LEFT_OPEN_clicked()
{
    QString str;
    char cmd[255];
    sprintf ( cmd,"M7041=1" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    flagtcs.Mirror_Cover_stop = false;
}

void Derotator::on_pushButton_COVER_RIGHT_OPEN_clicked()
{
    QString str;
    char cmd[255];
    sprintf ( cmd,"M7040=1" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    flagtcs.Mirror_Cover_stop = false;
}

void Derotator::on_pushButton_COVER_LEFT_CLOSE_clicked()
{
    QString str;
    char cmd[255];
    sprintf ( cmd,"M7037=1" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    flagtcs.Mirror_Cover_stop = false;;
}

void Derotator::on_pushButton_COVER_RIGHT_CLOSE_clicked()
{
    QString str;
    char cmd[255];
    sprintf ( cmd,"M7036=1" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    flagtcs.Mirror_Cover_stop = false;
}

void Derotator::on_pushButton_COVER_STOP_clicked()
{
    QString str;
    char cmd[255];
    sprintf ( cmd,"M7036=0M7037=0M7040=0M7041=0" );
    SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    flagtcs.Mirror_Cover_stop = true;
    flagtcs.Mirror_Cover_left_close = false;
    flagtcs.Mirror_Cover_left_open = false;
    flagtcs.Mirror_Cover_right_close = false;
    flagtcs.Mirror_Cover_right_open = false;
}

bool Derotator::checkCover()
{
    QString str;
    char  cmd[255];
    sprintf ( cmd,"M7002M7003M7004M7005" ); //M7002:#1开，M7003：#2开，M7004：#1关，M7005：#2关。
    QStringList  list;
    int n=4;
    GetStatusFromUMAC ( cmd,n, &list );

    if ( !QString::compare ( list[0], "0" ) )
    {
        ui->label_COVER_RIGHT_COLOR->setStyleSheet("background-color:rgb(0, 170, 0)");
        ui->label_COVER_RIGHT_TEXT->setText("Right Open");
        flagtcs.Mirror_Cover_right_open = true;
        sprintf ( cmd,"M7040=0" );
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    }

    if ( !QString::compare ( list[1], "0" ) )
    {
        ui->label_COVER_LEFT_COLOR->setStyleSheet("background-color:rgb(0, 170, 0)");
        ui->label_COVER_LEFT_TEXT->setText("Left Open");
        flagtcs.Mirror_Cover_left_open = true;
        sprintf ( cmd,"M7041=0" );
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    }

    if ( !QString::compare ( list[2], "0" ) )
    {
        ui->label_COVER_RIGHT_COLOR->setStyleSheet("background-color:rgb(255, 0, 0)");
        ui->label_COVER_RIGHT_TEXT->setText("Right Close");
        flagtcs.Mirror_Cover_right_close = true;
        sprintf ( cmd,"M7036=0" );
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    }

    if ( !QString::compare ( list[3], "0" ) )
    {
        ui->label_COVER_LEFT_COLOR->setStyleSheet("background-color:rgb(255, 0, 0)");
        ui->label_COVER_LEFT_TEXT->setText("Left Close");
        flagtcs.Mirror_Cover_left_close = true;
        sprintf ( cmd,"M7037=0" );
        SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
    }

    if( !flagtcs.Mirror_Cover_stop )
    {
        if( !QString::compare ( list[0], "1" ) && !QString::compare ( list[2], "1" ) )
        {
                ui->label_COVER_RIGHT_COLOR->setStyleSheet("background-color:rgb(255, 255, 0)");
                ui->label_COVER_RIGHT_TEXT->setText("Running...");
                flagtcs.Mirror_Cover_right_close = false;
                flagtcs.Mirror_Cover_right_open = false;
        }
    }
    else
    {
        ui->label_COVER_RIGHT_COLOR->setStyleSheet("background-color:rgb(100, 100, 100)");
        ui->label_COVER_RIGHT_TEXT->setText("Right Stop");
    }

    if( !flagtcs.Mirror_Cover_stop )
    {
        if( !QString::compare ( list[1], "1" ) && !QString::compare ( list[3], "1" ) )
        {
            ui->label_COVER_LEFT_COLOR->setStyleSheet("background-color:rgb(255, 255, 0)");
            ui->label_COVER_LEFT_TEXT->setText("Running...");
            flagtcs.Mirror_Cover_left_close = false;
            flagtcs.Mirror_Cover_left_open = false;
        }
    }
    else
    {
        ui->label_COVER_LEFT_COLOR->setStyleSheet("background-color:rgb(100, 100, 100)");
        ui->label_COVER_LEFT_TEXT->setText("Left Stop");
    }
}

void Derotator::GetStatusFromUMAC ( char cmd[],int n,QStringList* Status )
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

void Derotator::on_pushButton_COVER_OPEN_clicked()
{
    flagtcs.Mirror_Cover_stop = false;
    on_pushButton_COVER_LEFT_OPEN_clicked();

    QTime dieTime;
    dieTime = QTime::currentTime().addMSecs(5000);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );

    on_pushButton_COVER_RIGHT_OPEN_clicked();
}

void Derotator::on_pushButton_COVER_CLOSE_clicked()
{
    flagtcs.Mirror_Cover_stop = false;
    on_pushButton_COVER_RIGHT_CLOSE_clicked();

    QTime dieTime;
    dieTime = QTime::currentTime().addMSecs(5000);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );

    on_pushButton_COVER_LEFT_CLOSE_clicked();
}

void Derotator::on_pushButton_fiber_step_P_clicked()
{
    QString str;
    char cmd[255];
    float fiberStep = ui->lineEdit_fiber_step_set_value->text().toFloat();
    sprintf ( cmd,"#8J:%f", fiberStep);
    SQUMAC::instance_p()->QcommCmdPMAC(SET_FIBER_SWITCH_OFF, str);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}
void Derotator::on_pushButton_fiber_step_N_clicked()
{
    QString str;
    char cmd[255];
    float fiberStep =ui->lineEdit_fiber_step_set_value->text().toFloat();
    sprintf ( cmd,"#8J:-%f", fiberStep);
    SQUMAC::instance_p()->QcommCmdPMAC(SET_FIBER_SWITCH_OFF, str);
    SQUMAC::instance_p()->QcommCmdPMAC(cmd, str);
}

void Derotator::on_pushButton_fiber_stop_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC("#8K", str);
}

void Derotator::on_pushButton_fiber_brake_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC("M7025=1", str);
}

void Derotator::on_pushButton_fiber_release_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC("M7025=0", str);
}

void Derotator::on_pushButton_CCD_IN_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC("enable plc15", str);
}

void Derotator::on_pushButton_CCD_OUT_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC("enable plc16", str);
}
bool Derotator::checkDiskLock()
{
    char  cmd[255];
    sprintf(cmd, "M721M722M621M622");
    QStringList  list;
    int n = 4;
    getStatusFromUMAC(cmd, n, &list);

    if(QString::compare(list[0], "0")==0 &&
       QString::compare(list[1], "0")!=0 )
    {
        ui->label_CCD_IN->setStyleSheet("background-color:rgb(0, 170, 0)");    //yellow rgb(255, 255, 0)
        ui->label_CCD_IN->setText("IN");
        ui->label_CCD_OUT->setStyleSheet("background-color:rgb(170, 0, 0)");    //yellow rgb(255, 255, 0)
        ui->label_CCD_OUT->setText("OUT");
    }
    else if(QString::compare(list[0], "1")==0 &&
            QString::compare(list[1], "1")==0 )
    {
        ui->label_CCD_IN->setStyleSheet("background-color:rgb(170, 170, 0)");    //yellow rgb(255, 255, 0)
        ui->label_CCD_IN->setText("Mid");
        ui->label_CCD_OUT->setStyleSheet("background-color:rgb(170, 170, 0)");    //yellow rgb(255, 255, 0)
        ui->label_CCD_OUT->setText("Mid");
    }
    else  if(QString::compare(list[0], "0")!=0 &&
             QString::compare(list[1], "0")==0)
    {
        ui->label_CCD_OUT->setStyleSheet("background-color:rgb(170, 0, 0)");    //yellow rgb(255, 255, 0)
        ui->label_CCD_OUT->setText("IN");
        ui->label_CCD_IN->setStyleSheet("background-color:rgb(0, 170, 0)");    //yellow rgb(255, 255, 0)
        ui->label_CCD_IN->setText("OUT");
    }
    else  if(QString::compare(list[0], "0")==0 &&
             QString::compare(list[1], "0")==0)
    {
        ui->label_CCD_OUT->setStyleSheet("background-color:rgb(170, 170, 170)");    //yellow rgb(255, 255, 0)
        ui->label_CCD_OUT->setText("Err");
        ui->label_CCD_IN->setStyleSheet("background-color:rgb(170, 170, 170)");    //yellow rgb(255, 255, 0)
        ui->label_CCD_IN->setText("Err");
    }
 ///////////////////////////////////////////////////////////////////
    if(QString::compare(list[2], "0")==0 &&
       QString::compare(list[3], "0")!=0 )
    {
        ui->label_HS_IN->setStyleSheet("background-color:rgb(0, 170, 0)");    //yellow rgb(255, 255, 0)
        ui->label_HS_IN->setText("IN");
        ui->label_HS_OUT->setStyleSheet("background-color:rgb(170, 0, 0)");    //yellow rgb(255, 255, 0)
        ui->label_HS_OUT->setText("OUT");
    }
    else if(QString::compare(list[2], "1")==0 &&
            QString::compare(list[3], "1")==0 )
    {
        ui->label_HS_IN->setStyleSheet("background-color:rgb(170, 170, 0)");    //yellow rgb(255, 255, 0)
        ui->label_HS_IN->setText("Mid");
        ui->label_HS_OUT->setStyleSheet("background-color:rgb(170, 170, 0)");    //yellow rgb(255, 255, 0)
        ui->label_HS_OUT->setText("Mid");
    }
    else  if(QString::compare(list[2], "0")!=0 &&
             QString::compare(list[3], "0")==0)
    {
        ui->label_HS_OUT->setStyleSheet("background-color:rgb(170, 0, 0)");    //yellow rgb(255, 255, 0)
        ui->label_HS_OUT->setText("IN");
        ui->label_HS_IN->setStyleSheet("background-color:rgb(0, 170, 0)");    //yellow rgb(255, 255, 0)
        ui->label_HS_IN->setText("OUT");
    }
    else  if(QString::compare(list[2], "0")==0 &&
             QString::compare(list[3], "0")==0)
    {
        ui->label_HS_OUT->setStyleSheet("background-color:rgb(170, 170, 170)");    //yellow rgb(255, 255, 0)
        ui->label_HS_OUT->setText("Err");
        ui->label_HS_IN->setStyleSheet("background-color:rgb(170, 170, 170)");    //yellow rgb(255, 255, 0)
        ui->label_HS_IN->setText("Err");
    }
}

void Derotator::on_pushButton_HS_IN_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC("enable plc17", str);
}

void Derotator::on_pushButton_HS_OUT_clicked()
{
    QString str;
    SQUMAC::instance_p()->QcommCmdPMAC("enable plc18", str);
}
