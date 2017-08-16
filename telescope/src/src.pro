#include (/home/dong/log4qt/src/log4qt/log4qt.pri)

TEMPLATE = app

TARGET = telescope

CONFIG += warn_on \
	  thread \
          qt
CONFIG += exceptions
CONFIG += qtestlib

INCLUDEPATH += /usr/local/qwt-5.2.0/include #/usr/local/Trolltech/Qt-4.7.4/include#

UI_DIR = ../temp
MOC_DIR = ../temp
OBJECTS_DIR = ../temp
DESTDIR = ../bin

QT += network

LIBS += -L/home/book/workspace/QtCode/telescope/star \#-L/home/scope/program/telescope/star \
  #-L/home/scope/program/telescope/star/libxlbscsearch.a \
  #-L/home/scope/program/telescope/star/libtimeandcoordinate.a \
  -L/home/book/workspace/QtCode/telescope/gail
#-L/home/scope/program/telescope/Galil \        #-lGalil
#LIBS += /usr/local/qwt-5.2.0/lib/libqwt.so
#LIBS += -lqwt #if you want to use like this style,you must copy your library into /lib or /usr/lib .  And ld.so.conf.cache will not work here.i do not know why.
#LIBS += -lPvApi

OTHER_FILES += \
    #../resource/resource.pro

HEADERS += main_window.h \
            direct_azalt.h \
            UMAC.h \
            singleton.h \
            terminal.h \
            tracking.h \
            astro.h \
            circum.h \
            CalculateThread.h \
            setup.h \
            simulationOCS.h \
            sendthread.h \
            fortuneserver.h \
            fortunethread.h \
            SimOCSserver.h \
            simOCSserverthread.h \
            writelog.h \
            checkmessage.h \
            piconnect.h \
            dialog_m2.h \
            signs.h \
            define.h \
            derotator.h \
            adc.h \
            ADefine.h \
            plotter.h \
    plottersetting.h

SOURCES += main.cpp \
           main_window.cpp \
           direct_azalt.cpp \
           UMAC.cpp \
           qumac.cpp \
           terminal.cpp \
           tracking.cpp \
           StarTest.cpp \
           CalculateThread.cpp \
           setup.cpp \
           simulationOCS.cpp \
           sendthread.cpp \
           fortuneserver.cpp \
           fortunethread.cpp \
           SimOCSserver.cpp \
           simOCSserverthread.cpp \
           writelog.cpp \
           checkmessage.cpp \
           signs.cpp \
           dialog_m2.cpp \
           derotator.cpp \
           adc.cpp \
    plotter.cpp \
    plottersetting.cpp

FORMS += telescope.ui \
            direct_azalt.ui \
            terminal.ui \
            tracking.ui \
            setup.ui \
            simulationOCS.ui \
            dialog_m2.ui \
            derotator.ui \
            adc.ui \
    plottersetting.ui

RESOURCES = application.qrc \
            ../resource/toobar.qrc
