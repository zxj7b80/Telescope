#ifndef CREATETRACEFILE_H
#define CREATETRACEFILE_H

#include <QObject>
#include <QDateTime>
#include "ATimeSpace.h"

struct DateTime{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int msec;
};

using namespace AstroUtil;
class createtracefile : public QObject
{
    Q_OBJECT
public:
    createtracefile();
     double caculate_epoctime(QDateTime datetime);
     double caculate_lmstime(double mjd);
     bool createtrackfile(QDateTime beginDateTime,QDateTime endDateTime,int startype,double ra,double dec);

signals:

public slots:
private:
     QString startabpath;
     ATimeSpace ats;
     void getazaltrot(double ra,double dec,QDateTime datetime,char mode, double airpressure, double temperature, double &az, double &alt, double &rot,double &ha);


};

#endif // CREATETRACEFILE_H
