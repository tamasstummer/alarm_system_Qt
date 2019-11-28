#include "alarm.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include <QVector>
#include <iostream>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "mytimer.h"



Alarm::Alarm()
{

    this->selfTestResult = true;
}


Alarm::~Alarm()
{


}


void Alarm::SetPortName(QString name)
{
    this->PortName = name;
}


void Alarm::SetIsAvailable(bool status)
{
    this->isAvailable = status;

}
bool Alarm::GetIsAvailable(void)
{
    return this->isAvailable;

}

void Alarm::SetHumidityAndTemperature(QVector<int> datas)
{
    this->humidityAndTemperature = datas;
}

QVector<int> Alarm::GetHumidityAndTemperature()
{

    return this->humidityAndTemperature;

}






QString Alarm::GetPassword()
{
    return this->password;
}

QString Alarm::GetPortName()
{
    return this->PortName;

}

void Alarm::SetLog(QString log)
{
    this->log = log;

}
QString Alarm::GetLog(void)
{

    return this->log;

}
void Alarm::SetBattery(int value)
{
    this->battery = value;

}
int Alarm::GetBattery(void)
{
    return this->battery;
}
void Alarm::SetRelay(bool status)
{
    this->relay = status;

}
bool Alarm::GetRelay(void)
{

    return this->relay;

}






