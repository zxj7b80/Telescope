#ifndef FORTUNETHREAD_H
#define FORTUNETHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "main_window.h"

//! [0]
class FortuneThread : public QThread
{
    Q_OBJECT

public:
    FortuneThread(int sock , MainWindow *parent);
    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private slots:
    void readData();

private:
    int socketDescriptor;
    QString text;
    QTcpSocket *tcpSocket;
    quint16 blockSize;
    MainWindow *parentwindow;
};
//! [0]

#endif // FORTUNETHREAD_H
