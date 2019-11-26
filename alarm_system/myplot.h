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
    QVector<double> y;
    MyPlot() : x(10),y(10){}

public slots:
    void PlotSlot();



};
#endif // MYPLOT_H
