/********************************************************************************
** Form generated from reading UI file 'terminal.ui'
**
** Created: Sat Oct 15 12:05:00 2016
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TERMINAL_H
#define UI_TERMINAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog_terminal
{
public:
    QTabWidget *tabWidget;
    QWidget *tab_umac;
    QPlainTextEdit *textEdit;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButton;

    void setupUi(QDialog *Dialog_terminal)
    {
        if (Dialog_terminal->objectName().isEmpty())
            Dialog_terminal->setObjectName(QString::fromUtf8("Dialog_terminal"));
        Dialog_terminal->resize(585, 285);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dialog_terminal->sizePolicy().hasHeightForWidth());
        Dialog_terminal->setSizePolicy(sizePolicy);
        Dialog_terminal->setMinimumSize(QSize(580, 250));
        Dialog_terminal->setSizeGripEnabled(true);
        tabWidget = new QTabWidget(Dialog_terminal);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(50, 10, 461, 261));
        tabWidget->setDocumentMode(true);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(true);
        tab_umac = new QWidget();
        tab_umac->setObjectName(QString::fromUtf8("tab_umac"));
        textEdit = new QPlainTextEdit(tab_umac);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(10, 10, 228, 192));
        widget = new QWidget(tab_umac);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(244, 10, 211, 62));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout->addWidget(pushButton);

        tabWidget->addTab(tab_umac, QString());

        retranslateUi(Dialog_terminal);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Dialog_terminal);
    } // setupUi

    void retranslateUi(QDialog *Dialog_terminal)
    {
        Dialog_terminal->setWindowTitle(QApplication::translate("Dialog_terminal", "Tools", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Dialog_terminal", "send", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_umac), QApplication::translate("Dialog_terminal", "UMAC", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog_terminal: public Ui_Dialog_terminal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERMINAL_H
