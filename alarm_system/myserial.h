#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <alarm.h>

class mySerial: public QObject
{

    Q_OBJECT

public:
    mySerial(Alarm*);
    bool ConnectToTheDevice(const QString, Alarm*);
    bool CheckPort(QString, Alarm*);
    void SendData(QString command, Alarm*);


     QSerialPort *serialport;
     Alarm *alarm;


public slots:
    void serialSlot(void);


private:

};

#endif // MYSERIAL_H
