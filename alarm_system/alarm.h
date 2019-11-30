#ifndef ALARM_H
#define ALARM_H

#include <QMainWindow>
#include <QSerialPort>


class Alarm: public QObject
{

    Q_OBJECT

public:
    Alarm();


    QString GetPassword(void);
    bool GetIsAvailable(void);
    void SetIsAvailable(bool);
    void SetHumidityAndTemperature(QVector<int>);
    QVector<int> GetHumidityAndTemperature();
    void SetLog(QString);
    QString GetLog(void);
    void SetBattery(int);
    int GetBattery(void);
    void SetRelay(bool);
    bool GetRelay(void);
    bool selfTestResult;

signals:
    void updatedDataFormSerial();

private:
    const QString password = "1234";
    bool isAvailable;
    int battery;
    bool relay;
    QVector<int> humidityAndTemperature;
    QString log;



};

#endif // ALARM_H
