#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "wiringPi/wiringPi.h"
#include "wiringPi/mcp23s17.h"
#include "wiringPi/mcp23017.h"
#include "wiringPi/sr595.h"

// The first pin number for each device

#define BASE_SPI        123
#define BASE_I2C        456
#define BASE_SR         789

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int i, bit ;

    wiringPiSetup () ;
    mcp23017Setup (BASE_I2C, 0x20) ;
    mcp23s17Setup (BASE_SPI, 0, 0) ;
    sr595Setup    (BASE_SR, 10, 0, BASE_SPI, BASE_I2C) ;

    qDebug("Raspberry Pi - MCP23S17 + MCP23017 + 74x595 Test") ;

  // For the push button to stop it...

    pinMode         (BASE_I2C + 15, INPUT) ;
    pullUpDnControl (BASE_I2C + 15, PUD_UP) ;

    for (;;)
    {
      for (i = 0 ; i < 1024 ; ++i)
      {
        for (bit = 0 ; bit < 10 ; ++bit)
          digitalWrite (BASE_SR + bit, i & (1 << bit)) ;
        delay (5) ;
        while (digitalRead (BASE_I2C + 15) == 0)
          delay (1) ;
      }
    }
}
