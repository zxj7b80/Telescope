//
// C++ Implementation: plot_m3
//
// Description: 
//
//
// Author: donglongchao <donglongchao@163.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <qapplication.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include "plot_m3.h"
#include "direct_m3.h"
#include "Galil.h"
//#include "signs.h"
#include <tr1/memory> //for shared_ptr
#include <memory> //for auto_ptr
#include <tr1/cstdint> //for int32_t,if you want to use cstdint directly,you must add -std=c++0x as a optionnfor gcc.

class CpuCurve: public QwtPlotCurve
{
public:
    CpuCurve(const QString &title):
        QwtPlotCurve(title)
    {
#if QT_VERSION >= 0x040000
        setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    }

    void setColor(const QColor &color)
    {
#if QT_VERSION >= 0x040000
        QColor c = color;
        c.setAlpha(150);

        setPen(c);
        setBrush(c);
#else
        setPen(color);
        setBrush(QBrush(color, Qt::Dense4Pattern));
#endif
    }
};

PlotM3::PlotM3(DirectM3Thread *parent):dataCount(0)
{
//	setAttribute(Qt::WA_DeleteOnClose);//destory this object when close the window.
	m_parent = parent;
	setAutoReplot(false);
	plotLayout()->setAlignCanvasToScales(true);
	setAxisTitle(QwtPlot::xBottom, " Time");
//	setAxisScaleDraw(QwtPlot::xBottom,new QwtScaleDraw(QTime::currentTime()));
	setAxisScale(QwtPlot::xBottom, 0, HISTORY);
	setAxisLabelRotation(QwtPlot::xBottom, -50.0);
	setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);
	QwtScaleWidget *scaleWidget = axisWidget(QwtPlot::xBottom);
    const int fmh = QFontMetrics(scaleWidget->font()).height();
    scaleWidget->setMinBorderDist(0, 2*fmh);
	setAxisTitle(QwtPlot::yLeft, "Pos");
	setAxisTitle(QwtPlot::yRight, "Vel");
    setAxisScale(QwtPlot::yLeft, -50, 50);
setAxisScale(QwtPlot::yRight, 0, 10);
CpuCurve *curve;

    curve = new CpuCurve("vel");
    curve->setColor(Qt::red);
    curve->attach(this);
    data[Vel].curve = curve;

    curve = new CpuCurve("pos");
    curve->setColor(Qt::blue);
    curve->setZ(curve->z() - 1);
    curve->attach(this);
    data[Pos].curve = curve;
	
	data[Vel].curve->setVisible(true);
	data[Pos].curve->setVisible(true);
	replot();
	
for ( int i = 0; i < HISTORY; i++ )
        timeData[HISTORY - 1 - i] = i;


//	std::vector<std::tr1::shared_ptr<Galil> > vec;
//	std::tr1::shared_ptr<Galil> po(new Galil);
//	vec.push_back(po);
}
PlotM3::~PlotM3()
{
	qDebug("plotm3");
	delete data[Vel].curve;
	delete data[Pos].curve;
}

void PlotM3::showCurve(std::vector<char> g)
{
	for ( int i = dataCount; i > 0; i-- )
    {
        for ( int c = 0; c < Num; c++ )
        {
            if ( i < HISTORY )
                data[c].data[i] = data[c].data[i-1];
        }
    }
//	data[Vel].data[0]=abs(m_parent->galil->sourceValue(g,"_TVA"));
//	data[Pos].data[0]=(m_parent->galil->sourceValue(g,"_TPA"))/5.0;
	if ( dataCount < HISTORY )
        dataCount++;
	setAxisScale(QwtPlot::xBottom, 
        timeData[HISTORY - 1], timeData[0]);
	for ( int c = 0; c < Num; c++ )
    {
        data[c].curve->setRawData(
            timeData, data[c].data, dataCount);
    }
//	Singleton<test>::instance()->run();
    replot();
}