#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bcm2835.h"

#define PIN RPI_GPIO_P1_11

void Init_GPIO11()
{
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
}

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define PIN  24 // P1-18
#define POUT 17//4  // P1-07

static int
GPIOExport(int pin)
{
#define BUFFER_MAX 3
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (-1 == fd) {
        fprintf(stdout, "Failed to open export for writing!\n");
        return(-1);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

static int
GPIOInterrupt(int pin)
{
#define BUFFER_MAX 4
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/gpio%d/edge", O_WRONLY);
    if (-1 == fd) {
        fprintf(stdout, "Failed to open export for writing!\n");
        return(-1);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "both");
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

static int
GPIOUnexport(int pin)
{
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
        fprintf(stdout, "Failed to open unexport for writing!\n");
        return(-1);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

static int
GPIODirection(int pin, int dir)
{
    static const char s_directions_str[]  = "in\0out";

#define DIRECTION_MAX 35
    char path[DIRECTION_MAX];
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd) {
        fprintf(stdout, "Failed to open gpio direction for writing!\n");
        return(-1);
    }

    if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
        fprintf(stdout, "Failed to set direction!\n");
        return(-1);
    }

    close(fd);
    return(0);
}

static int
GPIORead(int pin)
{
#define VALUE_MAX 30
    char path[VALUE_MAX];
    char value_str[3];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY);
    if (-1 == fd) {
        fprintf(stdout, "Failed to open gpio value for reading!\n");
        return(-1);
    }

    if (-1 == read(fd, value_str, 3)) {
        fprintf(stdout, "Failed to read value!\n");
        return(-1);
    }

    close(fd);

    return(atoi(value_str));
}

static int
GPIOWrite(int pin, int value)
{
    static const char s_values_str[] = "01";

    char path[VALUE_MAX];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd) {
        fprintf(stdout, "Failed to open gpio value for writing!\n");
        return(-1);
    }

    if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
        fprintf(stdout, "Failed to write value!\n");
        return(-1);
    }

    close(fd);
    return(0);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        //bcm2835_init();
        //Init_GPIO11();
        GPIOExport(PIN);
        GPIOExport(POUT);
        GPIODirection(POUT, OUT);
        GPIODirection(PIN, IN);
        GPIOInterrupt(PIN);
        ui->setupUi(this);
        connect(&m_timerLow, &QTimer::timeout, this, &MainWindow::on_pushButton_STop_clicked);
        connect(&m_timerHigh, &QTimer::timeout, this, &MainWindow::on_pushButton_Stop_clicked);
        m_timerLow.start(1000);
        char path[VALUE_MAX];
        snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", PIN);
        m_fd =open(path, O_RDONLY | O_NOCTTY |O_NONBLOCK);
        //FILE* file = fopen(path, "r");
        //setvbuf ( file , NULL , _IONBF , 0 );
        //qDebug()<<m_file.open(m_fd, QIODevice::ReadOnly);
        m_file.open(m_fd, QIODevice::ReadOnly);
        //m_fd =open(path, O_WRONLY | O_NOCTTY |O_NONBLOCK);
        /*m_file.open(m_fd, QIODevice::WriteOnly);*/
        m_socketNotifier = new QSocketNotifier(m_fd, QSocketNotifier::Read);
        m_socketNotifier->setEnabled(true);
        connect(m_socketNotifier, SIGNAL(activated(int)), this, SLOT(readyRead()));
        /*connect(&m_timerReader, &QTimer::timeout, this, &MainWindow::readyRead);
        m_timerReader.start(500);*/
        //connect(&m_file, SIGNAL(readyRead()), this, SLOT(readyRead()));

}
/***** Start Button *******/
void MainWindow::on_pushButton_STop_clicked()
{
    qDebug("Low!");
    m_timerLow.stop();
    //bcm2835_gpio_write(PIN,LOW);
    GPIOWrite(POUT, LOW);
    m_timerHigh.start(1000);
}
/***** Stop Button *******/
void MainWindow::on_pushButton_Stop_clicked()
{
    qDebug("High!");
    m_timerHigh.stop();
    //bcm2835_gpio_write(PIN,HIGH);
    GPIOWrite(POUT, HIGH);
    m_timerLow.start(1000);
}


void MainWindow::readyRead()
{
    char value_str[3];
    /*if (-1 == read(m_fd, value_str, 3)) {
        qDebug("Failed to read value!\n");
        return;
    }
    qDebug()<<"Value read : "<<value_str;*/
    //qDebug()<<m_file.readAll();
    //char value_str[3];
    //char path[VALUE_MAX];
    //snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", PIN);
    //m_fd =open(path, O_RDONLY);// | O_NOCTTY |O_NONBLOCK);
    //m_file.open(m_fd, QIODevice::ReadOnly);
    //m_file.read(value_str, 3);
    //qDebug()<<atoi(value_str);
    //m_file.close();
    //if(GPIORead(PIN) != 0)
        qDebug()<<GPIORead(PIN);
}

/***** Quit Button *******/
void MainWindow::on_pushButton_Quit_clicked()
{

}
MainWindow::~MainWindow()
{
    delete ui;
    delete m_socketNotifier;
}
