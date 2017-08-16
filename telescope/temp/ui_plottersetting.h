/********************************************************************************
** Form generated from reading UI file 'plottersetting.ui'
**
** Created: Sat Oct 15 12:05:00 2016
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTTERSETTING_H
#define UI_PLOTTERSETTING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_plottersetting
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit_MotionCurveDir;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_selectMotionCurveDir;
    QPushButton *pushButton_plot;

    void setupUi(QDialog *plottersetting)
    {
        if (plottersetting->objectName().isEmpty())
            plottersetting->setObjectName(QString::fromUtf8("plottersetting"));
        plottersetting->resize(655, 110);
        widget = new QWidget(plottersetting);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 20, 641, 71));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit_MotionCurveDir = new QLineEdit(widget);
        lineEdit_MotionCurveDir->setObjectName(QString::fromUtf8("lineEdit_MotionCurveDir"));

        horizontalLayout->addWidget(lineEdit_MotionCurveDir);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_selectMotionCurveDir = new QPushButton(widget);
        pushButton_selectMotionCurveDir->setObjectName(QString::fromUtf8("pushButton_selectMotionCurveDir"));

        horizontalLayout_2->addWidget(pushButton_selectMotionCurveDir);

        pushButton_plot = new QPushButton(widget);
        pushButton_plot->setObjectName(QString::fromUtf8("pushButton_plot"));

        horizontalLayout_2->addWidget(pushButton_plot);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(plottersetting);

        QMetaObject::connectSlotsByName(plottersetting);
    } // setupUi

    void retranslateUi(QDialog *plottersetting)
    {
        plottersetting->setWindowTitle(QApplication::translate("plottersetting", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("plottersetting", "MotionCurve Directory:", 0, QApplication::UnicodeUTF8));
        pushButton_selectMotionCurveDir->setText(QApplication::translate("plottersetting", "select file", 0, QApplication::UnicodeUTF8));
        pushButton_plot->setText(QApplication::translate("plottersetting", "plot now", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class plottersetting: public Ui_plottersetting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTTERSETTING_H
