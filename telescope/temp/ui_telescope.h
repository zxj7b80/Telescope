/********************************************************************************
** Form generated from reading UI file 'telescope.ui'
**
** Created: Sat Oct 15 12:05:00 2016
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TELESCOPE_H
#define UI_TELESCOPE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTableView>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAz_Alt;
    QAction *actionSetup;
    QAction *actionTerminal;
    QAction *actionWindows;
    QAction *actionMotif;
    QAction *actionGtk;
    QAction *actionCDE;
    QAction *actionTracking;
    QAction *actionDerotator;
    QAction *actionSimulationOCS;
    QAction *actionDero;
    QAction *actionADC;
    QAction *actionM2;
    QAction *actionPloter;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QMdiArea *mdiArea;
    QMenuBar *menubar;
    QMenu *menuDirect_D;
    QMenu *menuTracking;
    QMenu *menuDiagnosis;
    QMenu *menuSetup;
    QMenu *menuStyle;
    QMenu *menuSimulation;
    QStatusBar *statusbar;
    QToolBar *toolbar;
    QDockWidget *dockWidget_data;
    QWidget *dockWidgetContents_data;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QTableView *tableView_tracking;
    QTableWidget *NettableWidget;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1440, 800);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/star7.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setAnimated(true);
        actionAz_Alt = new QAction(MainWindow);
        actionAz_Alt->setObjectName(QString::fromUtf8("actionAz_Alt"));
        actionAz_Alt->setCheckable(false);
        actionAz_Alt->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icon/\346\234\233\350\277\234\351\225\234.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAz_Alt->setIcon(icon1);
        actionAz_Alt->setMenuRole(QAction::TextHeuristicRole);
        actionSetup = new QAction(MainWindow);
        actionSetup->setObjectName(QString::fromUtf8("actionSetup"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icon/Settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetup->setIcon(icon2);
        actionTerminal = new QAction(MainWindow);
        actionTerminal->setObjectName(QString::fromUtf8("actionTerminal"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icon/ter.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTerminal->setIcon(icon3);
        actionWindows = new QAction(MainWindow);
        actionWindows->setObjectName(QString::fromUtf8("actionWindows"));
        actionMotif = new QAction(MainWindow);
        actionMotif->setObjectName(QString::fromUtf8("actionMotif"));
        actionGtk = new QAction(MainWindow);
        actionGtk->setObjectName(QString::fromUtf8("actionGtk"));
        actionCDE = new QAction(MainWindow);
        actionCDE->setObjectName(QString::fromUtf8("actionCDE"));
        actionTracking = new QAction(MainWindow);
        actionTracking->setObjectName(QString::fromUtf8("actionTracking"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icon/\346\234\233\350\277\234\351\225\2345.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTracking->setIcon(icon4);
        actionDerotator = new QAction(MainWindow);
        actionDerotator->setObjectName(QString::fromUtf8("actionDerotator"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icon/star.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDerotator->setIcon(icon5);
        actionSimulationOCS = new QAction(MainWindow);
        actionSimulationOCS->setObjectName(QString::fromUtf8("actionSimulationOCS"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icon/H.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSimulationOCS->setIcon(icon6);
        actionDero = new QAction(MainWindow);
        actionDero->setObjectName(QString::fromUtf8("actionDero"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("../resource/\346\230\237\347\220\203.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDero->setIcon(icon7);
        actionADC = new QAction(MainWindow);
        actionADC->setObjectName(QString::fromUtf8("actionADC"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8("../resource/20071128105357345.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionADC->setIcon(icon8);
        actionM2 = new QAction(MainWindow);
        actionM2->setObjectName(QString::fromUtf8("actionM2"));
        actionM2->setCheckable(false);
        actionM2->setChecked(false);
        actionM2->setEnabled(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8("../resource/rigui.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionM2->setIcon(icon9);
        actionPloter = new QAction(MainWindow);
        actionPloter->setObjectName(QString::fromUtf8("actionPloter"));
        actionPloter->setIconVisibleInMenu(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        mdiArea = new QMdiArea(centralwidget);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        mdiArea->setViewMode(QMdiArea::SubWindowView);
        mdiArea->setTabShape(QTabWidget::Triangular);
        mdiArea->setTabPosition(QTabWidget::South);

        gridLayout_2->addWidget(mdiArea, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1440, 25));
        menuDirect_D = new QMenu(menubar);
        menuDirect_D->setObjectName(QString::fromUtf8("menuDirect_D"));
        menuTracking = new QMenu(menubar);
        menuTracking->setObjectName(QString::fromUtf8("menuTracking"));
        menuDiagnosis = new QMenu(menubar);
        menuDiagnosis->setObjectName(QString::fromUtf8("menuDiagnosis"));
        menuSetup = new QMenu(menubar);
        menuSetup->setObjectName(QString::fromUtf8("menuSetup"));
        menuStyle = new QMenu(menubar);
        menuStyle->setObjectName(QString::fromUtf8("menuStyle"));
        menuSimulation = new QMenu(menubar);
        menuSimulation->setObjectName(QString::fromUtf8("menuSimulation"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolbar = new QToolBar(MainWindow);
        toolbar->setObjectName(QString::fromUtf8("toolbar"));
        toolbar->setEnabled(true);
        toolbar->setIconSize(QSize(32, 32));
        toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolbar);
        dockWidget_data = new QDockWidget(MainWindow);
        dockWidget_data->setObjectName(QString::fromUtf8("dockWidget_data"));
        sizePolicy.setHeightForWidth(dockWidget_data->sizePolicy().hasHeightForWidth());
        dockWidget_data->setSizePolicy(sizePolicy);
        dockWidget_data->setMinimumSize(QSize(1440, 200));
        dockWidget_data->setMaximumSize(QSize(1440, 200));
        dockWidget_data->setFloating(false);
        dockWidget_data->setFeatures(QDockWidget::DockWidgetFeatureMask);
        dockWidgetContents_data = new QWidget();
        dockWidgetContents_data->setObjectName(QString::fromUtf8("dockWidgetContents_data"));
        gridLayout = new QGridLayout(dockWidgetContents_data);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(dockWidgetContents_data);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy1);
        splitter->setOrientation(Qt::Vertical);
        tableView_tracking = new QTableView(splitter);
        tableView_tracking->setObjectName(QString::fromUtf8("tableView_tracking"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(100);
        sizePolicy2.setHeightForWidth(tableView_tracking->sizePolicy().hasHeightForWidth());
        tableView_tracking->setSizePolicy(sizePolicy2);
        tableView_tracking->setMaximumSize(QSize(1440, 900));
        tableView_tracking->setLayoutDirection(Qt::LeftToRight);
        tableView_tracking->setFrameShape(QFrame::Panel);
        tableView_tracking->setFrameShadow(QFrame::Sunken);
        tableView_tracking->setDragEnabled(true);
        tableView_tracking->setTextElideMode(Qt::ElideMiddle);
        tableView_tracking->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tableView_tracking->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        splitter->addWidget(tableView_tracking);

        gridLayout->addWidget(splitter, 2, 0, 1, 1);

        NettableWidget = new QTableWidget(dockWidgetContents_data);
        if (NettableWidget->columnCount() < 2)
            NettableWidget->setColumnCount(2);
        if (NettableWidget->rowCount() < 4)
            NettableWidget->setRowCount(4);
        NettableWidget->setObjectName(QString::fromUtf8("NettableWidget"));
        sizePolicy.setHeightForWidth(NettableWidget->sizePolicy().hasHeightForWidth());
        NettableWidget->setSizePolicy(sizePolicy);
        NettableWidget->setMaximumSize(QSize(1440, 900));
        NettableWidget->setRowCount(4);
        NettableWidget->setColumnCount(2);
        NettableWidget->verticalHeader()->setCascadingSectionResizes(true);

        gridLayout->addWidget(NettableWidget, 1, 0, 1, 1);

        dockWidget_data->setWidget(dockWidgetContents_data);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_data);

        menubar->addAction(menuDirect_D->menuAction());
        menubar->addAction(menuTracking->menuAction());
        menubar->addAction(menuDiagnosis->menuAction());
        menubar->addAction(menuSetup->menuAction());
        menubar->addAction(menuSimulation->menuAction());
        menubar->addAction(menuStyle->menuAction());
        menuDirect_D->addAction(actionAz_Alt);
        menuDirect_D->addAction(actionDerotator);
        menuDirect_D->addSeparator();
        menuDirect_D->addSeparator();
        menuDirect_D->addSeparator();
        menuTracking->addAction(actionTracking);
        menuDiagnosis->addSeparator();
        menuSetup->addSeparator();
        menuSetup->addAction(actionSetup);
        menuStyle->addAction(actionWindows);
        menuStyle->addAction(actionMotif);
        menuStyle->addAction(actionGtk);
        menuStyle->addAction(actionCDE);
        menuSimulation->addAction(actionSimulationOCS);
        toolbar->addAction(actionAz_Alt);
        toolbar->addAction(actionDero);
        toolbar->addAction(actionADC);
        toolbar->addAction(actionM2);
        toolbar->addSeparator();
        toolbar->addAction(actionTracking);
        toolbar->addSeparator();
        toolbar->addAction(actionSetup);
        toolbar->addAction(actionSimulationOCS);
        toolbar->addSeparator();
        toolbar->addAction(actionTerminal);
        toolbar->addSeparator();
        toolbar->addAction(actionPloter);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "SONG Telescope", 0, QApplication::UnicodeUTF8));
        actionAz_Alt->setText(QApplication::translate("MainWindow", "Az/Alt", 0, QApplication::UnicodeUTF8));
        actionSetup->setText(QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        actionTerminal->setText(QApplication::translate("MainWindow", "Terminal", 0, QApplication::UnicodeUTF8));
        actionWindows->setText(QApplication::translate("MainWindow", "Windows", 0, QApplication::UnicodeUTF8));
        actionMotif->setText(QApplication::translate("MainWindow", "Motif", 0, QApplication::UnicodeUTF8));
        actionGtk->setText(QApplication::translate("MainWindow", "Gtk", 0, QApplication::UnicodeUTF8));
        actionCDE->setText(QApplication::translate("MainWindow", "CDE", 0, QApplication::UnicodeUTF8));
        actionTracking->setText(QApplication::translate("MainWindow", "Tracking", 0, QApplication::UnicodeUTF8));
        actionDerotator->setText(QApplication::translate("MainWindow", "Derotator", 0, QApplication::UnicodeUTF8));
        actionSimulationOCS->setText(QApplication::translate("MainWindow", "OCS Simulation", 0, QApplication::UnicodeUTF8));
        actionDero->setText(QApplication::translate("MainWindow", "Miscellaneous", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionDero->setToolTip(QApplication::translate("MainWindow", "Open Derotator", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionADC->setText(QApplication::translate("MainWindow", "ADC", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionADC->setToolTip(QApplication::translate("MainWindow", "Open ADC", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionM2->setText(QApplication::translate("MainWindow", "M2", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionM2->setToolTip(QApplication::translate("MainWindow", "M2", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPloter->setText(QApplication::translate("MainWindow", "Ploter", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPloter->setToolTip(QApplication::translate("MainWindow", "Ploter", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        menuDirect_D->setTitle(QApplication::translate("MainWindow", "Direct", 0, QApplication::UnicodeUTF8));
        menuTracking->setTitle(QApplication::translate("MainWindow", "Tracking", 0, QApplication::UnicodeUTF8));
        menuDiagnosis->setTitle(QApplication::translate("MainWindow", "Diagnosis", 0, QApplication::UnicodeUTF8));
        menuSetup->setTitle(QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        menuStyle->setTitle(QApplication::translate("MainWindow", "Style", 0, QApplication::UnicodeUTF8));
        menuSimulation->setTitle(QApplication::translate("MainWindow", "Simulation", 0, QApplication::UnicodeUTF8));
        toolbar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        dockWidget_data->setWindowTitle(QApplication::translate("MainWindow", "Information", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TELESCOPE_H
