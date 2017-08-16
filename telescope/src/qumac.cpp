//
// C++ Implementation: qumac

//
#include "signs.h"
#include "UMAC.h"
extern bool UMAC_ETHERNET;

QUMAC::QUMAC ( const std::string& address )
{
    umac = new UMAC ( address );
    //	timer_directazalt = new QTimer;
    //	connect ( timer_directazalt,SIGNAL ( timeout() ),this,SLOT ( throwUMACInfo() ) );
    //timer_directazalt->start ( 100 );
    qDebug("New QUMAC");
}

QUMAC::~QUMAC()
{
    //timer_directazalt->deleteLater();
    delete umac;
    qDebug("Delete QUMAC");
}

void QUMAC::QcommCmdPMAC ( const char cCommand[],QString& cResponse )
{
    QMutexLocker locker ( &cmd_mutex );
    if ( umac->commCmdPMAC ( cCommand,cResponse ) == -1 )
        throw  QString ( strerror ( errno ) ) +"Net connection to UMAC error!";
}

void QUMAC::QctrlCmdPMAC ( const int cCommand,QString& cResponse )
{
    QMutexLocker locker ( &cmd_mutex );
    if ( umac->ctrlCmdPMAC ( cCommand,cResponse ) == -1 )
        throw  QString ( strerror ( errno ) ) +"Net connection to UMAC error!";
}
/*
void QUMAC::QsendDataDPRAM ( const char cData[],long offset )
{
 //QMutexLocker locker ( &cmd_mutex );
 if ( umac->sendDataDPRAM ( cData,offset ) == -1 )
  throw  QString ( strerror ( errno ) ) +"Net connection to UMAC error!" ;
}

void QUMAC::QgetDataDPRAM ( QString& data,long offset,int data_length )
{
 QMutexLocker locker ( &cmd_mutex );
 if ( umac->getDataDPRAM ( data,offset,data_length ) == -1 )
  throw  QString ( strerror ( errno ) ) +"Net connection to UMAC error!" ;
}*/


//add
//////////////float/////////
void QUMAC::QsendDataDPRAM (const float fData[], uint16_t offset, int iDatalength)
{
    QMutexLocker locker (&cmd_mutex);
    if(umac -> sendDataDPRAM( fData, offset,iDatalength) == -1)
        throw QString ( strerror ( errno) ) + "Net connection to UMAC error";
}

void QUMAC::QgetDataDPRAM (float* data, uint16_t offset, int length)
{
    QMutexLocker locker (&cmd_mutex);
    if(umac -> getDataDPRAM ( data, offset, length) == -1)
        throw QString ( strerror (errno)) + "Net connection to UMAC error!" ;
}

///////////////////int///////////////
void QUMAC::QsendDataDPRAM (const int iData[], uint16_t offset, int iDatalength)
{
    QMutexLocker locker (&cmd_mutex);
    if(umac -> sendDataDPRAM( iData, offset,iDatalength) == -1)
        throw QString ( strerror ( errno)) + "Net connection to UMAC error";
}

void QUMAC::QgetDataDPRAM (int* data, uint16_t offset, int length)
{
    QMutexLocker Locker (&cmd_mutex);
    if(umac -> getDataDPRAM ( data, offset,length) == -1)
        throw QString (strerror (errno) )+ "Net connection to UMAC error";
}

//add
void QUMAC::QsendDataDPRAM ( const char cData[],uint16_t offset )
{
    //QMutexLocker locker ( &cmd_mutex );
    if ( umac->sendDataDPRAM ( cData,offset ) == -1 )
        throw  QString ( strerror ( errno ) ) +"Net connection to UMAC error!" ;
}

void QUMAC::QgetDataDPRAM ( QString& data,uint16_t offset,int data_length )
{
    QMutexLocker locker ( &cmd_mutex );
    if ( umac->getDataDPRAM ( data,offset,data_length ) == -1 )
        throw  QString ( strerror ( errno ) ) +"Net connection to UMAC error!" ;
}

void QUMAC::throwUMACInfo()
{
    QHash<QString,QString> map;
    QString info;
    //QString cmd = "m161 m162 m164 m165 m174 m261 m262 m264 m265 m274 m142 m242 m342 m141 m241 m341 m154 m254 m354 m138 m238 m338  m461 m462 m464 m465 m474";
    QString cmd = "m161 m162 m164 m165 m174 m261 m262 m264 m265 m274 m142 m242 m342 m141 m241 m341 m154 m254 m354 m138 m238 m338 m461 m462 m46 4m465 m474";
    //QString cmd = "m161 m162 m164 m165 m174 m261 m262 m264 m265 m274 m142 m242 m342 m141 m241 m341 m154 m254 m354 m138 m238 m338 ";
    try
    {
        QcommCmdPMAC ( cmd.toAscii(),info );
    }
    catch ( QString excepts )
    {
        if(UMAC_ETHERNET)
        {	QMessageBox mb ( QMessageBox::Warning,
                                 tr ( "About to close" ),
                                 tr ( ( excepts+"\n PLease close  this subwindow and check your connection and open this subwindow again." ).toAscii() ),
                                 QMessageBox::Ok );

            //                  UMAC_ETHERNET=false;
            //                    qDebug()<<"UMAC_ETHERNET=false";

            if ( mb.exec() ==QMessageBox::Ok )
            {
                timer_directazalt->stop();
                timer_directazalt->deleteLater();
                delete umac;//is this code necessary? yes.
                SQUMAC::release();
                return;
            }
        }
    }

    QStringList cmd_key = cmd.split ( QRegExp ( "\\s+" ) );
    QStringList info_value = info.split ( QRegExp ( "\\s+" ) /*,QString::SkipEmptyParts */);
    if (cmd_key.size() != info_value.size())
    {
        qDebug() << "Info uncomplate";
        return;
    }
    QStringList::iterator i;//STL Style
    QStringList::iterator j;
    for ( j = info_value.begin(),i = cmd_key.begin();i != cmd_key.end()/* && j != info_value.end()*/ ;++j,++i )
        map.insert ( *i,*j );
    emit UMACInfoThrown ( map );
}

void QUMAC::QgetDataDPRAM(long long* data, uint16_t offset, int length)
{
    QMutexLocker Locker(&cmd_mutex);
    if(umac -> getDataDPRAM(data, offset, length) == -1)
        throw QString( strerror (errno)) + "Net connection to UMAC error";
}


///
void QUMAC::Qgetinformation(uint16_t offset,long long & followingError, int & servoCommand, int& servoStatus, int& generalStatus, long long& positionBias, int& filteredActuralVelocity, long long& masterPosition, long long& netActualPosition)
{
    QMutexLocker Locker(&cmd_mutex);
    if(umac -> getinformation(offset, followingError, servoCommand, servoStatus, generalStatus,  positionBias, filteredActuralVelocity,  masterPosition,  netActualPosition) == -1)
        throw QString( strerror (errno)) + "Net connection to UMAC error";

}
