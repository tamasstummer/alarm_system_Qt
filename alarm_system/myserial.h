#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <QSerialPort>
#include <QSerialPortInfo>


class mySerial: public QObject
{

    Q_OBJECT

public:
    mySerial();
    bool ConnectToTheDevice(const QString);
    bool CheckPort(QString);
    void SendData(QString);

     QSerialPort *serialport;


signals:
    void gotCommand(QString command);

private slots:
    void readSerial();
    void sendStatusCommand();


};

#endif // MYSERIAL_H
