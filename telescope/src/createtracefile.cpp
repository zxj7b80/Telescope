#include "createtracefile.h"
#include <QtGui>
#include "BSCSearch.h"
#include "TimeAndCoordinate.h"
#include "ADefine.h"

double GLongitude,GLatitude;
QString GStarTraceListfile;
int periodTime=100;

createtracefile::createtracefile()
{

}

double createtracefile::caculate_epoctime(QDateTime datetime)
{
    int year,month,day,minute,second,microsec,hour;
    double dhour;

    //TimeAndCoordinate timecoordinate;
    //UTC time
    datetime = datetime.toUTC();
    QDate date = datetime.date();
    QTime time = datetime.time();
    year = date.year();
    month = date.month();
    day = date.day();
    hour = time.hour();
    minute = time.minute();
    second = time.second();
    microsec = time.msec() ;

    dhour = double(hour) + double(minute)/60 + double(second)/3600 + double(microsec)/3600000;
    //qDebug()<<"dhour="<<dhour;
    return ats.GetEpoch(year,month,day, dhour);
    //return timecoordinate.GetEpoch(year,month,day,hour,minute,second,microsec);
}

double createtracefile::caculate_lmstime(double mjd)
{

    return ats.LocalMeanSiderialTime(mjd, GLongitude);

}


void createtracefile::getazaltrot(double ra,double de,QDateTime datetime,char mode,double airpressure, double temperature, double &ret_az, double &ret_alt, double &ret_rot,double &ret_ha)
{

    int year,month,day,hour,minute,second,microsec;
    double dhour;

    datetime = datetime.toUTC();
    QDate date = datetime.date();
    QTime time = datetime.time();
    year = date.year();
    month = date.month();
    day = date.day();
    hour = time.hour();
    minute = time.minute();
    second = time.second();
    microsec = time.msec() ;

    //qDebug()<<"hour="<<hour;

    dhour = double(hour) + double(minute)/60 + double(second)/3600 + double(microsec)/3600000;
    //qDebug()<< "dhour=" << dhour;


    double mjd = ats.ModifiedJulianDay(year, month, day, dhour);

    //printf ("JD1 = %f\n", mjd + 2400000.5);

    double lmst= ats.LocalMeanSiderialTime(mjd, GLongitude);
    ra = ra * GtoR;	// J2000
    de = de * GtoR;
    //qDebug()<<"epoch 2000 ra="<<ra * RtoG<<" dec="<< de * RtoG;


    double ep = ats.GetEpoch(year, month, day, dhour);
    double azi, alt,rot,ref;
    double ha;
    char szra[35], szdec[35];
    char szaz[35], szalt[35];
    int hh, mm, dd, sign;
    double ss;
    ats.Epoch2Actual(2000, ra, de, ep, ra, de);
    ha = lmst - ra;
    ats.Eq2AltAzi(ha , de, GLatitude, azi, alt);
    ats.Deg2DMS(azi * RtoG, dd, mm, ss, sign);
    if (sign == 1) szaz[0] = '+';
    else           szaz[0] = '-';
    sprintf (szaz + 1, "%d:%02d:%06.3f", dd, mm, ss);
    ats.Deg2DMS(alt * RtoG, dd, mm, ss, sign);
    if (sign == 1) szalt[0] = '+';
    else           szalt[0] = '-';
    sprintf (szalt + 1, "%d:%02d:%06.3f", dd, mm, ss);

    ats.RADbl2Str(ra * RtoG / 15, szra);
    ats.DECDbl2Str(de * RtoG, szdec);

    ats.Hour2HMS(lmst * RtoG / 15, hh, mm, ss);

    //qDebug() << "alt=" <<alt<<"airpressure="<<airpressure<<"temperature="<<temperature;
    ref = ats.RefractReal(alt, airpressure, temperature);
    //qDebug() << "ref=" <<ref * RtoS ;


    ret_az = azi * RtoG;
    ret_alt = (alt + ref) * RtoG;

    rot = ats.ParAngle(ha, de, GLatitude, mode);
    ret_rot = rot * RtoG;

    ret_ha =ha * RtoG;
    if (ret_ha > 180)       ret_ha -= 360;
    else if (ret_ha < -180) ret_ha += 360;

    //ats.ParAngle(ha, de, GLatitude, mode);
}

bool createtracefile::createtrackfile(QDateTime beginDateTime,QDateTime endDateTime,int starType,double ra,double dec)
{

    DateTime DT;
    int i_flag=0;
    double  az, alt, rot, ha,alt0,az0,rot0;

   starType  = 0 ; //delete warning
   i_flag = starType; //delete warning

    float f_temperature,f_airpressure;
    char mode;
    double ret_az,ret_alt,ret_rot,ret_ha;


    DT.year=beginDateTime.date().year();
    DT.month=beginDateTime.date().month();
    DT.day=beginDateTime.date().day();

    DT.hour=beginDateTime.time().hour();
    DT.minute=beginDateTime.time().minute();
    DT.second=beginDateTime.time().second();
//    DT.msec=beginDateTime.time().msec();

    DT.msec=0;

    //get derotator id
    mode = 'N';// "N" or "+" or "-"


        //qDebug() << "++++++++++++++++++++ra = " << ra << ", dec = " << dec;
        QFile file(GStarTraceListfile);

        if ( !file.open ( QFile::WriteOnly) ){
            //if ( (GAuto_plan == false) || ( Gtrack_hand_flag == true))
            //{
               QMessageBox::critical(0, "open file error",
                                  "Can't open list file.\n"
                                  "Click Cancel to exit.", QMessageBox::Cancel);
          //}
            //Gtrack_hand_flag = false;
            return false;
        }

        QTextStream out ( &file );


        char s_0[400];
        char s_1[60];
        char s_2[60];
        char s_3[20];
        char s_4[20];
        char s_5[20];
        char s_6[20];
        char s_7[20];


        sprintf ( s_0,"  yyyymmdd   hhmmss       AZ          ALT          DERO            HA          DEC \n" );
        out << s_0;

        //judge the temperaure and air pressure value
                /*if( (GShow_value[GWEATHER_temperature]=="") || (GShow_value[GWEATHER_pressure]==""))
                {

                    if ( (GAuto_plan == false) || ( Gtrack_hand_flag == true))
                    {
                       if( (QMessageBox::question(0, "Warning", "Can't get right temperature or air pressure value ,use default value,Do you want to continue", QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No), QMessageBox::Yes) == QMessageBox::No))
                       {
                           Gtrack_hand_flag = false;
                           return false;
                       }
                       else
                       {
                           f_temperature = GTemperature_default;
                           f_airpressure = GAirpressure_default;
                       }
                   }
                    else
                    {
                        f_temperature = GTemperature_default;
                        f_airpressure = GAirpressure_default;
                    }
                }
                else
                {

                   f_temperature = GShow_value[GWEATHER_temperature].toFloat();
                   f_airpressure = GShow_value[GWEATHER_pressure].toFloat();
                   qDebug()<<"temper="<<f_temperature<<"airpress="<<f_airpressure;
                }*/

       // f_temperature = GTemperature_default;
        //f_airpressure = GAirpressure_default;
        //qDebug()<<"temperature="<< f_temperature<<"pressure="<<f_airpressure;

        while(endDateTime > beginDateTime){
            QCoreApplication::processEvents();
            sleep(0);

            getazaltrot(ra,dec,beginDateTime,mode,f_airpressure,f_temperature,ret_az, ret_alt, ret_rot,ret_ha);
            az = ret_az;
            rot = ret_rot;
            alt = ret_alt;
            ha = ret_ha;

            //qDebug()<<"az="<<az<<"alt="<<alt;
            /*if(i_flag==0)//first time
            {
                i_flag = 1;
                alt0 = alt;
                az0 = az;
                rot0 = rot;
            }
            else
            {
                if ((qAbs(alt - alt0) > 1) || (qAbs(az - az0) > 1) || (qAbs(rot - rot0) > 1))//to the limit turn back
                {

                    qDebug()<<"alt="<< alt - alt0 << "az=" << az - az0 << "rot=" << rot - rot0 ;
                    qDebug()<<"az="<<az<< "az0="<<az0;

                    if ( (GAuto_plan == false) || ( Gtrack_hand_flag == true))
                    {
                       QMessageBox::critical(0, "Track warnning",
                                          "The postion is to the limit.", QMessageBox::Cancel);
                    }
                    file.close();
                    Gtrack_hand_flag = false;
                    return false;
                }
                else
                {
                    az0 = az;
                    rot0 = rot;
                    alt0 = alt;
                }

            }*/

            /*if ( alt > 89.9 ){
                 if ( (GAuto_plan == false) || ( Gtrack_hand_flag == true))
                {
                   QMessageBox::critical(0, "Track warnning",
                                      "The altitude in tracking is greater than 89.9 degree.\n"
                                      "Click Cancel to exit.", QMessageBox::Cancel);
                }
                file.close();
                Gtrack_hand_flag = false;
                return false;
            }

            if ( alt < 7.5 ){
                if ( (GAuto_plan == false) || ( Gtrack_hand_flag == true))
                {
                   QMessageBox::critical(0, "Track warnning",
                                      "The altitude in tracking is less than 7.5 degree.\n"
                                      "Click Cancel to exit.", QMessageBox::Cancel);
                }
                file.close();
                Gtrack_hand_flag = false;
                return false;
            }*/


            //create list file
            sprintf ( s_1,"  %d%02d%02d",DT.year,DT.month,DT.day );
            //second=double(DT.second)+double( DT.msec/1000.0 );
            //sprintf ( s_2,"  %02d%02d%05.2f",DT.hour,DT.minute,second );
            sprintf ( s_2,"  %02d%02d%02d.%02d",DT.hour,DT.minute,DT.second,int (DT.msec / 10));
            sprintf ( s_3,"  %10.7f",az );
            sprintf ( s_4,"  %10.7f",alt );
            sprintf ( s_5,"  %10.7f",rot );
            sprintf ( s_6,"  %10.7f",ha );
            sprintf ( s_7,"  %10.7f",dec);
            //sprintf ( s_0,"%s%s%s%s%s%s%s\n",s_1,s_2,s_3,s_4,s_5,s_6,s_7 );
            sprintf ( s_0,"%s%s%s%s%s%s%s\n",s_1,s_2,s_3,s_4,s_5,s_6,s_7);
            out << s_0;

            DT.msec=DT.msec+periodTime;			//every 0.01s get a star position
            while ( DT.msec>=1000 )
            {
                    DT.msec=DT.msec-1000;
                    DT.second=DT.second+1;
            }
            while ( DT.second>=60 )
            {
                    DT.second=DT.second-60;
                    DT.minute=DT.minute+1;
            }
            while ( DT.minute>=60 )
            {
                    DT.minute=DT.minute-60;
                    DT.hour=DT.hour+1;
            }

            beginDateTime = beginDateTime.addMSecs(periodTime);
        }

        file.close();
         /*if ( (GAuto_plan == false) || ( Gtrack_hand_flag == true))
        {
           Gtrack_hand_flag = false;
           QMessageBox::information(0, "done", "Create list file success.\n");
        }*/


    return true;
}


