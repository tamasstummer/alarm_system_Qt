#include "myserial.h"
#include "mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>

mySerial::mySerial()
{
    this->serialport = new QSerialPort();
    QObject::connect(this->serialport, SIGNAL(readyRead()), this, SLOT(readSerial()));

}




bool mySerial::ConnectToTheDevice(const QString comport)
{

       bool isThereAnyError = false;

       this->serialport->setPortName(comport);
       if(! this->serialport->setBaudRate(QSerialPort::Baud9600))         {isThereAnyError = true;qDebug() << "error during setting baud";}
       if(! this->serialport->setDataBits(QSerialPort::Data8))            {isThereAnyError = true;qDebug() << "error during setting data7";}
       if(! this->serialport->setParity(QSerialPort::NoParity))           {isThereAnyError = true;qDebug() << "error during setting no parity";}
       if(! this->serialport->setStopBits(QSerialPort::OneStop))          {isThereAnyError = true;qDebug() << "error during setting opne stop";}
       if(! this->serialport->setFlowControl(QSerialPort::NoFlowControl)) {isThereAnyError = true;qDebug() << "error during setting non folw control";}
       if(! this->serialport->open(QIODevice::ReadWrite))                 {isThereAnyError = true;qDebug() << "error during open serial port";}


       if(this->serialport->isOpen() == true)
            {
           qDebug() << "comport open";
            }

       else
            {qDebug() << "comport closed";}

           if(isThereAnyError == true)
           {
               qDebug() << "there is no available device" << endl;
               return false;
           }
           else {return true;}


}

void mySerial::readSerial()
{
    qDebug() << "Incoming data: ";
    QByteArray serialData = this->serialport->readAll();
    QString data = QString::fromStdString(serialData.toStdString());
    qDebug() << data << endl;
    emit gotCommand(data);
    //serialport->flush();
}




bool mySerial::CheckPort(QString comport)
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

    return true;


}

void mySerial::SendData(const QString command)
{

        serialport ->write(command.toStdString().c_str());
        qDebug() << "Data sent";


}


void mySerial::sendStatusCommand()
{


    this->SendData("STATUS");
    this->serialport->waitForReadyRead(300);
}


