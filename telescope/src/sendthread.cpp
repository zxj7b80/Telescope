//sendthread.cpp
#include <QHostAddress>
#include "sendthread.h"
#include <QMessageBox>
#define  TimeWaitForConnected				1000

sendthread::sendthread(MainWindow* parent, QString message,QString ip,int port)
{
    parentwindow = parent;
    mess= message;
    ipaddr = ip;
    i_port = port;
    tcp_socket = new QTcpSocket ( this );
}

void sendthread::run()
{
    qDebug()<<"SendtoOCS:"<<mess;
    tcp_socket->connectToHost (ipaddr,i_port);
    //qDebug()<<"SendtoOCS running "<<ipaddr<<i_port;
    if (!tcp_socket->waitForConnected(TimeWaitForConnected))
    {
        /*QMessageBox::information ( parentwindow, tr ( "Client socket" ),
                                   tr ( "remote host error" )
                                   );*/
        qDebug()<<"Can't reply to OCS, net communication error";
        return;
    }
    qDebug("simulationOCS connect to mainwindow successfully!");

    QByteArray cmdSend;
    QDataStream out(&cmdSend,QIODevice::ReadWrite);
    out.setVersion ( QDataStream::Qt_4_0 );
    out << (quint16)0;
    out << mess;
    out.device()->seek(0);
    out << ( quint16 ) ( cmdSend.size() - sizeof ( quint16 ) );
    if(tcp_socket->write(cmdSend)==-1)
    {
       qDebug()<<"error come";
    }
//    parentwindow->showInfoSend(mess,ipaddr);

    //if ( (tcp_socket->write(cmdSend))==-1)
    //qDebug()<<"SendtoOCS finished "<<ipaddr<<i_port;

    tcp_socket->close();
}

