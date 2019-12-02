QT += testlib
QT -= gui

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = tst_utest
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

CONFIG += c++11
QMAKE_CXXFLAGS_CXX11 = std=c++ly
QT += quick

INCLUDEPATH +=  ../alarm_system

TEMPLATE = app

SOURCES +=  tst_utest.cpp \
    ../alarm_system/alarm.cpp \

HEADERS += \
    ../alarm_system/alarm.h \


FORMS += \

