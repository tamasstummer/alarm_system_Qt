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
        return false;

    }

}


bool Alarm::ConnectToTheDevice(QString comport)
{
    if(this->isAvailable == true)
    {
       bool isThereAnyError = false;

       this->Communication.setPortName(comport);
       if(! this->Communication.open(QSerialPort::ReadWrite))               {isThereAnyError = true; qDebug() << " eror during open serial port";}
       if(! this->Communication.setBaudRate(QSerialPort::Baud9600))         {isThereAnyError = true;qDebug() << "error during setting baud";}
       if(! this->Communication.setDataBits(QSerialPort::Data7))            {isThereAnyError = true;qDebug() << "error during setting data7";}
       if(! this->Communication.setParity(QSerialPort::NoParity))           {isThereAnyError = true;qDebug() << "error during setting no parity";}
       if(! this->Communication.setStopBits(QSerialPort::OneStop))          {isThereAnyError = true;qDebug() << "error during setting opne stop";}
       if(! this->Communication.setFlowControl(QSerialPort::NoFlowControl)) {isThereAnyError = true;qDebug() << "error during setting non folw control";}
           if(isThereAnyError == true)
           {
               return false;
           }
           else {return true;}
    }
    else
    {
        qDebug() << "there is no available device" << endl;

    }

}



void Alarm::ReadSerialData(void)
{


QByteArray serialData = this->Communication.readLine();
QString data = QString::fromStdString(serialData.toStdString());
qDebug() << data << endl;



}



QString Alarm::GetPassword()
{
    return this->password;
}

QString Alarm::GetPortName()
{
    return this->PortName;

}
