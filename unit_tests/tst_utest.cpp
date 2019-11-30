#include <QtTest>
#include <QString>
#include "alarm.h"
#include "mainwindow.h"

class utest : public QObject
{
    Q_OBJECT
    Alarm alarm1;

public:
    utest();
    ~utest();

private slots:
    void testifavailable();
    void testpassword();
    void testhumidityandtemp();

};

utest::utest()
    :alarm1()
{

}

utest::~utest()
{

}

void utest::testifavailable()
{
   alarm1.SetIsAvailable(true);
   QVERIFY2(alarm1.GetIsAvailable() == true, "Availability is set");
   alarm1.SetIsAvailable(false);
   QVERIFY2(alarm1.GetIsAvailable() == false, "Availability is unset");
}

void utest::testpassword()
{
   QVERIFY2(alarm1.GetPassword() == "1234", "Correct password returned");
}

void utest::testhumidityandtemp()
{
   QVector<int> trial;
   trial << 1 << 20 << 5 << 100;
   alarm1.SetHumidityAndTemperature(trial);
   QVERIFY2(alarm1.GetHumidityAndTemperature() == trial, "H&T datas stored and restored correctly");
}

QTEST_APPLESS_MAIN(utest)

#include "tst_utest.moc"
