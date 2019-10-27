#ifndef ALARM_H
#define ALARM_H

#include <QMainWindow>
#include <QSerialPort>



class Alarm{

public:
    Alarm();
    ~Alarm();
    void AvailablePorts(void);
    void ConnectToTheDevice(void);


private:
    static const int password = 1234;
    const int VendorID = 6790;
    const int ProductID = 29987;
    QString PortName;
    bool isAvailable;
    QSerialPort Communication;


};

#endif // ALARM_H
