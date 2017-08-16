//simulationOCS.h
#ifndef SIMULATIONOCS_H
#define SIMULATIONOCS_H
#include <QtGui>
#include "ui_simulationOCS.h"
#include <QtNetwork>
#include "signs.h"

#define OCS_SELFCHECK       "2.0.0.1.1"
class ServerSimOCS;
class MainWindow;

class SimulationOCS:public QWidget,private Ui::SimulationOCS
{
    Q_OBJECT
public:
    SimulationOCS ( MainWindow *parent );
    virtual ~SimulationOCS();
    MainWindow *m_parent;
    ServerSimOCS *serverocs;

private:
    //		QTcpSocket *tcp_socket;
    QTimer *timer;
    int isPackageBroken(QDataStream&);
    void SendOutCommand(QString hash);
    void iniTableWidget();
    void iniTableWidget_OCSout();
    void iniNetwork();
    void MakeCommandString(char CmdCUID[100],QString DataCmd,QString *hash);
    void StartOcsTcpServer();
    void refreshOCSouttable (QString str);

private slots:
    void showConnected();
    void showDisconnected();
    void on_pushButton_connect_clicked();
    void on_pushButton_ocs_selfcheck_clicked();

    void on_pushButton_home_az_clicked();
    void on_pushButton_point_az_clicked();
    void on_pushButton_stop_az_clicked();

    void on_pushButton_home_alt_clicked();
    void on_pushButton_point_alt_clicked();
    void on_pushButton_stop_alt_clicked();

    void on_pushButton_home_dero_clicked();
    void on_pushButton_start_dero_clicked();
    void on_pushButton_stop_dero_clicked();

    void on_pushButton_start_track_clicked();
    void on_pushButton_stop_track_clicked();

    void on_pushButton_guidingStar_clicked();

    void on_pushButton_check_status_clicked();

    void readData();
    void showError(QAbstractSocket::SocketError socketError);
    void refreshOCStable (QString str);
};
#endif


