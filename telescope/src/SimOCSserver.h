//    SimOCSserver.h

//SimOCSserver.h
// Description: just for simulation OCS

#ifndef SIMOCSSERVER_H
#define SIMOCSSERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QtNetwork>
#include "simulationOCS.h"

class SimulationOCS;
//! [0]
class SimOcsServer : public QTcpServer
{
    Q_OBJECT

public:
    SimOcsServer(SimulationOCS *parent = 0);

protected:
    void incomingConnection(int socketDescriptor);


private:
    QStringList fortunes;
    int sock;
    QTcpSocket *tcpSocket;
    SimulationOCS* parentwindow;
};
//! [0]


#endif  
