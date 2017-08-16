#ifndef DIALOG_M2_H
#define DIALOG_M2_H

#include <QDialog>
#include<QTime>
//#include <QApplication>
#include <QtCore/QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <QDebug>

class QTcpSocket;
class QMessageBox;
class QDataStream;

namespace Ui {
    class Dialog_m2;
}

class Dialog_m2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_m2(QWidget *parent = 0);
    ~Dialog_m2();
 //   void connect_pi();


private slots:
    void on_pushButton_getIDN_clicked();
    void on_pushButton_getspi_clicked();
    void on_pushButton_ini_clicked();
    void on_pushButton_enable_clicked();
    void on_pushButton_disable_clicked();
    void on_pushButton_x_go_clicked();
    void on_pushButton_y_go_clicked();
    void on_pushButton_z_go_clicked();
    void on_pushButton_u_go_clicked();
    void on_pushButton_v_go_clicked();
    void on_pushButton_all_go_clicked();
    void on_pushButton_all_stop_clicked();
    void on_pushButton_getpos_clicked();
    void on_pushButton_setspi_clicked();
    void setvel();
    void getpos_x();
    void getpos_y();
    void getpos_z();
    void getpos_u();
    void getpos_v();

private:
    Ui::Dialog_m2 *ui;

};

#endif // DIALOG_M2_H
