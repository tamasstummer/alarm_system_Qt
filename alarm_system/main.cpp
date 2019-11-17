#include "mainwindow.h"

#include <QApplication>
#include <QWindow>
#include <QSerialPortInfo>
#include <QComboBox>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    int x=w.width()*0.8;
    int y=w.height()*0.85;
    w.setFixedSize(x,y);
    w.show();
    return a.exec();


}
