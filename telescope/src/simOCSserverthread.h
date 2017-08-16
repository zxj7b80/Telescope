//simOCSserverthread.h
// Description: just for simulation OCS
#ifndef SIMOCSSERVERTHREAD_H
#define SIMOCSSERVERTHREAD_H
#include <QThread>
#include <QTcpSocket>
 #include "simulationOCS.h"

//! [0]
class SimOcsThread : public QThread
{
    Q_OBJECT

public:
    SimOcsThread(int sock , SimulationOCS *parent);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);
    void newMessage(QString hash);
private slots:
    void readData();

private:
    int socketDescriptor;
    QString text;
    QTcpSocket *tcpSocket;
    quint16 blockSize;
    SimulationOCS *parentwindow;
};
//! [0]

#endif  
