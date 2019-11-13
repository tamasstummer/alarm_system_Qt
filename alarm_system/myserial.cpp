#include "myserial.h"

#include <alarm.h>
#include "mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtDebug>
#include <QMessageBox>

mySerial::mySerial(Alarm* gettedAlarm)
{
    this->alarm = gettedAlarm;
    serialport = new QSerialPort(this);
    connect(serialport, SIGNAL(readyRead()), this, SLOT(serialSlot()) );

}

void mySerial::serialSlot()
{
    qDebug() << "Incoming data" << endl;
    QByteArray serialData = serialport->readAll();
    QString data = QString::fromStdString(serialData.toStdString());
    qDebug() << data << endl;
    serialport->flush();
}



bool mySerial::ConnectToTheDevice(const QString comport, Alarm* alarm)
{
    if(alarm->GetIsAvailable() == true)
    {
       bool isThereAnyError = false;

       serialport->setPortName(comport);
       if(! serialport->setBaudRate(QSerialPort::Baud9600))         {isThereAnyError = true;qDebug() << "error during setting baud";}
       if(! serialport->setDataBits(QSerialPort::Data8))            {isThereAnyError = true;qDebug() << "error during setting data7";}
       if(! serialport->setParity(QSerialPort::NoParity))           {isThereAnyError = true;qDebug() << "error during setting no parity";}
       if(! serialport->setStopBits(QSerialPort::OneStop))          {isThereAnyError = true;qDebug() << "error during setting opne stop";}
       if(! serialport->setFlowControl(QSerialPort::NoFlowControl)) {isThereAnyError = true;qDebug() << "error during setting non folw control";}
       if(! serialport->open(QIODevice::ReadWrite))                 {isThereAnyError = true;qDebug() << "error during open serial port";}


       if(serialport->isOpen() == true)
            {qDebug() << "comport open";}

       else
            {qDebug() << "comport closed";}

           if(isThereAnyError == true)
           {
               return false;
           }
           else {return true;}
    }
    else
    {
        qDebug() << "there is no available device" << endl;
        return false;

    }

}

bool mySerial::CheckPort(QString comport, Alarm * alarm)
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
                if(SerialPortInfo.vendorIdentifier() == alarm->VendorID && SerialPortInfo.productIdentifier() == alarm->ProductID)
                {
                    qDebug() << "Vendor ID: " << SerialPortInfo.vendorIdentifier() << endl << "Product ID: " << SerialPortInfo.productIdentifier() << endl << "COM Port: " << comport << endl;
                    alarm->SetPortName(comport);
                    alarm->SetIsAvailable(true);

                    return true;

                }

            }

        }
        return false;

    }

}

void mySerial::SendData(const QString command, Alarm* alarm)
{
    if(alarm->GetIsAvailable() == true)
    {
        if(serialport ->write(command.toStdString().c_str()) < 0)
        {
            qDebug() << serialport->error() << endl;
            qDebug() << "error string = " << serialport->errorString();

        }




    }
    else
    {
        qDebug() << "Error during sending data" << endl;

    }
}





