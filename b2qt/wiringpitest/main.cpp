#include "mainwindow.h"
#include <QApplication>

// run modprobe i2c-dev on the device before running the application
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
