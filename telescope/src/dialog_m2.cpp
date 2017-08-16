#include "dialog_m2.h"
#include "ui_dialog_m2.h"
#include "signs.h"
#include "fortuneserver.h"
#include "sendthread.h"

Dialog_m2::Dialog_m2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_m2)
{
    ui->setupUi(this);

    ui->lineEdit_x_target->setValidator(new MyDoubleValidator(-20,20,3,this));
    ui->lineEdit_y_target->setValidator(new MyDoubleValidator(-20,20,3,this));
    ui->lineEdit_z_target->setValidator(new MyDoubleValidator(-6.5,6.5,3,this));
    ui->lineEdit_u_target->setValidator(new MyDoubleValidator(-11,11,3,this));
    ui->lineEdit_v_target->setValidator(new MyDoubleValidator(-11,11,3,this));
    ui->lineEdit_vel->setValidator(new MyDoubleValidator(0,10,3,this));
}

Dialog_m2::~Dialog_m2()
{
    delete ui;
}

void Dialog_m2::on_pushButton_getIDN_clicked()
{
    int sockfd = 26;
 //   qDebug()<<"sockfd = "<< sockfd;
    char buffer[256];
    bzero(buffer,256);
    sprintf(buffer, "%s", "*IDN?\n");//
    write(sockfd,buffer,strlen(buffer));
    bzero(buffer,256);
     read(sockfd,buffer,255);
    ui->lineEdit_state->setText (buffer);
}

void Dialog_m2::on_pushButton_getspi_clicked()
{
    int sockfd = 26 ;
    char buffer[256];
    bzero(buffer,256);
    sprintf(buffer, "%s", "SPI?\n");//
    write(sockfd,buffer,strlen(buffer));
    bzero(buffer,256);
     read(sockfd,buffer,255);
    ui->lineEdit_spi->setText (buffer);
}

void Dialog_m2::on_pushButton_ini_clicked()
{
    int sockfd = 26 ;
    char buffer[256];
    bzero(buffer,256);
    sprintf(buffer, "%s", "INI\n");
    write(sockfd,buffer,strlen(buffer));

    on_pushButton_getpos_clicked();
    ui->label_initialize->setStyleSheet ( "background-color: rgb(0, 255, 0)" );
}

void Dialog_m2::on_pushButton_enable_clicked()
{
    int sockfd = 26 ;
    char buffer[256];
    bzero(buffer,256);
    sprintf(buffer, "%s", "SVO 1\n");
    write(sockfd,buffer,strlen(buffer));

    char buffer1[256];
    bzero(buffer1,256);
    sprintf(buffer1, "%s", "SVO?\n");
    write(sockfd,buffer1,strlen(buffer1));
    bzero(buffer1,256);
    read(sockfd,buffer1,255);

    if(atoi(buffer1))
    ui->label_enabled->setStyleSheet ( "background-color: rgb(0, 255, 0)" );
}

void Dialog_m2::on_pushButton_disable_clicked()
{
    int sockfd = 26 ;
    char buffer[256];
    bzero(buffer,256);
    sprintf(buffer, "%s", "SVO 0\n");
    write(sockfd,buffer,strlen(buffer));

    char buffer1[256];
    bzero(buffer1,256);
    sprintf(buffer1, "%s", "SVO?\n");
    write(sockfd,buffer1,strlen(buffer1));
    bzero(buffer1,256);
    read(sockfd,buffer1,255);

    if(!(atoi(buffer1)))
    ui->label_enabled->setStyleSheet ( "background-color: rgb(255, 0, 0)" );
}

void Dialog_m2::on_pushButton_x_go_clicked()
{
    setvel();
    int sockfd = 26 ;
    char *buffer;
    QString target = ui->lineEdit_x_target->text();
    QString temp = QString("MOV X").append(target).append("\n");
    QByteArray ba = temp.toLatin1();
    buffer = ba.data();
    write(sockfd,buffer,strlen(buffer));
    on_pushButton_getpos_clicked();
}

void Dialog_m2::on_pushButton_y_go_clicked()
{
    setvel();
    int sockfd = 26 ;
    char *buffer;
    QString target = ui->lineEdit_y_target->text();
    QString temp = QString("MOV Y").append(target).append("\n");
    QByteArray ba = temp.toLatin1();
    buffer = ba.data();
    write(sockfd,buffer,strlen(buffer));
    on_pushButton_getpos_clicked();
}

void Dialog_m2::on_pushButton_z_go_clicked()
{
    setvel();
    int sockfd = 26 ;
    char *buffer;
    QString target = ui->lineEdit_z_target->text();
    QString temp = QString("MOV Z").append(target).append("\n");
    QByteArray ba = temp.toLatin1();
    buffer = ba.data();
    write(sockfd,buffer,strlen(buffer));
    on_pushButton_getpos_clicked();
}

void Dialog_m2::on_pushButton_u_go_clicked()
{
    setvel();
    int sockfd = 26 ;
    char *buffer;
    QString target = ui->lineEdit_u_target->text();
    QString temp = QString("MOV U").append(target).append("\n");
    QByteArray ba = temp.toLatin1();
    buffer = ba.data();
    write(sockfd,buffer,strlen(buffer));
    on_pushButton_getpos_clicked();
}

void Dialog_m2::on_pushButton_v_go_clicked()
{
    setvel();
    int sockfd = 26 ;
    char *buffer;
    QString target = ui->lineEdit_v_target->text();
    QString temp = QString("MOV V").append(target).append("\n");
    QByteArray ba = temp.toLatin1();
    buffer = ba.data();
    write(sockfd,buffer,strlen(buffer));
    on_pushButton_getpos_clicked();
}

void Dialog_m2::on_pushButton_all_go_clicked()
{
    setvel();
    int sockfd = 26 ;
    char *buffer;
    QString target_x = ui->lineEdit_x_target->text();
    QString target_y = ui->lineEdit_y_target->text();
    QString target_z = ui->lineEdit_z_target->text();
    QString target_u = ui->lineEdit_u_target->text();
    QString target_v = ui->lineEdit_v_target->text();
    QString temp = QString("MOV X").append(target_x).append(" Y").append(target_y).append(" Z").append(target_z).append(" U").append(target_u).append(" V").append(target_v).append("\n");
    QByteArray ba = temp.toLatin1();
    buffer = ba.data();
    write(sockfd,buffer,strlen(buffer));
    on_pushButton_getpos_clicked();
}

void Dialog_m2::on_pushButton_all_stop_clicked()
{
    int sockfd = 26 ;
    char buffer[256];
    bzero(buffer,256);
    sprintf(buffer, "%s", "STOP\n");
    write(sockfd,buffer,strlen(buffer));
    on_pushButton_getpos_clicked();
}

void Dialog_m2::on_pushButton_getpos_clicked()
{
    getpos_x();
    getpos_y();
    getpos_z();
    getpos_u();
    getpos_v();
//    int sockfd = 26 ;
//    char buffer[256];
//    bzero(buffer,256);
//    sprintf(buffer, "%s", "POS? X\n");
//    write(sockfd,buffer,strlen(buffer));
//    bzero(buffer,256);
//    read(sockfd,buffer,255);

//    ui->lineEdit_pos->setText (buffer);
//    QString s;
//    for (int i=0;i<4;i++)
//    {   for (int j=0;j<6;j++)
//            {
//                s.append(buffer[11*i+j+3]);
//            }
//        if (i==0){ui->lineEdit_x_pos->setText(s);}
//        if (i==1){ui->lineEdit_y_pos->setText(s);}
//        if (i==2){ui->lineEdit_z_pos->setText(s);}
//        if (i==3){ui->lineEdit_u_pos->setText(s);}
//        if (i==4){ui->lineEdit_v_pos->setText(s);}
//        s.clear();
//    }
//    bzero(buffer,256);
}
 void Dialog_m2::getpos_x()
 {
     int sockfd = 26 ;
     char buffer[256];
     bzero(buffer,256);
     sprintf(buffer, "%s", "POS? X\n");
     write(sockfd,buffer,strlen(buffer));
     bzero(buffer,256);
     read(sockfd,buffer,255);
     QString s;
     for (int i=0;i<10;i++)
        {
            s.append(buffer[i+3]);
        }
     ui->lineEdit_x_pos->setText(s);
 }

 void Dialog_m2::getpos_y()
 {
     int sockfd = 26 ;
     char buffer[256];
     bzero(buffer,256);
     sprintf(buffer, "%s", "POS? Y\n");
     write(sockfd,buffer,strlen(buffer));
     bzero(buffer,256);
     read(sockfd,buffer,255);
     QString s;
     for (int i=0;i<10;i++)
        {
            s.append(buffer[i+3]);
        }
     ui->lineEdit_y_pos->setText(s);
 }

 void Dialog_m2::getpos_z()
 {
     int sockfd = 26 ;
     char buffer[256];
     bzero(buffer,256);
     sprintf(buffer, "%s", "POS? Z\n");
     write(sockfd,buffer,strlen(buffer));
     bzero(buffer,256);
     read(sockfd,buffer,255);
     QString s;
     for (int i=0;i<10;i++)
        {
            s.append(buffer[i+3]);
        }
     ui->lineEdit_z_pos->setText(s);
 }

 void Dialog_m2::getpos_u()
 {
     int sockfd = 26 ;
     char buffer[256];
     bzero(buffer,256);
     sprintf(buffer, "%s", "POS? U\n");
     write(sockfd,buffer,strlen(buffer));
     bzero(buffer,256);
     read(sockfd,buffer,255);
     QString s;
     for (int i=0;i<10;i++)
        {
            s.append(buffer[i+3]);
        }
     ui->lineEdit_u_pos->setText(s);
 }

 void Dialog_m2::getpos_v()
 {
     int sockfd = 26 ;
     char buffer[256];
     bzero(buffer,256);
     sprintf(buffer, "%s", "POS? V\n");
     write(sockfd,buffer,strlen(buffer));
     bzero(buffer,256);
     read(sockfd,buffer,255);
     QString s;
     for (int i=0;i<10;i++)
        {
            s.append(buffer[i+3]);
        }
     ui->lineEdit_v_pos->setText(s);
 }

void Dialog_m2::setvel()
{
    int sockfd = 26 ;
    char *buffer;
    QString vel = ui->lineEdit_vel->text();
    QString temp = QString("VEL ").append(vel).append("\n");
    QByteArray ba = temp.toLatin1();
    buffer = ba.data();
    write(sockfd,buffer,strlen(buffer));
}

void Dialog_m2::on_pushButton_setspi_clicked()
{
    int sockfd = 26 ;
    char *buffer;
    QString spi_x = ui->lineEdit_x_spi->text();
    QString spi_y = ui->lineEdit_y_spi->text();
    QString spi_z = ui->lineEdit_z_spi->text();
    QString temp = QString("SPI R").append(spi_x).append(" S").append(spi_y).append(" T").append(spi_z).append("\n");
    QByteArray ba = temp.toLatin1();
    buffer = ba.data();
    write(sockfd,buffer,strlen(buffer));

    on_pushButton_getspi_clicked();
}
