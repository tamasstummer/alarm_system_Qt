#include "mytimer.h"
#include "mainwindow.h"
#include "alarm.h"

#include <QtCore>
#include <QDebug>




MyTimer::MyTimer(Alarm* gettedAlarm)
{

    this->alarm = gettedAlarm;
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
   QString command = this->alarm->commands[this->alarm->commandNum];
   MainWindow().mSerial->SendData(command, this->alarm);

}
