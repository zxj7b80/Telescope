//
// C++ Implementation: direct_m3
//

#include "direct_m3.h"
//#include "main_window.h"
#include "signs.h"

DirectM3::DirectM3 ( MainWindow *parent )
{
	m_parent = parent;
	setupUi ( this );
//	radioButton_ro_vel_fast->installEventFilter(this);
//	connectDevice();
//	initUi();
}

DirectM3::~DirectM3()
{
	qDebug ( "direct m3" );
	deleteLater();
}

void DirectM3::connectDevice()
{
	connect ( SGalil8::instance_p(),SIGNAL ( galilInfoThrown ( std::vector<char> ) ),this,SLOT ( updateInterface ( std::vector<char> ) ) );
//	SGalil8::instance_p()->programDownloadFile ( "/home/dong/telescope/bin/test01.dmc" );
}

void DirectM3::initUi()
{
	switch ( SMySetting::instance_p()->value ( "m3/ro_vel" ).toInt() )
	{
		case VEL_FAST:radioButton_ro_vel_fast->setChecked ( TRUE );break;
		case VEL_MID: radioButton_ro_vel_mid->setChecked ( TRUE );break;
		case VEL_SLOW:radioButton_ro_vel_slow->setChecked ( TRUE );break;
		case VEL_SPEC:radioButton_ro_vel_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m3/ro_acc" ).toInt() )
	{
		case ACC_DEFAULT:radioButton_ro_acc_default->setChecked ( TRUE );break;
		case ACC_SPEC:radioButton_ro_acc_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m3/ro_step" ).toInt() )
	{
		case STEP1:radioButton_ro_step1->setChecked ( TRUE );break;
		case STEP01:radioButton_ro_step01->setChecked ( TRUE );break;
		case STEP001:radioButton_ro_step001->setChecked ( TRUE );break;
		case STEP_SPEC:radioButton_ro_step_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m3/es_vel" ).toInt() )
	{
		case VEL_FAST:radioButton_es_vel_fast->setChecked ( TRUE );break;
		case VEL_MID: radioButton_es_vel_mid->setChecked ( TRUE );break;
		case VEL_SLOW:radioButton_es_vel_slow->setChecked ( TRUE );break;
		case VEL_SPEC:radioButton_es_vel_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m3/es_acc" ).toInt() )
	{
		case ACC_DEFAULT:radioButton_es_acc_default->setChecked ( TRUE );break;
		case ACC_SPEC:radioButton_es_acc_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m3/es_step" ).toInt() )
	{
		case STEP1:radioButton_es_step1->setChecked ( TRUE );break;
		case STEP01:radioButton_es_step01->setChecked ( TRUE );break;
		case STEP001:radioButton_es_step001->setChecked ( TRUE );break;
		case STEP_SPEC:radioButton_es_step_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m3/tr_vel" ).toInt() )
	{
		case VEL_FAST:radioButton_tr_vel_fast->setChecked ( TRUE );break;
		case VEL_MID: radioButton_tr_vel_mid->setChecked ( TRUE );break;
		case VEL_SLOW:radioButton_tr_vel_slow->setChecked ( TRUE );break;
		case VEL_SPEC:radioButton_tr_vel_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m3/tr_acc" ).toInt() )
	{
		case ACC_DEFAULT:radioButton_tr_acc_default->setChecked ( TRUE );break;
		case ACC_SPEC:radioButton_tr_acc_spec->setChecked ( TRUE );break;
		default:break;
	}
	switch ( SMySetting::instance_p()->value ( "m3/tr_step" ).toInt() )
	{
		case STEP1:radioButton_tr_step1->setChecked ( TRUE );break;
		case STEP01:radioButton_tr_step01->setChecked ( TRUE );break;
		case STEP001:radioButton_tr_step001->setChecked ( TRUE );break;
		case STEP_SPEC:radioButton_tr_step_spec->setChecked ( TRUE );break;
		default:break;
	}

	QString cmd="XQ#M3_TR";
	SGalil8::instance_p()->command ( cmd.toStdString() );
}
void DirectM3::closeEvent ( QCloseEvent *event )
{
	parentWidget()->hide();
	event->ignore();
}

void DirectM3::updateInterface ( std::vector<char> g )
{
	lineEdit_tr_pos->setText ( QString::number ( ( SGalil8::instance_p()->sourceValue ( g,"_TPA" ) ) /M3_TR_EN ) );//need to check later
	lineEdit_tr_poserr->setText ( QString::number ( ( SGalil8::instance_p()->sourceValue ( g,"_TEA" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );
	lineEdit_tr_vel->setText ( QString::number ( abs ( SGalil8::instance_p()->sourceValue ( g,"_TVA" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );
}

void DirectM3::on_radioButton_ro_vel_fast_toggled ( bool on )
{
//	if ( on )
// 		ro_vel = SMySetting::instance_p()->value ( "m3/ro_vel_fast" ).toDouble();

}
void DirectM3::on_radioButton_ro_vel_mid_toggled ( bool on )
{
// 	if ( on )
// 		ro_vel = SMySetting::instance_p()->value ( "m3/ro_vel_mid" ).toDouble();
}
void DirectM3::on_radioButton_ro_vel_slow_toggled ( bool on )
{
//	if ( on )
//		ro_vel = SMySetting::instance_p()->value ( "m3/ro_vel_slow" ).toDouble();
}

void DirectM3::on_lineEdit_ro_vel_spec_returnPressed()
{
//	emit ro_vel_spec ( lineEdit_ro_vel_spec->text().toFloat() );
}

void DirectM3::on_radioButton_ro_acc_default_toggled ( bool on )
{
// 	if ( on )
// 		ro_acc = SMySetting::instance_p()->value ( "m3/ro_acc_default" ).toDouble();
}

void DirectM3::on_lineEdit_ro_acc_spec_returnPressed()
{
//	emit ro_acc_spec ( lineEdit_ro_acc_spec->text().toFloat() );
}

void DirectM3::on_pushButton_ro_go_clicked()
{
//	emit ro_acc_spec ( lineEdit_ro_acc_spec->text().toFloat() );
}

void DirectM3::on_pushButton_ro_stop_clicked()
{

}

void DirectM3::on_pushButton_ro_F1_clicked()
{
	//0
}

void DirectM3::on_pushButton_ro_F2_clicked()
{
	//90
}

void DirectM3::on_pushButton_ro_F3_clicked()
{
	//180
}

void DirectM3::on_pushButton_ro_F4_clicked()
{
	//270
}

void DirectM3::on_pushButton_ro_stepM_clicked()
{
// 	if(radioButton_ro_step1->isChecked())
// 		return emit ro_stepM(SMySetting::instance_p()->value ( "m3/ro_step1" ).toDouble()) ;
// 	if(radioButton_ro_step01->isChecked())
// 		return emit ro_stepM(SMySetting::instance_p()->value ( "m3/ro_step01" ).toDouble()) ;
// 	if(radioButton_ro_step001->isChecked())
// 		return emit ro_stepM(SMySetting::instance_p()->value ( "m3/ro_step001" ).toDouble()) ;
// 	if(radioButton_ro_step_spec->isChecked())
// 		return emit ro_stepM(lineEdit_ro_step_spec->text().toFloat()) ;
}

void DirectM3::on_pushButton_ro_stepP_clicked()
{
// 	if(radioButton_ro_step1->isChecked())
// 		return emit ro_stepP(SMySetting::instance_p()->value ( "m3/ro_step1" ).toDouble()) ;
// 	if(radioButton_ro_step01->isChecked())
// 		return emit ro_stepP(SMySetting::instance_p()->value ( "m3/ro_step01" ).toDouble()) ;
// 	if(radioButton_ro_step001->isChecked())
// 		return emit ro_stepP(SMySetting::instance_p()->value ( "m3/ro_step001" ).toDouble()) ;
// 	if(radioButton_ro_step_spec->isChecked())
// 		return emit ro_stepP(lineEdit_ro_step_spec->text().toFloat()) ;
}

void DirectM3::on_radioButton_es_vel_fast_toggled ( bool on )
{
// 	if ( on )
// 		es_vel = SMySetting::instance_p()->value ( "m3/es_vel_fast" ).toDouble();
}
void DirectM3::on_radioButton_es_vel_mid_toggled ( bool on )
{
// 	if ( on )
// 		es_vel = SMySetting::instance_p()->value ( "m3/es_vel_mid" ).toDouble();
}
void DirectM3::on_radioButton_es_vel_slow_toggled ( bool on )
{
// 	if ( on )
// 		es_vel = SMySetting::instance_p()->value ( "m3/es_vel_slow" ).toDouble();
}

void DirectM3::on_lineEdit_es_vel_spec_returnPressed()
{
//	emit es_vel_spec ( lineEdit_es_vel_spec->text().toFloat() );
}

void DirectM3::on_radioButton_es_acc_default_toggled ( bool on )
{
// 	if ( on )
// 		es_acc = SMySetting::instance_p()->value ( "m3/es_acc_default" ).toDouble();
}

void DirectM3::on_lineEdit_es_acc_spec_returnPressed()
{
//	emit es_acc_spec ( lineEdit_es_acc_spec->text().toFloat() );
}

void DirectM3::on_pushButton_es_go_clicked()
{
//	emit ro_acc_spec ( lineEdit_ro_target->text().toFloat() );
}

void DirectM3::on_pushButton_es_stop_clicked()
{

}

void DirectM3::on_pushButton_es_stepM_clicked()
{
// 	if(radioButton_es_step1->isChecked())
// 		return emit es_stepM(SMySetting::instance_p()->value ( "m3/es_step1" ).toDouble()) ;
// 	if(radioButton_es_step01->isChecked())
// 		return emit es_stepM(SMySetting::instance_p()->value ( "m3/es_step01" ).toDouble()) ;
// 	if(radioButton_es_step001->isChecked())
// 		return emit es_stepM(SMySetting::instance_p()->value ( "m3/es_step001" ).toDouble()) ;
// 	if(radioButton_es_step_spec->isChecked())
// 		return emit es_stepM(lineEdit_es_step_spec->text().toFloat()) ;
}

void DirectM3::on_pushButton_es_stepP_clicked()
{
// 	if(radioButton_es_step1->isChecked())
// 		return emit es_stepP(SMySetting::instance_p()->value ( "m3/es_step1" ).toDouble()) ;
// 	if(radioButton_es_step01->isChecked())
// 		return emit es_stepP(SMySetting::instance_p()->value ( "m3/es_step01" ).toDouble()) ;
// 	if(radioButton_es_step001->isChecked())
// 		return emit es_stepP(SMySetting::instance_p()->value ( "m3/es_step001" ).toDouble()) ;
// 	if(radioButton_es_step_spec->isChecked())
// 		return emit es_stepP(lineEdit_es_step_spec->text().toFloat()) ;
}

void DirectM3::on_radioButton_tr_vel_fast_toggled ( bool on )
{
	if ( on )
	{
		tr_vel = SMySetting::instance_p()->value ( "m3/tr_vel_fast" ).toDouble();
		QString cmd="SPA=";
		qDebug() << QString::number ( MMTOCTS ( tr_vel ) );
		cmd+=QString::number ( MMTOCTS ( tr_vel ) );
		SGalil8::instance_p()->command ( cmd.toStdString() );
		cmd="VEL_A=";
		cmd+=QString::number ( MMTOCTS ( tr_vel ) );
		SGalil8::instance_p()->command ( cmd.toStdString() );
	}
}
void DirectM3::on_radioButton_tr_vel_mid_toggled ( bool on )
{
	if ( on )
	{
		tr_vel = SMySetting::instance_p()->value ( "m3/tr_vel_mid" ).toDouble();
		QString cmd="SPA=";
		cmd+=QString::number ( MMTOCTS ( tr_vel ) );
		SGalil8::instance_p()->command ( cmd.toStdString() );
		cmd="VEL_A=";
		cmd+=QString::number ( MMTOCTS ( tr_vel ) );
		SGalil8::instance_p()->command ( cmd.toStdString() );
	}
}
void DirectM3::on_radioButton_tr_vel_slow_toggled ( bool on )
{
	if ( on )
	{
		tr_vel = SMySetting::instance_p()->value ( "m3/tr_vel_slow" ).toDouble();
		QString cmd="SPA=";
		cmd+=QString::number ( MMTOCTS ( tr_vel ) );
		SGalil8::instance_p()->command ( cmd.toStdString() );
		cmd="VEL_A=";
		cmd+=QString::number ( MMTOCTS ( tr_vel ) );
		SGalil8::instance_p()->command ( cmd.toStdString() );
	}
}

void DirectM3::on_lineEdit_tr_vel_spec_returnPressed()
{
	tr_vel =lineEdit_tr_vel_spec->text().toFloat();
	QString cmd="SPA=";
	cmd+=QString::number ( MMTOCTS ( tr_vel ) );
	SGalil8::instance_p()->command ( cmd.toStdString() );
	cmd="VEL_A=";
	cmd+=QString::number ( MMTOCTS ( tr_vel ) );
	qDebug() << cmd;
	SGalil8::instance_p()->command ( cmd.toStdString() );
}

void DirectM3::on_radioButton_tr_acc_default_toggled ( bool on )
{
	if ( on )
	{
		tr_acc = SMySetting::instance_p()->value ( "m3/tr_acc" ).toDouble();
		QString cmd="ACA=";
		cmd+=QString::number ( MMTOCTS ( tr_acc ) );
		SGalil8::instance_p()->command ( cmd.toStdString() );
	}
}

void DirectM3::on_lineEdit_tr_acc_spec_returnPressed()
{
	tr_acc =lineEdit_tr_acc_spec->text().toFloat();
	QString cmd="ACA=";
	cmd+=QString::number ( MMTOCTS ( tr_acc ) );
	SGalil8::instance_p()->command ( cmd.toStdString() );
}


void DirectM3::on_pushButton_tr_go_clicked()
{
	tr_target = lineEdit_tr_target->text().toFloat();
	try
	{
		QString cmd="TARGET_A=";
		cmd+=QString::number ( tr_target*M3_TR_EN );
		SGalil8::instance_p()->command ( cmd.toStdString() );
		cmd="XQ#M3_GO";
// 	cmd="#M3\rIF TARGET_A<_TPA\rJG -VEL_A\rSHA\rBGA\r#LOOP1\rJP #LOOP1,_TPA>TARGET_A+1\rSTA\rELSE\rJG VEL_A\rSHA\rBGA\r#LOOP2\rJP #LOOP2,_TPA<TARGET_A-1\rSTA\rEN";
// 	SGalil8::instance_p()->programDownload(cmd.toStdString());
// 	cmd="XQ#M3";
		SGalil8::instance_p()->command ( cmd.toStdString() );
	}
	catch ( std::string excepts )
	{
		qDebug() << QString::fromStdString ( excepts );
		return;
	}
}

void DirectM3::on_pushButton_tr_stop_clicked()
{
	QString cmd;
	cmd="STA";
	SGalil8::instance_p()->command ( cmd.toStdString() );
}

void DirectM3::on_pushButton_tr_stepM_clicked()
{
	if ( radioButton_tr_step1->isChecked() )
		{tr_step = SMySetting::instance_p()->value ( "m3/tr_step1" ).toDouble() ;goto stepM;}
	if ( radioButton_tr_step01->isChecked() )
		{tr_step = SMySetting::instance_p()->value ( "m3/tr_step01" ).toDouble();goto stepM;}
	if ( radioButton_tr_step001->isChecked() )
		{tr_step = SMySetting::instance_p()->value ( "m3/tr_step001" ).toDouble();goto stepM;}
	if ( radioButton_tr_step_spec->isChecked() )
		{tr_step = lineEdit_tr_step_spec->text().toFloat();goto stepM;}

stepM:	QString cmd="PRA=";
	tr_step=-tr_step;
	cmd+=QString::number ( MMTOCTS ( tr_step ) );
	SGalil8::instance_p()->command ( cmd.toStdString() );
	cmd = "BGA";
	SGalil8::instance_p()->command ( cmd.toStdString() );
}

void DirectM3::on_pushButton_tr_stepP_clicked()
{
//	plot = new PlotM3(direct_m3_thread);
//	connect(direct_m3_thread,SIGNAL(galilInfoThrown(std::vector<char>)),plot,SLOT(showCurve(std::vector<char>)));
//	plot->show();
	try
	{
		if ( radioButton_tr_step1->isChecked() )
			{tr_step = SMySetting::instance_p()->value ( "m3/tr_step1" ).toDouble() ;goto stepP;}
		if ( radioButton_tr_step01->isChecked() )
			{tr_step = SMySetting::instance_p()->value ( "m3/tr_step01" ).toDouble();goto stepP;}
		if ( radioButton_tr_step001->isChecked() )
			{tr_step = SMySetting::instance_p()->value ( "m3/tr_step001" ).toDouble();goto stepP;}
		if ( radioButton_tr_step_spec->isChecked() )
			{tr_step = lineEdit_tr_step_spec->text().toFloat();goto stepP;}

	stepP:	QString cmd="PRA=";
		cmd+=QString::number ( MMTOCTS ( tr_step ) );
		SGalil8::instance_p()->command ( cmd.toStdString() );
		cmd = "BGA";
		SGalil8::instance_p()->command ( cmd.toStdString() );
	}
	catch ( std::string str ) {qDebug() << QString::fromStdString ( str );}
}

void DirectM3::on_pushButton_tr_reset_clicked()
{
	QString cmd;
	cmd="XQ#M3_RE";
	try{SGalil8::instance_p()->command ( cmd.toStdString() );}
	catch ( std::string excepts )
	{
		qDebug() << QString::fromStdString ( excepts );
		return;
	}
}
