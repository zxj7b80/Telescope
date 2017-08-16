//main_window.h

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui>
#include "ui_telescope.h"
#include <memory>		//for auto_ptr,not used yet by now.
#include <QTcpServer>
#include "direct_azalt.h"
#include "writelog.h"
#include "adc.h"
#include "dialog_m2.h"
//by zxj-----
#include "plottersetting.h"

extern bool UMAC_ETHERNET;
class UMAC;
class ADC;
class DirectAzalt;
class Dialog_m2;
class Derotator;
class CSendMessageTCS;
class SimulationOCS;
class Setup;
class Server;
class Tracking;
class Terminal;

class MainWindow:public QMainWindow,private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow ( QWidget *parent=0 );
    virtual ~MainWindow();

    ADC *adc;
    DirectAzalt *direct_azalt;
    Derotator *derotator;
    Dialog_m2  *dialog_m2;
    Setup *setup;
    SimulationOCS *simulationOcs;
    Tracking *tracking;
    Terminal *terminal;

    //DirectDero *direct_dero;
    //OCS *ocs;
    //std::auto_ptr<DirectAzalt> direct_azalt;//don't mix smart_ptr and qt's memory manage.
    //DirectM3 *direct_m3;
    //DirectM2 *direct_m2;
    //DiagDero *diag_dero;
    //DiagAzalt *diag_azalt;
    //ServiceThread *service;

    Server *server;
    QTimer *timer;
    QStandardItemModel *model_tracking;

    void refreshNettable();
    QString cmd;
    void updateInterface ( QHash<QString,QString> );
    void showInfoOCS ( QString );
    void StartTcpServer();
    //void StartTcpServerOCS();
    //by zxj-----
    plottersetting mplottersetting;//use point, then .show, will flash, why?
    plottersetting *pointerplottersetting;


protected:
    virtual void closeEvent ( QCloseEvent *event );

public slots:
    void startAzalt();
    void startDero();
    void startADC();
    void startM2();
    void startTracking ();
    void startTerminal();
    void startTerminal(char* Jog);
    void startSetup();
    void startSimulationOCS();//simulation of OCS

    void on_actionWindows_triggered();
    void on_actionMotif_triggered();
    void on_actionGtk_triggered();
    void on_actionCDE_triggered();

    void showdata_main ( /*QHash<QString,QString> info_track*/ );//timer0
    void receiveMessage();

    //void startDiagDero();
    //void startDiagAzalt();
    //void  OFF();
    //void  ON();
    //void  AltLock();
    //void  AltUnLock();
    //QByteArray readDataIntoBuffer ( int maxSize );
    //void slotReadMessage();
    //void slotDisplayError(QAbstractSocket::SocketError socketError);
    //void slotReadMessage();
    //int readDataIntoBuffer(int maxSize);

private slots:
    //void on_actionDero_triggered();
    //void on_actionM2_triggered();
    void on_actionPloter_triggered();

signals:
    void startAzaltOCS();
    void startTrackingOCS();
    //by zxj---
    void JogFromAndroid();

private:
    writelog *writeLog;
    quint16 m_BlockSize;

    int getsubstr ( char *sourcestr, char *targetstr, int i_begin, int i_length );
    int strtrim ( char *sourcestr, char *targetstr );
    int strrtrim ( char *sourcestr, char *targetstr );
    int pr_get_commandvalue ( char *command, char *item, char *value );
    bool pr_get_guidedata ( char *command, int *time, double *AzGuide,  double *AltGuide );
    bool ParseOCSCommand ( char* pStr,char *newUID,char *pCmdCUID );//Mount分析TCS发来的命令

    char  buf_Az[128],buf_Alt[128],buf_Dero[128];
    QTcpServer *TCPServerOCS;
    QTcpSocket *clientConnection;

    void iniNetwork();
    void iniTableWidget();
    void iniTableTracking();
    void iniDockWidget();

    void MakeString(char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp);
    void CombineString ( char StatusCUID[100],char ExeStatus[50],char bExeStatus[10],QString* s_temp );

    void SendToOCS(QString str);
    void SendEmergenStopToOCS();
    void SendTrackingToOCS();

    void TcsEnableAz();
    void TcsDisableAz();
    void TcsCheckAz();
    void TcsPointAz(char* pStr);
    void TcsStopAz();
    void TcsHomeAz();

    void TcsEnableAlt();
    void TcsDisableAlt();
    void TcsCheckAlt();
    void TcsPointAlt(char* pStr);
    void TcsStopAlt();
    void TcsHomeAlt();
    void TcsAltLock();
    void TcsAltUnlock();

    void TcsEnableDero();
    void TcsDisableDero();
    void TcsCheckDero();
    void TcsPointDero(char* pStr);
    void TcsStopDero();
    void TcsHomeDero();
    void TcsCoverOpen();
    void TcsCoverClose();
    void TcsCoverStop();

    void TcsEnableM3();
    void TcsDisableM3();
    void TcsCheckM3();
    void TcsPointM3T(char* pStr);
    void TcsStopM3T();
    void TcsHomeM3T();
    void TcsLockM3T();
    void TcsUnLockM3T();
    void TcsPointM3R(char* pStr);
    void TcsStopM3R();
    void TcsPointFL(char* pStr);
    void TcsStopFL();

    void TcsEnableADC1();
    void TcsDisableADC1();
    void TcsCheckADC1();
    void TcsPointADC1(char* pStr);
    void TcsStopADC1();
    void TcsHomeADC1();

    void TcsEnableADC2();
    void TcsDisableADC2();
    void TcsCheckADC2();
    void TcsPointADC2(char* pStr);
    void TcsStopADC2();
    void TcsHomeADC2();

    void TcsStartTracking();
    void TcsStopTracking();
    void TcsStopTracking(char* pStr);
    void TcsGetPosition();
    void AutoGuiding(char* pStr);

    //by zxj---
    void TcsStartTerminal(char* pStr);

    void mousePressEvent(QMouseEvent *event);
};

class MyApplication: public QApplication
{
    Q_OBJECT
public:
    MyApplication ( int argc, char *argv[] ) : QApplication ( argc, argv ) {}
    bool notify ( QObject *receiver, QEvent *event )
    {
        try
        {
            return QApplication::notify ( receiver, event );
        }
        catch ( QString excepts )
        {

            if(UMAC_ETHERNET)
            {
                QMessageBox mb ( QMessageBox::Warning,
                                 tr ( "About to close! (MyApplication)" ),
                                 tr ( ( excepts+"\n PLease close this window,check your connection to UMAC and open it again." ).toAscii() ),
                                 //						 tr ( "About to close" ),
                                 QMessageBox::Ok );

                UMAC_ETHERNET=false;
                qDebug()<<"UMAC_ETHERNET=false";
                if ( mb.exec() ==QMessageBox::Ok )
                    return false;
            }
        }
        catch ( ... )
        {
            QMessageBox mb ( QMessageBox::Critical,
                             tr ( "About to close!!!  " ),
                             tr ( "Unknown excepts occurs.\n The app will quit now." ),
                             //						 tr ( "About to close" ),
                             QMessageBox::Ok );
            if ( mb.exec() ==QMessageBox::Ok )
            {
                quit();
            }
        }
    }
};
#endif
