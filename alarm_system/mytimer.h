#ifndef MYTIMER_H
#define MYTIMER_H

#include <QtCore>
#include "alarm.h"




class MyTimer: public QObject
{
    Q_OBJECT

public:
    MyTimer(Alarm*);
    QTimer *timer;
    Alarm *alarm;
    void startTimer(int);
    void stopTimer(void);


public slots:
    void TimerSlot();



};

#endif // MYTIMER_H
