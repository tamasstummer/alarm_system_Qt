#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <alarm.h>
#include "mytimer.h"
#include "myserial.h"
#include "qcustomplot.h"
#include "myplot.h"

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
    bool SelfTestOption;
    int progressBarActualValue;

   Alarm myAlarm;
   MyTimer *mTimer = new MyTimer();
   mySerial *mSerial = new mySerial();
   MyTimer *developerTimer = new MyTimer();
   MyTimer *selfTestTimer = new MyTimer();

   MyTimer *sliceTimer = new MyTimer();
   MyTimer *plotTimer = new MyTimer();
   MyPlot *plotBattData = new MyPlot();
   MyPlot *plotTempData = new MyPlot();
   MyPlot *plotHumidData = new MyPlot();



private slots:

   void processSerialData(QString );

   void updateWindowAfterSAtatusChanged();

   void fillMyAlarmForDevelopment();

   void IncreaseProgressBar();

   void on_buttonPasswordOK_clicked();

   void on_buttonArmTheSystem_clicked();

   void on_buttonConnect_clicked();

   void on_celarContentBtn_clicked();

   void clearPlotData();

   void updatePlotData();

   void gatherPlotData();

   void on_buttonBattPlot_clicked();

   //void on_buttonAlarmPlot_clicked(); //not implemented yet

   //void on_buttonTempPlot_clicked(); //not implemented yet


private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H


