//CalculateThread.cpp


// 本程序不用

#include "CalculateThread.h"


#include <QTextEdit>
#include <QDebug>
#include <stdio.h>
#include  <qdatetime.h>
#include "StarTest.cpp"
extern int TableNum;

//////////////////////////////
double RA_h,RA_m,RA_s;//RA of star
double DEC_d,DEC_m,DEC_s;//Dec of star
double Temperature,Pressure;//mBa;

//double AZ[TableNum],ALT[TableNum],FR[TableNum],HA[TableNum],DEC[TableNum];
double AZ[TableNumMAX],ALT[TableNumMAX],FR[TableNumMAX],HA[TableNumMAX],DEC[TableNumMAX];
//DateTime DT[TableNum];//save day and time structure
DateTime DT[TableNumMAX];//save day and time structure
int cal_flag=0;//0:no calculate star,1:caculate start,2:calculate finished
//star calculate
double ra,Dec;//ra and dec at epoch of date
double ha,alt,az,fpa,ha1,dec1,ha_2,ha1_2,dec1_2,fpa2;//hour angle,altitude and azimuth
double utc,deltaUT,lst0,lst,lst0_2;
Now np;

double dec_star,ha_star;//save,degree,hour,
double utc2,lst2,alt2,az2=0;
double mjd0,mjd1,mjd1_2;//mjd0 modified julian date, counted from J1900.0
double lsn,rsn,lsn2,rsn2;//true geoc lng of sun,dist from sun to earth


///////////////////////////////calculate star table
//input:	double RA_h=0,RA_m=0,RA_s=0;//RA of star
//input:	double DEC_d=0,DEC_m=0,DEC_s=0;//Dec of star
//input:        double temperature//degree,pressure//mBa;
//////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////

// WorkThread::WorkThread(QObject *parent)
//     : QThread(parent)
// {
// }
////////////////////////////////////////////////////////////////////////////////////////////
void calculate ( double RA_h, double RA_m,double RA_s,
                 double DEC_d,double DEC_m,double DEC_s,
                 double Temperature,
                 double Pressure,
                 double *AZ_cal,
                 double *ALT_cal,double *FR_cal,double *ha_cal,double *dec_cal,DateTime dt )
{
    /////////////计算星的方位 高度///////////////////////////////

    ra= hrrad ( RA_h+RA_m/60.+RA_s/3600. );
    if ( DEC_d>0 ) dec_star=DEC_d+DEC_m/60.0+DEC_s/3600.;//star 的真正赤纬:单位 度
    else if ( DEC_d==0 )
    {
        if ( DEC_m>=0 ) dec_star=DEC_m/60.0+DEC_s/3600.;
        else dec_star=DEC_m/60.0-DEC_s/3600.;
    }
    else dec_star=DEC_d-DEC_m/60.0-DEC_s/3600.;
    Dec= degrad ( dec_star );
    *dec_cal=dec_star;//保存赤纬

    np.n_pressure=Pressure;
    np.n_temp=Temperature;

    cal_mjd ( dt.month,dt.day,dt.year,&mjd0 );
    time_t timep;
    struct tm *p;
    time(&timep);
    p=gmtime(&timep); //得到time_t类型的UTC时间
    //utc=(dt.hour-8)*3600+dt.minute*60.+dt.second+dt.msec/1000.0;//change system time to real utc
    utc= p->tm_hour*3600+  p->tm_min*60.+ p->tm_sec+dt.msec/1000.0;//当前时间    real utc


    deltaUT=deltat ( mjd0 );
    mjd1=mjd0+ ( utc+deltaUT ) /86400.0;


    //np.n_lat=degrad ( 40+23./60.+36./3600. );//xinglong
    //np.n_lng=degrad ( 117+34./60.+30./3600. );

    // np.n_lat=degrad ( 43+44.77/60. );//43°44.77  Russia
    //np.n_lng=degrad ( 42+31.42/60. );//42°31.42'

    //     np.n_lat=degrad ( 32.061 );//32.061° Nanjing latitude
    // np.n_lng=degrad ( 118.79125 );//118.79125°    longitude

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
    *ha_cal=ha_star;//保存时角


    hadec_aa ( np.n_lat,ha,Dec,&alt,&az );//calculate alt az from ha dec

    refract ( np.n_pressure,np.n_temp,alt,&alt );

    aa_hadec ( np.n_lat,alt,az,&ha1,&dec1 );
    ma ( np.n_lat, ha1, dec1, &alt, &az, &fpa );
    *AZ_cal=az*180/PI;
    *ALT_cal=alt*180/PI;
    *FR_cal=fpa*180/PI;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void CalculateThread::run()
{
    //while(true)
    //for (int n = 0; n < 10;++n) {
    //printf("%d%d%d%d%d%d%d%d\n",n,n,n,n,n,n,n,n);
    //  qDebug()<<n;

    qDebug()<<cal_flag;

    QDate date[TableNumMAX];
    QDateTime dt[TableNumMAX];
    QTime time[TableNumMAX];
    //QDate date[TableNum];
    //QDateTime dt[TableNum];
    //QTime time[TableNum];
    dt[0]=QDateTime::currentDateTime();


    date[0]=dt[0].date();
    time[0]=dt[0].time();

    DT[0].year=date[0].year();
    DT[0].month=date[0].month();
    DT[0].day=date[0].day();

    DT[0].hour=time[0].hour();
    DT[0].minute=time[0].minute();
    DT[0].second=time[0].second();
    DT[0].msec=time[0].msec();

    calculate (RA_h,RA_m,RA_s,DEC_d,DEC_m,DEC_s,Temperature,Pressure,&AZ[0],&ALT[0],&FR[0],&HA[0],&DEC[0],DT[0] );


    for ( long i=1;i<TableNum;i++ )
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
        calculate (RA_h,RA_m,RA_s,DEC_d,DEC_m,DEC_s,Temperature,Pressure,&AZ[i],&ALT[i],&FR[i],&HA[i],&DEC[i],DT[i] );

        //	calculate(&AZ[i],&ALT[i],time[i]);
    }

    cal_flag=2;//calculate star finished

    qDebug()<<cal_flag;
}

