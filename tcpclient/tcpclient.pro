QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = tcpclient
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp

HEADERS  += client.h

FORMS    += client.ui

