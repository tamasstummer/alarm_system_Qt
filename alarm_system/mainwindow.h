#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <alarm.h>
#include "mytimer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MyTimer mTimer;


private slots:


    void on_buttonPasswordOK_clicked();

    void on_buttonArmTheSystem_clicked();

    void on_buttonConnect_clicked();

private:
    Ui::MainWindow *ui;
    Alarm myAlarm;

};
#endif // MAINWINDOW_H
