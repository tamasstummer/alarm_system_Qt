#include "alarm.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "mytimer.h"



Alarm::Alarm()
{


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

void Alarm::SetHumidityAndTemperature(int temperature, int humidity)
{
    this->humidityAndTemperature[0] = humidity;
    this->humidityAndTemperature[1] = temperature;
}

int Alarm::GetHumidity()
{

    return this->humidityAndTemperature[0];

}


int Alarm::GetTemperature()
{

    return this->humidityAndTemperature[1];
}




QString Alarm::GetPassword()
{
    return this->password;
}

QString Alarm::GetPortName()
{
    return this->PortName;

}
