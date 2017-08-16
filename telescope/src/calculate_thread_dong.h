//
// C++ Interface: calculate_thread
//
// Description:
//
//
// Author: donglongchao <donglongchao@163.com>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CALCULATE_THREAD_H
#define CALCULATE_THREAD_H

#include <QObject>
#include <QThread>
#include <QDateTime>
#include "signs.h"
class CalculateThread:public QThread
{
	Q_OBJECT
	private:
		double m_RA_h;
		double m_RA_m;
		double m_RA_s;
		double m_DEC_d;
		double m_DEC_m;
		double m_DEC_s;

		double m_HA;
		double m_DEC;

		double m_temperature;
		double m_pressure;

//		Now np;

		double ra,Dec;//ra and dec at epoch of date
		double ha,alt,az,fpa,ha1,dec1,ha_2,ha1_2,dec1_2,fpa2;//hour angle,altitude and azimuth
		double utc,deltaUT,lst0,lst,lst0_2;

		double dec_star,ha_star;//save,degree,hour,
		double utc2,lst2,alt2,az2;
		double mjd0,mjd1,mjd1_2;//mjd0 modified julian date, counted from J1900.0
		double lsn,rsn,lsn2,rsn2;//true geoc lng of sun,dist from sun to earth

		void calculate (  double RA_h,
		                 double RA_m,
		                 double RA_s,
		                 double DEC_d,
		                 double DEC_m,
		                 double DEC_s,
		                 double Temperature,
		                 double Pressure,
		                 double *AZ_cal,
		                 double *ALT_cal,
		                 double *FR_cal,
		                 double *HA_cal,
		                 double *DEC_cal,
		                 DateTime dt );

	public:
		double AZ[TABLE_NUM];
		double ALT[TABLE_NUM];
		double FR[TABLE_NUM];
		double HA[TABLE_NUM];
		double DEC[TABLE_NUM];
//		QDateTime DT[TABLE_NUM];
		DateTime DT[TABLE_NUM];

		CalculateThread (  double RA_h,
		                  double RA_m,
		                  double RA_s,
		                  double DEC_d,
		                  double DEC_m,
		                  double DEC_s );
		~CalculateThread();
	protected:
		void run();
};
#endif
