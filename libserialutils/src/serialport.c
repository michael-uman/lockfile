/**
 * @file    serialport.c
 * @author  Michael Uman
 * @date    June 4, 2019
 */

#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "serialport.h"
#include "lockfile.h"

//#define VERBOSE_DEBUG

/* Global variables */

static char *   serial_port_name    = NULL;
static int      serial_port_fd      = -1;

/**
 * Open the serial device and return file descriptor.
 *
 * Opens the serial device named by the string serial_device_name using the baud
 * rate specified by baud_rate. Upon error the function returns -1 and sets
 * user_errno to the code representing the cause of the error.
 *
 * @param serial_device_name    Name of serial device to open.
 * @param baud_rate             Baud rate to use for serial device.
 * @param errno                 Pointer to int where error code is stored.
 * @return                      File descriptor for serial port, or -1 on failure
 */
int serialport_open(const char *serial_device_name, int baud_rate, int * user_errno) {
    int serial_fd = -1;

    /* Fail if any passed value is NULL */
    assert(serial_device_name != NULL);
    assert(user_errno != NULL);

    if (serial_port_fd != -1) {
        *user_errno = EBUSY;
#ifdef VERBOSE_DEBUG
        fprintf(stderr, "serialport module supports only one open port\n");
#endif
        return -1;
    }

    /* Check lock files status */
    if (lock_filename(serial_device_name) != 0) {
        *user_errno = EAGAIN;
#ifdef VERBOSE_DEBUG
        fprintf(stderr, "Serial device already open, lockfile exists\n");
#endif
        return -1;
    }

    serial_fd = open(serial_device_name, O_RDWR | O_NOCTTY);
    if (serial_fd < 0) {
        *user_errno = errno;
        return -1;
    }

    // Set attributes
    struct termios tio;
    if (tcgetattr(serial_fd, &tio) < 0) {
        *user_errno = errno;
#ifdef VERBOSE_DEBUG
        fprintf(stderr, "tcgetattr error: %d (%s)\n", errno, strerror(errno));
#endif
        return -1;
    }

    cfsetospeed(&tio, (speed_t)baud_rate);
    cfsetispeed(&tio, (speed_t)baud_rate);

    tio.c_cflag |= (CLOCAL | CREAD);    // ignore modem controls
    tio.c_cflag &= ~CSIZE;
    tio.c_cflag |= CS8;         // 8-bit characters
    tio.c_cflag &= ~PARENB;     // no parity bit
    tio.c_cflag &= ~CSTOPB;     // 1 stop bit
    tio.c_cflag &= ~CRTSCTS;    // no hardware flowcontrol

    // setup for non-canonical mode
    tio.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tio.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tio.c_oflag &= ~OPOST;

    tio.c_cc[VMIN] = 4; // Wait for 4 bytes
    tio.c_cc[VTIME] = 3; // 300 ms timeout

    if (tcsetattr(serial_fd, TCSANOW, &tio) < 0) {
        *user_errno = errno;
#ifdef VERBOSE_DEBUG
        fprintf(stderr, "tcsetattr error: %d (%s)\n", errno, strerror(errno));
#endif
        return -1;
    }

    tcflush(serial_fd, TCIOFLUSH);

    /* Save the values in global storage... */
    serial_port_fd      = serial_fd;
    serial_port_name    = strdup(serial_device_name);

    return serial_fd;
}

/**
 *
 * @param handle
 * @return
 */
int serialport_close(int fdes, int * user_errno) {
    if (fdes != serial_port_fd) {
#ifdef VERBOSE_DEBUG
        fprintf(stderr, "Invalid handle, does not represent serialport opened with serialport_open()\n");
#endif
        *user_errno = EINVAL;
        return -1;
    }

    if (close(fdes) != 0) {
        *user_errno = errno;
#ifdef VERBOSE_DEBUG
        fprintf(stderr, "Unable to close serial port\n");
#endif
        return -1;
    }

    if (unlock_filename(serial_port_name) != 0) {
        *user_errno = ENXIO;
#ifdef VERBOSE_DEBUG
        fprintf(stderr, "Unable to unlock serial port\n");
#endif
        return -1;
    }

    free(serial_port_name);
    serial_port_name = NULL;
    serial_port_fd = -1;

    return 0;
}
