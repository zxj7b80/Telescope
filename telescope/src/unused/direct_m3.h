//
// C++ Interface: direct_m3
//
// Description:
//
//
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DIRECT_M3_H
#define DIRECT_M3_H
#include <QtGui>
#include "ui_direct_m3.h"
class Galil;
//class PlotM3;
class MainWindow;
class DirectM3:public QScrollArea,private Ui::DirectM3
{
		Q_OBJECT
	public:
		DirectM3 (MainWindow * parent);
		~DirectM3();

	private:
		void connectDevice();
		void initUi();
		MainWindow *m_parent;
		double tr_target;
		double tr_vel;
		double tr_acc;
		double tr_step;
	protected:
		virtual void closeEvent ( QCloseEvent *event );
	private slots:
		void updateInterface(std::vector<char> );

		void on_lineEdit_ro_vel_spec_returnPressed();
		void on_lineEdit_ro_acc_spec_returnPressed();

		void on_lineEdit_es_vel_spec_returnPressed();
		void on_lineEdit_es_acc_spec_returnPressed();
		
		void on_lineEdit_tr_vel_spec_returnPressed();
		void on_lineEdit_tr_acc_spec_returnPressed();

		void on_radioButton_ro_vel_fast_toggled ( bool on );
		void on_radioButton_ro_vel_mid_toggled ( bool on );
		void on_radioButton_ro_vel_slow_toggled ( bool on );
		void on_radioButton_ro_acc_default_toggled ( bool on );

		void on_radioButton_es_vel_fast_toggled ( bool on );
		void on_radioButton_es_vel_mid_toggled ( bool on );
		void on_radioButton_es_vel_slow_toggled ( bool on );
		void on_radioButton_es_acc_default_toggled ( bool on );
		
		void on_radioButton_tr_vel_fast_toggled ( bool on );
		void on_radioButton_tr_vel_mid_toggled ( bool on );
		void on_radioButton_tr_vel_slow_toggled ( bool on );
		void on_radioButton_tr_acc_default_toggled ( bool on );

		void on_pushButton_ro_F1_clicked();
		void on_pushButton_ro_F2_clicked();
		void on_pushButton_ro_F3_clicked();
		void on_pushButton_ro_F4_clicked();

		void on_pushButton_ro_go_clicked();
		void on_pushButton_es_go_clicked();
		void on_pushButton_tr_go_clicked();

		void on_pushButton_ro_stepM_clicked();
		void on_pushButton_ro_stepP_clicked();
		void on_pushButton_es_stepM_clicked();
		void on_pushButton_es_stepP_clicked();
		void on_pushButton_tr_stepM_clicked();
		void on_pushButton_tr_stepP_clicked();

		void on_pushButton_ro_stop_clicked();
		void on_pushButton_es_stop_clicked();
		void on_pushButton_tr_stop_clicked();

		void on_pushButton_tr_reset_clicked();
	signals:
		void galilInfoThrown(std::vector<char> );
};

#endif
