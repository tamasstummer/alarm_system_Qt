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
this->commandNum = 0;

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




QString Alarm::GetPassword()
{
    return this->password;
}

QString Alarm::GetPortName()
{
    return this->PortName;

}
