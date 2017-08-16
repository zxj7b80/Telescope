//utility.h
#include <qstring.h>
#ifndef UTILITY_H
#define UTILITY_H


//#define  TableNum 90000    //星表行数90000 ,2.5
#define  TableNum 108000    //星表行数108000,3小时
//#define CTS  0.1125   //编码器ratio
#define CTS  0.0140625   //编码器ratio,Az
#define CTSDero  0.0140625   //derotator 编码器ratio
//#define CTSAlt  0.1125   //编码器ratio,Alt
#define CTSAlt  0.0140625   //编码器ratio,Alt

extern long  nConnection;//
#include <QTcpServer>
//#include "StarTest.cpp"

  //double RA_h=0,RA_m=0,RA_s=0;//RA of star
//	double DEC_d=0,DEC_m=0,DEC_s=0;//Dec of star
//    double Temperature=0,Pressure=0;//mBa;
struct DateTime
{
int year;	
int month;	
int day;
int hour;
int minute;
int second;
int msec;


};

struct StarTarget
{
QString cDate;	
QString cTime;	
QString cAzimuth;
QString cAltitude;
QString cFR;

};

struct TrackStar
{
QString cDateTime;		//example:20030801102306
QString cCmdtoAz;		//command parameter set (Az)
QString cAzPosition;               //AZ real position
QString cAzTarget;               //AZ target position
QString cAzTrackError;               //AZ tracking error

QString cCmdtoAlt;		//command parameter set (Alt)
QString cAltPosition;               //Alt real position
QString cAltTarget;               //Alt real position
QString cAltTrackError;               //Alt tracking error

QString cCmdtoDero;		//command parameter set (Derotator)
QString cDeroPosition;               //Derotator real position
QString cDeroTarget;               //Derotator real position
QString cDeroTrackError;               //Derotator tracking error

};

#endif


