/**
 *
 * @file        lockfile.h
 * @author      Michael Uman
 * @date        June 2, 2019
 * @brief       Functions to maintain resource lock files.
 *
 */

#ifndef LOCKFILE_SIOT_LOCKFILE_H
#define LOCKFILE_SIOT_LOCKFILE_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

//const char *    _get_lockfile_name(const char * filename, char * lf_storage, size_t lf_len);
int             siot_lock_filename(const char *filename);
int             siot_unlock_filename(const char *filename);

#ifdef __cplusplus
}
#endif

#endif //LOCKFILE_SIOT_LOCKFILE_H
