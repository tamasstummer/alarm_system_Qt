#include "mainwindow.h"

#include <QApplication>
#include <QWindow>
#include <QSerialPortInfo>
#include <QComboBox>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    w.show();




    return a.exec();
}
