#ifndef MYTIMER_H
#define MYTIMER_H

#include <QtCore>




class MyTimer: public QObject
{
    Q_OBJECT

public:
    MyTimer();
    QTimer *timer;
    void startTimer(int);
    void stopTimer(void);


public slots:
    void TimerSlot();



};

#endif // MYTIMER_H
