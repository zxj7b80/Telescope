//
// C++ Interface: terminal
//
// Description: 
//
//
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TERMINAL_H
#define TERMINAL_H
#include <QtGui>
#include "ui_terminal.h"

class MainWindow;
class QGalil;
class Terminal:public QDialog,private Ui::Dialog_terminal
{
    Q_OBJECT
public:
    Terminal(MainWindow *parent);
    ~Terminal();
    //		QGalil *galil;
    QGalil *galil2;
    //by zxj---
    void getJogCmd(char* Jog);
protected:
    virtual void closeEvent ( QCloseEvent *event );
private:
    MainWindow *m_parent;
    void connectDevice();
    void initUi();
    void initUi2();
private slots:
    void updateInterface(std::vector<char> );
    void updateInterface2(std::vector<char> );
    void on_pushButton_clicked();
};
#endif
