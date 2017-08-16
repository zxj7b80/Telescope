
//simulationOCS.cpp
//OCS(simulation) IP and Port are defined on the screen,don't care config.ini
//Mount  IP and Port are defined on the screen,don't care config.ini

#include "simulationOCS.h"
#include "signs.h"
#include "server_simOCS.h"
#include "sendthread.h"
#include "SimOCSserver.h"

#define  Column		2
#define  Row		2
#define  tableWidth     1200
#define  tableHeight	100

#define	column_TableOCSin_1	0
#define	column_TableOCSin_2	1
#define width_OCSinTable_1	1000
#define width_OCSinTable_2	170

#define	column_TableOCSout_1	0
#define	column_TableOCSout_2	1
#define width_OCSoutTable_1	1000
#define width_OCSoutTable_2	170

extern int OcsPort;							//Port of OCS (TCP/IP),simulation
extern QString OcsIP; 							//IP of OCS(TCP/IP),simulation
extern int LocalhostPort;						//Port of Mount(Az/Alt/Dero)
extern QString LocalhostIP; 						//IP of Mount(Az/Alt/Dero)

long rowOCS=0;
long rowOCSout=0;
extern QString CommandToMount;
extern FlagTCS flagtcs;
SimulationOCS::SimulationOCS ( MainWindow *parent )
{
    m_parent = parent;

    setupUi(this);
    
    //	tcp_socket = new QTcpSocket ( this );
    //connect( tcp_socket,SIGNAL(connected()),this,SLOT(showConnected()));
    //	connect( tcp_socket,SIGNAL(disconnected()),this,SLOT(showDisconnected()));
    //connect ( tcp_socket, SIGNAL ( readyRead() ), this, SLOT ( readData() ) );
    //	connect ( tcp_socket, SIGNAL ( error ( QAbstractSocket::SocketError ) ),this, SLOT ( showError ( QAbstractSocket::SocketError ) ) );
    iniTableWidget();
    iniTableWidget_OCSout();
    //StartOcsTcpServer();
    iniNetwork();
    flagtcs.IsOpened_simulationOCS=true;
    pushButton_home_az->setEnabled(true);
    pushButton_point_az->setEnabled(true);
    pushButton_stop_az->setEnabled(true);
    pushButton_home_alt->setEnabled(true);
    pushButton_point_alt->setEnabled(true);
    pushButton_stop_alt->setEnabled(true);
    pushButton_home_dero->setEnabled(true);
    pushButton_start_dero->setEnabled(true);
    pushButton_stop_dero->setEnabled(true);
    pushButton_start_track->setEnabled(true);
    pushButton_stop_track->setEnabled(true);
    //	pushButton_connect->setEnabled(false);
    pushButton_guidingStar->setEnabled(true);
}

SimulationOCS::~SimulationOCS()
{
    flagtcs.IsOpened_simulationOCS=false;
    qDebug() <<"Close simulation OCS";
}
void SimulationOCS::iniNetwork()
{
    //OcsIP=SMySetting::instance_p()->value ( "misc/OcsIP" ).toString();
    qDebug() << "simulation OCS IP" << OcsIP <<" OCS Port"<<OcsPort;

    //lineEdit_simOCSport->text().toInt();
    StartOcsTcpServer();		//OCS sever simulation
}

void SimulationOCS::iniTableWidget()//show received data
{

    ///////////////table setup////////////////////////
    //tableWidget_OCS->setColumnWidth ( column, width );
    tableWidget_OCS->setColumnCount ( Column );
    tableWidget_OCS->setRowCount ( Row );
    tableWidget_OCS->setSortingEnabled ( false );
    QStringList labels;
    labels << tr ( "Received Data (Ethernet) " ) << tr ( "Time" );
    tableWidget_OCS->setHorizontalHeaderLabels ( labels );
    tableWidget_OCS->resize(tableWidth, tableHeight);
    tableWidget_OCS->setColumnWidth ( column_TableOCSin_1, width_OCSinTable_1 );		//1# column width
    tableWidget_OCS->setColumnWidth ( column_TableOCSin_2, width_OCSinTable_2 );		//2# column width
    //tableWidget_OCS->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //tableWidget_OCS->verticalHeader()->setResizeMode(QHeaderView::Stretch);

}

void SimulationOCS::iniTableWidget_OCSout()//show out data
{
    ///////////////table setup////////////////////////
    //tableWidget_OCS->setColumnWidth ( column, width );
    tableWidget_OCSout->setColumnCount ( Column );
    tableWidget_OCSout->setRowCount ( Row );
    tableWidget_OCSout->setSortingEnabled ( false );
    QStringList labels;
    labels << tr ( "Send Out Data (Ethernet) " ) << tr ( "Time" );
    tableWidget_OCSout->setHorizontalHeaderLabels ( labels );
    tableWidget_OCSout->resize(tableWidth, tableHeight);
    tableWidget_OCSout->setColumnWidth ( column_TableOCSout_1, width_OCSoutTable_1 );		//1# column width
    tableWidget_OCSout->setColumnWidth ( column_TableOCSout_2, width_OCSoutTable_2 );		//2# column width

    rowOCSout=0;
}
int SimulationOCS::isPackageBroken(QDataStream& in)
{
    // 	quint16 block_size;
    // 	if ( tcp_socket->bytesAvailable() < ( int ) sizeof ( quint16 ) )//You must make sure that enough data is available before attempting to read it using operator>>().
    // 			return 1;
    // 	in >> block_size;
    // 	if ( tcp_socket->bytesAvailable() < block_size )
    // 		return 1;//some error happen here.
    return 0;
}

void SimulationOCS::showConnected()
{
    label_status->setText("Net Connected.");
    pushButton_home_az->setEnabled(true);
    pushButton_point_az->setEnabled(true);
    pushButton_stop_az->setEnabled(true);
    pushButton_home_alt->setEnabled(true);
    pushButton_point_alt->setEnabled(true);
    pushButton_stop_alt->setEnabled(true);
    pushButton_home_dero->setEnabled(true);
    pushButton_start_dero->setEnabled(true);
    pushButton_stop_dero->setEnabled(true);
    pushButton_start_track->setEnabled(true);
    pushButton_stop_track->setEnabled(true);
    pushButton_connect->setEnabled(false);
    pushButton_guidingStar->setEnabled(true);
}


void SimulationOCS::showDisconnected()
{
    label_status->setText("Net Disconnected.");
    // 	pushButton_check_az->setEnabled(false);
    // 	pushButton_point_az->setEnabled(false);
    // 	pushButton_stop_az->setEnabled(false);
    // 	pushButton_check_alt->setEnabled(false);
    // 	pushButton_point_alt->setEnabled(false);
    // 	pushButton_stop_alt->setEnabled(false);
    // 	pushButton_check_dero->setEnabled(false);
    // 	pushButton_start_dero->setEnabled(false);
    // 	pushButton_stop_dero->setEnabled(false);
    // 	pushButton_start_track->setEnabled(false);
    // 	pushButton_stop_track->setEnabled(false);
    // 	pushButton_connect->setEnabled(true);
}

void SimulationOCS::readData()
{
    // 	QDataStream in ( tcp_socket/*,QIODevice::ReadOnly */);
    // 	in.setVersion ( QDataStream::Qt_4_0 );
    // 	QString str;
    // 	in >> str;
    // 	refreshOCStable(str);
}


void SimulationOCS::refreshOCSouttable (  QString str)
{
    QDateTime dt=QDateTime::currentDateTime();
    QTime time=dt.time();
    QString dtstr;
    dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );

    if ( rowOCSout>(Row-1))
    {tableWidget_OCSout->insertRow ( rowOCSout );}

    QTableWidgetItem *OcsOutItem0 = new QTableWidgetItem (str );
    tableWidget_OCSout->setItem ( rowOCSout, 0, OcsOutItem0 );
    QTableWidgetItem *OcsOutItem1 = new QTableWidgetItem ( dtstr );
    tableWidget_OCSout->setItem ( rowOCSout, 1, OcsOutItem1 );
    rowOCSout++;
    tableWidget_OCSout->scrollToItem ( OcsOutItem0 );
}

void SimulationOCS::refreshOCStable (  QString str)
{
    QDateTime dt=QDateTime::currentDateTime();
    QTime time=dt.time();
    QString dtstr;
    dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );

    if ( rowOCS>(Row-1))
        tableWidget_OCS->insertRow ( rowOCS );

    QTableWidgetItem *NetItem0 = new QTableWidgetItem (str );
    tableWidget_OCS->setItem ( rowOCS, 0, NetItem0 );
    QTableWidgetItem *NetItem1 = new QTableWidgetItem ( dtstr );
    tableWidget_OCS->setItem ( rowOCS, 1, NetItem1 );
    rowOCS++;
    tableWidget_OCS->scrollToItem ( NetItem0 );
}


void SimulationOCS::on_pushButton_connect_clicked()
{/*
 tcp_socket->connectToHost ( comboBox_address->currentText(),
                            lineEdit_port->text().toInt() );//by default Read*/
    //qDebug()<<"ConnectServer"<<MountIP<<MountPort;
    //	tcp_socket->connectToHost (MountIP,MountPort);
    //tcp_socket->close();
}

void SimulationOCS::on_pushButton_ocs_selfcheck_clicked()
{
    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,OCS_SELFCHECK);
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_home_az_clicked()
{
    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,TCSCHECKAZ);
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_point_az_clicked() //2.1.0.2.1TcsPointing
{
    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,"%s",TCSPOINTAZ);
    DataCmd=QString ( "<Pos=%1>" ).arg ( lineEdit_Az_point->text().toFloat() );
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_stop_az_clicked()
{
    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,TCSSTOPAZ);
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_home_alt_clicked()
{
    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,TCSHOMEALT);
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_point_alt_clicked()
{
    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,"%s",TCSPOINTALT);
    DataCmd=QString ( "<Pos=%1>" ).arg ( lineEdit_Alt_point->text().toFloat() );
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_stop_alt_clicked()
{
    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,TCSSTOPALT);
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_home_dero_clicked()
{
    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,TCSHOMEDERO);
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_start_dero_clicked()
{
    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,"%s",TCSPOINTDERO);
    DataCmd=QString ( "<Pos=%1>" ).arg ( lineEdit_Dero_point->text().toFloat() );
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_stop_dero_clicked()
{

    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,TCSCHECKDERO);//TCSSTOPDERO
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_check_status_clicked()
{
    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,TCSGETPOSITION);
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_start_track_clicked()
{

    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,TCSSTARTTRACKING);
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
}


//pushButton_guidingStar
void SimulationOCS::on_pushButton_guidingStar_clicked()
{
    QString hash;
    // 	char StatusCUID[100];
    // 	QString DataCmd="";
    //  	sprintf(StatusCUID,"%s",AUTOGUIDING);
    //  	DataCmd=QString ( "<Azimuth=%1><Altitude=%2><Derotator=%3>" ).arg ( lineEdit_Az_guiding->text().toFloat() ).arg ( lineEdit_Alt_guiding->text().toFloat() ).arg ( lineEdit_Dero_guiding->text().toFloat() );
    // 	MakeCommandString(StatusCUID,DataCmd,&hash);
    hash ="guide 163221 +123.45 -67.89";
    SendOutCommand(hash);
}

void SimulationOCS::on_pushButton_stop_track_clicked()
{
    QString hash;
    char StatusCUID[100];
    QString DataCmd="";
    sprintf(StatusCUID,TCSSTOPTRACKING);
    MakeCommandString(StatusCUID,DataCmd,&hash);
    SendOutCommand(hash);
    //qDebug()<<hash;
}

// void SimulationOCS::SendOutCommand(QString hash)
// {
//        	refreshOCSouttable (hash);
//  	serverocs->ConnectServer(hash); 
//  	CommandToMount=hash;
// }

void SimulationOCS::SendOutCommand(QString hash)
{
    refreshOCSouttable (hash);
    sendthread *send = new sendthread(m_parent,hash,LocalhostIP,LocalhostPort);
//    sendthread *send = new sendthread(m_parent,hash,OcsIP,OcsPort);
    send->run();
}


/* CmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4

  DataCmd:	Additional data ,eg. positon
*/
void SimulationOCS::MakeCommandString(char CmdCUID[100],QString DataCmd,QString* str)       // hash = "<CmdCUID=2.4.0.2.1.20100701.123456.4>";
{
    QDateTime datetime;
    QString s_datetime;

    datetime = QDateTime::currentDateTime();
    s_datetime = datetime.toString ( "yyyyMMdd.hhmmss.zzz" );//RelatedCmdCUID
    (*str) =QString( "<CmdCUID=%1." ).arg( CmdCUID ) + s_datetime + QString( ">" ) + DataCmd;
    qDebug()<<*str;
}


void SimulationOCS::showError ( QAbstractSocket::SocketError socketError )
{
    // 	switch ( socketError )
    // 	{
    // 		case QAbstractSocket::RemoteHostClosedError:
    // 			break;
    // 		case QAbstractSocket::HostNotFoundError:
    // 			QMessageBox::information ( this, tr ( "OCS Client" ),
    // 			                           tr ( "The host of Mount was not found. Please check the "
    // 			                                "host name and port settings." ) );
    // 			break;
    // 		case QAbstractSocket::ConnectionRefusedError:
    // 			QMessageBox::information ( this, tr ( "OCS Client" ),
    // 			                           tr ( "The connection was refused by the peer. "
    // 			                                "Make sure the Mount server is running, "
    // 			                                "and check that the host name and port "
    // 			                                "settings are correct." ) );
    // 			break;
    // 		default:
    // 			QMessageBox::information ( this, tr ( "OCS Client" ),
    // 			                           tr ( "The following error occurred: %1." )
    // 			                           .arg ( tcp_socket->errorString() ) );
    // 	}
}


void SimulationOCS::StartOcsTcpServer()
{
    // 	serverocs = new ServerSimOCS ( this );
    // 	if ( !serverocs->listen ( QHostAddress::Any, OcsPort ) )
    // 	{
    // 		qDebug ( "OCS Server start error!" );
    //
    // 		return;
    // 	}
    SimOcsServer *socketserver;
    socketserver = new SimOcsServer(this);
    if (!socketserver->listen(QHostAddress::Any, OcsPort))//OcsIP=159.226.75.214,OcsPort=5002
    {
        QMessageBox::critical(this, tr("Threaded Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(socketserver->errorString()));
        close();
        return;
    }

    //qDebug()<<"OCS Port"<<OcsPort;
}
