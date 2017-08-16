//
// C++ Interface: qumac
//
// Description:
//
//
// Author: donglongchao <donglongchao@163.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QObject>
#include <QMutex>
#include "singleton.h"

class UMAC;
class QUMAC:public QObject
{
    DECLARE_SINGLETON_CLASS ( QUMAC )
    Q_OBJECT

public:
    int QcommCmdPMAC ( const char cCommand[],QString& cResponse );
    int QctrlCmdPMAC ( const int cCommand,QString& cResponse );
    int QsendDataDPRAM ( const char cData[],long offset );
    int QgetDataDPRAM ( QString& data,long offset,int data_length );
    void Qclean_close();
protected:
private:
    QUMAC (const std::string& address = IP_UMAC);
    virtual ~QUMAC();

    UMAC *umac;
    QMutex cmd_mutex;
signals:

};
typedef Singleton<QUMAC> SQUMAC;
