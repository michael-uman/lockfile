# README

## Overview

This repository contains code to maintain a Linux standard lock file as specified @ https://refspecs.linuxfoundation.org/FHS_3.0/fhs/ch05s09.html.

## Functions

The `lockfile.h` header exposes the following functions:

```
const char *    get_lockfile_name(const char * filename, char * lf_storage, size_t lf_len);
int             lock_filename(const char * filename);
int             unlock_filename(const char * filename);
```

The `serialport.h` header exposes the following functions:

```
int serialport_open(const char *serial_device_name, int baud_rate, int * user_errno);
int serialport_close(int fdes, int * user_errno);
```

## Example

See the `main.c` file for example usage.

```
/**
 * Test serial port operation.
 */

void test_serialport() {
    int serial_fd = -1;

    if ((serial_fd = serialport_open(serialDevice, B115200, &myerrno)) != -1) {
        printf("Serial port opened [115200 Baud 8N1]!\n");

        printf("Sending data to serial port...\n");

        for (int i = 0 ; i < 10 ; i++) {
            dprintf(serial_fd, "Test #%d : ABCDEFGHIJKLMNOPQRSTUVWXYZ\n", i);
            sleep(1);
        }

        if (serialport_close(serial_fd, &myerrno) == 0) {
            printf("Serial port closed!\n");
        } else {
            printf("Unable to close serial port [errno = %d (%s)]\n", myerrno, strerror(myerrno));
        }
    } else {
        printf("Unable to open serial port [errno = %d (%s)]\n", myerrno, strerror(myerrno));
    }
}
```