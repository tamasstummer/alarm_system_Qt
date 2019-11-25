#ifndef MYPLOT_H
#define MYPLOT_H
#include <QtCore>

class MyPlot: public QObject
{
    Q_OBJECT

public:
    MyPlot();
    double x;
    double y;

public slots:
    void PlotSlot();



};
#endif // MYPLOT_H
