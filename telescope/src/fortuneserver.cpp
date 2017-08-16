#include "fortuneserver.h"
#include "fortunethread.h"

#include <stdlib.h>

//! [0]
FortuneServer::FortuneServer(MainWindow *parent)
    : QTcpServer(parent)
{
    parentwindow = parent;
}

void FortuneServer::incomingConnection(int socketDescriptor)
{

    FortuneThread *thread = new FortuneThread(socketDescriptor,parentwindow);
    thread->start();
    qDebug() << "socketDescriptor:" << socketDescriptor;
}
