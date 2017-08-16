#include "setup.h"
#include "main_window.h"
#include "signs.h"

#define AZ_FAST_BOTTOM      0
#define AZ_FAST_TOP       36000

#define AZ_MID_BOTTOM         0
#define AZ_MID_TOP          3600

#define AZ_SLOW_BOTTOM        0
#define AZ_SLOW_TOP         100

#define AZ_PARK_BOTTOM      0
#define AZ_PARK_TOP         360
#define AZ_PARK_decimals    10

#define AZ_STEP_LONG_BOTTOM      0
#define AZ_STEP_LONG_TOP         36000  
#define AZ_STEP_LONG_decimals    3

#define AZ_STEP_MID_BOTTOM      0
#define AZ_STEP_MID_TOP         3600
#define AZ_STEP_MID_decimals    3

#define AZ_STEP_SHORT_BOTTOM      0
#define AZ_STEP_SHORT_TOP         60
#define AZ_STEP_SHORT_decimals    3

#define ALT_STEP_LONG_BOTTOM      0
#define ALT_STEP_LONG_TOP         36000  
#define ALT_STEP_LONG_decimals    3

#define ALT_STEP_MID_BOTTOM      0
#define ALT_STEP_MID_TOP         3600
#define ALT_STEP_MID_decimals    3

#define ALT_STEP_SHORT_BOTTOM      0
#define ALT_STEP_SHORT_TOP         60
#define ALT_STEP_SHORT_decimals    3

#define ALT_FAST_BOTTOM        0
#define ALT_FAST_TOP       36000

#define ALT_MID_BOTTOM         0
#define ALT_MID_TOP          3600

#define ALT_SLOW_BOTTOM        0
#define ALT_SLOW_TOP         100

#define ALT_PARK_BOTTOM      0
#define ALT_PARK_TOP         360
#define ALT_PARK_decimals    10

#define C1_FAST_BOTTOM       0
#define C1_FAST_TOP       7200

#define C1_MID_BOTTOM        0
#define C1_MID_TOP         600

#define C1_SLOW_BOTTOM       0
#define C1_SLOW_TOP         60

#define N1_FAST_BOTTOM       0
#define N1_FAST_TOP       7200

#define N1_MID_BOTTOM        0
#define N1_MID_TOP         600

#define N1_SLOW_BOTTOM       0
#define N1_SLOW_TOP         60

#define N2_FAST_BOTTOM       0
#define N2_FAST_TOP       7200

#define N2_MID_BOTTOM        0
#define N2_MID_TOP         600

#define N2_SLOW_BOTTOM       0
#define N2_SLOW_TOP         60


#define C1_STEP_LONG_BOTTOM      0
#define C1_STEP_LONG_TOP         36000  
#define C1_STEP_LONG_decimals    3

#define C1_STEP_MID_BOTTOM      0
#define C1_STEP_MID_TOP         3600
#define C1_STEP_MID_decimals    3

#define C1_STEP_SHORT_BOTTOM      0
#define C1_STEP_SHORT_TOP         60
#define C1_STEP_SHORT_decimals    3

#define N2_STEP_LONG_BOTTOM      0
#define N2_STEP_LONG_TOP         36000  
#define N2_STEP_LONG_decimals    3

#define N2_STEP_MID_BOTTOM      0
#define N2_STEP_MID_TOP         3600
#define N2_STEP_MID_decimals    3

#define N2_STEP_SHORT_BOTTOM      0
#define N2_STEP_SHORT_TOP         60
#define N2_STEP_SHORT_decimals    3

#define N1_STEP_LONG_BOTTOM      0
#define N1_STEP_LONG_TOP         36000  
#define N1_STEP_LONG_decimals    3

#define N1_STEP_MID_BOTTOM      0
#define N1_STEP_MID_TOP         3600
#define N1_STEP_MID_decimals    3

#define N1_STEP_SHORT_BOTTOM      0
#define N1_STEP_SHORT_TOP         60
#define N1_STEP_SHORT_decimals    3

#define  C1_PARK_BOTTOM		-180
#define  C1_PARK_TOP		180
#define  C1_PARK_decimals	3

#define  N2_PARK_BOTTOM		-180
#define  N2_PARK_TOP		180
#define  N2_PARK_decimals	3

#define  N1_PARK_BOTTOM		-180
#define  N1_PARK_TOP		180
#define  N1_PARK_decimals	3

//Misc
#define LONG_D_BOTTOM     -180.0
#define LONG_D_TOP         180.0
#define LongD_decimals       12       //int decimals - the validator's maximum number of digits after the decimal point

#define LONG_M_BOTTOM        0
#define LONG_M_TOP          59


#define LONG_S_BOTTOM        0
#define LONG_S_TOP          59
#define LongS_decimals      10    //int decimals - the validator's maximum number of digits after the decimal point

#define LATI_D_BOTTOM       -90
#define LATI_D_TOP          90
#define LatiD_decimals      12   //int decimals - the validator's maximum number of digits after the decimal point

#define LATI_M_BOTTOM       	 		0
#define LATI_M_TOP          			59

#define LATI_S_BOTTOM        			0
#define LATI_S_TOP          			59
#define LatiS_decimals      			10    //int decimals - the validator's maximum number of digits after the decimal point

#define OT_H_BOTTOM          			0
#define OT_H_TOP        			5	//observation time

#define OT_M_BOTTOM          			0
#define OT_M_TOP            			60

#define OT_S_BOTTOM          			0
#define OT_S_TOP            			60

#define	OcsPort_BOTTOM				0
#define	OcsPort_TOP				65535

#define	LocalhostPort_BOTTOM			0
#define	LocalhostPort_TOP			65535



#define SampleTime_BOTTOM			2
#define SampleTime_TOP                     	30

#define OpticAxisCCD_BOTTOM        600
#define OpticAxisCCD_TOP           1400



#include "QFileDialog"

//Azalt default
#define AZ_FAST_DEFAULT     			10800
#define AZ_MID_DEFAULT      			3600
#define AZ_SLOW_DEFAULT     			45

#define ALT_FAST_DEFAULT     			10800
#define ALT_MID_DEFAULT      			3600
#define ALT_SLOW_DEFAULT     			45

#define AZ_STEP_LONG_DEFAULT     		3600
#define AZ_STEP_MID_DEFAULT      		60
#define AZ_STEP_SLOW_DEFAULT     		1

#define ALT_STEP_LONG_DEFAULT     		3600
#define ALT_STEP_MID_DEFAULT      		60
#define ALT_STEP_SLOW_DEFAULT     		1

#define AZ_PARK_DEFAULT		 		180
#define ALT_PARK_DEFAULT	 		80

//dero default
#define C1_FAST_DEFAULT     			3600
#define C1_MID_DEFAULT      			600
#define C1_SLOW_DEFAULT     			15

#define N2_FAST_DEFAULT     			3600
#define N2_MID_DEFAULT      			600
#define N2_SLOW_DEFAULT     			15

#define N1_FAST_DEFAULT     			3600
#define N1_MID_DEFAULT      			600
#define N1_SLOW_DEFAULT     			15

#define C1_STEP_LONG_DEFAULT     		3600
#define C1_STEP_MID_DEFAULT      		60
#define C1_STEP_SLOW_DEFAULT     		1

#define N2_STEP_LONG_DEFAULT     		3600
#define N2_STEP_MID_DEFAULT      		60
#define N2_STEP_SLOW_DEFAULT     		1

#define N1_STEP_LONG_DEFAULT     		3600
#define N1_STEP_MID_DEFAULT      		60
#define N1_STEP_SLOW_DEFAULT     		1

#define C1_PARK_DEFAULT				45
#define N2_PARK_DEFAULT				45
#define N1_PARK_DEFAULT				45

//Misc default
#define ObservationTimeH_DEFAULT		5
#define ObservationTimeM_DEFAULT		0
#define ObservationTimeS_DEFAULT		0

#define LongitudeD_DEFAULT			118
#define LongitudeM_DEFAULT			49	
#define LongitudeS_DEFAULT			8.888	
#define LatitudeD_DEFAULT			32	
#define LatitudeM_DEFAULT			4
#define LatitudeS_DEFAULT			42.51

#define OcsIP_DEFAULT     			"192.6.94.20"
#define OcsPort_DEFAULT     	 		5003
#define LocalhostIP_DEFAULT     		"192.6.94.15"
#define LocalhostPort_DEFAULT  	 		5001

#define DataFileDirectory_DEFAULT 	 	"/home/book/telescope20141227/data"//"/home/scope/program/telescope/data"
#define StarTableFile_DEFAULT  	 		"/home/book/telescope20141227/StarTable.txt"//"/home/scope/program/telescope/StarTable.txt"
#define LogFileDirectory_DEFAULT  	 	"/home/book/telescope20141227/log"//"/home/scope/program/telescope/log"
#define ModelFile_DEFAULT  	 		"/home/book/telescope20141227/model/DataModel.dat"//"/home/scope/program/telescope/model/DataModel.dat"

#define SAMPLETIME_DEFAULT                      10
#define OpticAxisCCDX_DEFAULT        256
#define OpticAxisCCDY_DEFAULT        256
#define Pixel_Second_DEFAULT         0.08973

extern int LocalhostPort;						//Port of Mount(Az/Alt/Dero)
extern QString LocalhostIP; 						//IP of Mount(Az/Alt/Dero)
extern int OcsPort;							//Port of OCS (TCP/IP),simulation
extern QString OcsIP; 							//IP of OCS(TCP/IP),simulation
extern FlagTCS flagtcs;
extern int TableNum;
extern int SampleTime;
extern int OpticAxis_X;
extern int OpticAxis_Y;
extern double Pixel_Second;

Setup::Setup(MainWindow *parent)
{
    m_parent = parent;
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    initUi();
    setValidator();
    flagtcs.IsOpened_setup=true;
}

Setup::~Setup()
{
    deleteLater();
    m_parent->setup=NULL;
    qDebug("Close setup");
    flagtcs.IsOpened_setup=false;
}

void Setup::initUi()
{
    lineEdit_az_fast->setText(QString::number(SMySetting::instance_p()->value ( "az_alt1/az_vel_fast" ).toInt()));
    lineEdit_az_mid->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/az_vel_mid" ).toInt()));
    lineEdit_az_slow->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/az_vel_slow" ).toInt()));
    lineEdit_az_Park->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/az_park" ).toDouble()));

    lineEdit_azStep_long->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/az_stepLong" ).toDouble()));
    lineEdit_azStep_mid->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/az_stepMid" ).toDouble()));
    lineEdit_azStep_short->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/az_stepShort" ).toDouble()));

    lineEdit_alt_fast->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/alt_vel_fast" ).toInt()));
    lineEdit_alt_mid->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/alt_vel_mid" ).toInt()));
    lineEdit_alt_slow->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/alt_vel_slow" ).toInt()));
    lineEdit_alt_Park->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/alt_park" ).toDouble()));

    lineEdit_azStep_long->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/alt_stepLong" ).toDouble()));
    lineEdit_azStep_mid->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/alt_stepMid" ).toDouble()));
    lineEdit_azStep_short->setText(QString::number(SMySetting::instance_p()->value ( "az_alt/alt_stepShort" ).toDouble()));

    lineEdit_c1_fast->setText(QString::number(SMySetting::instance_p()->value ( "dero/c1_vel_fast" ).toInt()));
    lineEdit_c1_mid->setText(QString::number(SMySetting::instance_p()->value ( "dero/c1_vel_mid" ).toInt()));
    lineEdit_c1_slow->setText(QString::number(SMySetting::instance_p()->value ( "dero/c1_vel_slow" ).toInt()));

    lineEdit_n1_fast->setText(QString::number(SMySetting::instance_p()->value ( "dero/n1_vel_fast" ).toInt()));
    lineEdit_n1_mid->setText(QString::number(SMySetting::instance_p()->value ( "dero/n1_vel_mid" ).toInt()));
    lineEdit_n1_slow->setText(QString::number(SMySetting::instance_p()->value ( "dero/n1_vel_slow" ).toInt()));

    lineEdit_n2_fast->setText(QString::number(SMySetting::instance_p()->value ( "dero/n2_vel_fast" ).toInt()));
    lineEdit_n2_mid->setText(QString::number(SMySetting::instance_p()->value ( "dero/n2_vel_mid" ).toInt()));
    lineEdit_n2_slow->setText(QString::number(SMySetting::instance_p()->value ( "dero/n2_vel_slow" ).toInt()));

    lineEdit_c1Step_long->setText(QString::number(SMySetting::instance_p()->value ( "dero/c1_stepLong" ).toDouble()));
    lineEdit_c1Step_mid->setText(QString::number(SMySetting::instance_p()->value ( "dero/c1_stepMid" ).toDouble()));
    lineEdit_c1Step_short->setText(QString::number(SMySetting::instance_p()->value ( "dero/c1_stepShort" ).toDouble()));
    lineEdit_c1_Park->setText(QString::number(SMySetting::instance_p()->value ( "dero/c1_park" ).toDouble()));

    lineEdit_n2Step_long->setText(QString::number(SMySetting::instance_p()->value ( "dero/n2_stepLong" ).toDouble()));
    lineEdit_n2Step_mid->setText(QString::number(SMySetting::instance_p()->value ( "dero/n2_stepMid" ).toDouble()));
    lineEdit_n2Step_short->setText(QString::number(SMySetting::instance_p()->value ( "dero/n2_stepShort" ).toDouble()));
    lineEdit_n2_Park->setText(QString::number(SMySetting::instance_p()->value ( "dero/n2_park" ).toDouble()));

    lineEdit_n1Step_long->setText(QString::number(SMySetting::instance_p()->value ( "dero/n1_stepLong" ).toDouble()));
    lineEdit_n1Step_mid->setText(QString::number(SMySetting::instance_p()->value ( "dero/n1_stepMid" ).toDouble()));
    lineEdit_n1Step_short->setText(QString::number(SMySetting::instance_p()->value ( "dero/n1_stepShort" ).toDouble()));
    lineEdit_n1_Park->setText(QString::number(SMySetting::instance_p()->value ( "dero/n1_park" ).toDouble()));

    lineEdit_otH->setText(QString::number(SMySetting::instance_p()->value ( "misc/star_len_h" ).toInt()));
    lineEdit_otM->setText(QString::number(SMySetting::instance_p()->value ( "misc/star_len_m" ).toInt()));
    lineEdit_otS->setText(QString::number(SMySetting::instance_p()->value ( "misc/star_len_s" ).toInt()));

    lineEdit_longD->setText(QString::number(SMySetting::instance_p()->value ( "misc/longitudeD" ).toDouble()));
    lineEdit_longM->setText(QString::number(SMySetting::instance_p()->value ( "misc/longitudeM" ).toInt()));
    lineEdit_longS->setText(QString::number(SMySetting::instance_p()->value ( "misc/longitudeS" ).toDouble()));

    lineEdit_latiD->setText(QString::number(SMySetting::instance_p()->value ( "misc/latitudeD" ).toDouble()));
    lineEdit_latiM->setText(QString::number(SMySetting::instance_p()->value ( "misc/latitudeM" ).toInt()));
    lineEdit_latiS->setText(QString::number(SMySetting::instance_p()->value ( "misc/latitudeS" ).toDouble()));

    lineEdit_StarTableFile->setText(SMySetting::instance_p()->value ( "misc/StarTable" ).toString());// "misc/StarTable" route or else?
    lineEdit_ModelFile->setText(SMySetting::instance_p()->value ( "misc/ModelFile" ).toString());
    lineEdit_DataFileDirectory->setText(SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString());
    lineEdit_LogFileDirectory->setText(SMySetting::instance_p()->value ( "misc/LogDirectory" ).toString());

    lineEdit_OcsIP->setText(SMySetting::instance_p()->value ( "misc/OcsIP" ).toString());
    lineEdit_OcsPort->setText(QString::number(SMySetting::instance_p()->value ( "misc/OcsPort" ).toInt()));
    lineEdit_LocalhostIP->setText(SMySetting::instance_p()->value ( "misc/LocalhostIP" ).toString());
    lineEdit_LocalhostPort->setText(QString::number(SMySetting::instance_p()->value ( "misc/LocalhostPort" ).toInt()));

    lineEdit_otH->setText(QString::number(SMySetting::instance_p()->value ( "misc/star_len_h" ).toInt()));
    lineEdit_otM->setText(QString::number(SMySetting::instance_p()->value ( "misc/star_len_m" ).toInt()));
    lineEdit_otS->setText(QString::number(SMySetting::instance_p()->value ( "misc/star_len_s" ).toInt()));
    lineEdit_SampleTime->setText(QString::number(SMySetting::instance_p()->value ( "misc/SampleTime" ).toInt()));
    lineEdit_CCDX->setText(QString::number(SMySetting::instance_p()->value ( "misc/Optic_Axis_CCD_X" ).toInt()));
    lineEdit_CCDY->setText(QString::number(SMySetting::instance_p()->value ( "misc/Optic_Axis_CCD_Y" ).toInt()));
    lineEdit_Pixel2Second->setText(QString::number(SMySetting::instance_p()->value ( "misc/CCD_Pixel_Second" ).toDouble()));
}

void Setup::setValidator()
{
    lineEdit_az_fast->setValidator(new QIntValidator(AZ_FAST_BOTTOM,AZ_FAST_TOP,this));
    lineEdit_az_mid->setValidator(new QIntValidator(AZ_MID_BOTTOM,AZ_MID_TOP,this));
    lineEdit_az_slow->setValidator(new QIntValidator(AZ_SLOW_BOTTOM,AZ_SLOW_TOP,this));
    lineEdit_az_Park->setValidator(new MyDoubleValidator(AZ_PARK_BOTTOM,AZ_PARK_TOP,AZ_PARK_decimals,this));

    lineEdit_alt_fast->setValidator(new QIntValidator(ALT_FAST_BOTTOM,ALT_FAST_TOP,this));
    lineEdit_alt_mid->setValidator(new QIntValidator(ALT_MID_BOTTOM,ALT_MID_TOP,this));
    lineEdit_alt_slow->setValidator(new QIntValidator(ALT_SLOW_BOTTOM,ALT_SLOW_TOP,this));

    lineEdit_c1_fast->setValidator(new QIntValidator(C1_FAST_BOTTOM,C1_FAST_TOP,this));
    lineEdit_c1_mid->setValidator(new QIntValidator(C1_MID_BOTTOM,C1_MID_TOP,this));
    lineEdit_c1_slow->setValidator(new QIntValidator(C1_SLOW_BOTTOM,C1_SLOW_TOP,this));

    lineEdit_n1_fast->setValidator(new QIntValidator(N1_FAST_BOTTOM,N1_FAST_TOP,this));
    lineEdit_n1_mid->setValidator(new QIntValidator(N1_MID_BOTTOM,N1_MID_TOP,this));
    lineEdit_n1_slow->setValidator(new QIntValidator(N1_SLOW_BOTTOM,N1_SLOW_TOP,this));

    lineEdit_n2_fast->setValidator(new QIntValidator(N2_FAST_BOTTOM,N2_FAST_TOP,this));
    lineEdit_n2_mid->setValidator(new QIntValidator(N2_MID_BOTTOM,N2_MID_TOP,this));
    lineEdit_n2_slow->setValidator(new QIntValidator(N2_SLOW_BOTTOM,N2_SLOW_TOP,this));

    //lineEdit_longD->setValidator(new QIntValidator(LONG_D_BOTTOM,LONG_D_TOP,this));

    lineEdit_longD->setValidator(new MyDoubleValidator(LONG_D_BOTTOM,LONG_D_TOP,LongD_decimals,this));
    lineEdit_longM->setValidator(new QIntValidator(LONG_M_BOTTOM,LONG_M_TOP,this));
    lineEdit_longS->setValidator(new MyDoubleValidator(LONG_S_BOTTOM,LONG_S_TOP,LongS_decimals,this));

    lineEdit_latiD->setValidator(new MyDoubleValidator(LATI_D_BOTTOM,LATI_D_TOP,LatiD_decimals,this));
    lineEdit_latiM->setValidator(new QIntValidator(LATI_M_BOTTOM,LATI_M_TOP,this));
    lineEdit_latiS->setValidator(new MyDoubleValidator(LATI_S_BOTTOM,LATI_S_TOP,LatiS_decimals,this));

    lineEdit_azStep_long->setValidator(new MyDoubleValidator(AZ_STEP_LONG_BOTTOM,AZ_STEP_LONG_TOP,AZ_STEP_LONG_decimals,this));
    lineEdit_azStep_mid->setValidator(new MyDoubleValidator(AZ_STEP_MID_BOTTOM,AZ_STEP_MID_TOP,AZ_STEP_MID_decimals,this));
    lineEdit_azStep_short->setValidator(new MyDoubleValidator(AZ_STEP_SHORT_BOTTOM,AZ_STEP_SHORT_TOP,AZ_STEP_SHORT_decimals,this));

    lineEdit_altStep_long->setValidator(new MyDoubleValidator(ALT_STEP_LONG_BOTTOM,ALT_STEP_LONG_TOP,ALT_STEP_LONG_decimals,this));
    lineEdit_altStep_mid->setValidator(new MyDoubleValidator(ALT_STEP_MID_BOTTOM,ALT_STEP_MID_TOP,ALT_STEP_MID_decimals,this));
    lineEdit_altStep_short->setValidator(new MyDoubleValidator(ALT_STEP_SHORT_BOTTOM,ALT_STEP_SHORT_TOP,ALT_STEP_SHORT_decimals,this));

    lineEdit_c1Step_long->setValidator(new MyDoubleValidator(C1_STEP_LONG_BOTTOM,C1_STEP_LONG_TOP,C1_STEP_LONG_decimals,this));
    lineEdit_c1Step_mid->setValidator(new MyDoubleValidator(C1_STEP_MID_BOTTOM,C1_STEP_MID_TOP,C1_STEP_MID_decimals,this));
    lineEdit_c1Step_short->setValidator(new MyDoubleValidator(C1_STEP_SHORT_BOTTOM,C1_STEP_SHORT_TOP,C1_STEP_SHORT_decimals,this));

    lineEdit_n2Step_long->setValidator(new MyDoubleValidator(N2_STEP_LONG_BOTTOM,N2_STEP_LONG_TOP,N2_STEP_LONG_decimals,this));
    lineEdit_n2Step_mid->setValidator(new MyDoubleValidator(N2_STEP_MID_BOTTOM,N2_STEP_MID_TOP,N2_STEP_MID_decimals,this));
    lineEdit_n2Step_short->setValidator(new MyDoubleValidator(N2_STEP_SHORT_BOTTOM,N2_STEP_SHORT_TOP,N2_STEP_SHORT_decimals,this));

    lineEdit_n1Step_long->setValidator(new MyDoubleValidator(N1_STEP_LONG_BOTTOM,N1_STEP_LONG_TOP,N1_STEP_LONG_decimals,this));
    lineEdit_n1Step_mid->setValidator(new MyDoubleValidator(N1_STEP_MID_BOTTOM,N1_STEP_MID_TOP,N1_STEP_MID_decimals,this));
    lineEdit_n1Step_short->setValidator(new MyDoubleValidator(N1_STEP_SHORT_BOTTOM,N1_STEP_SHORT_TOP,N1_STEP_SHORT_decimals,this));

    lineEdit_c1_Park->setValidator(new MyDoubleValidator(C1_PARK_BOTTOM,C1_PARK_TOP,C1_PARK_decimals,this));
    lineEdit_n2_Park->setValidator(new MyDoubleValidator(N2_PARK_BOTTOM,N2_PARK_TOP,N2_PARK_decimals,this));
    lineEdit_n1_Park->setValidator(new MyDoubleValidator(N1_PARK_BOTTOM,N1_PARK_TOP,N1_PARK_decimals,this));
    //Misc
    lineEdit_OcsPort->setValidator(new QIntValidator(OcsPort_BOTTOM,OcsPort_TOP,this));
    lineEdit_LocalhostPort->setValidator(new QIntValidator(LocalhostPort_BOTTOM,LocalhostPort_TOP,this));

    lineEdit_otH->setValidator(new QIntValidator(OT_H_BOTTOM,OT_H_TOP,this));
    lineEdit_otM->setValidator(new QIntValidator(OT_M_BOTTOM,OT_M_TOP,this));
    lineEdit_otS->setValidator(new QIntValidator(OT_S_BOTTOM,OT_S_TOP,this));
    lineEdit_SampleTime->setValidator(new QIntValidator(SampleTime_BOTTOM,SampleTime_TOP,this));

    lineEdit_CCDX->setValidator(new QIntValidator(OpticAxisCCD_BOTTOM,OpticAxisCCD_TOP,this));
    lineEdit_CCDY->setValidator(new QIntValidator(OpticAxisCCD_BOTTOM,OpticAxisCCD_TOP,this));
}
/*
 lineEdit_otH->setText(QString::number(SMySetting::instance_p()->value ( "misc/star_len_h" ).toInt()));
 lineEdit_otM->setText(QString::number(SMySetting::instance_p()->value ( "misc/star_len_m" ).toInt()));
 lineEdit_otS->setText(QString::number(SMySetting::instance_p()->value ( "misc/star_len_s" ).toInt()));*/
void Setup::on_pushButton_ok_clicked()
{

    SMySetting::instance_p()->setValue ( "az_alt/az_vel_fast",lineEdit_az_fast->text().toInt());
    SMySetting::instance_p()->setValue ( "az_alt/az_vel_mid",lineEdit_az_mid->text().toInt());
    SMySetting::instance_p()->setValue ( "az_alt/az_vel_slow",lineEdit_az_slow->text().toInt());
    SMySetting::instance_p()->setValue ( "az_alt/az_park",lineEdit_az_Park->text().toDouble());


    SMySetting::instance_p()->setValue ( "az_alt/alt_vel_fast",lineEdit_alt_fast->text().toInt());
    SMySetting::instance_p()->setValue ( "az_alt/alt_vel_mid",lineEdit_alt_mid->text().toInt());
    SMySetting::instance_p()->setValue ( "az_alt/alt_vel_slow",lineEdit_alt_slow->text().toInt());
    SMySetting::instance_p()->setValue ( "az_alt/alt_park",lineEdit_alt_Park->text().toDouble());

    SMySetting::instance_p()->setValue ( "az_alt/az_stepLong",lineEdit_azStep_long->text().toDouble());
    SMySetting::instance_p()->setValue ( "az_alt/az_stepMid",lineEdit_azStep_mid->text().toDouble());
    SMySetting::instance_p()->setValue ( "az_alt/az_stepShort",lineEdit_azStep_short->text().toDouble());

    SMySetting::instance_p()->setValue ( "az_alt/alt_stepLong",lineEdit_azStep_long->text().toDouble());
    SMySetting::instance_p()->setValue ( "az_alt/alt_stepMid",lineEdit_azStep_mid->text().toDouble());
    SMySetting::instance_p()->setValue ( "az_alt/alt_stepShort",lineEdit_azStep_short->text().toDouble());



    SMySetting::instance_p()->sync();
}

void Setup::on_pushButton_cancel_clicked( )
{     

}
void Setup::closeEvent ( QCloseEvent *event )
{

}
void Setup::on_pushButton_default_clicked()
{
    lineEdit_az_fast->setText(QString::number(AZ_FAST_DEFAULT));
    lineEdit_az_mid->setText(QString::number(AZ_MID_DEFAULT));
    lineEdit_az_slow->setText(QString::number(AZ_SLOW_DEFAULT));

    lineEdit_alt_fast->setText(QString::number(ALT_FAST_DEFAULT));
    lineEdit_alt_mid->setText(QString::number(ALT_MID_DEFAULT));
    lineEdit_alt_slow->setText(QString::number(ALT_SLOW_DEFAULT));

    lineEdit_azStep_long->setText(QString::number(AZ_STEP_LONG_DEFAULT));
    lineEdit_azStep_mid->setText(QString::number(AZ_STEP_MID_DEFAULT));
    lineEdit_azStep_short->setText(QString::number(AZ_STEP_SLOW_DEFAULT));

    lineEdit_altStep_long->setText(QString::number(ALT_STEP_LONG_DEFAULT));
    lineEdit_altStep_mid->setText(QString::number(ALT_STEP_MID_DEFAULT));
    lineEdit_altStep_short->setText(QString::number(ALT_STEP_SLOW_DEFAULT));

    lineEdit_az_Park->setText(QString::number(AZ_PARK_DEFAULT));
    lineEdit_alt_Park->setText(QString::number(ALT_PARK_DEFAULT));

    //on_pushButton_ok_clicked();
}

void Setup::on_pushButton_SelectStarTableFile_clicked() 
{
    //


    // QString fileName = QFileDialog::getOpenFileName ( this );
    // 	QFile file ( fileName );
    // 	if ( !file.open ( QFile::WriteOnly|QFile::Truncate ) )
    // 	{
    // 		return  ;
    // 	}
    // 	QTextStream out ( &file );
    // file.close();
    // qDebug()<<fileName;



    QString path = QFileDialog::getOpenFileName(this, tr("Select Star Table to Access"), ".", tr("txt Files(*.txt)"));
    if(path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
        path=StarTableFile_DEFAULT;
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    }

    lineEdit_StarTableFile->setText(path);

}


void Setup::on_pushButton_SelectModelFile_clicked() 
{

    QString path = QFileDialog::getOpenFileName(this, tr("Select Star Table to Access"), ".", tr("All Files(*.*)\0*.*\0"));
    if(path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
        path=ModelFile_DEFAULT;
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    }

    lineEdit_ModelFile->setText(path);

}

////////////////////derotator/////////////////////////////////////
void Setup::on_pushButton_Dero_cancel_clicked()
{

}
void Setup::on_pushButton_Dero_ok_clicked()
{
    //VELOCITY
    SMySetting::instance_p()->setValue ( "dero/c1_vel_fast",lineEdit_c1_fast->text().toInt());
    SMySetting::instance_p()->setValue ( "dero/c1_vel_mid",lineEdit_c1_mid->text().toInt());
    SMySetting::instance_p()->setValue ( "dero/c1_vel_slow",lineEdit_c1_slow->text().toInt());

    SMySetting::instance_p()->setValue ( "dero/n1_vel_fast",lineEdit_n1_fast->text().toInt());
    SMySetting::instance_p()->setValue ( "dero/n1_vel_mid",lineEdit_n1_mid->text().toInt());
    SMySetting::instance_p()->setValue ( "dero/n1_vel_slow",lineEdit_n1_slow->text().toInt());

    SMySetting::instance_p()->setValue ( "dero/n2_vel_fast",lineEdit_n2_fast->text().toInt());
    SMySetting::instance_p()->setValue ( "dero/n2_vel_mid",lineEdit_n2_mid->text().toInt());
    SMySetting::instance_p()->setValue ( "dero/n2_vel_slow",lineEdit_n2_slow->text().toInt());

    //STEP
    SMySetting::instance_p()->setValue ( "dero/c1_stepLong",lineEdit_c1Step_long->text().toDouble());
    SMySetting::instance_p()->setValue ( "dero/c1_stepMid",lineEdit_c1Step_mid->text().toDouble());
    SMySetting::instance_p()->setValue ( "dero/c1_stepShort",lineEdit_c1Step_short->text().toDouble());

    SMySetting::instance_p()->setValue ( "dero/n2_stepLong",lineEdit_n2Step_long->text().toDouble());
    SMySetting::instance_p()->setValue ( "dero/n2_stepMid",lineEdit_n2Step_mid->text().toDouble());
    SMySetting::instance_p()->setValue ( "dero/n2_stepShort",lineEdit_n2Step_short->text().toDouble());

    SMySetting::instance_p()->setValue ( "dero/n1_stepLong",lineEdit_n1Step_long->text().toDouble());
    SMySetting::instance_p()->setValue ( "dero/n1_stepMid",lineEdit_n1Step_mid->text().toDouble());
    SMySetting::instance_p()->setValue ( "dero/n1_stepShort",lineEdit_n1Step_short->text().toDouble());

    SMySetting::instance_p()->setValue ( "dero/c1_park",lineEdit_c1_Park->text().toDouble());
    SMySetting::instance_p()->setValue ( "dero/n2_park",lineEdit_n2_Park->text().toDouble());
    SMySetting::instance_p()->setValue ( "dero/n1_park",lineEdit_n1_Park->text().toDouble());

}
void Setup::on_pushButton_Dero_default_clicked()
{
    lineEdit_c1_fast->setText(QString::number(C1_FAST_DEFAULT));
    lineEdit_c1_mid->setText(QString::number(C1_MID_DEFAULT));
    lineEdit_c1_slow->setText(QString::number(C1_SLOW_DEFAULT));

    lineEdit_n2_fast->setText(QString::number(N2_FAST_DEFAULT));
    lineEdit_n2_mid->setText(QString::number(N2_MID_DEFAULT));
    lineEdit_n2_slow->setText(QString::number(N2_SLOW_DEFAULT));

    lineEdit_n1_fast->setText(QString::number(N1_FAST_DEFAULT));
    lineEdit_n1_mid->setText(QString::number(N1_MID_DEFAULT));
    lineEdit_n1_slow->setText(QString::number(N1_SLOW_DEFAULT));

    lineEdit_c1Step_long->setText(QString::number(C1_STEP_LONG_DEFAULT));
    lineEdit_c1Step_mid->setText(QString::number(C1_STEP_MID_DEFAULT));
    lineEdit_c1Step_short->setText(QString::number(C1_STEP_SLOW_DEFAULT));

    lineEdit_n2Step_long->setText(QString::number(N2_STEP_LONG_DEFAULT));
    lineEdit_n2Step_mid->setText(QString::number(N2_STEP_MID_DEFAULT));
    lineEdit_n2Step_short->setText(QString::number(N2_STEP_SLOW_DEFAULT));

    lineEdit_n1Step_long->setText(QString::number(N1_STEP_LONG_DEFAULT));
    lineEdit_n1Step_mid->setText(QString::number(N1_STEP_MID_DEFAULT));
    lineEdit_n1Step_short->setText(QString::number(N1_STEP_SLOW_DEFAULT));

    lineEdit_c1_Park->setText(QString::number(C1_PARK_DEFAULT));
    lineEdit_n1_Park->setText(QString::number(N1_PARK_DEFAULT));
    lineEdit_n2_Park->setText(QString::number(N2_PARK_DEFAULT));

}


void Setup::on_pushButton_Misc_ok_clicked()
{
    //observation time
    float ObT;

    //ObT= SMySetting::instance_p()->value ( "misc/star_len_h" ).toDouble()*3600+SMySetting::instance_p()->value ( "misc/star_len_m" ).toInt()*60.0+SMySetting::instance_p()->value ( "misc/star_len_s" ).toDouble();
    ObT= lineEdit_otH->text().toInt()*3600+ lineEdit_otM->text().toInt()*60+ lineEdit_otS->text().toInt();
    if(ObT>18000) ObT= 18000;
    //qDebug()<<"obt"<<ObT;
    TableNum=int(ObT*10);
    qDebug()<<"table number"<<TableNum;
    SMySetting::instance_p()->setValue ( "misc/star_len_h",lineEdit_otH->text().toInt());
    SMySetting::instance_p()->setValue ( "misc/star_len_m",lineEdit_otM->text().toInt());
    SMySetting::instance_p()->setValue ( "misc/star_len_s",lineEdit_otS->text().toInt());

    SMySetting::instance_p()->setValue ( "misc/longitudeD",lineEdit_longD->text().toDouble());
    SMySetting::instance_p()->setValue ( "misc/longitudeM",lineEdit_longM->text().toInt());
    SMySetting::instance_p()->setValue ( "misc/longitudeS",lineEdit_longS->text().toDouble());

    SMySetting::instance_p()->setValue ( "misc/latitudeD",lineEdit_latiD->text().toDouble());
    SMySetting::instance_p()->setValue ( "misc/latitudeM",lineEdit_latiM->text().toInt());
    SMySetting::instance_p()->setValue ( "misc/latitudeS",lineEdit_latiS->text().toDouble());
    SMySetting::instance_p()->setValue ( "misc/StarTable",lineEdit_StarTableFile->text());  //Save star table
    SMySetting::instance_p()->setValue ( "misc/ModelFile",lineEdit_ModelFile->text());  //read Model
    SMySetting::instance_p()->setValue ( "misc/DataDirectory",lineEdit_DataFileDirectory->text());  //for all telescope file
    SMySetting::instance_p()->setValue ( "misc/LogDirectory",lineEdit_LogFileDirectory->text());  //for all telescope file
    SMySetting::instance_p()->setValue ( "misc/OcsIP",lineEdit_OcsIP->text());
    SMySetting::instance_p()->setValue ( "misc/OcsPort",lineEdit_OcsPort->text().toInt());
    SMySetting::instance_p()->setValue ( "misc/LocalhostIP",lineEdit_LocalhostIP->text());
    SMySetting::instance_p()->setValue ( "misc/LocalhostPort",lineEdit_LocalhostPort->text().toInt());

    //renew constant variables
    LocalhostIP=SMySetting::instance_p()->value ( "misc/LocalhostIP" ).toString();
    LocalhostPort=SMySetting::instance_p()->value ( "misc/LocalhostPort" ).toInt();
    OcsIP=SMySetting::instance_p()->value ( "misc/OcsIP" ).toString();
    OcsPort=SMySetting::instance_p()->value ( "misc/OcsPort" ).toInt();
    SMySetting::instance_p()->setValue ( "misc/SampleTime",lineEdit_SampleTime->text().toInt());
    SampleTime= SMySetting::instance_p()->value ( "misc/SampleTime" ).toInt();

    SMySetting::instance_p()->setValue ( "misc/Optic_Axis_CCD_X",lineEdit_CCDX->text().toInt());
    SMySetting::instance_p()->setValue ( "misc/Optic_Axis_CCD_Y",lineEdit_CCDY->text().toInt());
    SMySetting::instance_p()->setValue ( "misc/CCD_Pixel_Second",lineEdit_Pixel2Second->text().toDouble());
    OpticAxis_X= SMySetting::instance_p()->value ( "misc/Optic_Axis_CCD_X" ).toInt();
    OpticAxis_Y= SMySetting::instance_p()->value ( "misc/Optic_Axis_CCD_Y" ).toInt();
    Pixel_Second=SMySetting::instance_p()->value ( "misc/CCD_Pixel_Second" ).toDouble();
}


void Setup::on_pushButton_Misc_default_clicked()
{
    lineEdit_otH->setText(QString::number(ObservationTimeH_DEFAULT));
    lineEdit_otM->setText(QString::number(ObservationTimeM_DEFAULT));
    lineEdit_otS->setText(QString::number(ObservationTimeS_DEFAULT));

    lineEdit_longD->setText(QString::number(LongitudeD_DEFAULT));
    lineEdit_longM->setText(QString::number(LongitudeM_DEFAULT));
    lineEdit_longS->setText(QString::number(LongitudeS_DEFAULT));

    lineEdit_latiD->setText(QString::number(LatitudeD_DEFAULT));
    lineEdit_latiM->setText(QString::number(LatitudeM_DEFAULT));
    lineEdit_latiS->setText(QString::number(LatitudeS_DEFAULT));

    lineEdit_OcsIP->setText(OcsIP_DEFAULT);
    lineEdit_OcsPort->setText(QString::number(OcsPort_DEFAULT));
    lineEdit_LocalhostIP->setText(LocalhostIP_DEFAULT);
    lineEdit_LocalhostPort->setText(QString::number(LocalhostPort_DEFAULT));

    lineEdit_DataFileDirectory->setText(DataFileDirectory_DEFAULT);
    lineEdit_LogFileDirectory->setText(LogFileDirectory_DEFAULT);
    lineEdit_StarTableFile->setText(StarTableFile_DEFAULT);
    lineEdit_ModelFile->setText(ModelFile_DEFAULT);
    lineEdit_SampleTime->setText(QString::number(SAMPLETIME_DEFAULT));

    lineEdit_CCDX->setText(QString::number(OpticAxisCCDX_DEFAULT));
    lineEdit_CCDY->setText(QString::number(OpticAxisCCDY_DEFAULT));
    lineEdit_Pixel2Second->setText(QString::number(Pixel_Second_DEFAULT));
}


void Setup::on_pushButton_Misc_cancel_clicked()
{


}


void Setup::on_pushButton_SelectDataDirectory_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    if(path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any directory."));
        path=DataFileDirectory_DEFAULT;
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    }
    lineEdit_DataFileDirectory->setText(path);

}


void Setup::on_pushButton_SelectLogDirectory_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

    if(path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any directory."));
        path=LogFileDirectory_DEFAULT;
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    }
    lineEdit_LogFileDirectory->setText(path);

}
