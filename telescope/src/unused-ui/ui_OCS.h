/********************************************************************************
** Form generated from reading ui file 'OCS.ui'
**
** Created: Thu Apr 7 14:03:59 2011
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_OCS_H
#define UI_OCS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OCS
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_port;
    QLabel *label_3;
    QLineEdit *lineEdit_status;
    QLabel *label_4;
    QLineEdit *lineEdit_rece;
    QComboBox *comboBox_address;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_check_az;
    QPushButton *pushButton_check_alt;
    QPushButton *pushButton_check_dero;
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
    QTableWidget *tableWidget;
    QLabel *label_status;

    void setupUi(QWidget *OCS)
    {
        if (OCS->objectName().isEmpty())
            OCS->setObjectName(QString::fromUtf8("OCS"));
        OCS->resize(876, 550);
        layoutWidget = new QWidget(OCS);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(550, 360, 181, 161));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_port = new QLineEdit(layoutWidget);
        lineEdit_port->setObjectName(QString::fromUtf8("lineEdit_port"));

        gridLayout->addWidget(lineEdit_port, 1, 1, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        lineEdit_status = new QLineEdit(layoutWidget);
        lineEdit_status->setObjectName(QString::fromUtf8("lineEdit_status"));

        gridLayout->addWidget(lineEdit_status, 2, 1, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        lineEdit_rece = new QLineEdit(layoutWidget);
        lineEdit_rece->setObjectName(QString::fromUtf8("lineEdit_rece"));
        lineEdit_rece->setReadOnly(true);

        gridLayout->addWidget(lineEdit_rece, 3, 1, 1, 1);

        comboBox_address = new QComboBox(layoutWidget);
        comboBox_address->setObjectName(QString::fromUtf8("comboBox_address"));

        gridLayout->addWidget(comboBox_address, 0, 1, 1, 1);

        layoutWidget1 = new QWidget(OCS);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(20, 370, 491, 151));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton_check_az = new QPushButton(layoutWidget1);
        pushButton_check_az->setObjectName(QString::fromUtf8("pushButton_check_az"));
        pushButton_check_az->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_check_az->sizePolicy().hasHeightForWidth());
        pushButton_check_az->setSizePolicy(sizePolicy);
        pushButton_check_az->setFlat(false);

        gridLayout_2->addWidget(pushButton_check_az, 0, 0, 1, 1);

        pushButton_check_alt = new QPushButton(layoutWidget1);
        pushButton_check_alt->setObjectName(QString::fromUtf8("pushButton_check_alt"));
        pushButton_check_alt->setEnabled(false);

        gridLayout_2->addWidget(pushButton_check_alt, 0, 1, 1, 1);

        pushButton_check_dero = new QPushButton(layoutWidget1);
        pushButton_check_dero->setObjectName(QString::fromUtf8("pushButton_check_dero"));
        pushButton_check_dero->setEnabled(false);

        gridLayout_2->addWidget(pushButton_check_dero, 0, 2, 1, 1);

        pushButton_check_status = new QPushButton(layoutWidget1);
        pushButton_check_status->setObjectName(QString::fromUtf8("pushButton_check_status"));

        gridLayout_2->addWidget(pushButton_check_status, 0, 3, 1, 1);

        pushButton_connect = new QPushButton(layoutWidget1);
        pushButton_connect->setObjectName(QString::fromUtf8("pushButton_connect"));

        gridLayout_2->addWidget(pushButton_connect, 0, 4, 1, 1);

        pushButton_point_az = new QPushButton(layoutWidget1);
        pushButton_point_az->setObjectName(QString::fromUtf8("pushButton_point_az"));
        pushButton_point_az->setEnabled(false);

        gridLayout_2->addWidget(pushButton_point_az, 1, 0, 1, 1);

        pushButton_point_alt = new QPushButton(layoutWidget1);
        pushButton_point_alt->setObjectName(QString::fromUtf8("pushButton_point_alt"));
        pushButton_point_alt->setEnabled(false);

        gridLayout_2->addWidget(pushButton_point_alt, 1, 1, 1, 1);

        pushButton_start_dero = new QPushButton(layoutWidget1);
        pushButton_start_dero->setObjectName(QString::fromUtf8("pushButton_start_dero"));
        pushButton_start_dero->setEnabled(false);

        gridLayout_2->addWidget(pushButton_start_dero, 1, 2, 1, 1);

        pushButton_start_track = new QPushButton(layoutWidget1);
        pushButton_start_track->setObjectName(QString::fromUtf8("pushButton_start_track"));
        pushButton_start_track->setEnabled(false);

        gridLayout_2->addWidget(pushButton_start_track, 1, 3, 1, 1);

        pushButton_ocs_selfcheck = new QPushButton(layoutWidget1);
        pushButton_ocs_selfcheck->setObjectName(QString::fromUtf8("pushButton_ocs_selfcheck"));

        gridLayout_2->addWidget(pushButton_ocs_selfcheck, 1, 4, 1, 1);

        pushButton_stop_az = new QPushButton(layoutWidget1);
        pushButton_stop_az->setObjectName(QString::fromUtf8("pushButton_stop_az"));
        pushButton_stop_az->setEnabled(false);

        gridLayout_2->addWidget(pushButton_stop_az, 2, 0, 1, 1);

        pushButton_stop_alt = new QPushButton(layoutWidget1);
        pushButton_stop_alt->setObjectName(QString::fromUtf8("pushButton_stop_alt"));
        pushButton_stop_alt->setEnabled(false);

        gridLayout_2->addWidget(pushButton_stop_alt, 2, 1, 1, 1);

        pushButton_stop_dero = new QPushButton(layoutWidget1);
        pushButton_stop_dero->setObjectName(QString::fromUtf8("pushButton_stop_dero"));
        pushButton_stop_dero->setEnabled(false);

        gridLayout_2->addWidget(pushButton_stop_dero, 2, 2, 1, 1);

        pushButton_stop_track = new QPushButton(layoutWidget1);
        pushButton_stop_track->setObjectName(QString::fromUtf8("pushButton_stop_track"));
        pushButton_stop_track->setEnabled(false);

        gridLayout_2->addWidget(pushButton_stop_track, 2, 3, 1, 1);

        pushButton_guidingStar = new QPushButton(layoutWidget1);
        pushButton_guidingStar->setObjectName(QString::fromUtf8("pushButton_guidingStar"));
        pushButton_guidingStar->setEnabled(false);

        gridLayout_2->addWidget(pushButton_guidingStar, 2, 4, 1, 1);

        tableWidget = new QTableWidget(OCS);
        if (tableWidget->columnCount() < 1)
            tableWidget->setColumnCount(1);
        if (tableWidget->rowCount() < 4)
            tableWidget->setRowCount(4);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 70, 851, 201));
        tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableWidget->setRowCount(4);
        tableWidget->setColumnCount(1);
        tableWidget->horizontalHeader()->setDefaultSectionSize(800);
        tableWidget->horizontalHeader()->setMinimumSectionSize(16);
        label_status = new QLabel(OCS);
        label_status->setObjectName(QString::fromUtf8("label_status"));
        label_status->setGeometry(QRect(400, 340, 107, 27));

        retranslateUi(OCS);

        QMetaObject::connectSlotsByName(OCS);
    } // setupUi

    void retranslateUi(QWidget *OCS)
    {
        OCS->setWindowTitle(QApplication::translate("OCS", "OCS", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("OCS", "Server", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("OCS", "Port", 0, QApplication::UnicodeUTF8));
        lineEdit_port->setText(QApplication::translate("OCS", "5001", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("OCS", "Status", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("OCS", "Receiver", 0, QApplication::UnicodeUTF8));
        comboBox_address->clear();
        comboBox_address->insertItems(0, QStringList()
         << QApplication::translate("OCS", "192.168.1.100", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("OCS", "192.168.1.200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("OCS", "192.168.1.101", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("OCS", "127.0.0.1", 0, QApplication::UnicodeUTF8)
        );
        pushButton_check_az->setText(QApplication::translate("OCS", "Check AZ", 0, QApplication::UnicodeUTF8));
        pushButton_check_alt->setText(QApplication::translate("OCS", "Check ALT", 0, QApplication::UnicodeUTF8));
        pushButton_check_dero->setText(QApplication::translate("OCS", "Check DERO", 0, QApplication::UnicodeUTF8));
        pushButton_check_status->setText(QApplication::translate("OCS", "Check status", 0, QApplication::UnicodeUTF8));
        pushButton_connect->setText(QApplication::translate("OCS", "Connect", 0, QApplication::UnicodeUTF8));
        pushButton_point_az->setText(QApplication::translate("OCS", "Point AZ", 0, QApplication::UnicodeUTF8));
        pushButton_point_alt->setText(QApplication::translate("OCS", "Point ALT", 0, QApplication::UnicodeUTF8));
        pushButton_start_dero->setText(QApplication::translate("OCS", "Start DERO", 0, QApplication::UnicodeUTF8));
        pushButton_start_track->setText(QApplication::translate("OCS", "Start TRACK", 0, QApplication::UnicodeUTF8));
        pushButton_ocs_selfcheck->setText(QApplication::translate("OCS", "OCS Selfcheck", 0, QApplication::UnicodeUTF8));
        pushButton_stop_az->setText(QApplication::translate("OCS", "Stop AZ", 0, QApplication::UnicodeUTF8));
        pushButton_stop_alt->setText(QApplication::translate("OCS", "Stop ALT", 0, QApplication::UnicodeUTF8));
        pushButton_stop_dero->setText(QApplication::translate("OCS", "Stop DERO", 0, QApplication::UnicodeUTF8));
        pushButton_stop_track->setText(QApplication::translate("OCS", "Stop TRACK", 0, QApplication::UnicodeUTF8));
        pushButton_guidingStar->setText(QApplication::translate("OCS", "Guiding star", 0, QApplication::UnicodeUTF8));
        label_status->setText(QApplication::translate("OCS", "Net Disconnected", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(OCS);
    } // retranslateUi

};

namespace Ui {
    class OCS: public Ui_OCS {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OCS_H
