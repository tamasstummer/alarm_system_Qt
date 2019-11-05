#include "alarm.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>
#include "ui_mainwindow.h"
#include "mainwindow.h"


Alarm::Alarm()
{


}


Alarm::~Alarm()
{
if(this->Communication.isOpen())
    {
        this->Communication.close();

    }

}





bool Alarm::CheckPort(QString comport)
{


    int NumberOfAvailablePorts = QSerialPortInfo::availablePorts().length();
    if(NumberOfAvailablePorts == 0)
    {
        qDebug() << "There are no available ports!" << endl;
        QMessageBox msgBox;
        msgBox.setText("There are no available ports!");
        msgBox.exec();
        return false;

    }

    else
    {
        qDebug() << "The number of available ports: " << NumberOfAvailablePorts << endl;
        foreach (const QSerialPortInfo &SerialPortInfo, QSerialPortInfo::availablePorts())
        {
            if(SerialPortInfo.hasVendorIdentifier() == true && SerialPortInfo.hasProductIdentifier() == true && SerialPortInfo.portName() == comport)
            {
                if(SerialPortInfo.vendorIdentifier() == this->VendorID && SerialPortInfo.productIdentifier() == this->ProductID)
                {
                    qDebug() << "Vendor ID: " << SerialPortInfo.vendorIdentifier() << endl << "Product ID: " << SerialPortInfo.productIdentifier() << endl << "COM Port: " << comport << endl;
                    this->PortName = comport;
                    this->isAvailable = true;
                    return true;

                }

            }

        }

    }

}


void Alarm::ConnectToTheDevice(QString comport)
{
    if(this->isAvailable == true)
    {
        this->Communication.setPortName(comport);
        this->Communication.open(QSerialPort::ReadWrite);
        this->Communication.setBaudRate(QSerialPort::Baud9600);
        this->Communication.setDataBits(QSerialPort::Data7);
        this->Communication.setParity(QSerialPort::NoParity);
        this->Communication.setStopBits(QSerialPort::OneStop);
        this->Communication.setFlowControl(QSerialPort::NoFlowControl);
    }

}

QString Alarm::GetPassword()
{
    return this->password;
}

QString Alarm::GetPortName()
{
    return this->PortName;

}
