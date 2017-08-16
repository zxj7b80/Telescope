#ifndef PLOTTERSETTING_H
#define PLOTTERSETTING_H

#include <QDialog>
#include "plotter.h"

namespace Ui {
    class plottersetting;
}

class plottersetting : public QDialog
{
    Q_OBJECT

public:
    explicit plottersetting(QWidget *parent = 0);
    ~plottersetting();

    //by zxj-----
    Plotter plotter;
    void readFlightCurves(Plotter *plotter, const QString &fileName);
    void readMotionCurves(Plotter *plotter, const QString &fileName);

private slots:
    void on_pushButton_plot_clicked();

    void on_pushButton_selectMotionCurveDir_clicked();

private:
    Ui::plottersetting *ui;
};

#endif // PLOTTERSETTING_H
