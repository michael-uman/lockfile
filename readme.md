# README

## Overview

This repository contains code to maintain a Linux standard lock file as specified @ https://refspecs.linuxfoundation.org/FHS_3.0/fhs/ch05s09.html.

The project was originally developed using the **`Jetbrains CLion IDE`** and as a result uses the `cmake` build configuration system.

## Building the project

``` 
$> git clone https://github.com/michael-uman/lockfile.git
$> cd lockfile
$> mkdir build
$> cd build
$> cmake ..
$> make
```

### Cross-compiling project

```
$> cd lockfile
$> mkdir cmake-raspberrypi-release
$> cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/raspberry-pi.cmake ..
$> make
```

## Functions

The `lockfile.h` header exposes the following functions:

```
int             siot_lock_filename(const char * filename);
int             siot_unlock_filename(const char * filename);
```

The `serialport.h` header exposes the following functions:

```
int             siot_serialport_open(const char *serial_device_name,
                                     int baud_rate, int * user_errno);
int             siot_serialport_close(int fdes, int * user_errno);
```

The `procutils.h` header exposes the following functions:

``` 
int             siot_verify_process_id(pid_t id);
```

## Example

See the `main.c` file for example usage.

```
/**
 * Test serial port operation.
 */

void test_serialport() {
    int serial_fd = -1;

    if ((serial_fd = siot_serialport_open(serialDevice, B115200, &myerrno)) != -1) {
        printf("Serial port opened [115200 Baud 8N1]!\n");

        printf("Sending data to serial port...\n");

        for (int i = 0 ; i < 10 ; i++) {
            dprintf(serial_fd, "Test #%d : ABCDEFGHIJKLMNOPQRSTUVWXYZ\n", i);
            sleep(1);
        }

        if (siot_serialport_close(serial_fd, &myerrno) == 0) {
            printf("Serial port closed!\n");
        } else {
            printf("Unable to close serial port [errno = %d (%s)]\n", myerrno, strerror(myerrno));
        }
    } else {
        printf("Unable to open serial port [errno = %d (%s)]\n", myerrno, strerror(myerrno));
    }
}
```

# Appendix

## Excerpt from Specification

###5.9.1. Purpose

Lock files should be stored within the `/var/lock` directory structure.

Lock files for devices and other resources shared by multiple applications, such as the serial device lock files that were originally found in either `/usr/spool/locks` or `/usr/spool/uucp`, must now be stored in `/var/lock`. The naming convention which must be used is "`LCK..`" followed by the base name of the device. For example, to lock /dev/ttyS0 the file "LCK..ttyS0" would be created.

The format used for the contents of such lock files must be the HDB UUCP lock file format. The HDB format is to store the process identifier (PID) as a ten byte ASCII decimal number, with a trailing newline. For example, if process 1230 holds a lock file, it would contain the eleven characters: space, space, space, space, space, space, one, two, three, zero, and newline.
