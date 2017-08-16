//SimOCSserver.cpp
// Description: just for simulation OCS

#include "SimOCSserver.h"
#include "simOCSserverthread.h"
#include <stdlib.h>

//! [0]
SimOcsServer::SimOcsServer(SimulationOCS *parent)
    : QTcpServer(parent)
{
    parentwindow = parent;

}

void SimOcsServer::incomingConnection(int socketDescriptor)
{

    SimOcsThread *thread = new SimOcsThread(socketDescriptor,parentwindow);
    thread->start();

}
