//
// C++ Implementation: direct_m2
//
//
#include "direct_m2.h"
//#include "main_window.h"
#include "signs.h"

DirectM2::DirectM2 ( MainWindow *parent )
{
	m_parent = parent;
	setupUi ( this );
//	connectDevice();
//	initUi();
}

DirectM2::~DirectM2()
{
	qDebug ( "direct m2" );
	deleteLater();
}

void DirectM2::connectDevice()
{
	connect ( SGalil6_3::instance_p(),SIGNAL ( galilInfoThrown2 ( std::vector<char> ) ),this,SLOT ( updateInterface ( std::vector<char> ) ) );
//	SGalil6_3::instance_p()->programDownloadFile ( "/home/dong/telescope/bin/test02.dmc" );
	QString cmd="XQ#M2_TR";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DirectM2::initUi()
{
	switch ( SMySetting::instance_p()->value ( "m2/tz_vel" ).toInt() )
	{
		case VEL_FAST:radioButton_tz_vel_fast->setChecked ( TRUE );break;
		case VEL_MID: radioButton_tz_vel_mid->setChecked ( TRUE );break;
		case VEL_SLOW:radioButton_tz_vel_slow->setChecked ( TRUE );break;
		case VEL_SPEC:radioButton_tz_vel_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m2/tz_acc" ).toInt() )
	{
		case ACC_DEFAULT:radioButton_tz_acc_default->setChecked ( TRUE );break;
		case ACC_SPEC:radioButton_tz_acc_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m2/tz_step" ).toInt() )
	{
		case STEP1:radioButton_tz_step1->setChecked ( TRUE );break;
		case STEP01:radioButton_tz_step01->setChecked ( TRUE );break;
		case STEP001:radioButton_tz_step001->setChecked ( TRUE );break;
		case STEP_SPEC:radioButton_tz_step_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m2/alpha1_vel" ).toInt() )
	{
		case VEL_FAST:radioButton_alpha1_vel_fast->setChecked ( TRUE );break;
		case VEL_MID: radioButton_alpha1_vel_mid->setChecked ( TRUE );break;
		case VEL_SLOW:radioButton_alpha1_vel_slow->setChecked ( TRUE );break;
		case VEL_SPEC:radioButton_alpha1_vel_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m2/alpha1_acc" ).toInt() )
	{
		case ACC_DEFAULT:radioButton_alpha1_acc_default->setChecked ( TRUE );break;
		case ACC_SPEC:radioButton_alpha1_acc_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m2/alpha1_step" ).toInt() )
	{
		case STEP1:radioButton_alpha1_step1->setChecked ( TRUE );break;
		case STEP01:radioButton_alpha1_step01->setChecked ( TRUE );break;
		case STEP001:radioButton_alpha1_step001->setChecked ( TRUE );break;
		case STEP_SPEC:radioButton_alpha1_step_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m2/alpha2_vel" ).toInt() )
	{
		case VEL_FAST:radioButton_alpha2_vel_fast->setChecked ( TRUE );break;
		case VEL_MID: radioButton_alpha2_vel_mid->setChecked ( TRUE );break;
		case VEL_SLOW:radioButton_alpha2_vel_slow->setChecked ( TRUE );break;
		case VEL_SPEC:radioButton_alpha2_vel_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m2/alpha2_acc" ).toInt() )
	{
		case ACC_DEFAULT:radioButton_alpha2_acc_default->setChecked ( TRUE );break;
		case ACC_SPEC:radioButton_alpha2_acc_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m2/alpha2_step" ).toInt() )
	{
		case STEP1:radioButton_alpha2_step1->setChecked ( TRUE );break;
		case STEP01:radioButton_alpha2_step01->setChecked ( TRUE );break;
		case STEP001:radioButton_alpha2_step001->setChecked ( TRUE );break;
		case STEP_SPEC:radioButton_alpha2_step_spec->setChecked ( TRUE );break;
		default:break;
	}

}
void DirectM2::closeEvent ( QCloseEvent *event )
{
	parentWidget()->hide();
	event->ignore();
}

void DirectM2::updateInterface ( std::vector<char> g )
{
	lineEdit_tz_pos->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TPA" ) ) /M3_TR_EN ) );//need to check later
	lineEdit_tz_poserr->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TEA" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );
	lineEdit_tz_vel->setText ( QString::number ( abs ( SGalil6_3::instance_p()->sourceValue ( g,"_TVA" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );
}

void DirectM2::on_radioButton_tz_vel_fast_toggled ( bool on )
{
	if ( on )
	{
		tz_vel = SMySetting::instance_p()->value ( "m2/tz_vel_fast" ).toDouble();
		QString cmd="SPA=";
		cmd+=QString::number ( MMTOCTS ( tz_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		cmd="SPB=";
		cmd+=QString::number ( MMTOCTS ( tz_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		cmd="SPC=";
		cmd+=QString::number ( MMTOCTS ( tz_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
	}

}
void DirectM2::on_radioButton_tz_vel_mid_toggled ( bool on )
{
	if ( on )
	{
		tz_vel = SMySetting::instance_p()->value ( "m2/tz_vel_mid" ).toDouble();
		QString cmd="SPA=";
		cmd+=QString::number ( MMTOCTS ( tz_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		cmd="SPB=";
		cmd+=QString::number ( MMTOCTS ( tz_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		cmd="SPC=";
		cmd+=QString::number ( MMTOCTS ( tz_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
	}
}
void DirectM2::on_radioButton_tz_vel_slow_toggled ( bool on )
{
	if ( on )
	{
		tz_vel = SMySetting::instance_p()->value ( "m2/tz_vel_slow" ).toDouble();
		QString cmd="SPA=";
		cmd+=QString::number ( MMTOCTS ( tz_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		cmd="SPB=";
		cmd+=QString::number ( MMTOCTS ( tz_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		cmd="SPC=";
		cmd+=QString::number ( MMTOCTS ( tz_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
	}
}

void DirectM2::on_lineEdit_tz_vel_spec_returnPressed()
{
	tz_vel =lineEdit_tz_vel_spec->text().toFloat();
	QString cmd="SPA=";
	cmd+=QString::number ( MMTOCTS ( tz_vel ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd="SPB=";
	cmd+=QString::number ( MMTOCTS ( tz_vel ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd="SPC=";
	cmd+=QString::number ( MMTOCTS ( tz_vel ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DirectM2::on_radioButton_tz_acc_default_toggled ( bool on )
{
	if ( on )
	{
		tz_acc = SMySetting::instance_p()->value ( "m2/tz_acc" ).toDouble();
		QString cmd="ACA=";
		cmd+=QString::number ( MMTOCTS ( tz_acc ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		cmd="ACB=";
		cmd+=QString::number ( MMTOCTS ( tz_acc ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		cmd="ACC=";
		cmd+=QString::number ( MMTOCTS ( tz_acc ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
	}
}

void DirectM2::on_lineEdit_tz_acc_spec_returnPressed()
{
	tz_acc =lineEdit_tz_acc_spec->text().toFloat();
	QString cmd="ACA=";
	cmd+=QString::number ( MMTOCTS ( tz_acc ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd="ACB=";
	cmd+=QString::number ( MMTOCTS ( tz_acc ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd="ACC=";
	cmd+=QString::number ( MMTOCTS ( tz_acc ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DirectM2::on_pushButton_tz_go_clicked()
{
	tz_target = lineEdit_tz_target->text().toFloat();
	try
	{
		QString cmd="M2_TZ_GO=";
		cmd+=QString::number ( tz_target*M3_TR_EN );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		cmd="XQ#M2_TZ_GO";
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
	}
	catch ( std::string excepts )
	{
		qDebug() << QString::fromStdString ( excepts );
		return;
	}
}

void DirectM2::on_pushButton_tz_stop_clicked()
{
	try
	{
		QString cmd="XQ#M2_TZ_ST";
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
	}
	catch ( std::string excepts )
	{
		qDebug() << QString::fromStdString ( excepts );
		return;
	}
}

void DirectM2::on_pushButton_tz_stepM_clicked()
{
	if ( radioButton_tz_step1->isChecked() )
		{tz_step = SMySetting::instance_p()->value ( "m2/tz_step1" ).toDouble() ;goto stepM;}
	if ( radioButton_tz_step01->isChecked() )
		{tz_step = SMySetting::instance_p()->value ( "m2/tz_step01" ).toDouble();goto stepM;}
	if ( radioButton_tz_step001->isChecked() )
		{tz_step = SMySetting::instance_p()->value ( "m2/tz_step001" ).toDouble();goto stepM;}
	if ( radioButton_tz_step_spec->isChecked() )
		{tz_step = lineEdit_tz_step_spec->text().toFloat();goto stepM;}

stepM:
	tz_step=-tz_step;
	QString cmd="PRA=";
	cmd+=QString::number ( MMTOCTS ( tz_step ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd="PRB=";
	cmd+=QString::number ( MMTOCTS ( tz_step ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd="PRC=";
	cmd+=QString::number ( MMTOCTS ( tz_step ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "BG A,B,C";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DirectM2::on_pushButton_tz_stepP_clicked()
{
	if ( radioButton_tz_step1->isChecked() )
		{tz_step = SMySetting::instance_p()->value ( "m2/tz_step1" ).toDouble() ;goto stepP;}
	if ( radioButton_tz_step01->isChecked() )
		{tz_step = SMySetting::instance_p()->value ( "m2/tz_step01" ).toDouble();goto stepP;}
	if ( radioButton_tz_step001->isChecked() )
		{tz_step = SMySetting::instance_p()->value ( "m2/tz_step001" ).toDouble();goto stepP;}
	if ( radioButton_tz_step_spec->isChecked() )
		{tz_step = lineEdit_tz_step_spec->text().toFloat();goto stepP;}

stepP:
	QString cmd="PRA=";
	cmd+=QString::number ( MMTOCTS ( tz_step ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd="PRB=";
	cmd+=QString::number ( MMTOCTS ( tz_step ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd="PRC=";
	cmd+=QString::number ( MMTOCTS ( tz_step ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "BG A,B,C";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DirectM2::on_radioButton_alpha1_vel_fast_toggled ( bool on )
{
	if ( on )
	{
		tz_vel = SMySetting::instance_p()->value ( "m2/tz_vel_fast" ).toDouble();
		radioButton_tz_vel_fast->setChecked ( TRUE );
		xc_vel = SMySetting::instance_p()->value ( "m2/xc_vel_fast" ).toDouble();
		QString cmd = "SPD=";
		cmd += QString::number ( MMTOCTS ( xc_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		yc_vel = SMySetting::instance_p()->value ( "m2/yc_vel_fast" ).toDouble();
		cmd = "SPE=";
		cmd += QString::number ( MMTOCTS ( yc_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
	}
}
void DirectM2::on_radioButton_alpha1_vel_mid_toggled ( bool on )
{
	if ( on )
	{
		tz_vel = SMySetting::instance_p()->value ( "m2/tz_vel_mid" ).toDouble();
		radioButton_tz_vel_mid->setChecked ( TRUE );
		xc_vel = SMySetting::instance_p()->value ( "m2/xc_vel_mid" ).toDouble();
		QString cmd = "SPD=";
		cmd += QString::number ( MMTOCTS ( xc_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		yc_vel = SMySetting::instance_p()->value ( "m2/yc_vel_mid" ).toDouble();
		cmd = "SPE=";
		cmd += QString::number ( MMTOCTS ( yc_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
	}
}
void DirectM2::on_radioButton_alpha1_vel_slow_toggled ( bool on )
{
	if ( on )
	{
		tz_vel = SMySetting::instance_p()->value ( "m2/tz_vel_slow" ).toDouble();
		radioButton_tz_vel_slow->setChecked ( TRUE );
		xc_vel = SMySetting::instance_p()->value ( "m2/xc_vel_slow" ).toDouble();
		QString cmd = "SPD=";
		cmd += QString::number ( MMTOCTS ( xc_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		yc_vel = SMySetting::instance_p()->value ( "m2/yc_vel_slow" ).toDouble();
		cmd = "SPE=";
		cmd += QString::number ( MMTOCTS ( yc_vel ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
	}
}

void DirectM2::on_lineEdit_alpha1_vel_spec_returnPressed()
{
//	emit alpha1_vel_spec ( lineEdit_alpha1_vel_spec->text().toFloat() );
}

void DirectM2::on_radioButton_alpha1_acc_default_toggled ( bool on )
{
	if ( on )
	{
		tz_acc = SMySetting::instance_p()->value ( "m2/tz_acc" ).toDouble();
		radioButton_tz_acc_default->setChecked ( TRUE );
		xc_acc = SMySetting::instance_p()->value ( "m2/xc_acc" ).toDouble();
		QString cmd = "ACD=";
		cmd += QString::number ( MMTOCTS ( xc_acc ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
		yc_acc = SMySetting::instance_p()->value ( "m2/yc_acc" ).toDouble();
		cmd = "ACE=";
		cmd += QString::number ( MMTOCTS ( yc_acc ) );
		SGalil6_3::instance_p()->command ( cmd.toStdString() );
	}
}

void DirectM2::on_lineEdit_alpha1_acc_spec_returnPressed()
{
//	emit alpha1_acc_spec ( lineEdit_alpha1_acc_spec->text().toFloat() );
}

void DirectM2::on_pushButton_alpha1_go_clicked()
{
	double alpha1_vel = lineEdit_tz_target->text().toFloat();
	double tz_go = 282.5* ( 1-cos ( alpha1_vel ) );//叫面不常具里
	double tz_go1 = 2.0*457.7*sin ( alpha1_vel/2.0 ) *cos ( alpha1_vel/2.0 ) +cos ( asin ( ( 2.0*457.7*sin ( alpha1_vel/2.0 ) *sin ( alpha1_vel/2.0 ) ) /20.0 ) ) *20.0-20.0-tz_go;//A点己
	double tz_go23 = 457.7*sin ( alpha1_vel/2 ) *cos ( alpha1_vel/2 ) +cos ( asin ( ( 457.7*sin ( alpha1_vel/2 ) *sin ( alpha1_vel/2 ) ) /20 ) ) *20-20+tz_go;//BC点己
	double yc_go = 282.5*sin ( alpha1_vel );//E电机
}

void DirectM2::on_pushButton_alpha1_stop_clicked()
{

}

void DirectM2::on_pushButton_alpha1_stepM_clicked()
{
// 	if(radioButton_alpha1_step1->isChecked())
// 		return emit alpha1_stepM(SMySetting::instance_p()->value ( "m2/alpha1_step1" ).toDouble()) ;
// 	if(radioButton_alpha1_step01->isChecked())
// 		return emit alpha1_stepM(SMySetting::instance_p()->value ( "m2/alpha1_step01" ).toDouble()) ;
// 	if(radioButton_alpha1_step001->isChecked())
// 		return emit alpha1_stepM(SMySetting::instance_p()->value ( "m2/alpha1_step001" ).toDouble()) ;
// 	if(radioButton_alpha1_step_spec->isChecked())
// 		return emit alpha1_stepM(lineEdit_alpha1_step_spec->text().toFloat()) ;
}

void DirectM2::on_pushButton_alpha1_stepP_clicked()
{
// 	if(radioButton_alpha1_step1->isChecked())
// 		return emit alpha1_stepP(SMySetting::instance_p()->value ( "m2/alpha1_step1" ).toDouble()) ;
// 	if(radioButton_alpha1_step01->isChecked())
// 		return emit alpha1_stepP(SMySetting::instance_p()->value ( "m2/alpha1_step01" ).toDouble()) ;
// 	if(radioButton_alpha1_step001->isChecked())
// 		return emit alpha1_stepP(SMySetting::instance_p()->value ( "m2/alpha1_step001" ).toDouble()) ;
// 	if(radioButton_alpha1_step_spec->isChecked())
// 		return emit alpha1_stepP(lineEdit_alpha1_step_spec->text().toFloat()) ;
}

void DirectM2::on_radioButton_alpha2_vel_fast_toggled ( bool on )
{
	if ( on )
	{
		radioButton_alpha1_vel_fast->setChecked ( TRUE );
	}
}
void DirectM2::on_radioButton_alpha2_vel_mid_toggled ( bool on )
{
	if ( on )
	{
		radioButton_alpha1_vel_mid->setChecked ( TRUE );
	}
}
void DirectM2::on_radioButton_alpha2_vel_slow_toggled ( bool on )
{
	if ( on )
	{
		radioButton_alpha1_vel_slow->setChecked ( TRUE );
	}
}

void DirectM2::on_lineEdit_alpha2_vel_spec_returnPressed()
{
	
}

void DirectM2::on_radioButton_alpha2_acc_default_toggled ( bool on )
{
	if ( on )
	{
		radioButton_alpha1_acc_default->setChecked ( TRUE );
	}
}

void DirectM2::on_lineEdit_alpha2_acc_spec_returnPressed()
{
	
}


void DirectM2::on_pushButton_alpha2_go_clicked()
{

}

void DirectM2::on_pushButton_alpha2_stop_clicked()
{

}

void DirectM2::on_pushButton_alpha2_stepM_clicked()
{

}

void DirectM2::on_pushButton_alpha2_stepP_clicked()
{

}

void DirectM2::on_pushButton_alpha2_reset_clicked()
{
	
}
