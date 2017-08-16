#include "plottersetting.h"
#include "ui_plottersetting.h"
#include "main_window.h"
#include "signs.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

plottersetting::plottersetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plottersetting)
{
    ui->setupUi(this);
}

plottersetting::~plottersetting()
{
    delete ui;
}

void plottersetting::readFlightCurves(Plotter *plotter, const QString &fileName)
{
    QVector<QPointF> data[6];
    double factX = 0.0013;
    double factY[6] = { 0.0008, 0.1, 0.2, 0.2, 0.1, 0.8 };
    double offsY[6] = { +500, -55, +309, +308, 0, 0 };
    int pos[6] = { 3, 6, 7, 8, 9, 10 };
    QFile file(fileName);
    double offsX = 0.0;

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            //qDebug() << line;
            QStringList coords = line.split(' ', QString::SkipEmptyParts);
            //qDebug() << coords.count();
            if (coords.count() >= 6) {
                double x = factX * coords[0].toDouble();
                if (data[0].isEmpty())
                    offsX = x;
                for (int i = 0; i < 6; ++i) {
                    double y = coords[pos[i]].toDouble();
                    data[i].append(QPointF(x - offsX,
                                           factY[i] * (y - offsY[i])));
                }
            }
        }
    }

    plotter->setCurveData(0, data[0]);
    plotter->setCurveData(1, data[1]);
    plotter->setCurveData(2, data[2]);
    plotter->setCurveData(3, data[3]);
    plotter->setCurveData(4, data[4]);
    plotter->setCurveData(5, data[5]);
}

void plottersetting::readMotionCurves(Plotter *plotter, const QString &fileName)
{
    QVector<QPointF> data[6];
    QFile file(fileName);
    double offsX = 0.0;
    double linecount=0;
    double ymax=0,ymin=0;

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            linecount++;
            QString line = in.readLine();
            qDebug() << line;
            QStringList coords = line.split(' ', QString::SkipEmptyParts);
            qDebug() << coords.count();
            if (coords.count() >= 3) {
                double x = coords[0].toDouble();
                if (data[0].isEmpty())
                    offsX = x;
                for (int i = 0; i < 2; ++i) {
                    double y = coords[i+1].toDouble();
                    data[i].append(QPointF(x-offsX, y));
                    //comparison
                    if(y >= ymax) ymax=y;
                    if(y <= ymin) ymin=y;
                }
            }
        }
    }
    if(ymax <= -ymin) ymax=(-ymin);
    if(ymin >= -ymax) ymin=(-ymax);
    //axis setting
    PlotSettings settings;
    settings.minX = 0.0;
    settings.maxX = linecount;
    settings.minY = ymin;//-5.0;
    settings.maxY = ymax;//5.0;
    plotter->setPlotSettings(settings);
    //data
    plotter->setCurveData(0, data[0]);
    plotter->setCurveData(1, data[1]);
    plotter->setCurveData(2, data[2]);
    plotter->setCurveData(3, data[3]);
    plotter->setCurveData(4, data[4]);
    plotter->setCurveData(5, data[5]);
}

void plottersetting::on_pushButton_selectMotionCurveDir_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select MotionCurve file to Access"), ".", tr("txt Files(*.txt)"));
    if(path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
        path="home/book/telescope20141227/data/20160905_1553curvedata.txt";
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
    }

    ui->lineEdit_MotionCurveDir->setText(path);
}

void plottersetting::on_pushButton_plot_clicked()
{
    plotter.setWindowTitle(QObject::tr("Jambi Plotter"));
    //When this test finished,this line should be erased.
    ui->lineEdit_MotionCurveDir->setText("/home/book/workspace/QtCode/telescope/data/20160905_1620curvedata.txt");

#if 0
    readFlightCurves(&plotter, ":/in1.txt");
    //    plotter.timer->start(1);
#else
    readMotionCurves(&plotter, ui->lineEdit_MotionCurveDir->text());
#endif
    plotter.show();
}
