//
// C++ Interface: setup

//
#ifndef SETUP_H
#define SETUP_H

#include "ui_setup.h"
class MainWindow;
class Setup:public QScrollArea,private Ui::Setup
{
    Q_OBJECT
public:
    Setup(MainWindow *parent);
    ~Setup();
    MainWindow *m_parent;
    void  closeEvent ( QCloseEvent *event );
private:
    void initUi();
    void setValidator();
private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_SelectStarTableFile_clicked();
    void on_pushButton_SelectModelFile_clicked();
    void on_pushButton_default_clicked();
    void on_pushButton_Dero_cancel_clicked();
    void on_pushButton_Dero_ok_clicked();
    void on_pushButton_Dero_default_clicked();
    void on_pushButton_Misc_ok_clicked();
    void on_pushButton_Misc_default_clicked();
    void on_pushButton_Misc_cancel_clicked();
    void on_pushButton_SelectDataDirectory_clicked();
    void on_pushButton_SelectLogDirectory_clicked();
};
#endif
