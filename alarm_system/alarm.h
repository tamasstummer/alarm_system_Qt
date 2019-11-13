#ifndef ALARM_H
#define ALARM_H

#include <QMainWindow>
#include <QSerialPort>





class Alarm: public QObject
{

    Q_OBJECT

public:
    Alarm();
    ~Alarm();

    const int VendorID = 6790;
    const int ProductID = 29987;


    QString GetPassword(void);
    QString GetPortName(void);
    bool GetIsAvailable(void);
    void SetPortName(QString);
    void SetIsAvailable(bool);
    const QString commands[2] = {"ALARM0","ALARM1"};    // ALARM0 disarm the system, ALARM1 arm the system
    int commandNum;

private:
    const QString password = "1234";
    QString PortName;  // COM port name
    bool isAvailable;  // indicate if the COM port is available or not
    QVector<double> humidityAndTemperature;
    QString log;






};

#endif // ALARM_H
