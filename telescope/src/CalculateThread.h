//CalculateThread.h
#ifndef CALCULATETHREAD_H
#define CALCULATETHREAD_H

#include <QThread>
#include "signs.h"


class CalculateThread : public QThread
{
protected:
    //  		 WorkThread ( QObject * parent = 0 );
    void run();
    // private:
    // 		 QString str;
};

#endif
