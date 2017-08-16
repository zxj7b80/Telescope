#include "fortunethread.h"

#include <QtNetwork>
#include <QDebug>

//! [0]
FortuneThread::FortuneThread(int sock,MainWindow *parent)
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket->setSocketDescriptor(sock);
    connect ( tcpSocket, SIGNAL ( readyRead() ), this, SLOT ( readData() ) );
    parentwindow = parent;
    qDebug() << "new thread.";
}
//! [0]

//! [1]
void FortuneThread::run()
{


}

void FortuneThread::readData()
{
    QString hash;

//    QDataStream in ( tcpSocket /*,QIODevice::ReadWrite */ );
//    in.setVersion ( QDataStream::Qt_4_0 );

//    quint16 block_size;
//    if ( tcpSocket->bytesAvailable() < ( int ) sizeof ( quint16 ) ) //You must make sure that enough data is available before attempting to read it using operator>>().
//        return ;
//    in >> block_size;
//    qDebug() <<"block size:"<< block_size;
//    qDebug() <<"available bytes:"<<tcpSocket->bytesAvailable();
//    if ( tcpSocket->bytesAvailable() < block_size )
//        return ;//some error happen here.

//    in >> hash;
//    qDebug()<<"ocs command:"<<hash;
//by zxj---------------------
//If write as following,Telnet will success.But,simulationOCS will fail.
//If not,Telnet will fail.
    QByteArray Data = tcpSocket->readAll();
    QString ss = QVariant(Data).toString();
    hash = QString(ss);
//------------------

    parentwindow->showInfoOCS(hash);
    qDebug()<<"ocs command:"<<hash;
}
