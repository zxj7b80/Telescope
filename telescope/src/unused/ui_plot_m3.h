/********************************************************************************
** Form generated from reading ui file 'plot_m3.ui'
**
** Created: Fri Feb 26 15:05:28 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PLOT_M3_H
#define UI_PLOT_M3_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_PlotM3
{
public:
    QwtPlot *qwtPlot;

    void setupUi(QWidget *PlotM3)
    {
        if (PlotM3->objectName().isEmpty())
            PlotM3->setObjectName(QString::fromUtf8("PlotM3"));
        PlotM3->resize(658, 637);
        qwtPlot = new QwtPlot(PlotM3);
        qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));
        qwtPlot->setGeometry(QRect(50, 90, 531, 341));

        retranslateUi(PlotM3);

        QMetaObject::connectSlotsByName(PlotM3);
    } // setupUi

    void retranslateUi(QWidget *PlotM3)
    {
        PlotM3->setWindowTitle(QApplication::translate("PlotM3", "Plot M3", 0, QApplication::UnicodeUTF8));
        qwtPlot->setProperty("propertiesDocument", QVariant(QString()));
        Q_UNUSED(PlotM3);
    } // retranslateUi

};

namespace Ui {
    class PlotM3: public Ui_PlotM3 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOT_M3_H
