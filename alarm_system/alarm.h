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
    void SetHumidityAndTemperature(QVector<int>);
    QVector<int> GetHumidityAndTemperature();
    void SetLog(QString);
    QString GetLog(void);
    void SetBattery(int);
    int GetBattery(void);
    void SetRelay(bool);
    bool GetRelay(void);

signals:
    void updatedDataFormSerial();

private:
    const QString password = "1234";
    QString PortName;  // COM port name
    bool isAvailable;  // indicate if the COM port is available or not
    int battery;
    bool relay;
    QVector<int> humidityAndTemperature;
    QString log;

};

#endif // ALARM_H
