#ifndef DIRECT_M2_H
#define DIRECT_M2_H
#include <QtGui>
#include "ui_direct_m2.h"
//class PlotM2;
class MainWindow;
class DirectM2:public QScrollArea,private Ui::DirectM2
{
		Q_OBJECT
	public:
		DirectM2 (MainWindow * parent);
		~DirectM2();

	private:
		void connectDevice();
		void initUi();
		MainWindow *m_parent;
		double tz_target;
		double tz_vel;
		double tz_acc;
		double tz_step;
		double xc_vel;
		double xc_acc;
		double xc_step;
		double yc_vel;
		double yc_acc;
		double yc_step;
	protected:
		virtual void closeEvent ( QCloseEvent *event );
	private slots:
		void updateInterface(std::vector<char> );

		void on_lineEdit_tz_vel_spec_returnPressed();
		void on_lineEdit_tz_acc_spec_returnPressed();

		void on_lineEdit_alpha1_vel_spec_returnPressed();
		void on_lineEdit_alpha1_acc_spec_returnPressed();
		
		void on_lineEdit_alpha2_vel_spec_returnPressed();
		void on_lineEdit_alpha2_acc_spec_returnPressed();

		void on_radioButton_tz_vel_fast_toggled ( bool on );
		void on_radioButton_tz_vel_mid_toggled ( bool on );
		void on_radioButton_tz_vel_slow_toggled ( bool on );
		void on_radioButton_tz_acc_default_toggled ( bool on );

		void on_radioButton_alpha1_vel_fast_toggled ( bool on );
		void on_radioButton_alpha1_vel_mid_toggled ( bool on );
		void on_radioButton_alpha1_vel_slow_toggled ( bool on );
		void on_radioButton_alpha1_acc_default_toggled ( bool on );
		
		void on_radioButton_alpha2_vel_fast_toggled ( bool on );
		void on_radioButton_alpha2_vel_mid_toggled ( bool on );
		void on_radioButton_alpha2_vel_slow_toggled ( bool on );
		void on_radioButton_alpha2_acc_default_toggled ( bool on );

		void on_pushButton_tz_go_clicked();
		void on_pushButton_alpha1_go_clicked();
		void on_pushButton_alpha2_go_clicked();

		void on_pushButton_tz_stepM_clicked();
		void on_pushButton_tz_stepP_clicked();
		void on_pushButton_alpha1_stepM_clicked();
		void on_pushButton_alpha1_stepP_clicked();
		void on_pushButton_alpha2_stepM_clicked();
		void on_pushButton_alpha2_stepP_clicked();

		void on_pushButton_tz_stop_clicked();
		void on_pushButton_alpha1_stop_clicked();
		void on_pushButton_alpha2_stop_clicked();

		void on_pushButton_alpha2_reset_clicked();
};

#endif
