#-------------------------------------------------
#
# Project created by QtCreator 2017-07-11T15:21:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wiringpitest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        wiringPi/ads1115.c \
        wiringPi/bmp180.c \
        wiringPi/drcNet.c \
        wiringPi/drcSerial.c \
        wiringPi/ds18b20.c \
        wiringPi/htu21d.c \
        wiringPi/max31855.c \
        wiringPi/max5322.c \
        wiringPi/mcp23008.c \
        wiringPi/mcp23016.c \
        wiringPi/mcp23017.c \
        wiringPi/mcp23s08.c \
        wiringPi/mcp23s17.c \
        wiringPi/mcp3002.c \
        wiringPi/mcp3004.c \
        wiringPi/mcp3422.c \
        wiringPi/mcp4802.c \
        wiringPi/pcf8574.c \
        wiringPi/pcf8591.c \
        wiringPi/piHiPri.c \
        wiringPi/piThread.c \
        wiringPi/pseudoPins.c \
        wiringPi/rht03.c \
        wiringPi/sn3218.c \
        wiringPi/softPwm.c \
        wiringPi/softServo.c \
        wiringPi/softTone.c \
        wiringPi/sr595.c \
        wiringPi/wiringPi.c \
        wiringPi/wiringPiI2C.c \
        wiringPi/wiringPiSPI.c \
        wiringPi/wiringSerial.c \
        wiringPi/wiringShift.c \
        wiringPi/wpiExtensions.c

HEADERS += \
        mainwindow.h \
        wiringPi/*.h \
        version.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += ./wiringPi

LIBS += -lrt -lcrypt

target.path = /opt
INSTALLS += target

