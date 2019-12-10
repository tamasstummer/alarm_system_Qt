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


#define time_interval_ms 3000
#define time_interval_plot_ms 1000
#define time_interval_gather_ms 1000
#define time_interval_clear_ms 100
#define time_interval_self_test 10




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(mSerial, SIGNAL(gotCommand(QString)), this, SLOT(processSerialData(QString)));  // if received any command, process it
    connect(mTimer->timer, SIGNAL(timeout()), mSerial, SLOT(sendStatusCommand()));          // periodicly ask the alarm for status info
    connect(&myAlarm, SIGNAL(updatedDataFormSerial()), this, SLOT(updateWindowAfterSAtatusChanged()));    //if the incoming data processed, update the UI
    connect(selfTestTimer->timer, SIGNAL(timeout()),  this, SLOT(IncreaseProgressBar()));               //control the status bar during self test
    //Plot relative timers
    plotBattTimer->startTimer(time_interval_gather_ms);
    plotTimer->startTimer(time_interval_plot_ms);
    clearTimer->startTimer(time_interval_clear_ms);

    progressBarActualValue = 0;
    connect(&myAlarm, SIGNAL(updatedDataFormSerial()), this, SLOT(gatherPlotData()));
    //connect(&myAlarm, SIGNAL(updatePlotData()), this, SLOT(clearPlotData()));
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
    ui->progressBar->setEnabled(false);
}

/**
 * @brief Fill the alarm instance with fake data. Help in the development, if teammembers don't have the actual alarm circuit
 * @note The function emit the updatedDataFormSerial SIGNAL
 * @see MainWindow::updateWindowAfterSAtatusChanged
 */
void MainWindow::fillMyAlarmForDevelopment()
{

    myAlarm.SetBattery((std::rand()%10)*10);
    QVector<int> datas = {(std::rand()%50),(std::rand()%50)};
    myAlarm.SetHumidityAndTemperature(datas);
    emit myAlarm.updatedDataFormSerial();
}
/**
 * @brief Controll the progress bar during self test
 */
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

/**
 * @brief Update the UI after data received
 */
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

/**
 * @brief Plot functions
 */
void MainWindow::clearPlotData()
{
    //Init the plot area
    QVector<double> x(61);
    QVector<double> y(61);

    //Clear the class related variables
    for(int i = 0; i <= 60; i++)
    {
         plotData->y_batt[i] = 0;
    }

    plotData->index = 0;
    plotData->time  = 0;

    //Update the counter with the fresh value
    ui->lcdPlotTime->display(plotData->time);

    //Clear the plot area
    for(int i = 0; i <= 60; i++)
    {
        x[i] = i;
        y[i] = 0;
    }

    ui->customPlot->graph(0)->setData(x,y);
    ui->customPlot->update();
    ui->customPlot->replot();
}
/**
 * @brief Initialize the plotting area
 */
void MainWindow::initPlotData()
{
    //Init the plot area
    QVector<double> x(61);
    QVector<double> y(61);

    //Init the class related variables
    plotData->time  = 0;
    plotData->index = 0;

    for(int i = 0; i <= 60; i++)
    {
        x[i] = i;
        y[i] = 0;
    }

    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
}

/**
 * @brief Update the plotting area
 */
void MainWindow::updatePlotData()
{
    QVector<double> x(61);
    QVector<double> y(61);

    //Fill up the data arrays
    for(int i = 0; i <= 60; i++)
    {
        x[i] = i;
        y[i] = plotData->y_batt[i];
    }

    //plot the new graph
    ui->customPlot->graph(0)->setData(x,y);
    ui->customPlot->update();
    ui->customPlot->replot();

    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, 60);
    ui->customPlot->yAxis->setRange(0, 100);

    //Axis label
    ui->customPlot->xAxis->setLabel("time [s]");
    ui->customPlot->yAxis->setLabel("SOC [%]");

}
/**
 * @brief gather the battery value from the alarm instace for the plotting functions
 */
void MainWindow::gatherBattData()
{
    //Fill the current element of plotData with the freshly gathered battery data
    plotData->y_batt[plotData->index] =  myAlarm.GetBattery()*100; //in percents
    plotData->index ++;

    //Delet the data when plot range is over, and reinitialize
    if(plotData->index == 60)
    {
       plotData->index = 0;
       plotData->time++;

       //Display the minute count for current plot
       ui->lcdPlotTime->display(plotData->time);

       for(int i = 0; i <= 60; i++)
       {
            plotData->y_batt[i] = 0;
       }
    }

    //Overflow portection for the minute counter
    if(plotData->time >= 99)
        plotData->time = 0;
}

/**
 * @brief gather the temperature value from the alarm instace for the plotting functions
 */
void MainWindow::gatherTempData()
{
    QVector<int> HumidityTemperatureTemp    = myAlarm.GetHumidityAndTemperature();
    plotData->y_temp[plotData->index]       =  HumidityTemperatureTemp[1];
    plotData->index ++;

    if(plotData->index == 10)
    {
       plotData->index = 0;

       for(int i=0; i<=10; i++)
       {
            plotData->y_temp[i] = 0;
       }
    }
    connect(plotTempTimer->timer, SIGNAL(timeout()), this, SLOT(updatePlotData()));
}

/**
 * @brief gather the humidity value from the alarm instace for the plotting functions
 */
void MainWindow::gatherHumidData()
{
    QVector<int> HumidityTemperatureTemp    = myAlarm.GetHumidityAndTemperature();
    plotData->y_humid[plotData->index]      =  HumidityTemperatureTemp[0];
    plotData->index ++;

    if(plotData->index == 10)
    {
       plotData->index = 0;

       for(int i=0; i<=10; i++)
       {
            plotData->y_humid[i]    = 0;
       }
    }
    connect(plotHumidTimer->timer, SIGNAL(timeout()), this, SLOT(updatePlotData()));
}

void MainWindow::on_buttonBattPlot_clicked()
{
    //Init the plot and clear ,just to be sure nothing remains
    initPlotData();
    clearPlotData();

    //Call gather and update function by timeout (clicking on button agin has protection according to the timers)
    //timers only connected just for the first click, plotData->firstrun registers the first click
    if(plotData->firstrun != 1)
    {
        connect(plotBattTimer->timer, SIGNAL(timeout()), this, SLOT(gatherBattData()));
        connect(plotTimer->timer, SIGNAL(timeout()), this, SLOT(updatePlotData()));
        //Set the flag which indicates if it is the first run or not
        plotData->firstrun = 1;
    }
}

void MainWindow::on_buttonTempPlot_clicked()
{
    initPlotData();
    clearPlotData();

    // give the axes some labels:
    ui->customPlot->yAxis->setLabel("Temperature [C°]");

    //Call the update function
    connect(plotTempTimer->timer, SIGNAL(timeout()), this, SLOT(gatherTempData()));
}

void MainWindow::on_buttonHumidPlot_clicked()
{
    initPlotData();
    clearPlotData();

    // give the axes some labels:
    ui->customPlot->yAxis->setLabel("Humidity [%]");

    //Call the update function
    connect(plotHumidTimer->timer, SIGNAL(timeout()), this, SLOT(gatherHumidData()));
}

/**
 * @brief Process the response of the alarm, and update the Alarm instance's datafields with the getted parameters
 * @note The end of the function emit the "updatedDataFormSerial" SIGNAL
 * @see MainWindow::updateWindowAfterSAtatusChanged()
 * @param command
 */
void MainWindow::processSerialData(QString command)
{

    int batteryValue;
    int humidityValue;
    int temperatureValue;
    int relayValue;
    QVector<int> humidAndTemp;
    QString logMessage;


        qDebug() << "The getted command: " << command <<endl;
        if(command.startsWith("B"))  //Contains information about the sensors
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

        if(command.startsWith("S"))  //The result of the self test
        {
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


/**
 * @brief Check if the password is right. If developer mod is selected, a connection is made between a timer and a SLOT
 * @see MainWindow::fillMyAlarmForDevelopment
 */
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
        ui->progressBar->setEnabled(true);
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

/**
 * @brief SLOT of the buttonArmTheSystem button's action. Arm the alarmy system, and lock some UI part
 */
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
    ui->progressBar->setValue(0);
    ui->progressBar->setEnabled(false);
    sendArmCommand();

}

/**
 * @brief SLOT of the buttonConnect button's action. Establish the communication between the computer and the alarm
 */
void MainWindow::on_buttonConnect_clicked()
{

    QString selectedComPort = ui->comboBox_COM_Port->currentText();
    qDebug() << "Selected COM Port: " << selectedComPort << endl;
    qDebug() << "Is there any available COM Port: " << mSerial->CheckPort() << endl;
    this->SelfTestOption = ui->isSelfTestMode->isChecked();
    if(myAlarm.GetIsAvailable() == true)
    {
        qDebug() << "start the timer" << endl;
        sendDisarmCommand();
        return;

    }
    else
    {
        status = mSerial->ConnectToTheDevice(selectedComPort);
    }

    qDebug() << "Communication established?: " << status << endl;

    if(status == true)
        {   ui->label_5->setText(selectedComPort);
            ui->label_6->setText(QString::number(mSerial->serialport->baudRate()));
            if(developerOption)
            {
                ui->label_12->setText("Developer");
            }
            else
            {
                ui->label_12->setText("User");
            }
            sendDisarmCommand();

        }

    if(this->SelfTestOption == true)
    {
        selfTestTimer->startTimer(time_interval_self_test);
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
    else
    {
        qDebug() << "Error during starting the communication" << endl;
    }
}


/**
 * @brief Disarm the system
 */
void MainWindow::sendDisarmCommand()
{
    mSerial->SendData("DISARM");
    mSerial->serialport->waitForReadyRead(2000);
}

/**
 * @brief Arm the system
 */
void MainWindow::sendArmCommand()
{
     mSerial->SendData("ARM");
     mSerial->serialport->waitForReadyRead(2000);

}

/**
 * @brief Clear the content of the UI's log window
 */
void MainWindow::on_celarContentBtn_clicked()
{
     ui->logWindow->setPlainText("");
}
