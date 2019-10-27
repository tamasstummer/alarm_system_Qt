#include "alarm.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtWidgets>


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



void Alarm::AvailablePorts(void)
{

    int NumberOfAvailablePorts = QSerialPortInfo::availablePorts().length();
    if(NumberOfAvailablePorts == 0)
    {
        qDebug() << "There are no available ports!" << endl;
        QMessageBox msgBox;
        msgBox.setText("There are no available ports!");
        msgBox.exec();

    }

    else
    {
        qDebug() << "The number of available ports: " << NumberOfAvailablePorts << endl;
        foreach (const QSerialPortInfo &SerialPortInfo, QSerialPortInfo::availablePorts())
        {
            if(SerialPortInfo.hasVendorIdentifier() == true && SerialPortInfo.hasProductIdentifier() == true)
            {
                if(SerialPortInfo.vendorIdentifier() == this->VendorID && SerialPortInfo.productIdentifier() == this->ProductID)
                {
                    qDebug() << "Vendor ID: " << SerialPortInfo.vendorIdentifier() << endl << "Product ID: " << SerialPortInfo.productIdentifier() << endl;
                    this->PortName = SerialPortInfo.portName();
                    this->isAvailable = true;

                }

            }

        }

    }



}


void Alarm::ConnectToTheDevice()
{
    if(this->isAvailable == true)
    {
        this->Communication.setPortName(this->PortName);
        this->Communication.open(QSerialPort::ReadWrite);
        this->Communication.setBaudRate(QSerialPort::Baud9600);
        this->Communication.setDataBits(QSerialPort::Data7);
        this->Communication.setParity(QSerialPort::NoParity);
        this->Communication.setStopBits(QSerialPort::OneStop);
        this->Communication.setFlowControl(QSerialPort::NoFlowControl);
    }

}
