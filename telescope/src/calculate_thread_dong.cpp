//
// C++ Implementation: calculate_thread
//
// Description: 
//
//
// Author: donglongchao <donglongchao@163.com>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "calculate_thread.h"
#include "astro.h"
#include "circum.h"
Now np;
CalculateThread::CalculateThread(	double RA_h,
		double RA_m,
		double RA_s,
		double DEC_d,
		double DEC_m,
		double DEC_s):m_RA_h(RA_h),m_RA_m(RA_m),m_RA_s(RA_s),m_DEC_d(DEC_d),m_DEC_m(DEC_m),m_DEC_s(DEC_s)
{
	
}
CalculateThread::~CalculateThread()
{
	qDebug("CalculateThread");
}
void CalculateThread::calculate(	double RA_h, 
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
				DateTime dt )
{
	ra= hrrad ( RA_h+RA_m/60.+RA_s/3600. );
	if ( DEC_d>0 ) dec_star=DEC_d+DEC_m/60.0+DEC_s/3600.;//star 的真正赤纬:单位 度
	else if ( DEC_d==0 )
	{
		if ( DEC_m>=0 ) dec_star=DEC_m/60.0+DEC_s/3600.;
		else dec_star=DEC_m/60.0-DEC_s/3600.;
	}
	else dec_star=DEC_d-DEC_m/60.0-DEC_s/3600.;
	m_DEC = degrad ( dec_star );
	*DEC_cal=dec_star;//保存赤纬

	np.n_pressure=Pressure;
	np.n_temp=Temperature;

	cal_mjd ( dt.month,dt.day,dt.year,&mjd0 );

	utc=dt.hour*3600+dt.minute*60.+dt.second+dt.msec/1000.0-8;//change system time to real utc

	deltaUT=deltat ( mjd0 );
	mjd1=mjd0+ ( utc+deltaUT ) /86400.0;


	//np.n_lat=degrad ( 40+23./60.+36./3600. );//xinglong
	//np.n_lng=degrad ( 117+34./60.+30./3600. );

        // np.n_lat=degrad ( 43+44.77/60. );//43°44.77  Russia
	//np.n_lng=degrad ( 42+31.42/60. );//42°31.42'
         
         np.n_lat=degrad ( 32.061 );//32.061° Nanjing latitude
	 np.n_lng=degrad ( 118.79125 );//118.79125°    longitude

	precess_p03 ( J2000,mjd1,&ra,&Dec );

	sunpos ( mjd1,&lsn,&rsn,NULL );

	nut_eq ( mjd1,&ra,&Dec );

	ab_eq ( mjd1,lsn,&ra,&Dec );

	now_lst ( mjd0,np.n_lng,&lst0 );//once
	lst = 1.002737909* ( utc ) /3600.0 +
	      lst0+ radhr ( np.n_lng );//every time
	range ( &lst, 24.0 );
	ha=hrrad ( lst )-ra;
	ha_star=ha*180/PI;
	*HA_cal=ha_star;//保存时角


	hadec_aa ( np.n_lat,ha,Dec,&alt,&az );//calculate alt az from ha dec

	refract ( np.n_pressure,np.n_temp,alt,&alt );

	aa_hadec ( np.n_lat,alt,az,&ha1,&dec1 );
	ma ( np.n_lat, ha1, dec1, &alt, &az, &fpa );
	*AZ_cal=az*180/PI;
	*ALT_cal=alt*180/PI;
	*FR_cal=fpa*180/PI;

};

void CalculateThread::run()
{
//	sleep(3);
	QDateTime dt[TABLE_NUM];
     
	dt[0]=QDateTime::currentDateTime();
	QDate date[TABLE_NUM];
	QTime time[TABLE_NUM];
	date[0]=dt[0].date();
	time[0]=dt[0].time();
	
	DT[0].year=date[0].year();
	DT[0].month=date[0].month();
	DT[0].day=date[0].day();

	DT[0].hour=time[0].hour();
	DT[0].minute=time[0].minute();
	DT[0].second=time[0].second();
	DT[0].msec=time[0].msec();

	calculate (m_RA_h,m_RA_m,m_RA_s,m_DEC_d,m_DEC_m,m_DEC_s,m_temperature,m_pressure,&AZ[0],&ALT[0],&FR[0],&HA[0],&DEC[0],DT[0] );

	for ( long i=1;i<TABLE_NUM;i++ )
	{
		DT[i]=DT[i-1];
		DT[i].msec=DT[i-1].msec+100;//every 0.1s get a star position
		while ( DT[i].msec>=1000 )
		{
			DT[i].msec=DT[i].msec-1000;
			DT[i].second=DT[i].second+1;
		}
		while ( DT[i].second>=60 )
		{
			DT[i].second=DT[i].second-60;
			DT[i].minute=DT[i].minute+1;
		}
		while ( DT[i].minute>=60 )
		{
			DT[i].minute=DT[i].minute-60;
			DT[i].hour=DT[i].hour+1;
		}
		while ( DT[i].hour>=24 )
		{
			DT[i].hour=DT[i].hour-24;
			DT[i].day=DT[i].day+1;
		}
		///日月年暂且忽略
	calculate (m_RA_h,m_RA_m,m_RA_s,m_DEC_d,m_DEC_m,m_DEC_s,m_temperature,m_pressure,&AZ[i],&ALT[i],&FR[i],&HA[i],&DEC[i],DT[i] );
        
              

		//	calculate(&AZ[i],&ALT[i],time[i]);
	}
}