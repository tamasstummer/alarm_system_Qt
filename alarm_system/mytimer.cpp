#include "mytimer.h"


/**
 * @brief MyTimer::MyTimer
 */
MyTimer::MyTimer()
{
    timer = new QTimer(this);
}

/**
 * @brief Start the timer with the given value
 * @param milisec
 */
void MyTimer::startTimer(int milisec)
{
    timer->start(milisec);
}

/**
 * @brief Stop the timer
 */
void MyTimer::stopTimer(void)
{
    timer->stop();

}

