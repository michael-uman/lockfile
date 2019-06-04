#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "lockfile.h"
#include "serialport.h"

const char * serialDevice = "/dev/ttyACM0";

int myerrno = 0; // local errno value

/**
 * Test lock file operation.
 */
void test_lockfile() {
    printf("Verifying lockfile generation for %s\n", serialDevice);

    if (lock_filename(serialDevice) == 0) {
        printf("Lock succeeded!\n");

        sleep(20);

        if (unlock_filename(serialDevice) == 0) {
            printf("Unlock succeeded!\n");
        } else {
            printf("Unlock failed!\n");
        }
    } else {
        printf("Lock failed!\n");
    }

    return;
}

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

int main() {
    test_lockfile();

    test_serialport();

    return 0;
}