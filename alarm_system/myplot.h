#ifndef MYPLOT_H
#define MYPLOT_H
#include <QtCore>
#include <QMessageBox>
#include <QPixmap>
#include <iostream>
#include <cstdlib>

class MyPlot: public QObject
{
    Q_OBJECT

public:
    QVector<double> x;
    QVector<double> y_batt;
    QVector<double> y_humid;
    QVector<double> y_temp;
    int index = 0;
    int time = 0;
    MyPlot() : x(31), y_batt(61), y_humid(11), y_temp(11) {}

public slots:
    void PlotSlot();

signals:
    void updatePlotData();
    void gatherPlotData();
    void clearPlotData();

};
#endif // MYPLOT_H
