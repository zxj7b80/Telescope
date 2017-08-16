/********************************************************************************
** Form generated from reading UI file 'dialog_m2.ui'
**
** Created: Fri Dec 19 17:32:40 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_M2_H
#define UI_DIALOG_M2_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog_m2
{
public:
    QLabel *label_11;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label_24;
    QLabel *label_15;
    QLabel *label;
    QLabel *label_initialize;
    QLabel *label_10;
    QLabel *label_enabled;
    QLineEdit *lineEdit_spi;
    QLineEdit *lineEdit_state;
    QPushButton *pushButton_ini;
    QPushButton *pushButton_getIDN;
    QPushButton *pushButton_enable;
    QPushButton *pushButton_getspi;
    QPushButton *pushButton_disable;
    QPushButton *pushButton_getpos;
    QWidget *widget1;
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QLabel *label_3;
    QLabel *label_8;
    QLabel *label_12;
    QLineEdit *lineEdit_x_pos;
    QLabel *label_5;
    QLineEdit *lineEdit_x_target;
    QLabel *label_14;
    QLineEdit *lineEdit_x_spi;
    QLabel *label_16;
    QPushButton *pushButton_x_go;
    QPushButton *pushButton_setspi;
    QLabel *label_13;
    QLineEdit *lineEdit_y_pos;
    QLabel *label_6;
    QLineEdit *lineEdit_y_target;
    QLabel *label_17;
    QLineEdit *lineEdit_y_spi;
    QLabel *label_19;
    QPushButton *pushButton_y_go;
    QPushButton *pushButton_all_go;
    QLabel *label_23;
    QLineEdit *lineEdit_z_pos;
    QLabel *label_18;
    QLineEdit *lineEdit_z_target;
    QLabel *label_25;
    QLineEdit *lineEdit_z_spi;
    QLabel *label_21;
    QPushButton *pushButton_z_go;
    QPushButton *pushButton_all_stop;
    QLabel *label_31;
    QLineEdit *lineEdit_u_pos;
    QLabel *label_26;
    QLineEdit *lineEdit_u_target;
    QLabel *label_33;
    QPushButton *pushButton_u_go;
    QLabel *label_39;
    QLineEdit *lineEdit_v_pos;
    QLabel *label_34;
    QLineEdit *lineEdit_v_target;
    QLabel *label_41;
    QPushButton *pushButton_v_go;
    QLabel *label_20;
    QLineEdit *lineEdit_vel;
    QLabel *label_22;

    void setupUi(QDialog *Dialog_m2)
    {
        if (Dialog_m2->objectName().isEmpty())
            Dialog_m2->setObjectName(QString::fromUtf8("Dialog_m2"));
        Dialog_m2->resize(851, 523);
        label_11 = new QLabel(Dialog_m2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(300, 40, 281, 31));
        label_11->setTextFormat(Qt::AutoText);
        label_11->setScaledContents(false);
        widget = new QWidget(Dialog_m2);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(100, 80, 631, 125));
        gridLayout = new QGridLayout(widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_24 = new QLabel(widget);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setTextFormat(Qt::AutoText);
        label_24->setScaledContents(false);

        gridLayout->addWidget(label_24, 0, 0, 1, 1);

        label_15 = new QLabel(widget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setTextFormat(Qt::AutoText);
        label_15->setScaledContents(false);

        gridLayout->addWidget(label_15, 1, 0, 1, 1);

        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 0, 1, 1);

        label_initialize = new QLabel(widget);
        label_initialize->setObjectName(QString::fromUtf8("label_initialize"));

        gridLayout->addWidget(label_initialize, 2, 1, 1, 1);

        label_10 = new QLabel(widget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 3, 0, 1, 1);

        label_enabled = new QLabel(widget);
        label_enabled->setObjectName(QString::fromUtf8("label_enabled"));

        gridLayout->addWidget(label_enabled, 3, 1, 1, 1);

        lineEdit_spi = new QLineEdit(widget);
        lineEdit_spi->setObjectName(QString::fromUtf8("lineEdit_spi"));

        gridLayout->addWidget(lineEdit_spi, 1, 1, 1, 3);

        lineEdit_state = new QLineEdit(widget);
        lineEdit_state->setObjectName(QString::fromUtf8("lineEdit_state"));

        gridLayout->addWidget(lineEdit_state, 0, 1, 1, 4);

        pushButton_ini = new QPushButton(widget);
        pushButton_ini->setObjectName(QString::fromUtf8("pushButton_ini"));

        gridLayout->addWidget(pushButton_ini, 2, 2, 1, 1);

        pushButton_getIDN = new QPushButton(widget);
        pushButton_getIDN->setObjectName(QString::fromUtf8("pushButton_getIDN"));

        gridLayout->addWidget(pushButton_getIDN, 3, 2, 1, 1);

        pushButton_enable = new QPushButton(widget);
        pushButton_enable->setObjectName(QString::fromUtf8("pushButton_enable"));

        gridLayout->addWidget(pushButton_enable, 2, 3, 1, 1);

        pushButton_getspi = new QPushButton(widget);
        pushButton_getspi->setObjectName(QString::fromUtf8("pushButton_getspi"));

        gridLayout->addWidget(pushButton_getspi, 3, 3, 1, 1);

        pushButton_disable = new QPushButton(widget);
        pushButton_disable->setObjectName(QString::fromUtf8("pushButton_disable"));

        gridLayout->addWidget(pushButton_disable, 2, 4, 1, 1);

        pushButton_getpos = new QPushButton(widget);
        pushButton_getpos->setObjectName(QString::fromUtf8("pushButton_getpos"));

        gridLayout->addWidget(pushButton_getpos, 3, 4, 1, 1);

        widget1 = new QWidget(Dialog_m2);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(100, 220, 631, 211));
        gridLayout_2 = new QGridLayout(widget1);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(widget1);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 1, 1, 1);

        label_3 = new QLabel(widget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 3, 1, 1);

        label_8 = new QLabel(widget1);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 0, 5, 1, 1);

        label_12 = new QLabel(widget1);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setTextFormat(Qt::AutoText);
        label_12->setScaledContents(false);

        gridLayout_2->addWidget(label_12, 1, 0, 1, 1);

        lineEdit_x_pos = new QLineEdit(widget1);
        lineEdit_x_pos->setObjectName(QString::fromUtf8("lineEdit_x_pos"));
        lineEdit_x_pos->setCursor(QCursor(Qt::ArrowCursor));
        lineEdit_x_pos->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_x_pos, 1, 1, 1, 1);

        label_5 = new QLabel(widget1);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 1, 2, 1, 1);

        lineEdit_x_target = new QLineEdit(widget1);
        lineEdit_x_target->setObjectName(QString::fromUtf8("lineEdit_x_target"));

        gridLayout_2->addWidget(lineEdit_x_target, 1, 3, 1, 1);

        label_14 = new QLabel(widget1);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_2->addWidget(label_14, 1, 4, 1, 1);

        lineEdit_x_spi = new QLineEdit(widget1);
        lineEdit_x_spi->setObjectName(QString::fromUtf8("lineEdit_x_spi"));

        gridLayout_2->addWidget(lineEdit_x_spi, 1, 5, 1, 1);

        label_16 = new QLabel(widget1);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_2->addWidget(label_16, 1, 6, 1, 1);

        pushButton_x_go = new QPushButton(widget1);
        pushButton_x_go->setObjectName(QString::fromUtf8("pushButton_x_go"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_x_go->sizePolicy().hasHeightForWidth());
        pushButton_x_go->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(pushButton_x_go, 1, 7, 1, 1);

        pushButton_setspi = new QPushButton(widget1);
        pushButton_setspi->setObjectName(QString::fromUtf8("pushButton_setspi"));

        gridLayout_2->addWidget(pushButton_setspi, 1, 8, 1, 1);

        label_13 = new QLabel(widget1);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setTextFormat(Qt::AutoText);
        label_13->setScaledContents(false);

        gridLayout_2->addWidget(label_13, 2, 0, 1, 1);

        lineEdit_y_pos = new QLineEdit(widget1);
        lineEdit_y_pos->setObjectName(QString::fromUtf8("lineEdit_y_pos"));
        lineEdit_y_pos->setCursor(QCursor(Qt::ArrowCursor));
        lineEdit_y_pos->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_y_pos, 2, 1, 1, 1);

        label_6 = new QLabel(widget1);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 2, 2, 1, 1);

        lineEdit_y_target = new QLineEdit(widget1);
        lineEdit_y_target->setObjectName(QString::fromUtf8("lineEdit_y_target"));

        gridLayout_2->addWidget(lineEdit_y_target, 2, 3, 1, 1);

        label_17 = new QLabel(widget1);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_2->addWidget(label_17, 2, 4, 1, 1);

        lineEdit_y_spi = new QLineEdit(widget1);
        lineEdit_y_spi->setObjectName(QString::fromUtf8("lineEdit_y_spi"));

        gridLayout_2->addWidget(lineEdit_y_spi, 2, 5, 1, 1);

        label_19 = new QLabel(widget1);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_2->addWidget(label_19, 2, 6, 1, 1);

        pushButton_y_go = new QPushButton(widget1);
        pushButton_y_go->setObjectName(QString::fromUtf8("pushButton_y_go"));
        sizePolicy.setHeightForWidth(pushButton_y_go->sizePolicy().hasHeightForWidth());
        pushButton_y_go->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(pushButton_y_go, 2, 7, 1, 1);

        pushButton_all_go = new QPushButton(widget1);
        pushButton_all_go->setObjectName(QString::fromUtf8("pushButton_all_go"));
        sizePolicy.setHeightForWidth(pushButton_all_go->sizePolicy().hasHeightForWidth());
        pushButton_all_go->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(pushButton_all_go, 2, 8, 1, 1);

        label_23 = new QLabel(widget1);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setTextFormat(Qt::AutoText);
        label_23->setScaledContents(false);

        gridLayout_2->addWidget(label_23, 3, 0, 1, 1);

        lineEdit_z_pos = new QLineEdit(widget1);
        lineEdit_z_pos->setObjectName(QString::fromUtf8("lineEdit_z_pos"));
        lineEdit_z_pos->setCursor(QCursor(Qt::ArrowCursor));
        lineEdit_z_pos->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_z_pos, 3, 1, 1, 1);

        label_18 = new QLabel(widget1);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout_2->addWidget(label_18, 3, 2, 1, 1);

        lineEdit_z_target = new QLineEdit(widget1);
        lineEdit_z_target->setObjectName(QString::fromUtf8("lineEdit_z_target"));

        gridLayout_2->addWidget(lineEdit_z_target, 3, 3, 1, 1);

        label_25 = new QLabel(widget1);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_2->addWidget(label_25, 3, 4, 1, 1);

        lineEdit_z_spi = new QLineEdit(widget1);
        lineEdit_z_spi->setObjectName(QString::fromUtf8("lineEdit_z_spi"));

        gridLayout_2->addWidget(lineEdit_z_spi, 3, 5, 1, 1);

        label_21 = new QLabel(widget1);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_2->addWidget(label_21, 3, 6, 1, 1);

        pushButton_z_go = new QPushButton(widget1);
        pushButton_z_go->setObjectName(QString::fromUtf8("pushButton_z_go"));
        sizePolicy.setHeightForWidth(pushButton_z_go->sizePolicy().hasHeightForWidth());
        pushButton_z_go->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(pushButton_z_go, 3, 7, 1, 1);

        pushButton_all_stop = new QPushButton(widget1);
        pushButton_all_stop->setObjectName(QString::fromUtf8("pushButton_all_stop"));
        sizePolicy.setHeightForWidth(pushButton_all_stop->sizePolicy().hasHeightForWidth());
        pushButton_all_stop->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(pushButton_all_stop, 3, 8, 1, 1);

        label_31 = new QLabel(widget1);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setTextFormat(Qt::AutoText);
        label_31->setScaledContents(false);

        gridLayout_2->addWidget(label_31, 4, 0, 1, 1);

        lineEdit_u_pos = new QLineEdit(widget1);
        lineEdit_u_pos->setObjectName(QString::fromUtf8("lineEdit_u_pos"));
        lineEdit_u_pos->setCursor(QCursor(Qt::ArrowCursor));
        lineEdit_u_pos->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_u_pos, 4, 1, 1, 1);

        label_26 = new QLabel(widget1);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_2->addWidget(label_26, 4, 2, 1, 1);

        lineEdit_u_target = new QLineEdit(widget1);
        lineEdit_u_target->setObjectName(QString::fromUtf8("lineEdit_u_target"));

        gridLayout_2->addWidget(lineEdit_u_target, 4, 3, 1, 1);

        label_33 = new QLabel(widget1);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_2->addWidget(label_33, 4, 4, 1, 1);

        pushButton_u_go = new QPushButton(widget1);
        pushButton_u_go->setObjectName(QString::fromUtf8("pushButton_u_go"));
        sizePolicy.setHeightForWidth(pushButton_u_go->sizePolicy().hasHeightForWidth());
        pushButton_u_go->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(pushButton_u_go, 4, 7, 1, 1);

        label_39 = new QLabel(widget1);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setTextFormat(Qt::AutoText);
        label_39->setScaledContents(false);

        gridLayout_2->addWidget(label_39, 5, 0, 1, 1);

        lineEdit_v_pos = new QLineEdit(widget1);
        lineEdit_v_pos->setObjectName(QString::fromUtf8("lineEdit_v_pos"));
        lineEdit_v_pos->setCursor(QCursor(Qt::ArrowCursor));
        lineEdit_v_pos->setReadOnly(true);

        gridLayout_2->addWidget(lineEdit_v_pos, 5, 1, 1, 1);

        label_34 = new QLabel(widget1);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_2->addWidget(label_34, 5, 2, 1, 1);

        lineEdit_v_target = new QLineEdit(widget1);
        lineEdit_v_target->setObjectName(QString::fromUtf8("lineEdit_v_target"));

        gridLayout_2->addWidget(lineEdit_v_target, 5, 3, 1, 1);

        label_41 = new QLabel(widget1);
        label_41->setObjectName(QString::fromUtf8("label_41"));

        gridLayout_2->addWidget(label_41, 5, 4, 1, 1);

        pushButton_v_go = new QPushButton(widget1);
        pushButton_v_go->setObjectName(QString::fromUtf8("pushButton_v_go"));
        sizePolicy.setHeightForWidth(pushButton_v_go->sizePolicy().hasHeightForWidth());
        pushButton_v_go->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(pushButton_v_go, 5, 7, 1, 1);

        label_20 = new QLabel(widget1);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_2->addWidget(label_20, 6, 0, 1, 1);

        lineEdit_vel = new QLineEdit(widget1);
        lineEdit_vel->setObjectName(QString::fromUtf8("lineEdit_vel"));

        gridLayout_2->addWidget(lineEdit_vel, 6, 1, 1, 1);

        label_22 = new QLabel(widget1);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        gridLayout_2->addWidget(label_22, 6, 2, 1, 1);

        lineEdit_x_pos->raise();
        label_7->raise();
        label_20->raise();
        label_12->raise();
        label_3->raise();
        label_23->raise();
        pushButton_all_stop->raise();
        lineEdit_u_pos->raise();
        lineEdit_y_pos->raise();
        label_5->raise();
        lineEdit_y_target->raise();
        lineEdit_x_target->raise();
        label_14->raise();
        lineEdit_vel->raise();
        label_22->raise();
        pushButton_x_go->raise();
        label_13->raise();
        label_6->raise();
        pushButton_u_go->raise();
        label_17->raise();
        pushButton_y_go->raise();
        pushButton_all_go->raise();
        lineEdit_z_pos->raise();
        label_18->raise();
        lineEdit_z_target->raise();
        label_25->raise();
        pushButton_z_go->raise();
        label_31->raise();
        label_26->raise();
        lineEdit_u_target->raise();
        label_33->raise();
        label_39->raise();
        lineEdit_v_pos->raise();
        label_34->raise();
        lineEdit_v_target->raise();
        label_41->raise();
        pushButton_v_go->raise();
        label_24->raise();
        label_initialize->raise();
        lineEdit_state->raise();
        label->raise();
        label_enabled->raise();
        pushButton_getIDN->raise();
        pushButton_getpos->raise();
        label_10->raise();
        pushButton_ini->raise();
        pushButton_enable->raise();
        pushButton_disable->raise();
        pushButton_getspi->raise();
        label_11->raise();
        label_15->raise();
        lineEdit_spi->raise();
        pushButton_setspi->raise();
        lineEdit_x_spi->raise();
        lineEdit_y_spi->raise();
        lineEdit_z_spi->raise();
        label_16->raise();
        label_19->raise();
        label_21->raise();
        label_8->raise();

        retranslateUi(Dialog_m2);

        QMetaObject::connectSlotsByName(Dialog_m2);
    } // setupUi

    void retranslateUi(QDialog *Dialog_m2)
    {
        Dialog_m2->setWindowTitle(QApplication::translate("Dialog_m2", "Dialog_m2", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Dialog_m2", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; font-size:20pt; font-weight:600; color:#000000;\">M2 Control System</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("Dialog_m2", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; color:#000000;\">State</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("Dialog_m2", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; color:#000000;\">Pivot Point</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Dialog_m2", "Initialized", 0, QApplication::UnicodeUTF8));
        label_initialize->setStyleSheet(QApplication::translate("Dialog_m2", "background-color: rgb(255, 0, 0);", 0, QApplication::UnicodeUTF8));
        label_initialize->setText(QString());
        label_10->setText(QApplication::translate("Dialog_m2", "Enabled", 0, QApplication::UnicodeUTF8));
        label_enabled->setStyleSheet(QApplication::translate("Dialog_m2", "background-color: rgb(255, 0, 0);", 0, QApplication::UnicodeUTF8));
        label_enabled->setText(QString());
        pushButton_ini->setText(QApplication::translate("Dialog_m2", "Initialize", 0, QApplication::UnicodeUTF8));
        pushButton_getIDN->setText(QApplication::translate("Dialog_m2", "Get IDN", 0, QApplication::UnicodeUTF8));
        pushButton_enable->setText(QApplication::translate("Dialog_m2", "Enable", 0, QApplication::UnicodeUTF8));
        pushButton_getspi->setText(QApplication::translate("Dialog_m2", "Get SPI", 0, QApplication::UnicodeUTF8));
        pushButton_disable->setText(QApplication::translate("Dialog_m2", "Disable", 0, QApplication::UnicodeUTF8));
        pushButton_getpos->setText(QApplication::translate("Dialog_m2", "Get Position", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Dialog_m2", "Position", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Dialog_m2", "Target", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Dialog_m2", "Pivot Point ", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Dialog_m2", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; color:#000000;\">X CENTER</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Dialog_m2", "mm", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Dialog_m2", "mm", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("Dialog_m2", "mm", 0, QApplication::UnicodeUTF8));
        pushButton_x_go->setText(QApplication::translate("Dialog_m2", "X-Go", 0, QApplication::UnicodeUTF8));
        pushButton_setspi->setText(QApplication::translate("Dialog_m2", "Set SPI", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Dialog_m2", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; color:#000000;\">Y CENTER</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("Dialog_m2", "mm", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("Dialog_m2", "mm", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("Dialog_m2", "mm", 0, QApplication::UnicodeUTF8));
        pushButton_y_go->setText(QApplication::translate("Dialog_m2", "Y-Go", 0, QApplication::UnicodeUTF8));
        pushButton_all_go->setText(QApplication::translate("Dialog_m2", "Go", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("Dialog_m2", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; color:#000000;\">Z FOCUS</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("Dialog_m2", "mm", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("Dialog_m2", "mm", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("Dialog_m2", "mm", 0, QApplication::UnicodeUTF8));
        pushButton_z_go->setText(QApplication::translate("Dialog_m2", "Z-Go", 0, QApplication::UnicodeUTF8));
        pushButton_all_stop->setText(QApplication::translate("Dialog_m2", "Stop", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("Dialog_m2", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; color:#000000;\">X ROT</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("Dialog_m2", "deg", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("Dialog_m2", "deg", 0, QApplication::UnicodeUTF8));
        pushButton_u_go->setText(QApplication::translate("Dialog_m2", "U-Go", 0, QApplication::UnicodeUTF8));
        label_39->setText(QApplication::translate("Dialog_m2", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Sans Serif'; color:#000000;\">Y ROT</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("Dialog_m2", "deg", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("Dialog_m2", "deg", 0, QApplication::UnicodeUTF8));
        pushButton_v_go->setText(QApplication::translate("Dialog_m2", "V-Go", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("Dialog_m2", "Velocity:", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("Dialog_m2", "mm/s", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog_m2: public Ui_Dialog_m2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_M2_H
