/********************************************************************************
** Form generated from reading UI file 'diag_azalt.ui'
**
** Created: Mon Jun 24 14:54:18 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIAG_AZALT_H
#define UI_DIAG_AZALT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DiagAzalt
{
public:
    QTableWidget *AzaltDiagTableWidget;
    QRadioButton *radioButton_AzaltDiagSave;
    QPushButton *ClearPushBtn;

    void setupUi(QWidget *DiagAzalt)
    {
        if (DiagAzalt->objectName().isEmpty())
            DiagAzalt->setObjectName(QString::fromUtf8("DiagAzalt"));
        DiagAzalt->resize(1102, 576);
        DiagAzalt->setMinimumSize(QSize(1100, 576));
        AzaltDiagTableWidget = new QTableWidget(DiagAzalt);
        if (AzaltDiagTableWidget->columnCount() < 5)
            AzaltDiagTableWidget->setColumnCount(5);
        if (AzaltDiagTableWidget->rowCount() < 16)
            AzaltDiagTableWidget->setRowCount(16);
        AzaltDiagTableWidget->setObjectName(QString::fromUtf8("AzaltDiagTableWidget"));
        AzaltDiagTableWidget->setGeometry(QRect(2, 22, 1089, 507));
        AzaltDiagTableWidget->setMinimumSize(QSize(1089, 507));
        AzaltDiagTableWidget->setRowCount(16);
        AzaltDiagTableWidget->setColumnCount(5);
        radioButton_AzaltDiagSave = new QRadioButton(DiagAzalt);
        radioButton_AzaltDiagSave->setObjectName(QString::fromUtf8("radioButton_AzaltDiagSave"));
        radioButton_AzaltDiagSave->setEnabled(true);
        radioButton_AzaltDiagSave->setGeometry(QRect(562, 536, 61, 21));
        radioButton_AzaltDiagSave->setCheckable(true);
        radioButton_AzaltDiagSave->setChecked(false);
        ClearPushBtn = new QPushButton(DiagAzalt);
        ClearPushBtn->setObjectName(QString::fromUtf8("ClearPushBtn"));
        ClearPushBtn->setGeometry(QRect(402, 536, 85, 27));

        retranslateUi(DiagAzalt);

        QMetaObject::connectSlotsByName(DiagAzalt);
    } // setupUi

    void retranslateUi(QWidget *DiagAzalt)
    {
        DiagAzalt->setWindowTitle(QApplication::translate("DiagAzalt", "Diagnosis Az/Alt", 0, QApplication::UnicodeUTF8));
        radioButton_AzaltDiagSave->setText(QApplication::translate("DiagAzalt", "SAVE", 0, QApplication::UnicodeUTF8));
        ClearPushBtn->setText(QApplication::translate("DiagAzalt", "CLEAR", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DiagAzalt: public Ui_DiagAzalt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIAG_AZALT_H
