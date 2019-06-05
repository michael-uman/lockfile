/**
 * @file    serialport.h
 * @author  Michael Uman
 * @date    June 4, 2019
 */

#ifndef LOCKFILE_SIOT_SERIALPORT_H
#define LOCKFILE_SIOT_SERIALPORT_H

#include <stdint.h>
#include <termios.h>

#ifdef __cplusplus
extern "C" {
#endif

int siot_serialport_open(const char *serial_device_name, int baud_rate, int *user_errno);
int siot_serialport_close(int fdes, int *user_errno);

#ifdef __cplusplus
}
#endif

#endif //LOCKFILE_SIOT_SERIALPORT_H
