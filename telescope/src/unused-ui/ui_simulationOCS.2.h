/********************************************************************************
** Form generated from reading UI file 'simulationOCS.ui'
**
** Created: Fri Dec 19 17:32:40 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMULATIONOCS_H
#define UI_SIMULATIONOCS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SimulationOCS
{
public:
    QTableWidget *tableWidget_OCS;
    QLabel *label_status;
    QWidget *layoutWidget_2;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_home_az;
    QPushButton *pushButton_home_alt;
    QPushButton *pushButton_home_dero;
    QPushButton *pushButton_check_status;
    QPushButton *pushButton_connect;
    QPushButton *pushButton_point_az;
    QPushButton *pushButton_point_alt;
    QPushButton *pushButton_start_dero;
    QPushButton *pushButton_start_track;
    QPushButton *pushButton_ocs_selfcheck;
    QPushButton *pushButton_stop_az;
    QPushButton *pushButton_stop_alt;
    QPushButton *pushButton_stop_dero;
    QPushButton *pushButton_stop_track;
    QPushButton *pushButton_guidingStar;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_3;
    QLineEdit *lineEdit_Az_point;
    QLineEdit *lineEdit_Alt_point;
    QLineEdit *lineEdit_Az_guiding;
    QLineEdit *lineEdit_Alt_guiding;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *lineEdit_Dero_point;
    QLineEdit *lineEdit_Dero_guiding;
    QTableWidget *tableWidget_OCSout;

    void setupUi(QWidget *SimulationOCS)
    {
        if (SimulationOCS->objectName().isEmpty())
            SimulationOCS->setObjectName(QString::fromUtf8("SimulationOCS"));
        SimulationOCS->resize(1200, 400);
        SimulationOCS->setMinimumSize(QSize(1200, 400));
        tableWidget_OCS = new QTableWidget(SimulationOCS);
        if (tableWidget_OCS->columnCount() < 2)
            tableWidget_OCS->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_OCS->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (tableWidget_OCS->rowCount() < 10)
            tableWidget_OCS->setRowCount(10);
        tableWidget_OCS->setObjectName(QString::fromUtf8("tableWidget_OCS"));
        tableWidget_OCS->setGeometry(QRect(0, 104, 1127, 95));
        tableWidget_OCS->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_OCS->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_OCS->setRowCount(10);
        tableWidget_OCS->setColumnCount(2);
        tableWidget_OCS->horizontalHeader()->setDefaultSectionSize(800);
        tableWidget_OCS->horizontalHeader()->setMinimumSectionSize(16);
        label_status = new QLabel(SimulationOCS);
        label_status->setObjectName(QString::fromUtf8("label_status"));
        label_status->setGeometry(QRect(924, 234, 107, 27));
        layoutWidget_2 = new QWidget(SimulationOCS);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(412, 228, 491, 151));
        gridLayout_2 = new QGridLayout(layoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_home_az = new QPushButton(layoutWidget_2);
        pushButton_home_az->setObjectName(QString::fromUtf8("pushButton_home_az"));
        pushButton_home_az->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_home_az->sizePolicy().hasHeightForWidth());
        pushButton_home_az->setSizePolicy(sizePolicy);
        pushButton_home_az->setFlat(false);

        gridLayout_2->addWidget(pushButton_home_az, 0, 0, 1, 1);

        pushButton_home_alt = new QPushButton(layoutWidget_2);
        pushButton_home_alt->setObjectName(QString::fromUtf8("pushButton_home_alt"));
        pushButton_home_alt->setEnabled(false);

        gridLayout_2->addWidget(pushButton_home_alt, 0, 1, 1, 1);

        pushButton_home_dero = new QPushButton(layoutWidget_2);
        pushButton_home_dero->setObjectName(QString::fromUtf8("pushButton_home_dero"));
        pushButton_home_dero->setEnabled(false);

        gridLayout_2->addWidget(pushButton_home_dero, 0, 2, 1, 1);

        pushButton_check_status = new QPushButton(layoutWidget_2);
        pushButton_check_status->setObjectName(QString::fromUtf8("pushButton_check_status"));

        gridLayout_2->addWidget(pushButton_check_status, 0, 3, 1, 1);

        pushButton_connect = new QPushButton(layoutWidget_2);
        pushButton_connect->setObjectName(QString::fromUtf8("pushButton_connect"));

        gridLayout_2->addWidget(pushButton_connect, 0, 4, 1, 1);

        pushButton_point_az = new QPushButton(layoutWidget_2);
        pushButton_point_az->setObjectName(QString::fromUtf8("pushButton_point_az"));
        pushButton_point_az->setEnabled(false);

        gridLayout_2->addWidget(pushButton_point_az, 1, 0, 1, 1);

        pushButton_point_alt = new QPushButton(layoutWidget_2);
        pushButton_point_alt->setObjectName(QString::fromUtf8("pushButton_point_alt"));
        pushButton_point_alt->setEnabled(false);

        gridLayout_2->addWidget(pushButton_point_alt, 1, 1, 1, 1);

        pushButton_start_dero = new QPushButton(layoutWidget_2);
        pushButton_start_dero->setObjectName(QString::fromUtf8("pushButton_start_dero"));
        pushButton_start_dero->setEnabled(false);

        gridLayout_2->addWidget(pushButton_start_dero, 1, 2, 1, 1);

        pushButton_start_track = new QPushButton(layoutWidget_2);
        pushButton_start_track->setObjectName(QString::fromUtf8("pushButton_start_track"));
        pushButton_start_track->setEnabled(false);

        gridLayout_2->addWidget(pushButton_start_track, 1, 3, 1, 1);

        pushButton_ocs_selfcheck = new QPushButton(layoutWidget_2);
        pushButton_ocs_selfcheck->setObjectName(QString::fromUtf8("pushButton_ocs_selfcheck"));

        gridLayout_2->addWidget(pushButton_ocs_selfcheck, 1, 4, 1, 1);

        pushButton_stop_az = new QPushButton(layoutWidget_2);
        pushButton_stop_az->setObjectName(QString::fromUtf8("pushButton_stop_az"));
        pushButton_stop_az->setEnabled(false);

        gridLayout_2->addWidget(pushButton_stop_az, 2, 0, 1, 1);

        pushButton_stop_alt = new QPushButton(layoutWidget_2);
        pushButton_stop_alt->setObjectName(QString::fromUtf8("pushButton_stop_alt"));
        pushButton_stop_alt->setEnabled(false);

        gridLayout_2->addWidget(pushButton_stop_alt, 2, 1, 1, 1);

        pushButton_stop_dero = new QPushButton(layoutWidget_2);
        pushButton_stop_dero->setObjectName(QString::fromUtf8("pushButton_stop_dero"));
        pushButton_stop_dero->setEnabled(false);

        gridLayout_2->addWidget(pushButton_stop_dero, 2, 2, 1, 1);

        pushButton_stop_track = new QPushButton(layoutWidget_2);
        pushButton_stop_track->setObjectName(QString::fromUtf8("pushButton_stop_track"));
        pushButton_stop_track->setEnabled(false);

        gridLayout_2->addWidget(pushButton_stop_track, 2, 3, 1, 1);

        pushButton_guidingStar = new QPushButton(layoutWidget_2);
        pushButton_guidingStar->setObjectName(QString::fromUtf8("pushButton_guidingStar"));
        pushButton_guidingStar->setEnabled(false);

        gridLayout_2->addWidget(pushButton_guidingStar, 2, 4, 1, 1);

        gridLayoutWidget = new QWidget(SimulationOCS);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(230, 228, 160, 123));
        gridLayout_3 = new QGridLayout(gridLayoutWidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        lineEdit_Az_point = new QLineEdit(gridLayoutWidget);
        lineEdit_Az_point->setObjectName(QString::fromUtf8("lineEdit_Az_point"));

        gridLayout_3->addWidget(lineEdit_Az_point, 1, 2, 1, 1);

        lineEdit_Alt_point = new QLineEdit(gridLayoutWidget);
        lineEdit_Alt_point->setObjectName(QString::fromUtf8("lineEdit_Alt_point"));

        gridLayout_3->addWidget(lineEdit_Alt_point, 2, 2, 1, 1);

        lineEdit_Az_guiding = new QLineEdit(gridLayoutWidget);
        lineEdit_Az_guiding->setObjectName(QString::fromUtf8("lineEdit_Az_guiding"));

        gridLayout_3->addWidget(lineEdit_Az_guiding, 1, 3, 1, 1);

        lineEdit_Alt_guiding = new QLineEdit(gridLayoutWidget);
        lineEdit_Alt_guiding->setObjectName(QString::fromUtf8("lineEdit_Alt_guiding"));

        gridLayout_3->addWidget(lineEdit_Alt_guiding, 2, 3, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_3->addWidget(label_5, 0, 2, 1, 1);

        label_6 = new QLabel(gridLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 0, 3, 1, 1);

        label_8 = new QLabel(gridLayoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_3->addWidget(label_8, 1, 1, 1, 1);

        label_9 = new QLabel(gridLayoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_3->addWidget(label_9, 2, 1, 1, 1);

        label_10 = new QLabel(gridLayoutWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_3->addWidget(label_10, 3, 1, 1, 1);

        lineEdit_Dero_point = new QLineEdit(gridLayoutWidget);
        lineEdit_Dero_point->setObjectName(QString::fromUtf8("lineEdit_Dero_point"));

        gridLayout_3->addWidget(lineEdit_Dero_point, 3, 2, 1, 1);

        lineEdit_Dero_guiding = new QLineEdit(gridLayoutWidget);
        lineEdit_Dero_guiding->setObjectName(QString::fromUtf8("lineEdit_Dero_guiding"));

        gridLayout_3->addWidget(lineEdit_Dero_guiding, 3, 3, 1, 1);

        tableWidget_OCSout = new QTableWidget(SimulationOCS);
        if (tableWidget_OCSout->columnCount() < 2)
            tableWidget_OCSout->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_OCSout->setHorizontalHeaderItem(0, __qtablewidgetitem1);
        if (tableWidget_OCSout->rowCount() < 10)
            tableWidget_OCSout->setRowCount(10);
        tableWidget_OCSout->setObjectName(QString::fromUtf8("tableWidget_OCSout"));
        tableWidget_OCSout->setGeometry(QRect(0, 8, 1127, 95));
        tableWidget_OCSout->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_OCSout->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget_OCSout->setRowCount(10);
        tableWidget_OCSout->setColumnCount(2);
        tableWidget_OCSout->horizontalHeader()->setDefaultSectionSize(800);
        tableWidget_OCSout->horizontalHeader()->setMinimumSectionSize(16);

        retranslateUi(SimulationOCS);

        QMetaObject::connectSlotsByName(SimulationOCS);
    } // setupUi

    void retranslateUi(QWidget *SimulationOCS)
    {
        SimulationOCS->setWindowTitle(QApplication::translate("SimulationOCS", "Simulation OCS", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_OCS->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("SimulationOCS", "1", 0, QApplication::UnicodeUTF8));
        label_status->setText(QApplication::translate("SimulationOCS", "Net Disconnected", 0, QApplication::UnicodeUTF8));
        pushButton_home_az->setText(QApplication::translate("SimulationOCS", "Home AZ", 0, QApplication::UnicodeUTF8));
        pushButton_home_alt->setText(QApplication::translate("SimulationOCS", "Home ALT", 0, QApplication::UnicodeUTF8));
        pushButton_home_dero->setText(QApplication::translate("SimulationOCS", "Home DERO", 0, QApplication::UnicodeUTF8));
        pushButton_check_status->setText(QApplication::translate("SimulationOCS", "Check status", 0, QApplication::UnicodeUTF8));
        pushButton_connect->setText(QApplication::translate("SimulationOCS", "DisConnect", 0, QApplication::UnicodeUTF8));
        pushButton_point_az->setText(QApplication::translate("SimulationOCS", "Point AZ", 0, QApplication::UnicodeUTF8));
        pushButton_point_alt->setText(QApplication::translate("SimulationOCS", "Point ALT", 0, QApplication::UnicodeUTF8));
        pushButton_start_dero->setText(QApplication::translate("SimulationOCS", "Start DERO", 0, QApplication::UnicodeUTF8));
        pushButton_start_track->setText(QApplication::translate("SimulationOCS", "Start TRACK", 0, QApplication::UnicodeUTF8));
        pushButton_ocs_selfcheck->setText(QApplication::translate("SimulationOCS", "OCS Selfcheck", 0, QApplication::UnicodeUTF8));
        pushButton_stop_az->setText(QApplication::translate("SimulationOCS", "Stop AZ", 0, QApplication::UnicodeUTF8));
        pushButton_stop_alt->setText(QApplication::translate("SimulationOCS", "Stop ALT", 0, QApplication::UnicodeUTF8));
        pushButton_stop_dero->setText(QApplication::translate("SimulationOCS", "Stop DERO", 0, QApplication::UnicodeUTF8));
        pushButton_stop_track->setText(QApplication::translate("SimulationOCS", "Stop TRACK", 0, QApplication::UnicodeUTF8));
        pushButton_guidingStar->setText(QApplication::translate("SimulationOCS", "Guiding star", 0, QApplication::UnicodeUTF8));
        lineEdit_Az_point->setText(QApplication::translate("SimulationOCS", "180.000", 0, QApplication::UnicodeUTF8));
        lineEdit_Alt_point->setText(QApplication::translate("SimulationOCS", "45.000", 0, QApplication::UnicodeUTF8));
        lineEdit_Az_guiding->setText(QApplication::translate("SimulationOCS", "1.00", 0, QApplication::UnicodeUTF8));
        lineEdit_Alt_guiding->setText(QApplication::translate("SimulationOCS", "0.50", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("SimulationOCS", "Point", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("SimulationOCS", "Guiding", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("SimulationOCS", "Az", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("SimulationOCS", "Alt", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("SimulationOCS", "Dero", 0, QApplication::UnicodeUTF8));
        lineEdit_Dero_point->setText(QApplication::translate("SimulationOCS", "0.000", 0, QApplication::UnicodeUTF8));
        lineEdit_Dero_guiding->setText(QApplication::translate("SimulationOCS", "2.00", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_OCSout->horizontalHeaderItem(0);
        ___qtablewidgetitem1->setText(QApplication::translate("SimulationOCS", "1", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SimulationOCS: public Ui_SimulationOCS {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMULATIONOCS_H
