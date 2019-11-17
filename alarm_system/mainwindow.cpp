#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <alarm.h>
#include <QMessageBox>
#include <QPixmap>
#include "mytimer.h"
#include "myserial.h"


#define time_interval_ms 3000

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(mSerial, SIGNAL(gotCommand(QString)), this, SLOT(processSerialData(QString)));
    connect(mTimer->timer, SIGNAL(timeout()), mSerial, SLOT(sendStatusCommand()));
    connect(&myAlarm, SIGNAL(updatedDataFormSerial()), this, SLOT(updateWindowAfterSAtatusChanged()));

    this->developerOption = ui->isDeveloper->isChecked();

    //List the available serial COM ports
    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
        ui->comboBox_COM_Port->addItem(port.portName());
    }

        ui->comboBox_COM_Port->show();
        ui->groupBox_2->setStyleSheet("background-color: #ccdbdb");  // Valami fullos színt kéne állítani
        QPixmap pm("C:/Users/tamas/Desktop/alkalm_hazi/alkfejl2019-szszs/alarm_system/pictures/closed_padlock.png"); // <- path to image file
        ui->label_2->setPixmap(pm);
        ui->label_2->setScaledContents(true);
}

void MainWindow::updateWindowAfterSAtatusChanged()
{

    ui->logWindow->moveCursor(QTextCursor::End);
    ui->logWindow->append(myAlarm.GetLog());
    ui->logWindow->moveCursor(QTextCursor::End);
    ui->lcdBatteryStatus->display(myAlarm.GetBattery());
    QVector<int> humidAndTemp = myAlarm.GetHumidityAndTemperature();
    ui->lcdTempStatus->display(humidAndTemp[1]);
    ui->lcdHmidStatus->display(humidAndTemp[0]);
}

void MainWindow::processSerialData(QString command)
{

    int batteryValue;
    int humidityValue;
    int temperatureValue;
    int relayValue;
    QVector<int> humidAndTemp;
    QString logMessage;


        qDebug() << command <<endl;
        if(command.startsWith("B"))
        {
            unsigned char c_command[15];
            memcpy( c_command, command.toStdString().c_str() ,command.size());
            c_command[10] = 0;
            batteryValue = (c_command[1] - '0');
            humidityValue = (c_command[6] - '0')*10 + (c_command[7] - '0');
            temperatureValue = (c_command[3] - '0')*10 + (c_command[4] - '0');
            relayValue = ((c_command[9] - '0') == 0) ? false : true;
            humidAndTemp = {humidityValue, temperatureValue};
            logMessage.append("Battery: ");
            logMessage.append(QString::number(batteryValue*10));
            logMessage.append("%  Humidity: ");
            logMessage.append(QString::number(humidityValue));
            logMessage.append("%  Temp: ");
            logMessage.append(QString::number(temperatureValue));
            logMessage.append("C  Relay: ");
            logMessage.append(QString::number(relayValue));
            myAlarm.SetHumidityAndTemperature(humidAndTemp);
            myAlarm.SetBattery(batteryValue);
            myAlarm.SetRelay(relayValue);
            myAlarm.SetLog(logMessage);
            emit myAlarm.updatedDataFormSerial();
        }


}




MainWindow::~MainWindow()
{
    delete ui;
    mSerial->serialport->close();

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
        ui->statusWindow->setEnabled(true);

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
    mTimer->stopTimer();
    sendArmCommand();
    mSerial->serialport->close();




}

void MainWindow::on_buttonConnect_clicked()
{

    QString selectedComPort = ui->comboBox_COM_Port->currentText();
    qDebug() << "Selected COM Port: " << selectedComPort << endl;
    bool isAvailable =mSerial->CheckPort(selectedComPort);
    if(isAvailable == true)
    {

        bool status = mSerial->ConnectToTheDevice(selectedComPort);
        if(status == true)
        {   ui->label_5->setText(selectedComPort);
            ui->label_6->setText(QString::number(mSerial->serialport->baudRate()));
            if(developerOption){ui->label_12->setText("Developer");}
            else{ui->label_12->setText("User");}
            sendDisarmCommand();

           mTimer->startTimer(time_interval_ms);
           qDebug() << "start the timer" << endl;

        }
    }
}




void MainWindow::sendDisarmCommand()
{
    mSerial->SendData("DISARM");
    mSerial->serialport->waitForReadyRead(2000);
}

void MainWindow::sendArmCommand()
{
     mSerial->SendData("ARM");
     mSerial->serialport->waitForReadyRead(2000);

}

