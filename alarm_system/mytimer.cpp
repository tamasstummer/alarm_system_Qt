#include "mytimer.h"

#include <QtCore>
#include <QDebug>

MyTimer::MyTimer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot()));
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

 qDebug() << "timer works" << endl;



}
