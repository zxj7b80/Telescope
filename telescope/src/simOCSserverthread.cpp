//simOCSserverthread.cpp
// Description: just for simulation OCS
#include "simOCSserverthread.h"
#include <QtNetwork>
#include <QDebug>


//! [0]
SimOcsThread::SimOcsThread(int sock,SimulationOCS *parent)
{
    parentwindow = parent;
    tcpSocket = new QTcpSocket(this);
    tcpSocket->setSocketDescriptor(sock);
    connect ( tcpSocket, SIGNAL ( readyRead() ), this, SLOT ( readData() ) );
    connect ( this, SIGNAL ( newMessage ( QString ) ),parentwindow, SLOT ( refreshOCStable ( QString ) ) );

}
//! [0]

//! [1]
void SimOcsThread::run()
{


}

void SimOcsThread::readData()
{
    QString hash;

    QDataStream in ( tcpSocket /*,QIODevice::ReadWrite */ );
    in.setVersion ( QDataStream::Qt_4_0 );

    quint16 block_size;
    if ( tcpSocket->bytesAvailable() < ( int ) sizeof ( quint16 ) ) //You must make sure that enough data is available before attempting to read it using operator>>().
        return ;
    in >> block_size;
    qDebug() << block_size << "bloc";
    qDebug() << tcpSocket->bytesAvailable() << "av";
    if ( tcpSocket->bytesAvailable() < block_size )
        return ;						//some error happen here.

    in >> hash;
    emit newMessage( hash );

    // parentwindow->showInfoOCS(hash);
    qDebug() << hash << "Mount return value";
}
