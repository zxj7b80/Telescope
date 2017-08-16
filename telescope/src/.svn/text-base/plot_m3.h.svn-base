//
// C++ Interface: plot_m3
//
// Description: 
//
//
// Author: donglongchao <donglongchao@163.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PLOT_M3_H
#define PLOT_M3_H
#include <qwt_plot.h>
#include "ui_plot_m3.h"
#define HISTORY 60
class QwtPlot;
class QwtPlotCurve;
class DirectM3Thread;
class PlotM3:public QwtPlot,private Ui::PlotM3
{
	Q_OBJECT
	public:
		enum
		{
			Vel,
			Pos,
			Num
		};
		PlotM3(DirectM3Thread *);
		~PlotM3();
	public slots:
		void showCurve(std::vector<char>);
	private:
		struct
		{
			QwtPlotCurve *curve;
			double data[HISTORY];
		} data[Num];
		int dataCount;
		DirectM3Thread *m_parent;
		double timeData[HISTORY];
};
#endif