#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <alarm.h>
#include "mytimer.h"
#include "myserial.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sendDisarmCommand();
    void sendArmCommand();
    bool developerOption;

   Alarm myAlarm;
   MyTimer *mTimer = new MyTimer();
   mySerial *mSerial = new mySerial();
   MyTimer *developerTimer = new MyTimer();



private slots:

   void processSerialData(QString );

   void updateWindowAfterSAtatusChanged();

   void fillMyAlarmForDevelopment();

    void on_buttonPasswordOK_clicked();

    void on_buttonArmTheSystem_clicked();

    void on_buttonConnect_clicked();


    void on_celarContentBtn_clicked();

private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H


