//
// C++ Implementation: diag_m2
//
// Description:
//
//#include "main_window.h"
#include "diag_m2.h"
#include "signs.h"

DiagM2::DiagM2 ( MainWindow *parent )
{
	m_parent = parent;
	setupUi ( this );
//	connectDevice();
//	initUi();
}

DiagM2::~DiagM2()
{
	qDebug ( "diag m2" );
	deleteLater();
}

void DiagM2::connectDevice()
{
	connect ( SGalil6_3::instance_p(),SIGNAL ( galilInfoThrown ( std::vector<char> ) ),this,SLOT ( updateInterface ( std::vector<char> ) ) );
//	SGalil6_3::instance_p()->programDownloadFile ( "/home/dong/telescope/bin/test02.dmc" );
	QString cmd="XQ#M2_TR";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DiagM2::updateInterface ( std::vector<char> g )
{
	lineEdit_tilt1_pos->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TPA" ) ) /M3_TR_EN ) );//need to check later
	lineEdit_tilt1_poserr->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TEA" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );
	lineEdit_tilt1_vel->setText ( QString::number ( abs ( SGalil6_3::instance_p()->sourceValue ( g,"_TVA" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );

	lineEdit_tilt2_pos->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TPB" ) ) /M3_TR_EN ) );//need to check later
	lineEdit_tilt2_poserr->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TEB" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );
	lineEdit_tilt2_vel->setText ( QString::number ( abs ( SGalil6_3::instance_p()->sourceValue ( g,"_TVB" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );

	lineEdit_tilt3_pos->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TPC" ) ) /M3_TR_EN ) );//need to check later
	lineEdit_tilt3_poserr->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TEC" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );
	lineEdit_tilt3_vel->setText ( QString::number ( abs ( SGalil6_3::instance_p()->sourceValue ( g,"_TVC" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );

	lineEdit_xc_pos->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TPD" ) ) /M3_TR_EN ) );//need to check later
	lineEdit_xc_poserr->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TED" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );
	lineEdit_xc_vel->setText ( QString::number ( abs ( SGalil6_3::instance_p()->sourceValue ( g,"_TVD" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );

	lineEdit_yc_pos->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TPE" ) ) /M3_TR_EN ) );//need to check later
	lineEdit_yc_poserr->setText ( QString::number ( ( SGalil6_3::instance_p()->sourceValue ( g,"_TEE" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );
	lineEdit_yc_vel->setText ( QString::number ( abs ( SGalil6_3::instance_p()->sourceValue ( g,"_TVE" ) ) / ( M3_TR_EN*M3_TR_MO ) ) );
}

void DiagM2::initUi()
{
	SMySetting::instance_p()->beginGroup ( "m2" );
	SMySetting::instance_p()->endGroup();
}

void DiagM2::closeEvent ( QCloseEvent *event )
{
	parentWidget()->hide();
	event->ignore();
}

void DiagM2::on_pushButton_tilt1_go_clicked()
{
	struct velTar vt;
	vt.vel = lineEdit_tilt1_vel_spec->text().toFloat();
	vt.target = lineEdit_tilt1_target->text().toFloat();
	QString cmd="SPA=";
	cmd+=QString::number ( MMTOCTS ( vt.vel ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "PAA=";
	cmd+=QString::number ( MMTOCTS ( vt.target ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "BGA";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DiagM2::on_pushButton_tilt2_go_clicked()
{
	struct velTar vt;
	vt.vel = lineEdit_tilt2_vel_spec->text().toFloat();
	vt.target = lineEdit_tilt2_target->text().toFloat();
	QString cmd="SPB=";
	cmd+=QString::number ( MMTOCTS ( vt.vel ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "PAB=";
	cmd+=QString::number ( MMTOCTS ( vt.target ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "BGB";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DiagM2::on_pushButton_tilt3_go_clicked()
{
	struct velTar vt;
	vt.vel = lineEdit_tilt3_vel_spec->text().toFloat();
	vt.target = lineEdit_tilt3_target->text().toFloat();
	QString cmd="SPC=";
	cmd+=QString::number ( MMTOCTS ( vt.vel ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "PAC=";
	cmd+=QString::number ( MMTOCTS ( vt.target ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "BGC";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DiagM2::on_pushButton_xc_go_clicked()
{
	struct velTar vt;
	vt.vel = lineEdit_xc_vel_spec->text().toFloat();
	vt.target = lineEdit_xc_target->text().toFloat();
	QString cmd="SPD=";
	cmd+=QString::number ( MMTOCTS ( vt.vel ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "PAD=";
	cmd+=QString::number ( MMTOCTS ( vt.target ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "BGD";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DiagM2::on_pushButton_yc_go_clicked()
{
	struct velTar vt;
	vt.vel = lineEdit_yc_vel_spec->text().toFloat();
	vt.target = lineEdit_yc_target->text().toFloat();
	QString cmd="SPE=";
	cmd+=QString::number ( MMTOCTS ( vt.vel ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "PAE=";
	cmd+=QString::number ( MMTOCTS ( vt.target ) );
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
	cmd = "BGE";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DiagM2::on_pushButton_tilt1_stop_clicked()
{
	QString cmd="STA";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DiagM2::on_pushButton_tilt2_stop_clicked()
{
	QString cmd="STB";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DiagM2::on_pushButton_tilt3_stop_clicked()
{
	QString cmd="STC";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DiagM2::on_pushButton_xc_stop_clicked()
{
	QString cmd="STD";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}

void DiagM2::on_pushButton_yc_stop_clicked()
{
	QString cmd="STE";
	SGalil6_3::instance_p()->command ( cmd.toStdString() );
}
