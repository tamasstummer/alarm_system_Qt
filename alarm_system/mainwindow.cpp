#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <alarm.h>
#include <QMessageBox>
#include <QPixmap>
#include <iostream>
#include <cstdlib>
#include "mytimer.h"
#include "myserial.h"
#include "myplot.h"


#define time_interval_ms 1000
#define time_interval_plot_ms 1000
#define time_slice_plot_ms 100


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(mSerial, SIGNAL(gotCommand(QString)), this, SLOT(processSerialData(QString)));
    connect(mTimer->timer, SIGNAL(timeout()), mSerial, SLOT(sendStatusCommand()));
    connect(&myAlarm, SIGNAL(updatedDataFormSerial()), this, SLOT(updateWindowAfterSAtatusChanged()));
    connect(selfTestTimer->timer, SIGNAL(timeout()),  this, SLOT(IncreaseProgressBar()));

    progressBarActualValue = 0;

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
        ui->progressBar->setEnabled(false);
}


void MainWindow::fillMyAlarmForDevelopment()
{

    myAlarm.SetBattery((std::rand()%10)*10);
    QVector<int> datas = {(std::rand()%50),(std::rand()%50)};
    myAlarm.SetHumidityAndTemperature(datas);
    emit myAlarm.updatedDataFormSerial();
}

void MainWindow::IncreaseProgressBar()
{
if(progressBarActualValue <= 100)
    ui->progressBar->setValue(progressBarActualValue++);
else
    {
    this->selfTestTimer->stopTimer();
    mSerial->serialport->waitForReadyRead(1000);

    }

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

//Plot functions
void MainWindow::updatePlotData()
{
    QVector<double> x(10);
    double y;
    for(int i=0; i < 10; i++)
    {
        x[i] = i;
    }

    for(int i=0; i < 10; i++)
    {
        y = myAlarm.GetBattery();
        ui->customPlot->graph(0)->addData(x[i], y);
        ui->customPlot->update();
    }
    ui->customPlot->replot();
    //ui->customPlot->graph(0)->setData(x, y);

}

void MainWindow::gatherPlotData()
{
    QVector<double> y(10);
    y[0] = myAlarm.GetBattery();
}

void MainWindow::on_buttonBattPlot_clicked()
{
    //Init the plot area
    QVector<double> x(10);
    QVector<double> y(10);
    for(int i =0; i < 9; i++)
    {
        x[i] = i;
        y[i] = 0;
    }
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("time [s]");
    ui->customPlot->yAxis->setLabel("SOC [%]");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, 9);
    ui->customPlot->yAxis->setRange(0, 100);

    //Init the slider
    ui->timeSlide->setRange(0, 10);

    connect(ui->timeSlide, SIGNAL(valueChanged(int)), this, SLOT(on_timeSlide_valueChanged(int)));
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));

    //Call the update function if emit
    connect(plotTimer->timer, SIGNAL(timeout()), this, SLOT(updatePlotData()));
    connect(sliceTimer->timer, SIGNAL(timeout()), this, SLOT(gatherPlotData()));
    plotTimer->startTimer(time_interval_plot_ms);
    sliceTimer->startTimer(time_slice_plot_ms);

}

void MainWindow::on_timeSlide_valueChanged(int value)
{

  if (qAbs(ui->customPlot->xAxis->range().center()-value/10.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->customPlot->xAxis->setRange(value/10.0, ui->customPlot->xAxis->range().size(), Qt::AlignCenter);
    ui->customPlot->replot();
  }
}

void MainWindow::xAxisChanged(QCPRange range)
{
  ui->timeSlide->setValue(qRound(range.center()*10.0)); // adjust position of scroll bar slider
  ui->timeSlide->setPageStep(qRound(range.size()*10.0)); // adjust size of scroll bar slider
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

        if(command.startsWith("S"))
        {
            qDebug() << "hello from self test" << endl;
            if(command.endsWith("OK"))
            {
                ui->logWindow->append("Self test was successful!");
                myAlarm.selfTestResult = true;

            }
            else
            {
                ui->logWindow->append("Self test was unsuccessful!!!");
                myAlarm.selfTestResult = false;

            }


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
        this->developerOption = ui->isDeveloper->isChecked();
        if(ui->isDeveloper->isChecked() == true )
        {
            connect(developerTimer->timer, SIGNAL(timeout()), this, SLOT(fillMyAlarmForDevelopment())  );
            developerTimer->startTimer(time_interval_ms);

        }


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
    ui->statusWindow->setEnabled(false);
    ui->text_Password->setText("");
    mTimer->stopTimer();
    developerTimer->stopTimer();
    sendArmCommand();




}

void MainWindow::on_buttonConnect_clicked()
{

    QString selectedComPort = ui->comboBox_COM_Port->currentText();
    qDebug() << "Selected COM Port: " << selectedComPort << endl;
    bool isAvailable =mSerial->CheckPort(selectedComPort);
    if(mSerial->serialport->isOpen() == true)
    {
        mTimer->startTimer(time_interval_ms);
        qDebug() << "start the timer" << endl;
        sendDisarmCommand();
        return;

    }
    if(isAvailable == true)
    {

        bool status = mSerial->ConnectToTheDevice(selectedComPort);
        if(status == true)
        {   ui->label_5->setText(selectedComPort);
            ui->label_6->setText(QString::number(mSerial->serialport->baudRate()));
            if(developerOption){ui->label_12->setText("Developer");}
            else{ui->label_12->setText("User");}
            sendDisarmCommand();

        }
    }
    if(SelfTestOption == true)
    {
        selfTestTimer->startTimer(10);
        ui->progressBar->setEnabled(true);
        mSerial->SendData("SELFTEST");
        mSerial->serialport->waitForReadyRead(1000);


    }

    if(mSerial->serialport->isOpen() == true && myAlarm.selfTestResult == true )
    {
        mTimer->startTimer(time_interval_ms);
        qDebug() << "start the timer" << endl;

        sendDisarmCommand();
        return;

    }
    else{
        qDebug() << "Error during starting the communication" << endl;

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


void MainWindow::on_celarContentBtn_clicked()
{
     ui->logWindow->setPlainText("");
}


