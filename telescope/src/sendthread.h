#ifndef SENDTHREAD_H
#define SENDTHREAD_H


#include <QThread>
#include <QObject>
#include "main_window.h"
#include <QtGui>
#include <QtNetwork>

class sendthread : public QThread
{

public:
    sendthread(MainWindow* parent, QString message,QString ip,int port);
    void run();

private:
    MainWindow* parentwindow;
    QString mess;
    QString ipaddr;
    int     i_port;
    QTcpSocket *tcp_socket;
};

#endif // SENDTHREAD_H
