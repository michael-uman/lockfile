#include <stdio.h>
#include <unistd.h>
#include "lockfile.h"

const char * serialDevice = "/dev/ttyACM0";

int main() {
    printf("Verifying lockfile generation for %s\n", serialDevice);

    if (lock_filename(serialDevice) == 0) {
        printf("Lock succeeded!\n");
        sleep(10);
        if (unlock_filename(serialDevice) == 0) {
            printf("Unlock succeeded!\n");
        } else {
            printf("Unlock failed!\n");
        }
    } else {
        printf("Lock failed!\n");
    }

    return 0;
}