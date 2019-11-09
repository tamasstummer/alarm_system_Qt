#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <alarm.h>
#include <QMessageBox>
#include <QPixmap>
#include "mytimer.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
            ui->comboBox_COM_Port->addItem(port.portName());
        }
        ui->comboBox_COM_Port->show();
        ui->groupBox_2->setStyleSheet("background-color: #ccdbdb");  // Valami fullos színt kéne állítani
        QPixmap pm("C:/Users/tamas/Desktop/alkalm_hazi/alkfejl2019-szszs/alarm_system/pictures/closed_padlock.png"); // <- path to image file
        ui->label_2->setPixmap(pm);
        ui->label_2->setScaledContents(true);


    //myAlarm.AvailablePorts();



}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_buttonPasswordOK_clicked()
{
    QString input_password_by_user = ui->text_Password->text();
    if(myAlarm.GetPassword() == input_password_by_user)
    {
        ui->comboBox_COM_Port->setEnabled(true);
        ui->buttonConnect->setEnabled(true);
        ui->groupBox->setEnabled(true);
        ui->buttonArmTheSystem->setEnabled(true);
        ui->groupBox_2->setStyleSheet("background-color: #abffad");
        QPixmap pm("C:/Users/tamas/Desktop/alkalm_hazi/alkfejl2019-szszs/alarm_system/pictures/open_padlock.png"); // <- path to image file
        ui->label_2->setPixmap(pm);
        ui->label_2->setScaledContents(true);





    }
    else
    {

        QMessageBox::critical(this, "Wrong password", "The given password is wrong");
        ui->text_Password->setText("");

    }



}

void MainWindow::on_buttonArmTheSystem_clicked()
{

    ui->comboBox_COM_Port->setEnabled(false);
    ui->buttonConnect->setEnabled(false);
    ui->groupBox->setEnabled(false);
    ui->buttonArmTheSystem->setEnabled(false);
    ui->groupBox_2->setStyleSheet("background-color: #ccdbdb");  // Valami fullos színt kéne állítani
    QPixmap pm("C:/Users/tamas/Desktop/alkalm_hazi/alkfejl2019-szszs/alarm_system/pictures/closed_padlock.png"); // <- path to image file
    ui->label_2->setPixmap(pm);
    ui->label_2->setScaledContents(true);
    ui->text_Password->setText("");




}

void MainWindow::on_buttonConnect_clicked()
{

    QString selectedComPort = ui->comboBox_COM_Port->currentText();
    qDebug() << "Selected COM Port: " << selectedComPort << endl;
    bool isAvailable = myAlarm.CheckPort(selectedComPort);
    if(isAvailable == true)
    {

        bool status = myAlarm.ConnectToTheDevice(myAlarm.GetPortName());
        if(status == true)
        {

           mTimer.startTimer(1000);
           qDebug() << "start the fricken timer" << endl;

        }
    }
}
