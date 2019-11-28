#include "alarm.h"
#include <QDebug>
#include <QVector>
#include "ui_mainwindow.h"
#include "mainwindow.h"


/**
 * @brief Alarm::Alarm
 */
Alarm::Alarm()
{
    this->selfTestResult = true;
}


/**
 * @brief Set the used COM port's name
 * @param name - COM port's name
 */
void Alarm::SetPortName(QString name)
{
    this->PortName = name;
}

/**
 * @brief Get the used COM port's name
 * @return
 */
QString Alarm::GetPortName()
{
    return this->PortName;
}

/**
 * @brief Set the alarm availability
 * @param status
 */
void Alarm::SetIsAvailable(bool status)
{
    this->isAvailable = status;
}
/**
 * @brief Get the alarm availability
 * @return
 */
bool Alarm::GetIsAvailable(void)
{
    return this->isAvailable;
}

/**
 * @brief Store the temperature and humidity
 * @param datas
 */
void Alarm::SetHumidityAndTemperature(QVector<int> datas)
{
    this->humidityAndTemperature = datas;
}

/**
 * @brief Get the temperature and humidity
 * @return
 */
QVector<int> Alarm::GetHumidityAndTemperature()
{
    return this->humidityAndTemperature;
}


/**
 * @brief Get the alarm password
 * @return
 */
QString Alarm::GetPassword()
{
    return this->password;
}


/**
 * @brief Set the alarm's log
 * @param log
 */
void Alarm::SetLog(QString log)
{
    this->log = log;
}

/**
 * @brief Get the alarm's log
 * @return
 */
QString Alarm::GetLog(void)
{
    return this->log;
}
/**
 * @brief Store the battery capacity
 * @param value
 */
void Alarm::SetBattery(int value)
{
    this->battery = value;
}

/**
 * @brief Get the battery capacity
 * @return
 */
int Alarm::GetBattery(void)
{
    return this->battery;
}

/**
 * @brief Store the relay status
 * @param status
 */
void Alarm::SetRelay(bool status)
{
    this->relay = status;
}

/**
 * @brief Get the relay status
 * @return
 */
bool Alarm::GetRelay(void)
{
    return this->relay;
}






