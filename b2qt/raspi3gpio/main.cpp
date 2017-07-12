#include <QApplication>
#include "mainwindow.h"
#include "bcm2835.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}

// this is a simple test program that prints out what it will do rather than
// actually doing it
/*int main(int argc, char **argv)
{
    // Be non-destructive
    //bcm2835_set_debug(1);

    if (!bcm2835_init())
    return 1;

    // Configure some GPIO pins fo some testing
    // Set RPI pin P1-11 to be an output
    bcm2835_gpio_fsel(RPI_GPIO_P1_11, BCM2835_GPIO_FSEL_OUTP);
    // Set RPI pin P1-15 to be an input
    //bcm2835_gpio_fsel(RPI_GPIO_P1_15, BCM2835_GPIO_FSEL_INPT);
    //  with a pullup
    //bcm2835_gpio_set_pud(RPI_GPIO_P1_15, BCM2835_GPIO_PUD_UP);
    // And a low detect enable
    //bcm2835_gpio_len(RPI_GPIO_P1_15);
    // and input hysteresis disabled on GPIOs 0 to 27
    bcm2835_gpio_set_pad(RPI_GPIO_P1_11, BCM2835_PAD_SLEW_RATE_UNLIMITED|BCM2835_PAD_DRIVE_8mA);

#if 1
    // Blink
    while (1)
    {
    printf("High!\n");
    // Turn it on
    bcm2835_gpio_write(RPI_V2_GPIO_P1_26, HIGH);

    // wait a bit
    bcm2835_delay(1000);
    fflush(stdout);
    printf("Low!\n");
    // turn it off
    bcm2835_gpio_write(RPI_V2_GPIO_P1_26, LOW);

    // wait a bit
    bcm2835_delay(1000);
    fflush(stdout);
    }
#endif

#if 0
    // Read input
    while (1)
    {
    // Read some data
    uint8_t value = bcm2835_gpio_lev(RPI_GPIO_P1_15);
    printf("read from pin 15: %d\n", value);

    // wait a bit
    bcm2835_delay(500);
    }
#endif

#if 0
    // Look for a low event detection
    // eds will be set whenever pin 15 goes low
    while (1)
    {
    if (bcm2835_gpio_eds(RPI_GPIO_P1_15))
    {
        // Now clear the eds flag by setting it to 1
        bcm2835_gpio_set_eds(RPI_GPIO_P1_15);
        printf("low event detect for pin 15\n");
    }

    // wait a bit
    bcm2835_delay(500);
    }
#endif

    if (!bcm2835_close())
    return 1;

    return 0;
}
*/
/* blink.c
 *
 * Raspberry Pi GPIO example using sysfs interface.
 * Guillermo A. Amaral B. <g@maral.me>
 *
 * This file blinks GPIO 4 (P1-07) while reading GPIO 24 (P1_18).
 */

/*
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

int
main(int argc, char *argv[])
{
    int repeat = 10000;

    if (-1 == GPIOExport(POUT) || -1 == GPIOExport(PIN))
        return(1);

    if (-1 == GPIODirection(POUT, OUT) || -1 == GPIODirection(PIN, IN))
        return(2);

    do {
        if (-1 == GPIOWrite(POUT, repeat % 2))
            return(3);

        //printf("I'm reading %d in GPIO %d\n", GPIORead(PIN), PIN);
        printf("Wrote!\n");
        fflush(stdout);

        usleep(1000 * 1000);
    }
    while (repeat--);

    if (-1 == GPIOUnexport(POUT) || -1 == GPIOUnexport(PIN))
        return(4);

    return(0);
}
*/
