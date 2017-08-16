/********************************************************************************
** Form generated from reading UI file 'diag_dero.ui'
**
** Created: Mon Jun 24 14:54:18 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIAG_DERO_H
#define UI_DIAG_DERO_H

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

class Ui_DiagDero
{
public:
    QTableWidget *DeroDiagTableWidget;
    QRadioButton *radioButton_DeroDiagSave;
    QPushButton *ClearPushBtn;

    void setupUi(QWidget *DiagDero)
    {
        if (DiagDero->objectName().isEmpty())
            DiagDero->setObjectName(QString::fromUtf8("DiagDero"));
        DiagDero->resize(1102, 300);
        DiagDero->setMinimumSize(QSize(1100, 300));
        DeroDiagTableWidget = new QTableWidget(DiagDero);
        if (DeroDiagTableWidget->columnCount() < 5)
            DeroDiagTableWidget->setColumnCount(5);
        if (DeroDiagTableWidget->rowCount() < 5)
            DeroDiagTableWidget->setRowCount(5);
        DeroDiagTableWidget->setObjectName(QString::fromUtf8("DeroDiagTableWidget"));
        DeroDiagTableWidget->setGeometry(QRect(2, 22, 1089, 181));
        DeroDiagTableWidget->setRowCount(5);
        DeroDiagTableWidget->setColumnCount(5);
        radioButton_DeroDiagSave = new QRadioButton(DiagDero);
        radioButton_DeroDiagSave->setObjectName(QString::fromUtf8("radioButton_DeroDiagSave"));
        radioButton_DeroDiagSave->setEnabled(true);
        radioButton_DeroDiagSave->setGeometry(QRect(562, 222, 61, 21));
        radioButton_DeroDiagSave->setCheckable(true);
        radioButton_DeroDiagSave->setChecked(false);
        ClearPushBtn = new QPushButton(DiagDero);
        ClearPushBtn->setObjectName(QString::fromUtf8("ClearPushBtn"));
        ClearPushBtn->setGeometry(QRect(402, 222, 85, 27));

        retranslateUi(DiagDero);

        QMetaObject::connectSlotsByName(DiagDero);
    } // setupUi

    void retranslateUi(QWidget *DiagDero)
    {
        DiagDero->setWindowTitle(QApplication::translate("DiagDero", "Diagnosis Derotator", 0, QApplication::UnicodeUTF8));
        radioButton_DeroDiagSave->setText(QApplication::translate("DiagDero", "SAVE", 0, QApplication::UnicodeUTF8));
        ClearPushBtn->setText(QApplication::translate("DiagDero", "CLEAR", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DiagDero: public Ui_DiagDero {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIAG_DERO_H
