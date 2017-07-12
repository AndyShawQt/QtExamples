#-------------------------------------------------
#
# Project created by QtCreator 2013-12-27T13:21:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GPIOTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bcm2835.c

HEADERS  += mainwindow.h \
    bcm2835.h

FORMS    += mainwindow.ui

target.path += /root
INSTALLS += target
