/*
  C++ Implementation: direct_dero.cpp
 function: Service Mode of C1,N2,N1

*/
#include "direct_dero.h"
#include "signs.h"
#include "main_window.h"
#include "server.h"
#include "writelog.h"
#define c1_vel_spec_BOTTOM     -7200
#define c1_vel_spec_TOP         7200
#define c1_vel_spec_decimals      3
#define n2_vel_spec_BOTTOM     -7200
#define n2_vel_spec_TOP         7200
#define n2_vel_spec_decimals      3
#define n1_vel_spec_BOTTOM     -7200
#define n1_vel_spec_TOP         7200
#define n1_vel_spec_decimals      3

#define c1_targetD_BOTTOM       -360
#define c1_targetD_TOP          360
#define c1_targetD_decimals      10

#define c1_targetM_BOTTOM        -60
#define c1_targetM_TOP           60

#define c1_targetS_BOTTOM        -60
#define c1_targetS_TOP           60
#define c1_targetS_decimals      4

#define n1_targetD_BOTTOM       -360
#define n1_targetD_TOP          360
#define n1_targetD_decimals      10

#define n1_targetM_BOTTOM       -60
#define n1_targetM_TOP          60

#define n1_targetS_BOTTOM       -60
#define n1_targetS_TOP          60
#define n1_targetS_decimals      4


#define n2_targetD_BOTTOM       -360
#define n2_targetD_TOP          360
#define n2_targetD_decimals      10

#define n2_targetM_BOTTOM       -60
#define n2_targetM_TOP          60

#define n2_targetS_BOTTOM       -60
#define n2_targetS_TOP          60
#define n2_targetS_decimals      4

#define DeroInfoClock      	100						//time for timer_DeroInfo,500ms
#define DeroCheckStatusClock    2500						//time for timer_DeroCheckStatus,2500ms
#define DeroSaveClock    	100						//time for timer_save,100ms
#define DeroCheckCommandFromOCSClock    	1000				//time for timerDer_CheckCommandFromOCS,1000ms
#define DeroreplyOCSClock    	5000						//time for timer_DeroreplyOCS,5000ms



extern FlagTCS flagtcs;
extern double Dero_point;							//C1 target
sDero s_Dero;										//strcture of C1
sADC s_ADC2, s_ADC1;										//strcture of N1
extern OcsCOM OcsCom;								//structure of comminication with OCS/TCS

DirectDero::DirectDero ( MainWindow *parent )
 
{
	setupUi ( this );
	m_parent = parent;
	initUi();
	timer_show = new QTimer;

	timer_DeroInfo = new QTimer();
	connect ( timer_DeroInfo,SIGNAL ( timeout() ),this,SLOT ( DeroInfo() ) );
	timer_DeroInfo->start ( DeroInfoClock );

	timer_DeroCheckStatus = new QTimer();//check status
	connect ( timer_DeroCheckStatus,SIGNAL ( timeout() ),this,SLOT ( DeroCheckStatus() ) );
	timer_DeroCheckStatus->start ( DeroCheckStatusClock );

	timer_save = new QTimer();//save file
	connect ( timer_save,SIGNAL ( timeout() ),this,SLOT ( saveData() ) );
	timer_save->start ( DeroSaveClock );

	timerDer_CheckCommandFromOCS=new QTimer ( this );// check command from OCS every  second
	timerDer_CheckCommandFromOCS->connect ( timerDer_CheckCommandFromOCS, SIGNAL ( timeout() ), this, SLOT ( CheckCommandFromOCS() ) );
	timerDer_CheckCommandFromOCS->start ( DeroCheckCommandFromOCSClock);

	timer_DeroreplyOCS=new QTimer ( this );
	timer_DeroreplyOCS->connect ( timer_DeroreplyOCS, SIGNAL ( timeout() ), this, SLOT ( DeroreplyOCS() ) );

	flagtcs.IsOpened_direct_dero=true;

        setValidator();
}



DirectDero::~DirectDero()
{
	flagtcs.IsOpened_direct_dero=false;
	qDebug ( "direct_dero" );
	timer_show->deleteLater();
	timer_save->deleteLater();
}


void DirectDero::initUi()
{
     	radioButton_c1_vel_mid->setChecked ( TRUE );
	radioButton_c1_stepMid->setChecked ( TRUE );
     	radioButton_n2_vel_mid->setChecked ( TRUE );
	radioButton_n2_stepMid->setChecked ( TRUE );
     	radioButton_n1_vel_mid->setChecked ( TRUE );
	radioButton_n1_stepMid->setChecked ( TRUE );

        radioButton_c1_stepLong->setText(QString::number(SMySetting::instance_p()->value ( "dero/c1_stepLong" ).toDouble()));
	radioButton_c1_stepMid->setText(QString::number(SMySetting::instance_p()->value ( "dero/c1_stepMid" ).toDouble()));
	radioButton_c1_stepShort->setText(QString::number(SMySetting::instance_p()->value ( "dero/c1_stepShort" ).toDouble()));
        radioButton_n2_stepLong->setText(QString::number(SMySetting::instance_p()->value ( "dero/n2_stepLong" ).toDouble()));
	radioButton_n2_stepMid->setText(QString::number(SMySetting::instance_p()->value ( "dero/n2_stepMid" ).toDouble()));
	radioButton_n2_stepShort->setText(QString::number(SMySetting::instance_p()->value ( "dero/n2_stepShort" ).toDouble()));
        radioButton_n1_stepLong->setText(QString::number(SMySetting::instance_p()->value ( "dero/n1_stepLong" ).toDouble()));
	radioButton_n1_stepMid->setText(QString::number(SMySetting::instance_p()->value ( "dero/n1_stepMid" ).toDouble()));
	radioButton_n1_stepShort->setText(QString::number(SMySetting::instance_p()->value ( "dero/n1_stepShort" ).toDouble()));

	flag_C1Homing=false;
}
void DirectDero::closeEvent ( QCloseEvent *event )
{
	//parentWidget()->hide();
	//event->ignore();
}

void  DirectDero::setValidator()
{
	lineEdit_c1_vel_spec->setValidator ( new MyDoubleValidator ( c1_vel_spec_BOTTOM,c1_vel_spec_TOP,c1_vel_spec_decimals,this ) );
        lineEdit_n2_vel_spec->setValidator ( new MyDoubleValidator ( n2_vel_spec_BOTTOM,n2_vel_spec_TOP,n2_vel_spec_decimals,this ) );
	lineEdit_n1_vel_spec->setValidator ( new MyDoubleValidator ( n1_vel_spec_BOTTOM,n1_vel_spec_TOP,n1_vel_spec_decimals,this ) );

        lineEdit_c1_targetD->setValidator ( new MyDoubleValidator ( c1_targetD_BOTTOM,c1_targetD_TOP,c1_targetD_decimals,this ) );
	lineEdit_c1_targetM->setValidator ( new QIntValidator ( c1_targetM_BOTTOM,c1_targetM_TOP,this ) );
	lineEdit_c1_targetS->setValidator ( new MyDoubleValidator ( c1_targetS_BOTTOM,c1_targetS_TOP,c1_targetS_decimals,this ) );
       
	lineEdit_n1_targetD->setValidator ( new MyDoubleValidator ( n1_targetD_BOTTOM,n1_targetD_TOP,n1_targetD_decimals,this ) );
	lineEdit_n1_targetM->setValidator ( new QIntValidator ( n1_targetM_BOTTOM,n1_targetM_TOP,this ) );
	lineEdit_n1_targetS->setValidator ( new MyDoubleValidator ( n1_targetS_BOTTOM,n1_targetS_TOP,n1_targetS_decimals,this ) );

        lineEdit_n2_targetD->setValidator ( new MyDoubleValidator ( n2_targetD_BOTTOM,n2_targetD_TOP,n2_targetD_decimals,this ) );
	lineEdit_n2_targetM->setValidator ( new QIntValidator ( n2_targetM_BOTTOM,n2_targetM_TOP,this ) );
	lineEdit_n2_targetS->setValidator ( new MyDoubleValidator ( n2_targetS_BOTTOM,n2_targetS_TOP,n2_targetS_decimals,this ) );
}

void DirectDero::DeroInfo()
{
	GetC1Position();
	GetN2Position();
	GetN1Position();

}
void DirectDero::DeroCheckStatus()
{
	CheckAmplifier();
	CheckError();
	CheckLoopStatus();
	CheckBreak();
        CheckC1Homing();
	CheckN2Homing();
	CheckN1Homing();
	CheckLimit();
	CheckSoftLimit();
}



/*
 output:c1.c1PosReal_Degree,Global Variable
	c1.c1Seconds,Global Variable
	c1.c1Pointer,Global Variable
 input:	c1.C1DataTime,Global Variable
       	c1.C1Data,Global Variable
	c1.c1Seconds,Global Variable
	c1.c1Pointer,Global Variable
*/

void DirectDero::GetC1Position()
{
	char  cmd[255];
	sprintf ( cmd,"m461m462m464m465m474" );//m474:C1 average velocity
	int n=5;
	double c1FolErrUMAC=0;
	double c1Velocity;
	int C1_d,C1_m;
	double C1_s;
	double c1PosReal_Degree;

        GetDataFromUMAC ( cmd,n,CTS_Dero,&c1PosReal_Degree,&c1FolErrUMAC,&c1Velocity,s_Dero.Seconds,&s_Dero.Seconds,s_Dero.Pointer,&s_Dero.Pointer,s_Dero.Data,s_Dero.DataTime,Q);

	//show data
	D2DMS ( c1PosReal_Degree,&C1_d,&C1_m,&C1_s );

	lineEdit_c1_posD->setText ( QString::number ( C1_d ) );
	lineEdit_c1_posM->setText ( QString::number ( C1_m ) );
	lineEdit_c1_posS->setText ( QString::number ( C1_s,'f',2 ) );
	lineEdit_c1_poserr->setText ( QString::number ( c1FolErrUMAC,'f',2 ) );
	lineEdit_c1_vel->setText ( QString::number ( c1Velocity,'f',2 ) );

        s_Dero.PosReal_Degree=c1PosReal_Degree;		//output:c1.c1PosReal_Degree,Global Variable

}




//////////////////////////////////////////////////////////////
/*//input:
char cmd[];		//command that send out to UMAC
int n;     		//number of commands
double Seconds_old;	//last seconds,for save
int* Pointer;		//line of table
int PointerLast	        //last 	Pointer
double cts:		//	encoder ratio
int num:		//matrix Data rows

output:
double PosCmd:			command  position;//
double PosReal_Degree:		real position,degree
double FolErrUMAC:		following error  ,calculated by UMAC

double Velocity:
double Seconds:                 time for save date
double Seconds_old:             time for save date
int* Pointer:   		pointer of table line
char *DataStore:             	Data for save
double (*p)[5]:                 Data[Q][5]


*/
 

void DirectDero::GetDataFromUMAC ( char cmd[],int n,double cts,double* PosReal_Degree,double* FolErrUMAC,double* Velocity,double Seconds_old,double* Seconds ,int PointerLast,int* Pointer,double ( *p ) [6],QString DataTime[],int num )
{
	QString str;
	double PosCmd;		//command  position
	double PosReal;		//real position,seconds
	double FolErrPC;	//following error  ,calculated by PC

	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	QStringList  list = str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	int it=0;
	for ( QStringList::iterator point = list.begin(); point != list.end();++it, ++point )
		{}
	if ( it<n )
	{
//		UMAC_ETHERNET=false;
		return;
	};
//        UMAC_ETHERNET=true;
	PosCmd=list[0].toDouble() *cts/3072.0;			// command, float value

	PosReal= ( ( list[1].toDouble() +list[2].toDouble() ) /3072.0 ) *cts; // arcsec of real position
	*PosReal_Degree=PosReal/3600.0;// degree

	*FolErrUMAC=list[3].toDouble() *cts/3072.0;	// following error  ,calculated by UMAC
	FolErrPC=PosCmd-PosReal;			//following error ,calculated by PC,arcsec
	*Velocity=list[4].toDouble() *cts*1000*2/3072.0;// change counts/ms to "/s

	QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );
	int msecond=time.msec();
	*Seconds=time.second() *1000+msecond;//ms

	if ( fabs ( *Seconds-Seconds_old ) >=90&&*Pointer<= ( num-1 ) ) //every 100ms
	{
		* ( * ( p+*Pointer ) +0 ) =PosCmd;	//Data[Pointer][0]
		* ( * ( p+*Pointer ) +1 ) =PosReal;	//Data[Pointer][1]
		* ( * ( p+*Pointer ) +2 ) =*FolErrUMAC;	//Data[Pointer][2]
		* ( * ( p+*Pointer ) +3 ) =FolErrPC;	//Data[Pointer][3]


		DataTime[*Pointer]=dtstr;
		*Pointer=PointerLast+1;
		//*Seconds_old=*Seconds;
	}


}



////////////////////////////   n2////////////////////////////////////
/*
////output: 	n2.n2PosReal_Degree ,Global Variable
     		n2.n2Seconds,Global Variable
                n2.n2Pointer,Global Variable
    input:	n2.N2DataTime,Global Variable
       		n2.N2Data,Global Variable
                n2.n2Seconds,Global Variable
                n2.n2Pointer,Global Variable
*/
void DirectDero::GetN2Position()
{

	char  cmd[255];
	sprintf ( cmd,"m661m662m664m665m674" );//m674:N2 average velocity
	int n=5;
	double n2FolErrUMAC=0;
	double n2Velocity;
	int N2_d,N2_m;
	double N2_s;
        double n2PosReal_Degree;

        GetDataFromUMAC ( cmd,n,CTS_ADC,&n2PosReal_Degree,&n2FolErrUMAC,&n2Velocity,s_ADC2.Seconds,&s_ADC2.Seconds,s_ADC2.Pointer,&s_ADC2.Pointer,s_ADC2.Data,s_ADC2.DataTime,Q);

	//show data
	D2DMS ( n2PosReal_Degree,&N2_d,&N2_m,&N2_s );

	lineEdit_n2_posD->setText ( QString::number ( N2_d ) );
	lineEdit_n2_posM->setText ( QString::number ( N2_m ) );
	lineEdit_n2_posS->setText ( QString::number ( N2_s,'f',2 ) );
    	lineEdit_n2_poserr->setText ( QString::number ( n2FolErrUMAC,'f',2 ) );
	lineEdit_n2_vel->setText ( QString::number ( n2Velocity,'f',2 ) );

        s_ADC2.PosReal_Degree=n2PosReal_Degree; 		//output: n2.n2PosReal_Degree ,Global Variable

}
////////////////////////////   n1////////////////////////////////////
/*
 output: n1.n1PosReal_Degree ,Global Variable
	 n1.n1Seconds,Global Variable
	 n1.n1Pointer,Global Variable
input:	n1.N1Data,,Global Variable
     	n1.N1DataTime,Global Variable
	n1.n1Seconds,Global Variable
        n1.n1Pointer,Global Variable
*/
void DirectDero::GetN1Position()
{

	char  cmd[255];
	sprintf ( cmd,"m861m862m864m865m874" );//m874:N1 average velocity
	int n=5;
	double n1FolErrUMAC=0;
	double n1Velocity;
	int N1_d,N1_m;
	double N1_s;
	double n1PosReal_Degree;
	
        GetDataFromUMAC ( cmd,n,CTS_ADC,&n1PosReal_Degree,&n1FolErrUMAC,&n1Velocity,s_ADC1.Seconds,&s_ADC1.Seconds,s_ADC1.Pointer,&s_ADC1.Pointer,s_ADC1.Data,s_ADC1.DataTime,Q );


	//show data
	D2DMS ( n1PosReal_Degree,&N1_d,&N1_m,&N1_s );

	lineEdit_n1_posD->setText ( QString::number ( N1_d ) );
	lineEdit_n1_posM->setText ( QString::number ( N1_m ) );
	lineEdit_n1_posS->setText ( QString::number ( N1_s,'f',2 ) );
	lineEdit_n1_poserr->setText ( QString::number ( n1FolErrUMAC,'f',2 ) );
	lineEdit_n1_vel->setText ( QString::number ( n1Velocity,'f',2 ) );

        s_ADC1.PosReal_Degree=n1PosReal_Degree;
}

void DirectDero::D2DMS ( double Degree,int* Deg,int* Min,double* Sec ) //change degree vlaue as degrees+minute+seconds
{

	*Deg=int ( Degree );
	*Min=int ( ( Degree-*Deg ) *60 );
	*Sec= ( ( Degree-*Deg ) *60-*Min ) *60;

	if ( *Deg!=0 )
		*Min=abs ( *Min );
	if ( *Deg||*Min )
		*Sec=fabs ( *Sec );
}


/*
input:	c1.c1Pointer,Global Variable
	n2.n2Pointer,Global Variable
	n1.n1Pointer,Global Variable


*/

void DirectDero::saveData() //timer
{
	if ( !radioButton_DeroSave->isChecked() )
		return;

        if ( s_Dero.Pointer>=Q )
	{
		storeC1Data();
                s_Dero.Pointer=0;
	}
        if ( s_ADC2.Pointer>=Q )
	{
		storeN2Data();
                 s_ADC2.Pointer=0;
	}

        if (  s_ADC1.Pointer>=Q )
	{
		storeN1Data();
                 s_ADC1.Pointer=0;
	}


}

 

/*  function: save data to a file
//input:
 int num       //matrix AzData rows,#define Q 300  
double ( *p ) [5]:   save data,----C1Data,N1Data,N2Data
QString DataTime[]:save time
QString FileName: file name

*/

bool DirectDero::DataSave ( double ( *p ) [6],QString DataTime[],QString FileName,int num )
{

	char s_0[400] ;
	char s_1[60] ;
	char s_2[60];
	char s_3[60];
	char s_4[60];
	int i,j;
	double t_end,t_start,t;

	QStringList  list1 = DataTime[299].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );

	int it=0;
	for ( QStringList::iterator point = list1.begin(); point != list1.end();++it, ++point )
		{}
	it=0;
	QStringList  list_time =  list1[1].split ( ":" );
	for ( QStringList::iterator point = list_time.begin(); point != list_time.end();++it, ++point )
		{}
	t_end=list_time[0].toDouble() *3600+list_time[1].toDouble() *60+list_time[2].toDouble();
	//start time
	list1 = DataTime[0].split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );

	it=0;
	for ( QStringList::iterator point = list1.begin(); point != list1.end();++it, ++point )
		{}
	it=0;
	list_time =  list1[1].split ( ":" );
	for ( QStringList::iterator point = list_time.begin(); point != list_time.end();++it, ++point )
		{}
	t_start=list_time[0].toDouble() *3600+list_time[1].toDouble() *60+list_time[2].toDouble();

	t=t_end-t_start;
	if ( t<0 )
		t=30;
	////////////following error/////////////////////////////////////////
	double speed= ( * ( * ( p+num-1 ) +1 ) - * ( * ( p+0 ) +1 ) ) /t;	//Data[num-1][1]-Data[0][1]   get speed

	//get RMS of tracking error
	double Xi=0,Yi=0;
	for ( j=0;j<num;j++ ) 				//  N:   matrix AzData rows
		Xi= * ( * ( p+j ) +2 ) +Xi;				//Data[j][2]

	Xi=Xi/num;

	for ( j=0;j<num;j++ )
		Yi= ( * ( * ( p+j ) +2 )-Xi ) * ( * ( * ( p+j ) +2 )-Xi ) +Yi;//Data[j][2]

	Yi=sqrt ( Yi/num );

	//PV
	double min,max;
	double PV;
	max=min=* ( * ( p+0 ) +2 );				//Data[0][2]

	for ( j=0;j<num;j++ )
	{
		if ( max<=* ( * ( p+j ) +2 ) )     		//Data[j][2]
		{
			max=* ( * ( p+j ) +2 );             	//Data[j][2]
		}
		if ( min>* ( * ( p+j ) +2 ) )			//Data[j][2]
		{
			min=* ( * ( p+j ) +2 );		//Data[j][2]

		}
	}
	PV=max-min;



	////////////// save file////////////////////
	QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "MM_dd_yyyy" );
	QString fileName =SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString()+FileName+dtstr+".txt";
	QFile file ( fileName );
	if ( !file.open ( QFile::Append|QFile::Truncate ) )
	{
		QMessageBox::warning ( this,
		                       tr ( "Warn" ),
		                       tr ( "open error!" ),
		                       QMessageBox::Yes );
		return false;
	}
	QTextStream out ( &file );

	for ( i=0;i<num;i++ )
	{
		out<<"\n";
		sprintf ( s_1,"  %10.6f",* ( * ( p+i ) +0 ) /3600 );//command position（UMAC取出,"）     Data[i][0]
		sprintf ( s_2,"  %10.6f",* ( * ( p+i ) +1 ) /3600 );//real position,"                    Data[i][1]
		sprintf ( s_3,"  %10.4f",* ( * ( p+i ) +3 ) );//following error,"  ,  PC             Data[i][2]
		sprintf ( s_4,"  %10.4f",* ( * ( p+i ) +2 ) );//following error,"  ,  UMAC             Data[i][2]
		sprintf ( s_0,"%s%s%s%s       ",s_1,s_2,s_3,s_4 );
		QString string = QString ( QLatin1String ( s_0 ) ) + DataTime[i];
		out<<string;
	}

	sprintf ( s_1," V=  %6.3f''/s,",speed );
	sprintf ( s_2,"RMS=  %6.4f'' ",Yi );
	sprintf ( s_3,"PV=  %6.4f'' ",PV );
	sprintf ( s_0,"%s%s%s ",s_1,s_2,s_3 );
	out<<tr ( s_0 );
	sprintf ( s_1," max=  %6.4f'',",max );
	sprintf ( s_2," min=  %6.4f''",min );
	out<<tr ( s_1 );
	out<<tr ( s_2 );
	file.close();
	return true;


}



/*
//input: c1.C1Data ,Global Variable
         c1.C1DataTime,Global Variable
*/
bool DirectDero::storeC1Data()
{
	//C1Datamm_dd_yy.txt
	QString FileName="/C1Data";
        DataSave (s_Dero.Data,s_Dero.DataTime,FileName,Q );
	return TRUE;
}
/*
//input: 	n2.N2Data ,Global Variable
   		n2.N2DataTime,Global Variable
*/

bool DirectDero::storeN2Data()
{
//N2Datamm_dd_yy.txt
	QString FileName="/N2Data";
        DataSave ( s_ADC2.Data,s_ADC2.DataTime,FileName,Q);
	return TRUE;
}
/*
  input: n1.N1Data ,Global Variable
         n1.N1DataTime,Global Variable
*/
bool DirectDero::storeN1Data()
{
//N1Datamm_dd_yy.txt
	QString FileName="/N1Data";
        DataSave ( s_ADC1.Data,s_ADC1.DataTime,FileName,Q );
	return TRUE;

}

void DirectDero::on_pushButton_c1_enable_clicked()
{
	QString str;
	char comm[255];
	sprintf ( comm,"ENABLE PLC20" );  // ENABLE C1 MOTOR 1# ,2#
	SQUMAC::instance_p()->QcommCmdPMAC ( comm,str );
}

void DirectDero::on_pushButton_c1_disable_clicked()
{
	QString str;
	char comm[255];
	//sprintf(comm,"m154=0#1K");
	//qumac->QcommCmdPMAC(comm,str);
	//sprintf ( comm,"#4K#5K" );  // DISABLE C1 MOTOR 1# 2#
	sprintf ( comm,"DISABLE PLC7" );  // DISABLE C1 homing
	sprintf ( comm,"ENABLE PLC23" );  // DISABLE C1 MOTOR 1# 2#
	SQUMAC::instance_p()->QcommCmdPMAC ( comm,str );
}

void DirectDero::on_pushButton_c1_home_clicked()
{
	QString str;
	SQUMAC::instance_p()->QcommCmdPMAC ( "DISABLE PLC7",str );
	SQUMAC::instance_p()->QcommCmdPMAC ( "P464=0",str );
	SQUMAC::instance_p()->QcommCmdPMAC ( "P417=3",str );
	SQUMAC::instance_p()->QcommCmdPMAC ( "ENABLE PLC7",str );
	//SQUMAC::instance_p()->QcommCmdPMAC ( "P901=0",str );//flag
}

void DirectDero::on_pushButton_n1_enable_clicked()
{
	QString str;
	char comm[255];
	sprintf ( comm,"ENABLE PLC22" );  // ENABLE N1 MOTOR 1# ,2#
	SQUMAC::instance_p()->QcommCmdPMAC ( comm,str );
}

void DirectDero::on_pushButton_n1_disable_clicked()
{
	QString str;
	char comm[255];
	//sprintf(comm,"m154=0#1K");
	//qumac->QcommCmdPMAC(comm,str);
	//sprintf ( comm,"#8K#9K" );  // DISABLE N1 MOTOR 1#  2#
	sprintf ( comm,"ENABLE PLC25" );  // DISABLE N1 MOTOR 1#  2#
	SQUMAC::instance_p()->QcommCmdPMAC ( comm,str );
	SQUMAC::instance_p()->QcommCmdPMAC ( "DISABLE PLC9",str );
	SQUMAC::instance_p()->QcommCmdPMAC ( "P817=0",str );
}

void DirectDero::on_pushButton_n1_home_clicked()
{
	QString str;
	SQUMAC::instance_p()->QcommCmdPMAC ( "DISABLE PLC9",str );
	SQUMAC::instance_p()->QcommCmdPMAC ( "P864=0",str );
	SQUMAC::instance_p()->QcommCmdPMAC ( "P817=3",str );
	SQUMAC::instance_p()->QcommCmdPMAC ( "ENABLE PLC9",str );
      //SQUMAC::instance_p()->QcommCmdPMAC ( "P908=0",str );//flag
}

void DirectDero::on_pushButton_n2_enable_clicked()
{
	QString str;
	char comm[255];
	sprintf ( comm,"ENABLE PLC21" );  // ENABLE N2 MOTOR 1# ,2#
	SQUMAC::instance_p()->QcommCmdPMAC ( comm,str );
}

void DirectDero::on_pushButton_n2_disable_clicked()
{
	QString str;
	char comm[255];
	//sprintf(comm,"m154=0#1K");
	//qumac->QcommCmdPMAC(comm,str);
	//sprintf ( comm,"#6K#7K" );  // DISABLE N2 MOTOR 1#  2#
	sprintf ( comm,"ENABLE PLC24" );  // DISABLE N2 MOTOR 1#  2#
	SQUMAC::instance_p()->QcommCmdPMAC ( comm,str );

}

void DirectDero::on_pushButton_n2_home_clicked()
{
	QString str;
	SQUMAC::instance_p()->QcommCmdPMAC ( "DISABLE PLC8",str );
	SQUMAC::instance_p()->QcommCmdPMAC ( "P664=0",str );
	SQUMAC::instance_p()->QcommCmdPMAC ( "P617=3",str );
	SQUMAC::instance_p()->QcommCmdPMAC ( "ENABLE PLC8",str );

	//SQUMAC::instance_p()->QcommCmdPMAC ( "P1001=0",str );//flag
}

void DirectDero::on_lineEdit_c1_vel_spec_returnPressed()
{
	
        char cmd[255];
	QString str;
	 
        c1_vel = lineEdit_c1_vel_spec->text().toFloat() / ( CTS_Dero*1000 );
	sprintf ( cmd,"I422=%fI522=%f",c1_vel,c1_vel);
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );

}



void DirectDero::on_lineEdit_n1_vel_spec_returnPressed()
{       
        char cmd[255];
	QString str;
        n1_vel = lineEdit_n1_vel_spec->text().toFloat() / ( CTS_ADC*1000 );
        sprintf ( cmd,"I822=%fI922=%f",n1_vel,n1_vel);
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}



void DirectDero::on_lineEdit_n2_vel_spec_returnPressed()
{       
	char cmd[255];
	QString str;
        n2_vel = lineEdit_n2_vel_spec->text().toFloat() / ( CTS_ADC*1000 );
        sprintf ( cmd,"I622=%fI722=%f",n2_vel,n2_vel);
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );

}

void DirectDero::setPIDparam_C1_going()
{
	//C1
	QString str;
	double c1VelocityCmd=0;

	c1VelocityCmd=lineEdit_c1_vel_spec->text().toFloat();
	if ( fabs ( c1VelocityCmd ) >=3600 )
                {SQUMAC::instance_p()->QcommCmdPMAC (DeroPointParam,str );SQUMAC::instance_p()->QcommCmdPMAC (DeroPointParam,str );}
	else if (fabs ( c1VelocityCmd ) <3600 )
                {SQUMAC::instance_p()->QcommCmdPMAC (DeroTrackParam,str );SQUMAC::instance_p()->QcommCmdPMAC (DeroTrackParam,str );}
	
}





// void DirectDero::on_lineEdit_n2_acc_spec_returnPressed()
// {
// 	n2_acc = lineEdit_n2_acc_spec->text().toFloat()/( CTSDERO*1000 );
// }

void DirectDero::on_radioButton_c1_vel_fast_toggled ( bool on )
{
	if ( on )
	{
		char cmd[255];
		QString str;
		c1_vel = SMySetting::instance_p()->value ( "dero/c1_vel_fast" ).toDouble(); 
                lineEdit_c1_vel_spec->setText ( QString::number ( c1_vel) ); 
                 c1_vel=c1_vel / ( CTS_Dero*1000 );
                sprintf ( cmd,"I422=%fI522=%f",c1_vel,c1_vel );
		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	}
}



void DirectDero::on_radioButton_c1_vel_mid_toggled ( bool on )
{
	if ( on )
	{
		char cmd[255];
		QString str;
		c1_vel = SMySetting::instance_p()->value ( "dero/c1_vel_mid" ).toDouble(); 
                lineEdit_c1_vel_spec->setText ( QString::number ( c1_vel) ); 
                c1_vel=c1_vel / ( CTS_Dero*1000 );
		sprintf ( cmd,"I422=%fI522=%f",c1_vel,c1_vel );
		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	}

        
}

void DirectDero::on_radioButton_c1_vel_slow_toggled ( bool on )
{
	if ( on )
	{
		char cmd[255];
		QString str;
		c1_vel = SMySetting::instance_p()->value ( "dero/c1_vel_slow" ).toDouble(); 
                lineEdit_c1_vel_spec->setText ( QString::number ( c1_vel) ); 
                c1_vel=c1_vel / ( CTS_Dero*1000 );
		sprintf ( cmd,"I422=%fI522=%f",c1_vel,c1_vel );
		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	}
}
// void DirectDero::on_radioButton_c1_acc_default_toggled ( bool on )
// {
//
// }

void DirectDero::on_radioButton_n1_vel_fast_toggled ( bool on )
{
	if ( on )
	{
		char cmd[255];
		QString str;
		n1_vel = SMySetting::instance_p()->value ( "dero/n1_vel_fast" ).toDouble();
 		lineEdit_n1_vel_spec->setText ( QString::number ( n1_vel) ); 
                n1_vel =n1_vel /( CTS_ADC*1000 );
               	sprintf ( cmd,"I822=%fI922=%f",n1_vel,n1_vel );
		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	}
}

void DirectDero::on_radioButton_n1_vel_mid_toggled ( bool on )
{
	if ( on )
	{
		char cmd[255];
		QString str;
                n1_vel = SMySetting::instance_p()->value ( "dero/n1_vel_mid" ).toDouble();
 		lineEdit_n1_vel_spec->setText ( QString::number ( n1_vel) ); 
                n1_vel =n1_vel /( CTS_ADC*1000 );
		sprintf ( cmd,"I822=%fI922=%f",n1_vel,n1_vel );
		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	}
}

void DirectDero::on_radioButton_n1_vel_slow_toggled ( bool on )
{
	if ( on )
	{
		char cmd[255];
		QString str;
		n1_vel = SMySetting::instance_p()->value ( "dero/n1_vel_slow" ).toDouble();
 		lineEdit_n1_vel_spec->setText ( QString::number ( n1_vel) ); 
                n1_vel =n1_vel /( CTS_ADC*1000 );
		sprintf ( cmd,"I822=%fI922=%f",n1_vel,n1_vel );
		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	}
}

// void DirectDero::on_radioButton_n1_acc_default_toggled ( bool on )
// {
//
// }

// void DirectDero::SetVelcity( double vel,double cts,char  )
// {
//     		char cmd[255];
// 		QString str;
//                 n1_vel =n1_vel /( CTS_N1*1000 );
// 		sprintf ( cmd,"I822=%fI922=%f",n1_vel,n1_vel );
// 		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
// 
// }

void DirectDero::on_radioButton_n2_vel_fast_toggled ( bool on )
{       char cmd[255];
        QString str;
	if ( on )
	{
	 n2_vel = SMySetting::instance_p()->value ( "dero/n2_vel_fast" ).toDouble();
         lineEdit_n2_vel_spec->setText ( QString::number ( n2_vel) );
         n2_vel =n2_vel /( CTS_ADC*1000 );
	 sprintf ( cmd,"I622=%fI722=%f",n2_vel,n2_vel );
	 SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );	 
	}

  }

void DirectDero::on_radioButton_n2_vel_mid_toggled ( bool on )
{	char cmd[255];
	QString str;
	if ( on )
	{
	 n2_vel = SMySetting::instance_p()->value ( "dero/n2_vel_mid" ).toDouble();
         lineEdit_n2_vel_spec->setText ( QString::number ( n2_vel) );
         n2_vel =n2_vel /( CTS_ADC*1000 );
	 sprintf ( cmd,"I622=%fI722=%f",n2_vel,n2_vel );
	 SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	}
}

void DirectDero::on_radioButton_n2_vel_slow_toggled ( bool on )
{
	char cmd[255];
	QString str;
	if ( on )
	{
	n2_vel = SMySetting::instance_p()->value ( "dero/n2_vel_slow" ).toDouble();
         lineEdit_n2_vel_spec->setText ( QString::number ( n2_vel) );
         n2_vel =n2_vel /( CTS_ADC*1000 );
	 sprintf ( cmd,"I622=%fI722=%f",n2_vel,n2_vel );
	 SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );	
	}
}


void DirectDero::on_pushButton_c1_release_clicked()
{

	char cmd[255];
	QString str;

	sprintf ( cmd,"m7024=1" );
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}
void DirectDero::on_pushButton_c1_break_clicked()
{

	char cmd[255];
	QString str;

	sprintf ( cmd,"m7024=0" );
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}
void DirectDero::on_pushButton_n1_release_clicked()
{

	char cmd[255];
	QString str;

	sprintf ( cmd,"m7026=1" );
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}
void DirectDero::on_pushButton_n1_break_clicked()
{

	char cmd[255];
	QString str;

	sprintf ( cmd,"m7026=0" );
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}

void DirectDero::on_pushButton_n2_release_clicked()
{

	char cmd[255];
	QString str;

	sprintf ( cmd,"m7025=1" );
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}
void DirectDero::on_pushButton_n2_break_clicked()
{

	char cmd[255];
	QString str;

	sprintf ( cmd,"m7025=0" );
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}

void DirectDero::on_pushButton_c1_go_clicked()
{
	on_lineEdit_c1_vel_spec_returnPressed();
	char cmd[255];
	QString str;
        //bool Direction;
        setPIDparam_C1_going();
	c1_target = ( lineEdit_c1_targetD->text().toFloat() *3600+lineEdit_c1_targetM->text().toFloat() *60+lineEdit_c1_targetS->text().toFloat() ) ;

        if(flagtcs.Dero_SoftLimit_P&&(c1_target>s_Dero.PosReal_Degree*3600))
		return;	
	 
        if(flagtcs.Dero_SoftLimit_N&&(c1_target<s_Dero.PosReal_Degree*3600))
 		return;	 

        c1_target=c1_target/CTS_Dero;
        sprintf ( cmd,"2J=%f",c1_target );
 	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}

void DirectDero::on_pushButton_n1_go_clicked()
{
	on_lineEdit_n1_vel_spec_returnPressed();
	char cmd[255];
	QString str;
        setPIDparam_N1_going();
	n1_target =  lineEdit_n1_targetD->text().toFloat() *3600+lineEdit_n1_targetM->text().toFloat() *60+lineEdit_n1_targetS->text().toFloat();

        if(flagtcs.ADC1_SoftLimit_P&&(n1_target>s_ADC1.PosReal_Degree*3600))
		return;	
        if(flagtcs.ADC1_SoftLimit_N&&(n1_target<s_ADC1.PosReal_Degree*3600))
 		return;	 
        n1_target=n1_target/CTS_ADC;
        sprintf ( cmd,"#3J=%f",n1_target );
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}




void  DirectDero::setPIDparam_N1_going()
{

        QString str;
	double n1VelocityCmd=0;
	n1VelocityCmd=lineEdit_n1_vel_spec->text().toFloat();	
 	if ( fabs ( n1VelocityCmd ) >=3600 )
                {SQUMAC::instance_p()->QcommCmdPMAC (ADC1PointParam,str );SQUMAC::instance_p()->QcommCmdPMAC (ADC1PointParam,str );}
	else if (fabs ( n1VelocityCmd ) <3600 )
                {SQUMAC::instance_p()->QcommCmdPMAC (ADC1TrackParam,str );SQUMAC::instance_p()->QcommCmdPMAC (ADC1TrackParam,str );}
 
}
void  DirectDero::setPIDparam_N2_going()
{
        QString str;
	double n2VelocityCmd=0;

	n2VelocityCmd=lineEdit_n2_vel_spec->text().toFloat();	
 	if ( fabs ( n2VelocityCmd ) >=3600 )
                {SQUMAC::instance_p()->QcommCmdPMAC (ADC2PointParam,str );SQUMAC::instance_p()->QcommCmdPMAC (ADC2PointParam,str );}
	else if (fabs ( n2VelocityCmd ) <3600 )
                {SQUMAC::instance_p()->QcommCmdPMAC (ADC2TrackParam,str );SQUMAC::instance_p()->QcommCmdPMAC (ADC2TrackParam,str );}
	

}

void DirectDero::on_pushButton_c1_dirP_pressed()//positive direct
{
	QString str;
	on_lineEdit_c1_vel_spec_returnPressed();
	setPIDparam_C1_going();
        SQUMAC::instance_p()->QcommCmdPMAC ( "#2j+",str );

}
void DirectDero::on_pushButton_c1_dirP_released()
{
        on_pushButton_c1_stop_clicked();

}
void DirectDero::on_pushButton_c1_dirM_pressed()//negative direct
{
	QString str;
	on_lineEdit_c1_vel_spec_returnPressed();
	setPIDparam_C1_going();
        SQUMAC::instance_p()->QcommCmdPMAC ( "#2j-",str );
}
void DirectDero::on_pushButton_c1_dirM_released()
{
	on_pushButton_c1_stop_clicked();
}


void DirectDero::on_pushButton_n2_go_clicked()
{
	on_lineEdit_n2_vel_spec_returnPressed();
	char cmd[255];
        setPIDparam_N2_going();
	QString str;
	n2_target =  lineEdit_n2_targetD->text().toFloat() *3600+lineEdit_n2_targetM->text().toFloat() *60+lineEdit_n2_targetS->text().toFloat();

        if(flagtcs.ADC2_SoftLimit_P&&(n2_target>s_ADC2.PosReal_Degree*3600))
		return;	
        if(flagtcs.ADC2_SoftLimit_N&&(n2_target<s_ADC2.PosReal_Degree*3600))
 		return;	
 	
        n2_target=n2_target/CTS_ADC;
        sprintf ( cmd,"#4J=%f",n2_target );
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
}

void DirectDero::on_pushButton_n2_dirP_pressed()
{
	QString str;
	on_lineEdit_n2_vel_spec_returnPressed();
	setPIDparam_N2_going();
	SQUMAC::instance_p()->QcommCmdPMAC ( "#6j+#7j+",str );
}
void DirectDero::on_pushButton_n2_dirP_released()
{
 	on_pushButton_n2_stop_clicked();
}
void DirectDero::on_pushButton_n2_dirM_pressed()
{
	QString str;
	on_lineEdit_n2_vel_spec_returnPressed();
	setPIDparam_N2_going();
	SQUMAC::instance_p()->QcommCmdPMAC ( "#6j-#7j-",str );
}
void DirectDero::on_pushButton_n2_dirM_released()
{
	on_pushButton_n2_stop_clicked();
}

void DirectDero::on_pushButton_n1_dirP_pressed()
{
	QString str;
	on_lineEdit_n1_vel_spec_returnPressed();
	setPIDparam_N1_going();
	SQUMAC::instance_p()->QcommCmdPMAC ( "#8j+#9j+",str );
}
void DirectDero::on_pushButton_n1_dirP_released()
{
	 on_pushButton_n1_stop_clicked();
}
void DirectDero::on_pushButton_n1_dirM_pressed()
{
	QString str;
	on_lineEdit_n1_vel_spec_returnPressed();
	setPIDparam_N1_going();
	SQUMAC::instance_p()->QcommCmdPMAC ( "#8j-#9j-",str );
}
void DirectDero::on_pushButton_n1_dirM_released()
{
	 on_pushButton_n1_stop_clicked();
}

void DirectDero::on_pushButton_c1_stop_clicked()
{
	QString str;
	SQUMAC::instance_p()->QcommCmdPMAC ( "#4K#5K",str );
}

void DirectDero::on_pushButton_n1_stop_clicked()
{
	QString str;
	SQUMAC::instance_p()->QcommCmdPMAC ( "#8K#9K",str );
}

void DirectDero::on_pushButton_n2_stop_clicked()
{
	QString str;
	SQUMAC::instance_p()->QcommCmdPMAC ( "#6K#7K",str );

}

//c1 step ,positive
//input: c1.c1PosReal_Degree ,Global Variable
void DirectDero::on_pushButton_c1_stepP_clicked()
{
	int direction=1;
        double realpostion=s_Dero.PosReal_Degree;
 	C1step2target(direction,realpostion);
}

//input: c1.c1PosReal_Degree ,Global Variable
void DirectDero::on_pushButton_c1_stepM_clicked()//c1 step ,minus
{
 	int direction=-1;
        double realpostion=s_Dero.PosReal_Degree;
 	C1step2target(direction,realpostion);
}

void DirectDero::C1step2target(int direction,double realpos)
{
        double C1Step=0;
	double C1target;
 try
	{     
       	if(radioButton_c1_stepLong->isChecked())
		C1Step =  SMySetting::instance_p()->value ( "dero/c1_stepLong" ).toDouble();
        else if(radioButton_c1_stepMid->isChecked())
		C1Step =  SMySetting::instance_p()->value ( "dero/c1_stepMid" ).toDouble();
        else if(radioButton_c1_stepShort->isChecked())
		C1Step =  SMySetting::instance_p()->value ( "dero/c1_stepShort" ).toDouble();
	else if(radioButton_c1_step_spec->isChecked())
		C1Step =  lineEdit_c1_step_spec->text().toFloat();
        
        C1Step=C1Step*direction;
 	C1target=realpos+C1Step/3600;//  unit:degree
        ShowC1Target(C1target);//show  target
	on_pushButton_c1_go_clicked();
 
       }
 catch ( std::string Str ) {qDebug() << QString::fromStdString ( Str );}

}
void DirectDero::ShowC1Target(double C1Target)//show C1 target:degree to (degree,minute,second)
{
	int C1_d=0,C1_m=0;
	double C1_s=0;
	char s[255];
 
        D2DMS( C1Target,&C1_d,&C1_m,&C1_s );//degree to (degree,minute,second)
	sprintf ( s,"%3d",int ( C1_d ) );// target,
	lineEdit_c1_targetD->setText ( s ); //degree
	sprintf ( s,"%3d",int ( C1_m ) );
	lineEdit_c1_targetM->setText ( s );//minute
	sprintf ( s,"%6.3f",C1_s );
	lineEdit_c1_targetS->setText ( s );//second

}

//input: n2.n2PosReal_Degree ,Global Variable
void DirectDero::on_pushButton_n2_stepP_clicked()
{
	int direction=1;
        double realpostion=s_ADC2.PosReal_Degree;
 	N2step2target(direction,realpostion);
}

//input: n2.n2PosReal_Degree ,Global Variable
void DirectDero::on_pushButton_n2_stepM_clicked()
{
	int direction=-1;
        double realpostion=s_ADC2.PosReal_Degree;
 	N2step2target(direction,realpostion);
}
void DirectDero::N2step2target(int direction,double realpos)
{
        double N2Step=0;
	double N2target;
 try
	{     
       	if(radioButton_n2_stepLong->isChecked())
		N2Step =  SMySetting::instance_p()->value ( "dero/n2_stepLong" ).toDouble();
        else if(radioButton_n2_stepMid->isChecked())
		N2Step =  SMySetting::instance_p()->value ( "dero/n2_stepMid" ).toDouble();
        else if(radioButton_n2_stepShort->isChecked())
		N2Step =  SMySetting::instance_p()->value ( "dero/n2_stepShort" ).toDouble();
	else if(radioButton_n2_step_spec->isChecked())
		N2Step =  lineEdit_n2_step_spec->text().toFloat();
 
        N2Step=N2Step*direction;
 	N2target=realpos+N2Step/3600;//  unit:degree
        ShowN2Target(N2target);//show  target
	on_pushButton_n2_go_clicked();
       }
 catch ( std::string Str ) {qDebug() << QString::fromStdString ( Str );}

}

void DirectDero::ShowN2Target(double N2Target)//show N2 target:degree to (degree,minute,second)
{
	double N2_d=0,N2_m=0;
	double N2_s=0;
	char s[255];
	 
	N2_d=int ( N2Target );
	N2_m=int ( ( N2Target-N2_d ) *60 );
	N2_s= ( ( N2Target-N2_d ) *60-N2_m ) *60; 

	sprintf ( s,"%3d",int ( N2_d ) );// target,
	lineEdit_n2_targetD->setText ( s ); //degree
	sprintf ( s,"%3d",int ( N2_m ) );
	lineEdit_n2_targetM->setText ( s );//minute
	sprintf ( s,"%6.3f",N2_s );
	lineEdit_n2_targetS->setText ( s );//second
}

//input: n1.n1PosReal_Degree ,Global Variable
void DirectDero::on_pushButton_n1_stepP_clicked()
{
	int direction=1;
        double realpostion=s_ADC1.PosReal_Degree;
 	N1step2target(direction,realpostion);

}
//input: n1.c1PosReal_Degree ,Global Variable
void DirectDero::on_pushButton_n1_stepM_clicked()
{
	int direction=-1;
        double realpostion=s_ADC1.PosReal_Degree;
 	N1step2target(direction,realpostion);
}
void DirectDero::N1step2target(int direction,double realpos)
{
        double N1Step=0;
	double N1target;
 try
	{     
       	if(radioButton_n1_stepLong->isChecked())
		N1Step =  SMySetting::instance_p()->value ( "dero/n1_stepLong" ).toDouble();
        else if(radioButton_n1_stepMid->isChecked())
		N1Step =  SMySetting::instance_p()->value ( "dero/n1_stepMid" ).toDouble();
        else if(radioButton_n1_stepShort->isChecked())
		N1Step =  SMySetting::instance_p()->value ( "dero/n1_stepShort" ).toDouble();
	else if(radioButton_n1_step_spec->isChecked())
		N1Step =  lineEdit_n1_step_spec->text().toFloat();
 
        N1Step=N1Step*direction;
 	N1target=realpos+N1Step/3600;//  unit:degree
        ShowN1Target(N1target);//show  target
	on_pushButton_n1_go_clicked();
       }
 catch ( std::string Str ) {qDebug() << QString::fromStdString ( Str );}

}

void DirectDero::ShowN1Target(double N1Target)//show N1 target:degree to (degree,minute,second)
{
	double N1_d=0,N1_m=0;
	double N1_s=0;
	char s[255];
	 
	N1_d=int ( N1Target );
	N1_m=int ( ( N1Target-N1_d ) *60 );
	N1_s= ( ( N1Target-N1_d ) *60-N1_m ) *60; 

	sprintf ( s,"%3d",int ( N1_d ) );// target,
	lineEdit_n1_targetD->setText ( s ); //degree
	sprintf ( s,"%3d",int ( N1_m ) );
	lineEdit_n1_targetM->setText ( s );//minute
	sprintf ( s,"%6.3f",N1_s );
	lineEdit_n1_targetS->setText ( s );//second
}

void     DirectDero::CheckCommandFromOCS()
{
	if ( flagtcs.flag_TcsCheckDer )
	{
		TcsCheckDer();
		flagtcs.flag_TcsCheckDer=false;
	}


	if ( flagtcs.flag_PointByOCS_Dero )
		PointByOCS_Dero();

	if ( flagtcs.flag_PointStopByOCS_Dero )
		PointStopByOCS_Dero();


}


/* RelatedCmdCUID:Command from OCS/TCS,for example: 2.9.0.1.1.20061123.123456.4
   StatusCUID:return to OCS, new time
   ExeStatus:Start , Active , Done
	RV:    0:working normally;1:warning;-1:fault
       Info:   if no error:"OK"; or,imformation of error
 bExeStatus:  start working or finish working:"true";still working:false
 DataBack:	Additional data back to OCS,eg. positon

*/

void DirectDero::MakeString ( char StatusCUID[100],char ExeStatus[50],char RV[10],char Info[10],char bExeStatus[10],QString DataBack,QString* s_temp )
{
	QDateTime datetime;
	QString s_datetime;

	datetime = QDateTime::currentDateTime();
	s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );//RelatedCmdCUID

	QString str=QString ( "<RelatedCmdCUID=%1><StatusCUID=" ).arg ( StatusCUID ) +s_datetime+QString ( "><ExeStatus=%1><RV=%2><Info=%3><bExeStatus=%4>" ).arg ( ExeStatus ).arg ( RV ).arg ( Info ).arg ( bExeStatus ) +DataBack;
	( *s_temp ).append ( str );
}

void   DirectDero::SendToOCS ( QString str )
{
	QByteArray ba;
	char* comm;
	ba= str.toLatin1();
	comm = ba.data();

	CSendMessageTCS *sendMessageTCS ;
	sendMessageTCS = new CSendMessageTCS ( comm );
}


/*input:  
	OcsCom.StatusCUID_MA ,Global Variable

*/
void DirectDero::PointStopByOCS_Dero()
{
	flagtcs.flag_PointByOCS_Dero=false;
	flagtcs.flag_PointStopByOCS_Dero=false;
	flagtcs.flag_PointingByOCS_Dero=false;
	on_pushButton_c1_stop_clicked();
	
	QString s_temp;
	char ExeStatus[50]="Start";
	char RV[10]="0";
	char Info[10];
	char bExeStatus[10]="true";
	QString DataBack="";

        if ( (flagtcs.flag_DERO&&flagtcs.Dero_Amplifier)||(flagtcs.flag_ADC2&&flagtcs.ADC2_Amplifier) || (flagtcs.flag_ADC1&&flagtcs.ADC1_Amplifier) )
		sprintf ( Info,"OK" );
	else
		sprintf ( Info,"Fault" );

	MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
	SendToOCS ( s_temp );



// 	QString temp1;
// 	QDateTime datetime;
// 	QString s_datetime;
// 	QString s_temp="";
// 
// 
// 
// 
// 
// 	//START
// // 	datetime = QDateTime::currentDateTime();
// // 	s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );
// //
// // 	s_temp = "<RelatedCmdCUID=";
// //     QByteArray ba;
// // 	char* comm;
// //
// //         temp1.sprintf("%s",OcsCom.StatusCUID_MA);
// // 	 //qDebug()<<temp1;
// //         s_temp.append(temp1);
// // 	s_temp.append ( "><StatusCUID=" );
// //         s_temp.append ( s_datetime );
// // 	s_temp.append ( "><ExeStatus=START><bExeStatus=true>" );
// // m_parent->server->writeData ( s_temp );
// 
// 
// 	/* ba= s_temp.toLatin1();
// 	comm = ba.data();
// 	sendMessageTCS = new CSendMessageTCS ( comm );*/
// 	//"DONE"
// 	datetime = QDateTime::currentDateTime();
// 	s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );
// 	s_temp = "<RelatedCmdCUID=";
// 	temp1.sprintf ( "%s",OcsCom.StatusCUID_MA );
// 	//qDebug()<<temp1;
// 	s_temp.append ( temp1 );
// 	s_temp.append ( "><StatusCUID=" );
// 	s_temp.append ( s_datetime );
// 	s_temp.append ( "><ExeStatus=DONE><bExeStatus=true>" );
// //         ba= s_temp.toLatin1();
// // 	comm = ba.data();
// // 	sendMessageTCS = new CSendMessageTCS ( comm );
// 	m_parent->server->writeData ( s_temp );
}

/*input:  
	OcsCom.StatusCUID_MA ,Global Variable

*/
void     DirectDero::TcsCheckDer()
{
	QString s_temp;
	char ExeStatus[50]="Start";
	char RV[10]="0";
	char Info[10];
	char bExeStatus[10]="true";
	QString DataBack="";
        if ( (flagtcs.flag_DERO&&flagtcs.Dero_Amplifier)||(flagtcs.flag_ADC2&&flagtcs.ADC2_Amplifier) || (flagtcs.flag_ADC1&&flagtcs.ADC1_Amplifier) )
		sprintf ( Info,"OK" );
	else
		sprintf ( Info,"Fault" );

	MakeString ( OcsCom.StatusCUID_MA,ExeStatus,RV,Info,bExeStatus,DataBack,&s_temp );
	SendToOCS ( s_temp );

// 	QDateTime datetime;
// 	QString s_datetime;
// 	QString s_temp;
// 	datetime = QDateTime::currentDateTime();
// 	s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );
// 
// 	s_temp = "<RelatedCmdCUID=";
//  
// 
// 	QString temp1;
// 	temp1.sprintf ( "%s",OcsCom.StatusCUID_MA );
// 	//qDebug()<<temp1;
// 	s_temp.append ( temp1 );
// 	s_temp.append ( "><StatusCUID=" );
// 	s_temp.append ( s_datetime );
// 	s_temp.append ( "><ExeStatus=Start><RV=0><Info=" );
// 
// 	flagtcs.Der_Status=true;
// 
// 	if ( flagtcs.Der_Status )
// 		s_temp.append ( "OK>" );
// 	else
// 		s_temp.append ( "Fault>" );
// 
//  
// 
// 	m_parent->server->writeData ( s_temp );
 
}

/*input:  
	OcsCom.StatusCUID_MA ,Global Variable

*/
void DirectDero::PointByOCS_Dero()
{
// 	double tmp1;
// 	double Der_d=0,Der_m=0;
// 	double Der_s=0;
// 	char s[255];
// 	QDateTime datetime;
// 	QString s_datetime;
// 
// 	flagtcs.flag_PointByOCS_Dero=false;
// 	flagtcs.flag_PointingByOCS_Dero=true;
// 	flagtcs.flag_PointStopByOCS_Dero=false;
// 
// 	tmp1=Dero_point;//C1 target of pointing,from OCS, tcp
// 	Der_d=int ( tmp1 );
// 	Der_m=int ( ( tmp1-Der_d ) *60 );
// 	Der_s= ( ( tmp1-Der_d ) *60-Der_m ) *60;
// 
// 	sprintf ( s,"%3d",int ( Der_d ) );// target,
// 	lineEdit_c1_targetD->setText ( s ); // degree
// 	sprintf ( s,"%3d",int ( Der_m ) );
// 	lineEdit_c1_targetM->setText ( s );//minute
// 	sprintf ( s,"%6.3f",Der_s );
// 	lineEdit_c1_targetS->setText ( s );//second
// 
// 	on_pushButton_c1_go_clicked();
// 	if ( !timer_DeroreplyOCS->isActive () )
// 		timer_DeroreplyOCS->start ( DeroreplyOCSClock );//start check every 5 seconds
// 
// 	QString s_temp="start";
// 	datetime = QDateTime::currentDateTime();
// 	s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );
// 
// 	s_temp = "<RelatedCmdCUID=";
// 
// 	QString temp1;
// 	temp1.sprintf ( "%s",OcsCom.StatusCUID_MA );
// 	//qDebug()<<temp1;
// 	s_temp.append ( temp1 );
// 	s_temp.append ( "><StatusCUID=" );
// 	s_temp.append ( s_datetime );
// 	s_temp.append ( "><ExeStatus=START><bExeStatus=true>" );
// 
// 	m_parent->server->writeData ( s_temp );
}

void DirectDero::DeroreplyOCS() // replay OCS every 5 seconds,timer2_ALTreplyOCS
{
	if ( flagtcs.flag_PointingByOCS_Dero ) //ALT pointing
		PointingByOCS_Dero();
}

 
/*input:  
	OcsCom.StatusCUID_MA ,Global Variable
	c1.c1PosReal_Degree ,Global Variable
*/
void  DirectDero::PointingByOCS_Dero()
{
// 	//CSendMessageTCS *sendMessageTCS ;
// 	QString s_temp="";
// 	QString s_datetime;
// 	QDateTime datetime;
// 	QByteArray ba;
// 	QString temp1;
// 
// 	QString sALT;
// 	double c1PosReal_Degree;
// 	c1PosReal_Degree=c1.c1PosReal_Degree;   //input: c1.c1PosReal_Degree ,Global Variable
// 	sALT=QString ( "%1" ).arg (c1PosReal_Degree);
// 
// 	datetime = QDateTime::currentDateTime();
// 	s_datetime = datetime.toString ( "yyyyMMdd.hhmmss" );
// 	//char *p;
// 	//AZ reached target
// 	if ( fabs ( c1PosReal_Degree-Dero_point ) <=0.01 ) //second
// 	{
// 		flagtcs.flag_PointingByOCS_Dero=false;
// 		if ( timer_DeroreplyOCS->isActive () )
// 			timer_DeroreplyOCS->stop();
// 
// 		s_temp = "<RelatedCmdCUID=";
// 
// 		temp1.sprintf ( "%s",OcsCom.StatusCUID_MA );
// 
// 		s_temp.append ( temp1 );
// 
// 		// s_temp=s_temp+temp1;
// 		//   qDebug()<<s_temp;
// 		s_temp.append ( "><StatusCUID=" );
// 		s_temp.append ( s_datetime );
// 		s_temp.append ( "><ExeStatus=DONE><bExeStatus=true>" );
// 
// 
// 	}
// 	else
// 	{
// 		s_temp = "<RelatedCmdCUID=";
// 
// 		temp1.sprintf ( "%s",OcsCom.StatusCUID_MA );
// 		qDebug() <<temp1;
// 		s_temp.append ( temp1 );
// 		// s_temp=s_temp+temp1;
// 		//     qDebug()<<s_temp;
// 		s_temp.append ( "><StatusCUID=" );
// 		s_temp.append ( s_datetime );
// 		s_temp.append ( "><RV=0><Info=OK><Pos=" );
// 		temp1=QString::number ( c1PosReal_Degree,'f',8 );
// 		//   qDebug()<<temp1;
// 		s_temp.append ( temp1 );
// 		s_temp.append ( "><bExeStatus=false>" );
// 
// 
// 	}
// 
// 
// 	m_parent->server->writeData ( s_temp );

}

//Check amplifier status of C1 and N1 N2
void DirectDero::CheckAmplifier()
{
	QString str;
	char cmd[255];
	sprintf ( cmd,"M454M554M654M754M854M954" );
	QStringList  list;
	int n=6;
	GetStatusFromUMAC ( cmd,n, &list );

///////////amplifier 1     C1////////////////////////////
	if ( !QString::compare ( list[0], "1" ) ) //amplifier 1，=1：ok
	{
		C1_AmplifierLabel1->setStyleSheet ( "background-color:rgb(0,170, 0)" );  // green,no error
		stausDero.C1_Amplifier1=true;
	}
	else
	{
		C1_AmplifierLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
		stausDero.C1_Amplifier1=false;
	}

///////////amplifier 2     C1////////////////////////////
	if ( !QString::compare ( list[1], "1" ) ) //amplifier 1，=1：ok
	{
		C1_AmplifierLabel2->setStyleSheet ( "background-color:rgb(0,170, 0)" );  // green,no error
		stausDero.C1_Amplifier2=true;
	}
	else
	{
		C1_AmplifierLabel2->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
		stausDero.C1_Amplifier2=false;
	}

///////////amplifier 1     N2////////////////////////////
	if ( !QString::compare ( list[2], "1" ) ) //amplifier 1，=1：ok
	{
		N2_AmplifierLabel1->setStyleSheet ( "background-color:rgb(0,170, 0)" );  // green,no error
		stausDero.N2_Amplifier1=true;
	}
	else
	{
		N2_AmplifierLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
		stausDero.N2_Amplifier1=false;
	}

///////////amplifier 2     N2////////////////////////////
	if ( !QString::compare ( list[3], "1" ) ) //amplifier 1，=1：ok
	{
		N2_AmplifierLabel2->setStyleSheet ( "background-color:rgb(0,170, 0)" );  // green,no error
		stausDero.N1_Amplifier2=true;
	}
	else
	{
		N2_AmplifierLabel2->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
		stausDero.N1_Amplifier2=false;
	}

///////////amplifier 1     N1////////////////////////////
	if ( !QString::compare ( list[4], "1" ) ) //amplifier 1，=1：ok
	{
		N1_AmplifierLabel1->setStyleSheet ( "background-color:rgb(0,170, 0)" );  // green,no error
		stausDero.N1_Amplifier1=true;
	}
	else
	{
		N1_AmplifierLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
		stausDero.N1_Amplifier1=false;
	}

///////////amplifier 2     N1////////////////////////////
	if ( !QString::compare ( list[5], "1" ) ) //amplifier 1，=1：ok
	{
		N1_AmplifierLabel2->setStyleSheet ( "background-color:rgb(0,170, 0)" );  // green,no error
		stausDero.N1_Amplifier2=true;
	}
	else
	{
		N1_AmplifierLabel2->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
		stausDero.N1_Amplifier2=false;
	}

	if ( stausDero.C1_Amplifier1&stausDero.C1_Amplifier2 )
                flagtcs.Dero_Amplifier=true;


}


void DirectDero::CheckSoftLimit()	
{
///////////Soft LIM+   C1 ////////////////////////////
        if (  s_Dero.PosReal_Degree>=DeroSoftLimitP&&!flagtcs.Dero_SoftLimit_P)
	{
                flagtcs.Dero_SoftLimit_P=true;
		on_pushButton_c1_stop_clicked();
		label_c1_soft_limitP->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
		writeLog->writeerror("C1  Soft LIMIT+","	C1 Soft LIMIT+");
	}
        else if ( s_Dero.PosReal_Degree<DeroSoftLimitP)
	{
		label_c1_soft_limitP->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
                flagtcs.Dero_SoftLimit_P=false;
	}
///////////Soft LIM-   C1 ////////////////////////////
        if (  s_Dero.PosReal_Degree<=DeroSoftLimitN&&!flagtcs.Dero_SoftLimit_N)
	{
                flagtcs.Dero_SoftLimit_N=true;
		on_pushButton_c1_stop_clicked();
		label_c1_soft_limitN->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
		writeLog->writeerror("C1  Soft LIMIT-","	C1 Soft LIMIT-");
	}
        else if ( s_Dero.PosReal_Degree>DeroSoftLimitN)
	{
		label_c1_soft_limitN->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
                flagtcs.Dero_SoftLimit_N=false;
	}
 
///////////Soft LIM+   N2 ////////////////////////////
        if (  s_ADC2.PosReal_Degree>=ADC2SoftLimitP&&!flagtcs.ADC2_SoftLimit_P)
	{
                flagtcs.ADC2_SoftLimit_P=true;
		on_pushButton_n2_stop_clicked();
		label_n2_soft_limitP->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
		writeLog->writeerror("N2  Soft LIMIT+","	N2 Soft LIMIT+");
	}
        else if ( s_ADC2.PosReal_Degree<ADC2SoftLimitP)
	{
		label_n2_soft_limitP->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
                flagtcs.ADC2_SoftLimit_P=false;
	}
///////////Soft LIM-   N2 ////////////////////////////
        if (  s_ADC2.PosReal_Degree<=ADC2SoftLimitN&&!flagtcs.ADC2_SoftLimit_N)
	{
                flagtcs.ADC2_SoftLimit_N=true;
		on_pushButton_n2_stop_clicked();
		label_n2_soft_limitN->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
		writeLog->writeerror("N2  Soft LIMIT-","	N2 Soft LIMIT-");
	}
        else if ( s_ADC2.PosReal_Degree>ADC2SoftLimitN)
	{
		label_n2_soft_limitN->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
                flagtcs.ADC2_SoftLimit_N=false;
	}
 
///////////Soft LIM+   N1 ////////////////////////////
        if (  s_ADC1.PosReal_Degree>=ADC1SoftLimitP&&!flagtcs.ADC1_SoftLimit_P)
	{
                flagtcs.ADC1_SoftLimit_P=true;
		on_pushButton_n1_stop_clicked();
		label_n1_soft_limitP->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
		writeLog->writeerror("N1  Soft LIMIT+","	N1 Soft LIMIT+");
	}
        else if ( s_ADC1.PosReal_Degree<ADC1SoftLimitP)
	{
		label_n1_soft_limitP->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
                flagtcs.ADC1_SoftLimit_P=false;
	}
///////////Soft LIM-   N1 ////////////////////////////
        if (  s_ADC1.PosReal_Degree<=ADC1SoftLimitN&&!flagtcs.ADC1_SoftLimit_N)
	{
                flagtcs.ADC1_SoftLimit_N=true;
		on_pushButton_n1_stop_clicked();
		label_n1_soft_limitN->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
		writeLog->writeerror("N1  Soft LIMIT-","	N1 Soft LIMIT-");
	}
        else if ( s_ADC1.PosReal_Degree>ADC1SoftLimitN)
	{
		label_n1_soft_limitN->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
                flagtcs.ADC1_SoftLimit_N=false;
	}
	 
}



void DirectDero::CheckLimit()
{
	QString str;
	char cmd[255];
        sprintf ( cmd,"M231M232" );//error1~error3, warning1~warning3
	QStringList  list;
        int n=2;
	GetStatusFromUMAC ( cmd,n, &list );

/////////// LIM+   C1 ////////////////////////////
	if ( !QString::compare ( list[0], "1" ) )
	{
		label_c1_hard_limitP->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
		writeLog->writeerror("C1  LIMIT+","	C1 LIMIT+");
	}
	else
		label_c1_hard_limitP->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit

//////////LIM- C1  ////////////////////////////
 	if ( !QString::compare ( list[1], "1" ) )
	{
		label_c1_hard_limitM->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
		writeLog->writeerror("C1 LIMIT-","	C1  LIMIT-");
	}
	else
		label_c1_hard_limitM->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit
 /////////// LIM+  N1////////////////////////////
/*	if ( !QString::compare ( list[2], "1" ) )
	{
		label_n1_hard_limitP->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
		writeLog->writeerror("N1  LIMIT+","	N1  LIMIT+");
	}
	else
		label_n1_hard_limitP->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit

//////////LIM- N1   ////////////////////////////
 	if ( !QString::compare ( list[3], "1" ) )
	{
		label_n1_hard_limitM->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,reach limit
		writeLog->writeerror("N1  LIMIT-","	N1  LIMIT-");
	}
	else
		label_n1_hard_limitM->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no limit

*/
}

//Check ERROR information of C1 and N1 N2
void DirectDero::CheckError()
{
//"M442M542M642M742M842M942"; //error:C1_1,C1_2;N1_1,N1_2;N2_1,N2_2
//"M441M541M641M741M841M941"; //warning:C1_1,C1_2;N1_1,N1_2;N2_1,N2_2
	QString str;
	char cmd[255];
        sprintf ( cmd,"M241M242M341M342M441M442" );//error1~error3, warning1~warning3
	QStringList  list;
        int n=6;
	GetStatusFromUMAC ( cmd,n, &list );

///////////error and   warning,   Dero channel:  2#////////////////////////////
        if ( !QString::compare ( list[1], "1" ) )
		C1_ErrorLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
        else if ( !QString::compare ( list[0], "1" ) )
		C1_ErrorLabel1->setStyleSheet ( "background-color:rgb(255, 255, 0)" );// yellow color,warning
	else
		C1_ErrorLabel1->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error and no warning
///////////error and   warning,   ADC1 channel:  3# ////////////////////////////
        if ( !QString::compare ( list[3], "1" ) )
		C1_ErrorLabel2->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
        else if ( !QString::compare ( list[2], "1" ) )
		C1_ErrorLabel2->setStyleSheet ( "background-color:rgb(255, 255, 0)" );// yellow color,warning
	else
		C1_ErrorLabel2->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error and no warning

//////////error and   warning,   ADC2  channel:4#  ////////////////////////////
        if ( !QString::compare ( list[5], "1" ) )
		N2_ErrorLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
        else if ( !QString::compare ( list[4], "1" ) )
		N2_ErrorLabel1->setStyleSheet ( "background-color:rgb(255, 255, 0)" );// yellow color,warning
	else
		N2_ErrorLabel1->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error and no warning

}

/*
function :  Get Status From UMAC ,such as limit,home...
//input:
char cmd[];			//command that send out to UMAC
int n;     			//number of commands
 

output:
 
QStringList  Status		//Status of hardware
*/
void DirectDero::GetStatusFromUMAC ( char cmd[],int n,QStringList* Status)
{
	QString str;
  	int it=0;	 
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	 *Status= str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
         
	for ( QStringList::iterator point =  (*Status).begin(); point != (*Status).end();++it, ++point )
		{}
	if ( it<n )
	{
// 		UMAC_ETHERNET=false;
		return;
	};

//        UMAC_ETHERNET=true;

}


void DirectDero::CheckC1Homing()
{

	QString str;
	char  cmd[255];
	sprintf ( cmd,"P417" );
	QStringList  list;
	int n=1;

	GetStatusFromUMAC ( cmd,n, &list );
	if ( !QString::compare ( list[0], "1" ) ) //check P417 ,P417=1:finish homing,success
	{
		label_c1_homed->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,
		//sprintf ( cmd,"P801=1" );//
		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
		flag_C1Homing=FALSE;
		label_c1_home->setText ( "Homed" );
	}
	else if ( !QString::compare ( list[0], "2" ) ) //P417=2: homing
	{
		label_c1_homed->setStyleSheet ( "background-color:rgb(255, 255, 0)" );  // yellow
		label_c1_home->setText ( "Homing..." );
	}
	else                        //check P417,P417=3 or other value: no homing
	{
		label_c1_homed->setStyleSheet ( "background-color:rgb(255, 0, 0)" );  // red
		label_c1_home->setText ( "No Home" );
	}
}

void DirectDero::CheckN1Homing()
{
	QString str;
	char  cmd[255];
	sprintf ( cmd,"P817" );//
	QStringList  list;
	int n=1;

	GetStatusFromUMAC ( cmd,n, &list );

	if ( !QString::compare ( list[0], "1" ) ) //check P817 ,P817=1:finish homing,success
	{
		label_n1_homed->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,
		//sprintf ( cmd,"P801=1" );//
		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
		label_n1_home->setText ( "Homed" );
		flag_N1Homing=FALSE;
	}
	else if ( !QString::compare ( list[0], "2" ) ) //P817=2: homing
	{	
		label_n1_homed->setStyleSheet ( "background-color:rgb(255, 255, 0)" );  // yellow
		label_n1_home->setText ( "Homing" );
	}

	else                        //check P417,P817=3 or other value: no homing
	{
		label_n1_homed->setStyleSheet ( "background-color:rgb(255, 0, 0)" );  // red
		label_n1_home->setText ( "No Home" );
	}
}

void DirectDero::CheckN2Homing()
{

	QString str;
	char  cmd[255];
	sprintf ( cmd,"P617" );//
	QStringList  list;
	int n=1;

	GetStatusFromUMAC ( cmd,n, &list );

	if ( !QString::compare ( list[0], "1" ) ) //check P617 ,P617=1:finish homing,success
	{
		label_n2_homed->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,
		//sprintf ( cmd,"P801=1" );//
		SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
		label_n2_home->setText ( "Homed" );
		flag_N2Homing=FALSE;
	}
	else if ( !QString::compare ( list[0], "2" ) ) //P617=2: homing
	{
		label_n2_homed->setStyleSheet ( "background-color:rgb(255, 255, 0)" );  // yellow
		label_n2_home->setText ( "Homing" );
	}

	else                        //check P617,P617=3 or other value: no homing
	{
		label_n2_homed->setStyleSheet ( "background-color:rgb(255, 0, 0)" );  // red
		label_n2_home->setText ( "No Home" );
        }
}

void DirectDero::CheckLoopStatus()
{
	QString str;

	char  cmd[255];

	sprintf ( cmd,"M438M538M638M738M838M938" );//
	QStringList  list;
	int n=6;
	GetStatusFromUMAC ( cmd,n, &list );

///////////loop 1     C1_1 ////////////////////////////
	if ( !QString::compare ( list[0], "0" ) ) //loop 1，=0：ok
		C1_LoopLabel1->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
	else
		C1_LoopLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
///////////loop 2     C1_2 ////////////////////////////
	if ( !QString::compare ( list[1], "0" ) ) //alt loop 1，=0：ok
		C1_LoopLabel2->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
	else
		C1_LoopLabel2->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error

///////////loop1    N2_1 ////////////////////////////
	if ( !QString::compare ( list[2], "0" ) ) //alt loop2，=0：ok
		N2_LoopLabel1->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
	else
		N2_LoopLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
///////////loop2   N2_2 ////////////////////////////
	if ( !QString::compare ( list[3], "0" ) ) //alt loop2，=0：ok
		N2_LoopLabel2->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
	else
		N2_LoopLabel2->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
///////////loop1    N1_1 ////////////////////////////
	if ( !QString::compare ( list[4], "0" ) ) //alt loop2，=0：ok
		N1_LoopLabel1->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
	else
		N1_LoopLabel1->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error
///////////loop2   N1_2 ////////////////////////////
	if ( !QString::compare ( list[5], "0" ) ) //alt loop2，=0：ok
		N1_LoopLabel2->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
	else
		N1_LoopLabel2->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error


}

void DirectDero::CheckBreak()
{
	QString str;
	char  cmd[255];

	sprintf ( cmd,"M7024M7025M7026" );// C1,N2,N1  1:break open 0:close
	QStringList  list;
	int n=3;
	GetStatusFromUMAC ( cmd,n, &list );


///////////break    C1  ////////////////////////////
	if ( QString::compare ( list[0], "0" ) )
		C1_brake_label->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
	else
		C1_brake_label->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error

///////////break   N2  ////////////////////////////
	if ( QString::compare ( list[1], "0" ) )
		N2_brake_label->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
	else
		N2_brake_label->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error



///////////break    N1  ////////////////////////////
	if ( QString::compare ( list[2], "0" ) )
		N1_brake_label->setStyleSheet ( "background-color:rgb(0, 170, 0)" );  // green,no error
	else
		N1_brake_label->setStyleSheet ( "background-color:rgb(255, 0, 0)" );// red color,error


}

void DirectDero::on_radioButton_c1_stepLong_toggled ( bool on )
{
	if ( on )
	{
		QString str;
		double C1Step;
		C1Step = SMySetting::instance_p()->value ( "dero/c1_stepLong" ).toDouble();
		lineEdit_c1_step_spec->setText ( QString::number ( C1Step ) );
		
	}
}
void DirectDero::on_radioButton_c1_stepMid_toggled ( bool on )
{
	if ( on )
	{
		QString str;
		double C1Step;
		C1Step = SMySetting::instance_p()->value ( "dero/c1_stepMid" ).toDouble();
		lineEdit_c1_step_spec->setText ( QString::number ( C1Step ) );
		
	}
}
void DirectDero::on_radioButton_c1_stepShort_toggled ( bool on )
{
	if ( on )
	{
		QString str;
		double C1Step;
		C1Step = SMySetting::instance_p()->value ( "dero/c1_stepShort" ).toDouble();
		lineEdit_c1_step_spec->setText ( QString::number ( C1Step ) );
		
	}
}
void DirectDero::on_radioButton_n2_stepLong_toggled ( bool on )
{
	if ( on )
	{
		QString str;
		double N2Step;
		N2Step = SMySetting::instance_p()->value ( "dero/n2_stepLong" ).toDouble();
		lineEdit_n2_step_spec->setText ( QString::number ( N2Step ) );
		
	}

}
void DirectDero::on_radioButton_n2_stepMid_toggled ( bool on )
{
	if ( on )
	{
		QString str;
		double N2Step;
		N2Step = SMySetting::instance_p()->value ( "dero/n2_stepMid" ).toDouble();
		lineEdit_n2_step_spec->setText ( QString::number ( N2Step ) );
		
	}
}
void DirectDero::on_radioButton_n2_stepShort_toggled ( bool on )
{
	if ( on )
	{
		QString str;
		double N2Step;
		N2Step = SMySetting::instance_p()->value ( "dero/n2_stepShort" ).toDouble();
		lineEdit_n2_step_spec->setText ( QString::number ( N2Step ) );
		
	}
}
void DirectDero::on_radioButton_n1_stepLong_toggled ( bool on )
{
	if ( on )
	{
		QString str;
		double N1Step;
		N1Step = SMySetting::instance_p()->value ( "dero/n1_stepLong" ).toDouble();
		lineEdit_n1_step_spec->setText ( QString::number ( N1Step ) );
		
	}
}
void DirectDero::on_radioButton_n1_stepMid_toggled ( bool on )
{
	if ( on )
	{
		QString str;
		double N1Step;
		N1Step = SMySetting::instance_p()->value ( "dero/n1_stepMid" ).toDouble();
		lineEdit_n1_step_spec->setText ( QString::number ( N1Step ) );
		
	}
}
void DirectDero::on_radioButton_n1_stepShort_toggled ( bool on )
{
	if ( on )
	{
		QString str;
		double N1Step;
		N1Step = SMySetting::instance_p()->value ( "dero/n1_stepShort" ).toDouble();
		lineEdit_n1_step_spec->setText ( QString::number ( N1Step ) );
		
	}
}
void DirectDero::on_pushButton_c1_park_clicked()
{
	double c1_ParkTarget;
	QString str;
	
	c1_ParkTarget= SMySetting::instance_p()->value ( "dero/c1_park" ).toDouble();
	on_lineEdit_c1_vel_spec_returnPressed();
	setPIDparam_C1_going();
        ShowC1Target(c1_ParkTarget);//show park target
	on_pushButton_c1_go_clicked();
}

void DirectDero::on_pushButton_n1_park_clicked()
{
	double n1_ParkTarget;
	QString str;
	
	n1_ParkTarget= SMySetting::instance_p()->value ( "dero/n1_park" ).toDouble();
	on_lineEdit_n1_vel_spec_returnPressed();
	setPIDparam_N1_going();
        ShowN1Target(n1_ParkTarget);//show park target
	on_pushButton_n1_go_clicked();
}

void DirectDero::on_pushButton_n2_park_clicked()
{
	double n2_ParkTarget;
	QString str;
	
	n2_ParkTarget= SMySetting::instance_p()->value ( "dero/n2_park" ).toDouble();
	on_lineEdit_n2_vel_spec_returnPressed();
	setPIDparam_N2_going();
        ShowN2Target(n2_ParkTarget);//show park target
	on_pushButton_n2_go_clicked();
}

