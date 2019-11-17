#include "mytimer.h"
#include "mainwindow.h"
#include "alarm.h"

#include <QtCore>
#include <QDebug>




MyTimer::MyTimer()
{

    timer = new QTimer(this);

}

void MyTimer::startTimer(int milisec)
{
    timer->start(milisec);
}


void MyTimer::stopTimer(void)
{
    timer->stop();

}

void MyTimer::TimerSlot()
{



}
