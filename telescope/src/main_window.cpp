//main_window.cpp

#include "main_window.h"
#include "direct_azalt.h"
#include "tracking.h"
#include "terminal.h"
#include "UMAC.h"
#include "setup.h"
#include "signs.h"
#include "simulationOCS.h"
#include "fortuneserver.h"
#include "sendthread.h"
#include "writelog.h"
#include "checkmessage.h"
#include "dialog_m2.h"
#include "piconnect.h"
#include "derotator.h"
#include "adc.h"

///////          NetTable                  //////////////
#define  Column_NetTable 		2
#define  Row_NetTable			10
#define  tableWidth_NetTable    	400
#define  tableHeight_NetTable		200
#define  column_NetTable_1		0
#define  width_NetTable_1		900
#define  column_NetTable_2		1
#define  width_NetTable_2		450
//dockWidget////////////////
#define dockWidgetMaxWidth		1440
#define dockWidgetMaxHeight		900
#define dockWidgetMinWidth		1440
#define dockWidgetMinHeight		0

//TableTracking////////////////
#define TableTrackingColumn0		0
#define TableTrackingWidth_col0		160
#define TableTrackingWidth		130
#define Column_TableTracking		10
#define Row_TableTracking		10

#define LONG_MESSAGE_LENGTH 512					//#define MaxBufferSize 1024

extern TrackStar trackstar;
int TableNum=36000;						//star table lines,  hours

QTimer *timer0;
long nConnection_old=0;						//connection number
long nConnection=0;						//last  connection number
QTimer *timer00;
long rowTrack=0;
long rowNet=0;
//bool IsOpened_direct_azalt=false;
FlagTCS flagtcs;

int OcsPort;							//Port of OCS (TCP/IP)
QString OcsIP; 							//IP of OCS(TCP/IP)
int LocalhostPort;						//Port of Mount(Az/Alt/Dero),simulation
QString LocalhostIP; 						//IP of Mount(Az/Alt/Dero),simulation

//double az_guidedata=0;

double alt_guidedata=0;
double dero_guidedata=0;
extern double  Fiber_Light_pos;
extern sAxis  	s_Az,s_Alt,statusDero, statusM3,  statusM3Rot, statusADC1, statusADC2;						//structure of AZ
//structure of ALT
extern OcsCOM   OcsCom;						//structure of comminication with OCS/TCS

MainWindow::MainWindow ( QWidget *parent ) :QMainWindow ( parent )
{
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );//very important to avoid memory leak

    direct_azalt = NULL;
    tracking = NULL;
    //	timer = new QTimer;

    SMySetting::instance_p()->beginGroup ( "main_window" );
    resize ( SMySetting::instance_p()->value ( "size" ).toSize() );
    move ( SMySetting::instance_p()->value ( "pos" ).toPoint() );
    SMySetting::instance_p()->endGroup();

    float ObT;//observation time
    ObT= SMySetting::instance_p()->value ( "misc/star_len_h" ).toDouble() *3600
            +SMySetting::instance_p()->value ( "misc/star_len_m" ).toInt() *60.0
            +SMySetting::instance_p()->value ( "misc/star_len_s" ).toDouble();
    TableNum=int ( ObT*10 );

    writelog *writeLog = new writelog();
    writeLog->createlogfile();
    writeLog->writeregular("Mount start","	Mount start");

    connect ( this,SIGNAL ( startAzaltOCS() ), actionAz_Alt,SIGNAL ( triggered() ) );
    connect ( actionAz_Alt,SIGNAL ( triggered() ),this,SLOT ( startAzalt() ) );

    connect ( this,SIGNAL ( startTrackingOCS() ), actionTracking,SIGNAL ( triggered() ) );
    connect ( actionTracking,SIGNAL ( triggered() ),this,SLOT ( startTracking() ) );

    connect ( actionDero,SIGNAL ( triggered() ),this,SLOT ( startDero() ) );
    connect ( actionADC, SIGNAL(triggered()), this, SLOT(startADC()));
    connect ( actionM2,SIGNAL ( triggered() ),this,SLOT ( startM2() ) );
    connect ( actionSetup,SIGNAL ( triggered() ),this,SLOT ( startSetup() ) );
    connect ( actionSimulationOCS,SIGNAL ( triggered() ),this,SLOT ( startSimulationOCS() ) );
    connect ( actionTerminal,SIGNAL ( triggered() ),this,SLOT ( startTerminal() ) );
    //connect ( action_Derotator_Diagnosis,SIGNAL ( triggered() ),this,SLOT ( startDiagDero() ) );
    //connect ( actionAz_Alt_D,SIGNAL ( triggered() ),this,SLOT ( startDiagAzalt() ) );
    //connect ( actionOFF,SIGNAL ( triggered() ),this,SLOT ( OFF() ) );
    //connect ( actionON,SIGNAL ( triggered() ),thDialog_m2is,SLOT ( ON() ) );
    //connect ( actionALT_UNLOCK,SIGNAL ( triggered() ),this,SLOT ( AltUnLock() ) );
    //connect ( actionALT_LOCK,SIGNAL ( triggered() ),this,SLOT ( AltLock() ) );

    iniTableWidget();
    iniTableTracking();
    iniNetwork();

    timer00=new QTimer ( this );
    timer00->connect ( timer00, SIGNAL ( timeout() ), this,SLOT ( showdata_main() ) );
    //timer00->start ( 2000 );

    flagtcs.IsOpened_azalt=false;
    flagtcs.IsOpened_dero=false;
    flagtcs.IsOpened_adc=false;
    flagtcs.IsOpened_m2=false;
    flagtcs.IsOpened_tracking=false;
    flagtcs.IsOpened_setup=false;
    flagtcs.IsOpened_simulationOCS=false;
    flagtcs.IsOpened_terminal=false;

    OcsCom.flag_TcsEmergenStop_AzAlt=false;
    OcsCom.flag_TcsEmergenStop_M3=false;
    OcsCom.flag_TcsEmergenStop_ADC=false;

    OcsCom.flag_TcsEnableAz=false;
    OcsCom.flag_TcsDisableAz=false;
    OcsCom.flag_TcsCheckAz=false;
    OcsCom.flag_PointByOCS_AZ=false;
    OcsCom.flag_PointingByOCS_AZ=false;
    OcsCom.flag_PointStopByOCS_AZ=false;
    OcsCom.flag_TcsHomeAz=false;

    OcsCom.flag_TcsEnableAlt=false;
    OcsCom.flag_TcsDisableAlt=false;
    OcsCom.flag_TcsCheckAlt=false;
    OcsCom.flag_PointByOCS_ALT=false;
    OcsCom.flag_PointingByOCS_ALT=false;
    OcsCom.flag_PointStopByOCS_ALT=false;
    OcsCom.flag_TcsHomeAlt=false;
    OcsCom.flag_TcsLockAlt=false;
    OcsCom.flag_TcsUnlockAlt=false;

    OcsCom.flag_TcsEnableDero=false;
    OcsCom.flag_TcsDisableDero=false;
    OcsCom.flag_TcsCheckDero=false;
    OcsCom.flag_PointByOCS_Dero=false;
    OcsCom.flag_PointingByOCS_Dero=false;
    OcsCom.flag_PointStopByOCS_Dero=false;
    OcsCom.flag_TcsHomeDero=false;
    OcsCom.flag_TcsOpenCover=false;
    OcsCom.flag_TcsCloseCover=false;
    OcsCom.flag_TcsStopCover=false;


    OcsCom.flag_TcsEnableM3=false;
    OcsCom.flag_TcsDisableM3=false;
    OcsCom.flag_TcsCheckM3=false;
    OcsCom.flag_PointByOCS_M3T=false;
    OcsCom.flag_PointingByOCS_M3T=false;
    OcsCom.flag_PointStopByOCS_M3T=false;
    OcsCom.flag_TcsHomeM3=false;
    OcsCom.flag_TcsLockM3T=false;
    OcsCom.flag_TcsUnlockM3T=false;
    OcsCom.flag_PointByOCS_M3R=false;
    OcsCom.flag_PointByOCS_M3R=false;
    OcsCom.flag_PointStopByOCS_M3R=false;
    OcsCom.flag_PointByOCS_FL=false;
    OcsCom.flag_PointingByOCS_FL=false;
    OcsCom.flag_PointStopByOCS_FL=false;

    OcsCom.flag_TcsEnableADC1=false;
    OcsCom.flag_TcsDisableADC1=false;
    OcsCom.flag_TcsCheckADC1=false;
    OcsCom.flag_PointByOCS_ADC1=false;
    OcsCom.flag_PointingByOCS_ADC1=false;
    OcsCom.flag_PointStopByOCS_ADC1=false;
    OcsCom.flag_TcsHomeADC1=false;

    OcsCom.flag_TcsEnableADC2=false;
    OcsCom.flag_TcsDisableADC2=false;
    OcsCom.flag_TcsCheckADC2=false;
    OcsCom.flag_PointByOCS_ADC2=false;
    OcsCom.flag_PointingByOCS_ADC2=false;
    OcsCom.flag_PointStopByOCS_ADC2=false;
    OcsCom.flag_TcsHomeADC2=false;

    //QString str;
    //char cmd[255];
    //sprintf ( cmd,"M7025=0" );
    //SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}

MainWindow::~MainWindow()
{
    qDebug ( "close MainWindow" );
    deleteLater();
    timer00->deleteLater();
    writeLog->writeregular("Mount close","	Mount close");
}

void MainWindow::iniDockWidget()
{
    //dockWidget_data->setObjectName("Tracking And Network");
    dockWidget_data->setMaximumSize(dockWidgetMaxWidth,dockWidgetMaxHeight);
    dockWidget_data->setMinimumSize(dockWidgetMinWidth,dockWidgetMinHeight);
    //dockWidget_data->setFeatures(QDockWidget::AllDockWidgetFeatures);//QDockWidget.NoDockWidgetFeatures
    //dockWidget_data->setFixedWidth(dockWidgetMaxWidth);
    dockWidget_data->setFloating(true);
    //dockWidget_data->setFixedHeight(250);
    //dockWidget_data->hide();
}

void MainWindow::iniNetwork()
{
    OcsIP=SMySetting::instance_p()->value ( "misc/OcsIP" ).toString();
    OcsPort=SMySetting::instance_p()->value ( "misc/OcsPort" ).toInt();
    qDebug() <<"OCS IP"<<OcsIP<<"OCS Port"<<OcsPort;

    LocalhostIP=SMySetting::instance_p()->value ( "misc/LocalhostIP" ).toString();
    LocalhostPort=SMySetting::instance_p()->value ( "misc/LocalhostPort" ).toInt();
    qDebug() <<"Mount LocalhostIP"<<LocalhostIP<<"Mount LocalhostPort"<<LocalhostPort;

    StartTcpServer();		//localhost sever
}

void  MainWindow::CombineString ( char StatusCUID[100],char ExeStatus[50],char bExeStatus[10],QString* s_temp )
{
    QDateTime datetime;
    QString s_datetime;

    datetime = QDateTime::currentDateTime();
    s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

    QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +s_datetime+QString ( "><ExeStatus=%1><bExeStatus=true>" ).arg ( ExeStatus );
    ( *s_temp ).append ( str );
}

void  MainWindow::MakeString ( char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp )
{
    QDateTime datetime;
    QString s_datetime;

    datetime = QDateTime::currentDateTime();
    s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

    QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +s_datetime+QString ( "><ExeStatus=%1><RV=%2><Info=%3><bExeStatus=%4>" ).arg ( ExeStatus ).arg ( RV ).arg ( Info ).arg ( bExeStatus ) +DataBack;
    ( *s_temp ).append ( str );
}

void MainWindow::SendToOCS(QString str)
{
    sendthread *send = new sendthread(NULL,str,OcsIP,OcsPort);
    //sendthread *send = new sendthread(m_parent,str,LocalhostIP,OcsPort);
    send->run();

}
void MainWindow::SendEmergenStopToOCS()
{
    OcsCom.flag_TcsEmergenStop_AzAlt=true;
    OcsCom.flag_TcsEmergenStop_M3=true;
    OcsCom.flag_TcsEmergenStop_ADC=true;

    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";


    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_EmergenStop,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );

    sprintf ( ExeStatus, "Active" );
    //sprintf ( RV, "-1" );
    sprintf ( Info,"Tracking" );
    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    MakeString ( OcsCom.StatusCUID_EmergenStop,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_EmergenStop,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void  MainWindow::SendTrackingToOCS()
{
    QString s_temp,s_temp1,s_temp2;
    char ExeStatus[50]="Start";
    char RV[10]="0";
    char Info[10];
    char bExeStatus[10]="true";
    QString DataBack="";


    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
    CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );
    SendToOCS ( s_temp );

    sprintf ( ExeStatus, "Active" );
    sprintf ( RV, "-1" );
    sprintf ( Info,"Tracking" );
    //CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp );//OcsCom.TrackStopUid
    MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp1 );
    SendToOCS ( s_temp1 );

    sprintf(ExeStatus,"Done");
    //MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp2 );
    CombineString ( OcsCom.StatusCUID_MA,ExeStatus,bExeStatus,&s_temp2 );//OcsCom.TrackStopUid
    SendToOCS ( s_temp2 );
}

void MainWindow::iniTableWidget()
{
    //tableWidget = new QTableWidget(1, 2);
    ///////////////table setup////////////////////////

    NettableWidget->setColumnCount ( Column_NetTable );
    NettableWidget->setRowCount ( Row_NetTable);
    //item->setTextAlignment(Qt::AlignRight);
    QStringList labels;
    NettableWidget->setSortingEnabled ( false );
    labels << tr ( "Received Data (Ethernet) " ) << tr ( "Time" );
    NettableWidget->setHorizontalHeaderLabels ( labels );

    NettableWidget->setColumnWidth ( column_NetTable_1, width_NetTable_1 );		//1# column width
    NettableWidget->setColumnWidth ( column_NetTable_2, width_NetTable_2 );		//2# column width
    // NettableWidget->scrollToItem(newItem);
    NettableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    NettableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
}

void MainWindow::iniTableTracking()
{
    model_tracking = new QStandardItemModel ( 0,0,this );
    tableView_tracking->setModel( model_tracking );

    QStringList tcs_head = QStringList();
    tcs_head << "Time"<< "AZ Target" << "Position" << "Error" << "ALT Target" << "Position" << "Error" << "DERO Target" << "Position" << "Error";
    model_tracking->setHorizontalHeaderLabels ( tcs_head );
    tableView_tracking->setColumnWidth ( TableTrackingColumn0,TableTrackingWidth_col0);
    for(int i=1;i<10;i++)
        tableView_tracking->setColumnWidth ( i,TableTrackingWidth );

    model_tracking->setColumnCount(Column_TableTracking);
    model_tracking->setRowCount(Row_TableTracking);
    tableView_tracking->resizeRowsToContents();
}

void MainWindow::receiveMessage()
{
    //QTcpSocket *clientConnection;
    clientConnection = TCPServerOCS->nextPendingConnection();
    connect ( clientConnection, SIGNAL ( readyRead() ), this, SLOT ( slotReadMessage() ) );
    connect ( clientConnection, SIGNAL ( disconnected() ),clientConnection, SLOT ( deleteLater() ) );
}

/////////////////analyse command form OCS //////////////////
/*output: OcsCom.CommandFromOCS  ,Global Variable
   OcsCom.StatusCUID_MA  ,Global Variable
  OcsCom.NewCUID  ,Global Variable
*/
// void MainWindow::slotReadMessage()
// {
// 	QByteArray  m_Buffer;
// 	QString nextFortune;
// 	m_BlockSize = 0;
// 	m_Buffer.clear();
// 	m_Buffer=readDataIntoBuffer ( 1000 );
// 	nextFortune=m_Buffer;
// 	OcsCom.CommandFromOCS=nextFortune;
// 
// 	QByteArray ba;
// 	char* comm;
// 	ba= nextFortune.toLatin1();
// 	comm = ba.data();
// 
// 	char request1_Mount[1000];//received
// 	strcpy ( request1_Mount,comm );
// 	ParseOCSCommand ( request1_Mount,OcsCom.NewCUID,OcsCom.StatusCUID_MA );//
// 
// 	refreshNettable();
// 	qDebug() <<comm;
// 
// }
/*
QByteArray MainWindow::readDataIntoBuffer ( int maxSize )
{
 QByteArray  m_Buffer;

 QByteArray   datas ( "hello" );

 char *getValue=NULL;
 char *tmp;
 quint16 numBytesRead =0;
 quint16 numBytesAvailable =0;


 if ( clientConnection->bytesAvailable() < maxSize  &&  maxSize > MaxBufferSize )
  return 0;

 numBytesAvailable=clientConnection->bytesAvailable();
 getValue = ( char* ) malloc ( sizeof ( char ) *numBytesAvailable );


 clientConnection->read ( getValue,numBytesAvailable );

 memcpy ( &numBytesRead,getValue,sizeof ( quint16 ) );				//get the datas length

 tmp = getValue;

 for ( int i=0;i<numBytesAvailable;i++ )
 {
  m_Buffer.append ( *tmp++ );
 }


 if ( getValue ) free ( getValue );

 clientConnection->disconnectFromHost();

 //return m_Buffer.size();
 return m_Buffer;
}
*/


/////////////////analyse command form OCS //////////////////
int MainWindow::getsubstr ( char *sourcestr, char *targetstr, int i_begin, int i_length )
{
    int i = 0;

    for ( i=0;i<i_length;i++ )
    {
        targetstr[i] = sourcestr[i + i_begin];
    }

    targetstr[i]='\0';
    return 0;
}

int MainWindow::strtrim ( char *sourcestr, char *targetstr ) //left
{
    int i=0, j=0 ;

    while ( ( ( sourcestr[i]<33 ) || ( sourcestr[i]>126 ) ) && ( sourcestr[i]!='\0' ) )
    {
        i++;
    }

    j = strlen ( sourcestr ) - 1;

    while ( ( ( sourcestr[j]<33 ) || ( sourcestr[j]>126 ) ) && ( sourcestr[i]!='\0' ) )
    {
        j--;
    }

    getsubstr ( sourcestr, targetstr, i, j - i + 1 );

    return 0;
}

int MainWindow::strrtrim ( char *sourcestr, char *targetstr ) //right
{
    int i=0, j=0 ;

    j = strlen ( sourcestr ) - 1;

    while ( ( ( sourcestr[j]<33 ) || ( sourcestr[j]>126 ) ) && ( sourcestr[i]!='\0' ) )
    {
        j--;
    }

    getsubstr ( sourcestr, targetstr, i, j - i + 1 );

    return 0;
}

int MainWindow::pr_get_commandvalue ( char *command, char *item, char *value )
{
    char *p_tempb , *p_tempe;
    int i_len,j;
    char buf[LONG_MESSAGE_LENGTH]={"<"};

    strcat ( buf,item );
    p_tempb = strstr ( command,buf );


    if ( p_tempb==NULL )
    {
        value = NULL;
        return 0;
    }
    else
    {
        p_tempb = strstr ( p_tempb,"=" );

        if ( p_tempb==NULL )
        {
            value = NULL;
            return 0;
        }
        else
        {
            p_tempe = strstr ( p_tempb,">" );
            p_tempb = p_tempb + 1;
        }
    }

    i_len = p_tempe - p_tempb;

    for ( j=0;j<i_len;j++ )
    {
        value[j] =*p_tempb;
        p_tempb++;
    }

    value[j] = '\0';
    strcpy ( buf,value );
    strtrim ( buf,value );
    //pr_strmessagebox(buf);

    return 1;
}

bool MainWindow::pr_get_guidedata ( char *command, int *datatime, double *AzGuide,  double *AltGuide )
{
    char *p_tempb ;
    //char *valuetime,*valueaz,*valuealt;
    QStringList  list;
    //int i_len,j;
    char buf[6]={"guide"};

    //strcat ( buf,item );
    p_tempb = strstr ( command,buf );


    if ( p_tempb!=NULL )
    {
        //getsubstr ( char *command, char *targetstr, int i_begin, int i_length );
        /*getsubstr ( command,valuetime,5,6);
        getsubstr ( command,valueaz,11,7);
        getsubstr ( command,valuealt,18,7);
        *datatime=QString(valuetime).toInt();
        *AzGuide= QString(valueaz).toDouble();
        *AltGuide= QString(valuealt).toDouble();*/
        list= QString(command).split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
        if ( list.size() >=4 )
        {
            *datatime = list[1].toInt();
            *AzGuide = list[2].toDouble();
            *AltGuide = list[3].toDouble();
            return true;
        }
    }
    else return false;
}
////////////////////////////////////////////////////////////////////

void MainWindow::closeEvent ( QCloseEvent *event )
{
    QMessageBox mb ( QMessageBox::Question,tr ( "About to close" ),
                     tr ( "Do you want to close?      " ),
                     QMessageBox::Yes |QMessageBox::No |QMessageBox::Cancel );
    if ( mb.exec() == QMessageBox::Yes )
    {
        SMySetting::instance_p()->beginGroup ( "main_window" );
        SMySetting::instance_p()->setValue ( "size",size() );
        SMySetting::instance_p()->setValue ( "pos",pos() );
        SMySetting::instance_p()->endGroup();
        return;
    }
    event->ignore();
}

void MainWindow::updateInterface ( QHash<QString,QString> info_track )
{
    QDateTime dt=QDateTime::currentDateTime();
    QString dtstr;
    dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );

    QList<QStandardItem*> items_tcs;
    //	qDebug() <<info_track["AZ_TARGET"];
    items_tcs << new QStandardItem ( dtstr )
              << new QStandardItem ( info_track["AZ_TARGET"] )
              << new QStandardItem ( info_track["AZ_POSITION"] )
              << new QStandardItem ( info_track["AZ_ERROR"] )
              << new QStandardItem ( info_track["ALT_TARGET"] )
              << new QStandardItem ( info_track["ALT_POSITION"] )
              << new QStandardItem ( info_track["ALT_ERROR"]  );
    items_tcs << new QStandardItem ( info_track["DERO_TARGET"])
              << new QStandardItem ( info_track["DERO_POSITION"] )
              << new QStandardItem ( info_track["DERO_ERROR"] );
    model_tracking->appendRow ( items_tcs );
    //	QModelIndex index = model_tracking->index(model_tracking->rowCount(),model_tracking->columnCount());
    //	qDebug() << index.column();
    //	tableView_tracking->setCurrentIndex(model_tracking->index(model_tracking->rowCount()-1,0));
    tableView_tracking->scrollTo ( model_tracking->index ( model_tracking->rowCount()-1,0 ) );
}

/*input: OcsCom.CommandFromOCS  ,Global Variable

*/

void MainWindow::refreshNettable ( /* QString CommandFromOCS*/ )
{
    if ( nConnection==nConnection_old )
        return;
    nConnection_old=nConnection;
    //items_ocs << new QStandardItem(dtstr) << new QStandardItem(cmd);

    ///////////////Net table//////////////////////////////
    QDateTime dt=QDateTime::currentDateTime();
    QString dtstr;
    dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );

    if ( rowNet>8 )
        NettableWidget->insertRow ( rowNet );


    QTableWidgetItem *NetItem0 = new QTableWidgetItem ( OcsCom.CommandFromOCS );
    NettableWidget->setItem ( rowNet, 0, NetItem0 );
    QTableWidgetItem *NetItem1 = new QTableWidgetItem ( dtstr );
    NettableWidget->setItem ( rowNet, 1, NetItem1 );
    rowNet++;
    NettableWidget->scrollToItem ( NetItem0 );
}

void MainWindow::startAzalt ()
{
    if( !flagtcs.IsOpened_azalt )
    {
        qDebug("Open Azalt!");
        try
        {
            //std::auto_ptr<DirectAzalt> temp(new DirectAzalt ( this ));
            //direct_azalt = temp;
            direct_azalt = new DirectAzalt ( this );
        }
        catch ( QString excepts )
        {
            QMessageBox mb ( QMessageBox::Warning,
                             tr ( "About to close" ),
                             tr ( excepts.toAscii() ),
                             QMessageBox::Ok );

            QString s_temp;
            QString s_datetime = QDateTime::currentDateTime().toString ( "yyyy.MM.dd-hh:mm:ss" );

            // 		s_temp = QString ( "<RelatedCmdCUID=" ) +cmd;
            // 		s_temp.append ( "><StatusCUID=" );
            // 		s_temp.append ( s_datetime );
            // 		s_temp.append ( "><ExeStatus=START><bExeStatus=true>" );
            // 		server->writeData ( s_temp );

            if ( mb.exec() ==QMessageBox::Ok )
                return;
        }
        mdiArea->addSubWindow ( direct_azalt/*.get()*/ );
        direct_azalt->show();
    }
    //disconnect ( actionAz_Alt,SIGNAL ( triggered() ),this,SLOT ( startDirectAzalt() ) );
    //connect ( actionAz_Alt,SIGNAL ( triggered() ),direct_azalt->parentWidget(),SLOT ( showNormal() ) );
}



void MainWindow::startDero()
{
    if( !flagtcs.IsOpened_dero )
    {
        qDebug("Open derotator!");
        try
        {
            derotator = new Derotator ( this );
        }
        catch ( QString excepts )
        {
            QMessageBox mb ( QMessageBox::Warning,
                             tr ( "About to close" ),
                             tr ( excepts.toAscii() ),
                             //		                 tr ( "unknown error or excepts" ),
                             QMessageBox::Ok );
            if ( mb.exec() ==QMessageBox::Ok )
                return;
        }
        mdiArea->addSubWindow ( derotator );
        derotator->show();
    }
    //disconnect ( actionDerotator,SIGNAL ( triggered() ),this,SLOT ( startDirectDero() ) );
    //connect ( actionDerotator,SIGNAL ( triggered() ),direct_dero->parentWidget(),SLOT ( showNormal() ) );
}

void MainWindow::startADC()
{
    if( !flagtcs.IsOpened_adc )
    {
        qDebug("Open adc!");
        try
        {
            adc = new ADC ( this );
        }
        catch ( QString excepts )
        {
            QMessageBox mb ( QMessageBox::Warning,
                             tr ( "About to close" ),
                             tr ( excepts.toAscii() ),
                             //		                 tr ( "unknown error or excepts" ),
                             QMessageBox::Ok );
            if ( mb.exec() ==QMessageBox::Ok )
                return;
        }
        mdiArea->addSubWindow ( adc );
        adc->show();
    }
}

void MainWindow::startM2()
{
    if( !flagtcs.IsOpened_m2 )
    {
        qDebug("Open m2!");
        try
        {
            dialog_m2 = new Dialog_m2(this);
        }
        catch ( QString excepts )
        {
            QMessageBox mb ( QMessageBox::Warning,
                             tr ( "About to close" ),
                             tr ( excepts.toAscii() ),
                             //		                 tr ( "unknown error or excepts" ),
                             QMessageBox::Ok );
            if ( mb.exec() ==QMessageBox::Ok )
                return;
        }
        mdiArea->addSubWindow ( dialog_m2 );
        dialog_m2->show();
        connect_pi();
    }
}


void MainWindow::startTracking ()
{
    //qDebug()<<"start tracking!"<<"flagtcs.IsOpened_tracking"<<flagtcs.IsOpened_tracking;
    if( !flagtcs.IsOpened_tracking )
    {
        qDebug("open tracking");
        tracking = new Tracking ( this );

//        try
//        {
//            tracking = new Tracking ( this );
//        }
//        catch ( QString excepts )
//        {
//            qDebug() << "catch";
//            QMessageBox mb ( QMessageBox::Warning,
//                             tr ( "About to close" ),
//                             tr ( excepts.toAscii() ),
//                             QMessageBox::Ok );
//            if ( mb.exec() ==QMessageBox::Ok )
//                return;
//        }

        mdiArea->addSubWindow ( tracking );
        tracking->show();
    }
    //	disconnect ( actionTracking,SIGNAL ( triggered() ),this,SLOT ( startTracking() ) );
    //	connect ( actionTracking,SIGNAL ( triggered() ),tracking->parentWidget(),SLOT ( showNormal() ) );
}

/*void MainWindow::startDiagDero()
{
        if( !flagtcs.IsOpened_diag_dero)
  {
  try
  {
   diag_dero = new DiagDero ( this );
  }
  catch ( QString excepts )
  {
   QMessageBox mb ( QMessageBox::Warning,
                    tr ( "About to close" ),
                    tr ( excepts.toAscii() ),
                    QMessageBox::Ok );
   if ( mb.exec() ==QMessageBox::Ok )
    return;
  }
  mdiArea->addSubWindow ( diag_dero );
  diag_dero->show();
        }
}

void MainWindow::startDiagAzalt()
{
//
       if( !flagtcs.IsOpened_diag_azalt)
 {
  try
  {
   diag_azalt = new DiagAzalt ( this );
  }
  catch ( QString excepts )
  {
   QMessageBox mb ( QMessageBox::Warning,
                    tr ( "About to close" ),
                    tr ( excepts.toAscii() ),
                    QMessageBox::Ok );
   if ( mb.exec() ==QMessageBox::Ok )
    return;
  }
  mdiArea->addSubWindow ( diag_azalt );
  diag_azalt->show();
        }
//	disconnect ( actionAz_Alt_D,SIGNAL ( triggered() ),this,SLOT ( startDiagAzalt() ) );
//	connect ( actionAz_Alt_D,SIGNAL ( triggered() ),diag_azalt->parentWidget(),SLOT ( showNormal() ) );
}
*/

void MainWindow::startSimulationOCS()//simulation of OCS
{
    if( !flagtcs.IsOpened_simulationOCS )
    {
        qDebug("open simulationOCS");
        try
        {
            simulationOcs = new SimulationOCS ( this );
        }
        catch ( QString excepts )
        {
            QMessageBox mb ( QMessageBox::Warning,
                             tr ( "About to close" ),
                             tr ( excepts.toAscii() ),
                             QMessageBox::Ok );
            if ( mb.exec() ==QMessageBox::Ok )
                return;
        }
        mdiArea->addSubWindow ( simulationOcs );
        simulationOcs->show();
    }
}

/*void MainWindow::ON()
{
 QString str;
 SQUMAC::instance_p()->QcommCmdPMAC ( EnableAmplifiersCabinet,str );
  QCoreApplication::processEvents();
 sleep(2);
 SQUMAC::instance_p()->QcommCmdPMAC ( EnableAmplifiersCabinetRelease,str );

}
void MainWindow::OFF()
{

 QString str;
 SQUMAC::instance_p()->QcommCmdPMAC ( EmergencyStop,str );
  QCoreApplication::processEvents();
 sleep(2);
 SQUMAC::instance_p()->QcommCmdPMAC ( EmergencyStopRelease,str );


}

void MainWindow::AltLock()
{

 QString str;
 SQUMAC::instance_p()->QcommCmdPMAC ( ALTLOCKbuttonDown,str );
  QCoreApplication::processEvents();
 sleep(2);
 SQUMAC::instance_p()->QcommCmdPMAC ( ALTLOCKbuttonOn,str );


}
void MainWindow::AltUnLock()
{

 QString str;
 SQUMAC::instance_p()->QcommCmdPMAC ( ALTUNLOCKbuttonDown,str );
  QCoreApplication::processEvents();
 sleep(2);
 SQUMAC::instance_p()->QcommCmdPMAC ( ALTUNLOCKbuttonOn,str );


}
*/

void MainWindow::startTerminal()
{
    if( !flagtcs.IsOpened_terminal )
    {
        qDebug("open terminal");
        try
        {
            terminal = new Terminal ( this );
        }
        catch ( QString excepts )
        {
            QMessageBox mb ( QMessageBox::Warning,
                             tr ( "About to close" ),
                             tr ( excepts.toAscii() ),
                             QMessageBox::Ok );
            if ( mb.exec() ==QMessageBox::Ok )
                return;
        }
        mdiArea->addSubWindow ( terminal );
        terminal->show();
    }
    //disconnect ( actionTerminal,SIGNAL ( triggered() ),this,SLOT ( startTerminal() ) );
    //connect ( actionTerminal,SIGNAL ( triggered() ),terminal->parentWidget(),SLOT ( showNormal() ) );
}
//by zxj---overload
void MainWindow::startTerminal(char* Jog)//by zxj---add "Jog"
{
    if( !flagtcs.IsOpened_terminal )
    {
        //by zxj---show Terminal
        qDebug("open terminal");
        try
        {
            terminal = new Terminal ( this );
        }
        catch ( QString excepts )
        {
            QMessageBox mb ( QMessageBox::Warning,
                             tr ( "About to close" ),
                             tr ( excepts.toAscii() ),
                             QMessageBox::Ok );
            if ( mb.exec() ==QMessageBox::Ok )
                return;
        }
        mdiArea->addSubWindow ( terminal );
        terminal->show();
        terminal->getJogCmd(Jog);
        connect(this, SIGNAL(JogFromAndroid()), terminal, SLOT(on_pushButton_clicked()));
        emit JogFromAndroid();

//        //by zxj---show Plotter Test
//        qDebug() << "hello!";
//        mplottersetting.show();

//        pointerplottersetting = &mplottersetting;
//        //If use SLOT,the object before it must be a pointer!
//        connect(this, SIGNAL(JogFromAndroid()), pointerplottersetting, SLOT(on_pushButton_plot_clicked()));

//        emit JogFromAndroid();
    }
    //disconnect ( actionTerminal,SIGNAL ( triggered() ),this,SLOT ( startTerminal() ) );
    //connect ( actionTerminal,SIGNAL ( triggered() ),terminal->parentWidget(),SLOT ( showNormal() ) );
}

void MainWindow::startSetup()
{
    if( !flagtcs.IsOpened_setup )
    {
        qDebug("open Setup");
        try
        {
            setup = new Setup ( this );
        }
        catch ( QString excepts )
        {
            QMessageBox mb ( QMessageBox::Warning,
                             tr ( "About to close" ),
                             tr ( excepts.toAscii() ),
                             QMessageBox::Ok );
            if ( mb.exec() ==QMessageBox::Ok )
                return;
        }
        mdiArea->addSubWindow ( setup );
        setup->show();
    }
}

void MainWindow::on_actionWindows_triggered()
{
    timer->start ( 10 );
    QApplication::setStyle ( new QWindowsStyle );
}

void MainWindow::on_actionMotif_triggered()
{
    QApplication::setStyle ( new QMotifStyle );
}

void MainWindow::on_actionGtk_triggered()
{
    //	QApplication::setStyle ( new QGtkStyle );
}

void MainWindow::on_actionCDE_triggered()
{
    QApplication::setStyle ( new QCDEStyle );

}


void MainWindow::showdata_main ( /*QHash<QString,QString> info_track*/ ) //timer00
{
    /*if(trackstar.flagAzTracking || trackstar.flagAltTracking)
    {
        if( flagtcs.IsOpened_azalt )
        {
            flagtcs.IsOpened_azalt=false;

            direct_azalt->close();
            mdiArea->removeSubWindow(direct_azalt);
        }
    }*/
    /*if(trackstar.flagDeroTracking )
    {
        if( flagtcs.IsOpened_dero )
        {
            flagtcs.IsOpened_dero=false;
            derotator->close();
            mdiArea->removeSubWindow(derotator);
        }
    }*/
    /*if(trackstar.flagADC1Tracking || trackstar.flagADC2Tracking)
    {
        if( flagtcs.IsOpened_adc )
        {
            flagtcs.IsOpened_adc=false;
            adc->close();
            mdiArea->removeSubWindow(adc);
        }
    }*/
    /*if( flagtcs.IsOpened_m2 )
        {
            flagtcs.IsOpened_m2=false;
            mdiArea->removeSubWindow(dialog_m2);
        }*/

    //     refreshTracktable(info_track);
    //  	QDateTime dt=QDateTime::currentDateTime();
    // 	QString dtstr;
    // 	dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss" );
    //
    // 	QList<QStandardItem*> items_tcs;
    // 	qDebug() <<info_track["AZ_TARGET"];
    // 	items_tcs << new QStandardItem(dtstr) << new QStandardItem(info_track["AZ_TARGET"]) << new QStandardItem(info_track["AZ_POSITION"]) << new QStandardItem(info_track["AZ_ERROR"]) << new QStandardItem(info_track["ALT_TARGET"]) << new QStandardItem(info_track["ALT_POSITION"]) << new QStandardItem(info_track["ALT_ERROR"]);
    // 	model_tracking->appendRow(items_tcs);
    // 	QModelIndex index = model_tracking->index(model_tracking->rowCount(),model_tracking->columnCount());
    // 	qDebug() << index.column();
    // 	tableView_tracking->setCurrentIndex(model_tracking->index(model_tracking->rowCount()-1,0));
    //
    // 	tableView_tracking->scrollTo(model_tracking->index(model_tracking->rowCount()-1,0)/*,QAbstractItemView::PositionAtBottom*/);

}
// void MainWindow::refreshTrackTable(QHash<QString,QString> info_track)
// {
// 	QDateTime dt=QDateTime::currentDateTime();
// 	QString dtstr;
// 	dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss" );
//
// 	QList<QStandardItem*> items_tcs;
// //	qDebug() <<info_track["AZ_TARGET"];
// 	items_tcs << new QStandardItem(dtstr) << new QStandardItem(info_track["AZ_TARGET"]) << new QStandardItem(info_track["AZ_POSITION"]) << new QStandardItem(info_track["AZ_ERROR"]) << new QStandardItem(info_track["ALT_TARGET"]) << new QStandardItem(info_track["ALT_POSITION"]) << new QStandardItem(info_track["ALT_ERROR"]);
// 	model_tracking->appendRow(items_tcs);
// //	QModelIndex index = model_tracking->index(model_tracking->rowCount(),model_tracking->columnCount());
// //	qDebug() << index.column();
// //	tableView_tracking->setCurrentIndex(model_tracking->index(model_tracking->rowCount()-1,0));
//
// 	tableView_tracking->scrollTo(model_tracking->index(model_tracking->rowCount()-1,0)/*,QAbstractItemView::PositionAtBottom*/);
// }
// ////////////////////////////TCP Server ///////////////////////////////
void MainWindow::StartTcpServer()
{
    QString LocalhostIP = SMySetting::instance_p()->value ( "misc/LocalhostIP" ).toString();//LocalhostIP=159.226.75.214
    int LocalhostPort = SMySetting::instance_p()->value ( "misc/LocalhostPort" ).toInt();//LocalhostPort=5001
    qDebug() << "waitting for data from OCS.";
    // 	server = new Server ( this );
    //
    // 	if ( !server->listen ( QHostAddress::Any, LocalhostPort ) )
    // 	{
    // 		qDebug ( "Server start error!" );
    //
    // 		return;
    // 	}
    FortuneServer *socketserver;
    socketserver = new FortuneServer(this);
    if (!socketserver->listen(QHostAddress::Any, LocalhostPort))
    {
        QMessageBox::critical(this, tr("Threaded Fortune Server"),
                              tr("Unable to start the server: %1.").arg(socketserver->errorString()));
        close();
        return;
    }
}

/*
input:	  OcsCom.flag_PointByOCS_AZ,Global Variable
   OcsCom.flag_PointByOCS_ALT,Global Variable
 OcsCom.flag_PointStopByOCS_AZ,Global Variable
 OcsCom.flag_PointStopByOCS_ALT,Global Variable
 OcsCom.flag_TcsCheckAz,Global Variable
 OcsCom.flag_TcsCheckAlt,Global Variable
*/
/////////////////////////// TCP Server for OCS ///////////////////////////////
// void MainWindow::StartTcpServerOCS()
// {
// 	TCPServerOCS = new QTcpServer ( this );
// 	if ( !TCPServerOCS->listen ( QHostAddress::Any, OcsPort ) )
// 	{
// 		QMessageBox::critical ( this, tr ( "TCS Server" ),
// 		                        tr ( "Unable to start the server: %1." )
// 		                        .arg ( TCPServerOCS->errorString() ) );
// 		close();
// 		return;
// 	}
// 
// 	QMessageBox mb ( QMessageBox::Warning,
// 	                 tr ( "TCS TCP" ),
// 	                 tr ( "server port %1" ).arg ( TCPServerOCS->serverPort() ) ,
// 
// 	                 QMessageBox::Ok );
// 	if ( mb.exec() ==QMessageBox::Ok )
// 
// 		connect ( TCPServerOCS, SIGNAL ( newConnection() ), this, SLOT ( receiveMessage() ) );
// 
// }


/*input: OcsCom.CommandFromOCS  ,Global Variable

         OcsCom.StatusCUID_MA   ,Global Variable
         OcsCom.NewCUID         ,Global Variable
*/
void MainWindow::showInfoOCS ( QString infoOCS )
{
    OcsCom.CommandFromOCS = infoOCS;

    ParseOCSCommand ( infoOCS.toAscii().data(), OcsCom.NewCUID, OcsCom.StatusCUID_MA );//
//    qDebug(infoOCS.toAscii().data());

    refreshNettable();
}

//pStr:command from TCS
//newUID :TCS UID：2.1.0.1.2.，
//pCmdCUID:   "CmdCUID="2.9.0.1.1.20061123.123456.4
bool  MainWindow::ParseOCSCommand ( char* pStr,char *newUID,char *pCmdCUID )
{
    char *p;
    //char pCmdCUID[100];
    //char *newUID;
    //  	char buf_tmp[100];
    // 	char TimeYMD_TcsCommand[64];
    // 	char TimeHMS_TcsCommand[64];
    //         checkmessage *checkMessage = new checkmessage();
    //         QString uid = checkMessage->get_itemvalue(pStr,"CmdCUID");
    //         if(uid.indexOf("2.0.0.1")!=-1)
    // 	{
    //
    // 	}
    //
    // //char buf_CmdCUID[50];
    int icommand=pr_get_commandvalue ( pStr, "CmdCUID", pCmdCUID );
    //getsubstr ( pCmdCUID,TimeYMD_TcsCommand,10,8 );
    //getsubstr ( pCmdCUID,TimeHMS_TcsCommand,19,6 );
    //
    // qDebug()<<"received"<<*pStr;

    nConnection++;
    if ( nConnection>=65530 )
        nConnection=0;

    if(icommand)
    {
        if((p = strstr ( pStr,TCSEMERGENSTOP))!=NULL)
        {   strcpy(OcsCom.StatusCUID_EmergenStop,pCmdCUID);
            SendEmergenStopToOCS();return TRUE;}
        if((p = strstr ( pStr,TCSSTARTTRACKING))!=NULL)	//	2.4.0.1.1 	TcsStartTracking
        {   strcpy(OcsCom.StatusCUID_Track,pCmdCUID);
            TcsStartTracking();return TRUE;}
        else if((p = strstr ( pStr,TCSSTOPTRACKING))!=NULL)	//	2.4.0.2.1 	TcsStopTracking
        {   strcpy(OcsCom.StatusCUID_Track,pCmdCUID);
            TcsStopTracking();return TRUE;}
        if((p = strstr ( pStr,TCSGETPOSITION))!=NULL)	//	2.4.0.1.1
        {   strcpy(OcsCom.StatusCUID_Track,pCmdCUID);
            TcsGetPosition();return TRUE;}

        /*if(trackstar.flagAzTracking || trackstar.flagAltTracking)
        {
            SendTrackingToOCS();
            return false;
        }*/
        if((p = strstr ( pStr,TCSENABLEAZ))!=NULL)
        {   strcpy(OcsCom.StatusCUID_Az,pCmdCUID);
            TcsEnableAz();return TRUE;}
        else if((p = strstr ( pStr,TCSDISABLEAZ))!=NULL)
        {   strcpy(OcsCom.StatusCUID_Az,pCmdCUID);
            TcsDisableAz();return TRUE;}
        else if((p = strstr ( pStr,TCSCHECKAZ))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_Az,pCmdCUID);
            TcsCheckAz();return TRUE;}
        else if((p = strstr ( pStr,TCSPOINTAZ))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_AzPoint,pCmdCUID);
            TcsPointAz(pStr);return TRUE;}
        else if((p = strstr ( pStr,TCSSTOPAZ))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_Az,pCmdCUID);
            TcsStopAz();return TRUE;}
        else if((p = strstr ( pStr,TCSHOMEAZ))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_AzHome,pCmdCUID);
            TcsHomeAz();return TRUE;}


        if((p = strstr ( pStr,TCSENABLEALT))!=NULL)
        {   strcpy(OcsCom.StatusCUID_Alt,pCmdCUID);
            TcsEnableAlt();return TRUE;}
        else if((p = strstr ( pStr,TCSDISABLEALT))!=NULL)
        {   strcpy(OcsCom.StatusCUID_Alt,pCmdCUID);
            TcsDisableAlt();return TRUE;}
        else if((p = strstr ( pStr,TCSCHECKALT))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_Alt,pCmdCUID);
            TcsCheckAlt();return TRUE;}
        else if((p = strstr ( pStr,TCSPOINTALT))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_AltPoint,pCmdCUID);
            TcsPointAlt(pStr);return TRUE;}
        else if((p = strstr ( pStr,TCSSTOPALT))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_Alt,pCmdCUID);
            TcsStopAlt();return TRUE;}
        else if((p = strstr ( pStr,TCSHOMEALT))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_AltHome,pCmdCUID);
            TcsHomeAlt();return TRUE;}
        else if((p = strstr ( pStr,TCSALTLOCK))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_AltLock,pCmdCUID);
            TcsAltLock();return TRUE;}
        else if((p = strstr ( pStr,TCSALTUNLOCK))!=NULL)
        {   strcpy(OcsCom.StatusCUID_AltUnlock,pCmdCUID);
            TcsAltUnlock();return TRUE;}


        //
        if((p = strstr ( pStr,TCSENABLEDERO))!=NULL)
        {   strcpy(OcsCom.StatusCUID_Dero,pCmdCUID);
            TcsEnableDero();return TRUE;}
        else if((p = strstr ( pStr,TCSDISABLEDERO))!=NULL)
        {   strcpy(OcsCom.StatusCUID_Dero,pCmdCUID);
            TcsDisableDero();return TRUE;}
        else if((p = strstr ( pStr,TCSCHECKDERO))!=NULL)    //
        {   strcpy(OcsCom.StatusCUID_Dero,pCmdCUID);
            TcsCheckDero();return TRUE;}
        else if((p = strstr ( pStr,TCSPOINTDERO))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_DeroPoint,pCmdCUID);
            TcsPointDero(pStr);return TRUE;}
        else if((p = strstr ( pStr,TCSSTOPDERO))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_Dero,pCmdCUID);
            TcsStopDero();return TRUE;}
        else if((p = strstr ( pStr,TCSHOMEDERO))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_DeroHome,pCmdCUID);
            TcsHomeDero();return TRUE;}
        else if((p = strstr ( pStr,TCSCOVEROPEN))!=NULL)
        {   strcpy(OcsCom.StatusCUID_CoverOpen,pCmdCUID);
            TcsCoverOpen();return TRUE;}
        else if((p = strstr ( pStr,TCSCOVERCLOSE))!=NULL)
        {   strcpy(OcsCom.StatusCUID_CoverClose,pCmdCUID);
            TcsCoverClose();return TRUE;}
        else if((p = strstr ( pStr,TCSCOVERSTOP))!=NULL)
        {   strcpy(OcsCom.StatusCUID_CoverStop,pCmdCUID);
            TcsCoverStop();return TRUE;}

        if((p = strstr ( pStr,TCSENABLEADC1))!=NULL)
        {   strcpy(OcsCom.StatusCUID_ADC1,pCmdCUID);
            TcsEnableADC1();return TRUE;}
        else if((p = strstr ( pStr,TCSDISABLEADC1))!=NULL)
        {   strcpy(OcsCom.StatusCUID_ADC1,pCmdCUID);
            TcsDisableADC1();return TRUE;}
        else if((p = strstr ( pStr,TCSCHECKADC1))!=NULL)    //
        {   strcpy(OcsCom.StatusCUID_ADC1,pCmdCUID);
            TcsCheckADC1();return TRUE;}
        else if((p = strstr ( pStr,TCSPOINTADC1))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_ADC1Point,pCmdCUID);
            TcsPointADC1(pStr);return TRUE;}
        else if((p = strstr ( pStr,TCSSTOPADC1))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_ADC1,pCmdCUID);
            TcsStopADC1();return TRUE;}
        else if((p = strstr ( pStr,TCSHOMEADC1))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_ADC1Home,pCmdCUID);
            TcsHomeADC1();return TRUE;}

        if((p = strstr ( pStr,TCSENABLEADC2))!=NULL)
        {   strcpy(OcsCom.StatusCUID_ADC2,pCmdCUID);
            TcsEnableADC2();return TRUE;}
        else if((p = strstr ( pStr,TCSDISABLEADC2))!=NULL)
        {   strcpy(OcsCom.StatusCUID_ADC2,pCmdCUID);
            TcsDisableADC2();return TRUE;}
        else if((p = strstr ( pStr,TCSCHECKADC2))!=NULL)    //
        {   strcpy(OcsCom.StatusCUID_ADC2,pCmdCUID);
            TcsCheckADC2();return TRUE;}
        else if((p = strstr ( pStr,TCSPOINTADC2))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_ADC2Point,pCmdCUID);
            TcsPointADC2(pStr);return TRUE;}
        else if((p = strstr ( pStr,TCSSTOPADC2))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_ADC2,pCmdCUID);
            TcsStopADC2();return TRUE;}
        else if((p = strstr ( pStr,TCSHOMEADC2))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_ADC2Home,pCmdCUID);
            TcsHomeADC2();return TRUE;}

        if((p = strstr ( pStr,TCSENABLEM3))!=NULL)
        {   strcpy(OcsCom.StatusCUID_M3R,pCmdCUID);
            TcsEnableM3();return TRUE;}
        else if((p = strstr ( pStr,TCSDISABLEM3))!=NULL)
        {   strcpy(OcsCom.StatusCUID_M3R,pCmdCUID);
            TcsDisableM3();return TRUE;}
        else if((p = strstr ( pStr,TCSCHECKM3))!=NULL)    //
        {   strcpy(OcsCom.StatusCUID_M3R,pCmdCUID);
            TcsCheckM3();return TRUE;}
        else if((p = strstr ( pStr,TCSLOCKM3T))!=NULL)    //
        {   strcpy(OcsCom.StatusCUID_M3T,pCmdCUID);
            TcsLockM3T();return TRUE;}
        else if((p = strstr ( pStr,TCSUNLOCKM3T))!=NULL)    //
        {   strcpy(OcsCom.StatusCUID_M3T,pCmdCUID);
            TcsUnLockM3T();return TRUE;}
        else if((p = strstr ( pStr,TCSPOINTM3T))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_M3TPoint,pCmdCUID);
            TcsPointM3T(pStr);return TRUE;}
        else if((p = strstr ( pStr,TCSSTOPM3T))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_M3T,pCmdCUID);
            TcsStopM3T();return TRUE;}
        else if((p = strstr ( pStr,TCSHOMEM3T))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_M3THome,pCmdCUID);
            TcsHomeM3T();return TRUE;}

        if((p = strstr ( pStr,TCSPOINTM3R))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_M3RPoint,pCmdCUID);
            TcsPointM3R(pStr);return TRUE;}
        else if((p = strstr ( pStr,TCSSTOPM3R))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_M3R,pCmdCUID);
            TcsStopM3R();return TRUE;}
        if((p = strstr ( pStr,TCSPOINTFL))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_FiberLight,pCmdCUID);
            TcsPointFL(pStr);return TRUE;}
        else if((p = strstr ( pStr,TCSSTOPFL))!=NULL)	//	TCSLOCKM3T
        {   strcpy(OcsCom.StatusCUID_FiberLight,pCmdCUID);
            TcsStopFL();return TRUE;}

        //by zxj---open Terminal
        if((p = strstr ( pStr,TCSSTARTTERMINAL))!=NULL)	//
        {   strcpy(OcsCom.StatusCUID_TerminalStart,pCmdCUID);
            TcsStartTerminal(pStr);return TRUE;}
    }
    else
    {
        pr_get_guidedata(pStr,&trackstar.guidetime,&trackstar.az_guidedata,&trackstar.alt_guidedata);
        qDebug()<<"guidetime:"<<trackstar.guidetime
                <<"az:"<<trackstar.az_guidedata
                <<"alt:"<<trackstar.alt_guidedata;
    }
    return FALSE;
    //
    //
    // 	if((p = strstr ( pStr,TCSSTATUS))!=NULL)	//	"2.5.0.1.1" 	TcsStatus
    //            {TcsStatus();return TRUE;}
    //
    // 	if((p = strstr ( pStr,AUTOGUIDING))!=NULL)	//    	2.6.0.0.1   	AutoGuiding
    //  	   {AutoGuiding(pStr);return TRUE;}



    // 	p = strstr ( pStr,"2.1.0.1.1" );
    // 	if ( p!=NULL ) //是
    // 	{
    // 		OcsCom.flag_TcsCheckAz=true;
    // 		if ( !IsOpened_direct_azalt )
    // 			startDirectAzalt();
    //
    // 		return TRUE;
    // 	}



    /*
 p = strstr ( pStr,"2.1.0.2.1" );//    2.1.0.2.1   	TcsPointAz
 if ( p!=NULL ) //是
 {


   //wsprintf(MountReturnToTCS,"<ErrorCode=0>\n");
   OcsCom.flag_PointByOCS_AZ=true;
   OcsCom.flag_PointStopByOCS_AZ=false;
   pr_get_commandvalue ( pStr, "Pos", buf_tmp );

   Az.Az_point=atof ( buf_tmp );
   if ( !IsOpened_direct_azalt )
    startDirectAzalt();



  //qDebug() <<"TcsPointing";
  return TRUE;
 }*/


    // 	p = strstr ( pStr,"2.1.0.3.1" );	////////////////////STOP  AZ point////////////////////
    // 	if ( p!=NULL ) //是
    // 	{
    // 		OcsCom.flag_PointStopByOCS_AZ=true;
    // 		OcsCom.flag_PointByOCS_AZ=false;
    // 		if ( !IsOpened_direct_azalt )
    // 			startDirectAzalt();
    // 		return TRUE;
    // 	}



    //////////////////////   ALT            ////////////////////////////////////


    // 	p = strstr ( pStr,"2.2.0.1.1" );//     2.2.0.1.1 :    TcsCheckAlt
    // 	if ( p!=NULL )
    // 	{
    // 		OcsCom.flag_TcsCheckAlt=true;
    // 		if ( !IsOpened_direct_azalt )
    // 			startDirectAzalt();
    // 		qDebug() <<pStr;
    // 		return TRUE;
    // 	}
    //


    //3、2.2.0.2.1  ： ALT  point

    // 	p = strstr ( pStr,"2.2.0.2.1" );
    // 	if ( p!=NULL )
    // 	{
    //
    //
    // 			OcsCom.flag_PointByOCS_ALT=true;
    // 			OcsCom.flag_PointStopByOCS_ALT=false;
    // 			pr_get_commandvalue ( pStr, "Pos", buf_tmp );
    // 			Alt.Alt_point=atof ( buf_tmp );
    //
    // 		//qDebug() <<pStr;
    // 		if ( !IsOpened_direct_azalt )
    // 			startDirectAzalt();
    // 		return TRUE;
    // 	}
    //


    // 	////////////////////STOP  ALT point////////////////////
    // 	p = strstr ( pStr,"2.2.0.3.1" );
    // 	if ( p!=NULL )
    // 	{
    // 		OcsCom.flag_PointStopByOCS_ALT=true;
    // 		OcsCom.flag_PointByOCS_ALT=false;
    // 		if ( !IsOpened_direct_azalt )
    // 			startDirectAzalt();
    // 		return TRUE;
    // 	}


    ///////////////////////////    Derotator                           /////////////////////////////////////////////////////////////////////
    //2.3.0.1.1 TcsCheckDeRot
    // 	p = strstr ( pStr,"2.3.0.1.1" );
    // 	if ( p!=NULL )
    // 	{
    // 		flagtcs.flag_TcsCheckDer=true;
    // 		if ( !flagtcs.IsOpened_direct_dero )
    // 			startDirectDero();
    //
    // 		return TRUE;
    // 	}

    //2.3.0.2.1TcsStartRot
    // 	p = strstr ( pStr,"2.3.0.2.1" );
    // 	if ( p!=NULL )
    // 	{
    //
    // 		flagtcs.flag_PointByOCS_Dero=true;
    // 		flagtcs.flag_PointStopByOCS_Dero=false;
    // 		pr_get_commandvalue ( pStr, "Pos", buf_tmp );// target
    //
    // 		Dero_point=atof ( buf_tmp );
    // //			qDebug() <<Dero_point;
    //
    //
    // 		if ( !flagtcs.IsOpened_direct_dero ) // ，start dialog
    // 			startDirectDero();
    //
    // 		return TRUE;
    // 	}

    //2.3.0.3.1TcsStopRot
    // 	p = strstr ( pStr,"2.3.0.3.1" );
    // 	if ( p!=NULL )
    // 	{
    //
    // 		flagtcs.flag_PointByOCS_Dero=false;
    // 		flagtcs.flag_PointStopByOCS_Dero=true;
    //
    //
    // 		if ( !flagtcs.IsOpened_direct_dero ) // start dialog
    // 			startDirectDero();
    //
    // 		return TRUE;
    // 	}
    //

    /////////////////////////start tracking//////////////////////////////////////
    // 	p = strstr ( pStr,"2.4.0.1.1" );
    // 	if ( p!=NULL )
    // 	{
    // 		flagtcs.flag_TrackStarByOCS_MA=true;
    // 		flagtcs.flag_TrackStarStopByOCS_MA=false;
    // 		if ()
    // 			startTrackingOCS();
    //
    // 		return TRUE;
    // 	}

    ///////////////////////stop tracking//////////////////////////////////////////////////
    // 	p = strstr ( pStr,"2.4.0.2.1" );
    // 	if ( p!=NULL )
    // 	{
    // 		flagtcs.flag_TrackStarByOCS_MA=false;
    // 		flagtcs.flag_TrackStarStopByOCS_MA=true;
    // 		if ( !flagtcs.IsOpened_tracking )
    // 			startTrackingOCS();
    // 		return TRUE;
    // 	}


    /////////////////////////CCD  guiding star/////////////////////////////////////
    // 	p = strstr ( pStr,"2.6.0.0.1" );
    // 	if ( p!=NULL )
    // 	{
    // 		pr_get_commandvalue ( pStr, "Azimuth", buf_Az );
    // 		pr_get_commandvalue ( pStr, "Altitude", buf_Alt );
    //
    // 		Az_guidedata=Az_guidedata+atof ( buf_Az );
    // 		Alt_guidedata=Alt_guidedata+atof ( buf_Alt );
    // 		if ( !flagtcs.IsOpened_tracking )
    // 			startTrackingOCS();
    //
    // 		return TRUE;
    // 	}
}
////////////////////////////////////////////////////////////////////////////////
void MainWindow::TcsEnableAz()  			//     2.1.0.1.1 :    TcsCheckAz
{
    OcsCom.flag_TcsEnableAz=true;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}

void MainWindow::TcsDisableAz()  			//     2.1.0.1.1 :    TcsCheckAz
{
    OcsCom.flag_TcsDisableAz=true;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}

void MainWindow::TcsCheckAz()  			//     2.1.0.1.1 :    TcsCheckAz
{
    OcsCom.flag_TcsCheckAz=true;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}
void MainWindow::TcsPointAz(char* pStr) 	//    2.1.0.2.1   	TcsPointAz 
{
    char buf_tmp[100];
    OcsCom.flag_PointByOCS_AZ=true;
    OcsCom.flag_PointStopByOCS_AZ=false;
    pr_get_commandvalue ( pStr, "Pos", buf_tmp );
    s_Az.point=atof ( buf_tmp );
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}
void MainWindow::TcsStopAz() 
{
    OcsCom.flag_PointStopByOCS_AZ=true;
    OcsCom.flag_PointByOCS_AZ=false;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}

void MainWindow::TcsHomeAz()			//	2.1.0.3.1 	TcsHomeAz 
{
    OcsCom.flag_TcsHomeAz=true;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::TcsEnableAlt()
{
    OcsCom.flag_TcsEnableAlt=true;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}

void MainWindow::TcsDisableAlt()  			//
{
    OcsCom.flag_TcsDisableAlt=true;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}

void MainWindow::TcsCheckAlt()			//
{
    OcsCom.flag_TcsCheckAlt=true;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}
void MainWindow::TcsPointAlt(char* pStr)	//
{
    char buf_tmp[100];
    OcsCom.flag_PointByOCS_ALT=true;
    OcsCom.flag_PointStopByOCS_ALT=false;
    pr_get_commandvalue ( pStr, "Pos", buf_tmp );
    s_Alt.point=atof ( buf_tmp );
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();

}

void MainWindow::TcsStopAlt()	 		//
{
    OcsCom.flag_PointStopByOCS_ALT=true;
    OcsCom.flag_PointByOCS_ALT=false;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}

void MainWindow::TcsHomeAlt()			//
{
    OcsCom.flag_TcsHomeAlt=true;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}

void MainWindow::TcsAltLock()			//
{
    OcsCom.flag_TcsLockAlt=true;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}

void MainWindow::TcsAltUnlock()			//
{
    OcsCom.flag_TcsUnlockAlt=true;
    if ( !flagtcs.IsOpened_azalt  )
        startAzalt();
}

////////////////////////////////////////////////////////////////////////////////
void MainWindow::TcsEnableDero()  			//
{
    OcsCom.flag_TcsEnableDero=true;
    if ( !flagtcs.IsOpened_dero )
        startDero();
}

void MainWindow::TcsDisableDero()  			//
{
    OcsCom.flag_TcsDisableDero=true;
    if ( !flagtcs.IsOpened_dero )
        startDero();
}

void MainWindow::TcsCheckDero()		//
{
    OcsCom.flag_TcsCheckDero=true;
    if ( !flagtcs.IsOpened_dero )
        startDero();
}

void MainWindow::TcsPointDero(char* pStr)	//
{
    char buf_tmp[100];
    OcsCom.flag_PointByOCS_Dero=true;
    OcsCom.flag_PointStopByOCS_Dero=false;
    pr_get_commandvalue ( pStr, "Pos", buf_tmp );// target
    statusDero.point=atof ( buf_tmp );
    if ( !flagtcs.IsOpened_dero ) 	// start dialog
        startDero();
}
void MainWindow::TcsStopDero()	 		//
{
    OcsCom.flag_PointByOCS_Dero=false;
    OcsCom.flag_PointStopByOCS_Dero=true;
    if ( !flagtcs.IsOpened_dero ) 	// start dialog
        startDero();
}

void MainWindow::TcsHomeDero()			//
{
    OcsCom.flag_TcsHomeDero=true;
    if ( !flagtcs.IsOpened_dero  )
        startDero();
}

void MainWindow::TcsCoverOpen()			//
{
    OcsCom.flag_TcsOpenCover=true;
    if ( !flagtcs.IsOpened_dero )
        startDero();
}
void MainWindow::TcsCoverClose()			//
{
    OcsCom.flag_TcsCloseCover=true;
    if ( !flagtcs.IsOpened_dero )
        startDero();
}

void MainWindow::TcsCoverStop()			//
{
    OcsCom.flag_TcsStopCover=true;
    if ( !flagtcs.IsOpened_dero )
        startDero();
}

/////////////////////////////////////////////////////////////////////////////////
void  MainWindow::TcsEnableM3()
{
    OcsCom.flag_TcsEnableM3=true;
    if ( !flagtcs.IsOpened_dero )
        startDero();
}

void  MainWindow::TcsDisableM3()
{
    OcsCom.flag_TcsDisableM3=true;
    if ( !flagtcs.IsOpened_dero )
        startDero();
}

void  MainWindow::TcsCheckM3()
{
    OcsCom.flag_TcsCheckM3=true;
    if ( !flagtcs.IsOpened_dero )
        startDero();
}

void  MainWindow::TcsPointM3T(char* pStr)
{
    char buf_tmp[100];
    OcsCom.flag_PointByOCS_M3T=true;
    OcsCom.flag_PointStopByOCS_M3T=false;
    pr_get_commandvalue ( pStr, "Pos", buf_tmp );// target
    statusM3.point=atof ( buf_tmp );
    if ( !flagtcs.IsOpened_dero ) 	// start dialog
        startDero();
}

void  MainWindow::TcsStopM3T()
{
    OcsCom.flag_PointByOCS_M3T=false;
    OcsCom.flag_PointStopByOCS_M3T=true;
    if ( !flagtcs.IsOpened_dero ) 	// start dialog
        startDero();
}

void  MainWindow::TcsHomeM3T()
{
    OcsCom.flag_TcsHomeM3=true;
    if ( !flagtcs.IsOpened_dero  )
        startDero();
}

void  MainWindow::TcsLockM3T()
{
    OcsCom.flag_TcsLockM3T=true;
    OcsCom.flag_TcsUnlockM3T=false;

    if ( !flagtcs.IsOpened_dero  )
        startDero();
}

void  MainWindow::TcsUnLockM3T()
{
    OcsCom.flag_TcsUnlockM3T=true;
    OcsCom.flag_TcsLockM3T=false;
    if ( !flagtcs.IsOpened_dero  )
        startDero();
}

void  MainWindow::TcsPointM3R(char* pStr)
{
    char buf_tmp[100];
    OcsCom.flag_PointByOCS_M3R=true;
    OcsCom.flag_PointStopByOCS_M3R=false;
    pr_get_commandvalue ( pStr, "Pos", buf_tmp );// target
    statusM3Rot.point=atof ( buf_tmp );
    if ( !flagtcs.IsOpened_dero ) 	// start dialog
        startDero();
}

void  MainWindow::TcsStopM3R()
{
    OcsCom.flag_PointByOCS_M3R=false;
    OcsCom.flag_PointStopByOCS_M3R=true;
    if ( !flagtcs.IsOpened_dero ) 	// start dialog
        startDero();
}

void  MainWindow::TcsPointFL(char* pStr)
{
    char buf_tmp[100];
    OcsCom.flag_PointByOCS_FL=true;
    OcsCom.flag_PointStopByOCS_FL=false;
    if(pr_get_commandvalue ( pStr, "Pos", buf_tmp )==1)// target
        Fiber_Light_pos=atof ( buf_tmp );

    if ( !flagtcs.IsOpened_dero ) 	// start dialog
        startDero();
}

void  MainWindow::TcsStopFL()
{
    OcsCom.flag_PointByOCS_FL=false;
    OcsCom.flag_PointStopByOCS_FL=true;
    if ( !flagtcs.IsOpened_dero ) 	// start dialog
        startDero();
}

//////////////////////////////////////////////////////////////////////////////
void MainWindow::TcsEnableADC1()
{
    OcsCom.flag_TcsEnableADC1=true;
    if ( !flagtcs.IsOpened_adc )
        startADC();
}

void MainWindow::TcsDisableADC1()
{
    OcsCom.flag_TcsDisableADC1=true;
    if ( !flagtcs.IsOpened_adc )
        startADC();
}

void MainWindow::TcsCheckADC1()
{
    OcsCom.flag_TcsCheckADC1=true;
    if ( !flagtcs.IsOpened_adc )
        startADC();
}

void MainWindow::TcsPointADC1(char* pStr)
{
    char buf_tmp[100];
    OcsCom.flag_PointByOCS_ADC1=true;
    OcsCom.flag_PointStopByOCS_ADC1=false;
    pr_get_commandvalue ( pStr, "Pos", buf_tmp );// target
    statusADC1.point=atof ( buf_tmp );
    if ( !flagtcs.IsOpened_adc )
        startADC();
}

void MainWindow::TcsStopADC1()
{
    OcsCom.flag_PointStopByOCS_ADC1=true;
    if ( !flagtcs.IsOpened_adc )
        startADC();
}

void MainWindow::TcsHomeADC1()			//
{
    OcsCom.flag_TcsHomeADC1=true;
    if ( !flagtcs.IsOpened_adc  )
        startADC();
}
////////////////////////////////////////////////////////////////////////////////
void MainWindow::TcsEnableADC2()
{
    OcsCom.flag_TcsEnableADC2=true;
    if ( !flagtcs.IsOpened_adc )
        startADC();
}

void MainWindow::TcsDisableADC2()
{
    OcsCom.flag_TcsDisableADC2=true;
    if ( !flagtcs.IsOpened_adc )
        startADC();
}

void MainWindow::TcsCheckADC2()
{
    OcsCom.flag_TcsCheckADC2=true;
    if ( !flagtcs.IsOpened_adc )
        startADC();
}

void MainWindow::TcsPointADC2(char* pStr)
{
    char buf_tmp[100];
    OcsCom.flag_PointByOCS_ADC2=true;
    OcsCom.flag_PointStopByOCS_ADC2=false;
    pr_get_commandvalue ( pStr, "Pos", buf_tmp );// target
    statusADC2.point=atof ( buf_tmp );
    if ( !flagtcs.IsOpened_adc )
        startADC();
}

void MainWindow::TcsStopADC2()
{
    OcsCom.flag_PointStopByOCS_ADC2=true;
    if ( !flagtcs.IsOpened_adc )
        startADC();
}

void MainWindow::TcsHomeADC2()			//
{
    OcsCom.flag_TcsHomeADC2=true;
    if ( !flagtcs.IsOpened_adc  )
        startADC();
}

/////////////////////////////////////////////////////////////////////////////////
void MainWindow::TcsStartTracking()		//	2.6.0.1.1 	TcsStartTracking
{
    flagtcs.flag_TrackStarByOCS_MA=true;
    flagtcs.flag_TrackStarStopByOCS_MA=false;
    if ( !flagtcs.IsOpened_tracking )
        startTrackingOCS();
}
void MainWindow::TcsStopTracking()		//	2.6.0.2.1 	TcsStopTracking
{
    flagtcs.flag_TrackStarByOCS_MA=false;
    flagtcs.flag_TrackStarStopByOCS_MA=true;
    if ( !flagtcs.IsOpened_tracking )
        startTrackingOCS();
}

void MainWindow::TcsGetPosition()			//
{
    flagtcs.flag_GetTrackingPosByOCS_MA=true;
    if ( !flagtcs.IsOpened_tracking )
        startTrackingOCS();
    /*OcsCom.flag_TcsGetAzAltPosition=true;
    OcsCom.flag_TcsGetDeroPosition=true;

    if ( !flagtcs.IsOpened_dero ) 	// start dialog
        startDero();
    if ( !flagtcs.IsOpened_azalt )
        startAzalt();*/

}

void MainWindow::AutoGuiding(char* pStr)	//
{
    pr_get_commandvalue ( pStr, "Azimuth", buf_Az );
    pr_get_commandvalue ( pStr, "Altitude", buf_Alt );
    pr_get_commandvalue ( pStr, "Dero", buf_Dero );
    trackstar.az_guidedata=trackstar.az_guidedata+atof ( buf_Az );
    trackstar.alt_guidedata=trackstar.alt_guidedata+atof ( buf_Alt );
    trackstar.dero_guidedata=trackstar.dero_guidedata+atof ( buf_Dero );
    if ( !flagtcs.IsOpened_tracking )
        startTrackingOCS();
}

/////////////////////////////////////////////////////////////////////////////////
//by zxj---
void MainWindow::TcsStartTerminal(char* pStr)
{
    char buf_tmp[100];
//    OcsCom.flag_PointByOCS_ADC2=true;
//    OcsCom.flag_PointStopByOCS_ADC2=false;
    pr_get_commandvalue ( pStr, "Jog", buf_tmp );// target
    if ( !flagtcs.IsOpened_terminal )
        startTerminal(buf_tmp);
}

/////////////////////////////////////////////////////////////////////////////////
void  MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        iniDockWidget();
    }

}

/*void MainWindow::on_actionDero_triggered()
{

}*/

/*void MainWindow::on_actionM2_triggered()
{
    //qDebug
    dialog_m2 = new Dialog_m2(this);

    //Dialog_m2 w;
    //w.show();
   dialog_m2->show();
   //connect_pi();

}
*/


void MainWindow::on_actionPloter_triggered()
{
//    mplottersetting->show();
}


