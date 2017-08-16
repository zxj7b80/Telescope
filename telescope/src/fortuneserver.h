#ifndef FORTUNESERVER_H
#define FORTUNESERVER_H


#include <QStringList>
#include <QTcpServer>
#include <QtNetwork>
#include "main_window.h"

//! [0]
class FortuneServer : public QTcpServer
{
    Q_OBJECT

public:
    FortuneServer(MainWindow *parent = 0);

protected:
    void incomingConnection(int socketDescriptor);

private:
    QStringList fortunes;
    int sock;
    QTcpSocket *tcpSocket;
    MainWindow* parentwindow;
};
//! [0]


#endif // FORTUNESERVER_H
