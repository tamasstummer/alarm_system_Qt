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
    bool CheckPort(QString);
    bool ConnectToTheDevice(QString);
    QString GetPassword(void);
    QString GetPortName(void);


public slots:
    void ReadSerialData();





private:
    const QString password = "1234";
    const int VendorID = 6790;
    const int ProductID = 29987;
    QString PortName;  // COM port name
    bool isAvailable;  // indicate if the COM port is available or not
    QSerialPort Communication;
    QVector<double> humidityAndTemperature;
    QString log;




};

#endif // ALARM_H
