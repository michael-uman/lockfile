//
// Created by developer on 6/3/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <zconf.h>
#include <sys/stat.h>

#include "lockfile.h"

//#define VERBOSE_DEBUG

static const char * lockPrefix = "/var/lock/LCK..";


/**
 * Check if file exists.
 *
 * @param filepath Path to check. Uses system function stat()...
 * @return Zero if file exists, -1 if file does not exist.
 */
static int exists(const char * filepath) {
    struct stat file_stat;
    int result = -1;

    memset(&file_stat, 0, sizeof(file_stat));

    if (stat(filepath, &file_stat) == 0) {
#ifdef VERBOSE_DEBUG
        fprintf(stderr, "%s file exists\n", filepath);
#endif
        result = 0;
    }

    return result;
}

/**
 * Generate the lockfile name from the resource path...
 *
 * Example: filepath = /dev/ttyUSB0 -> lockfile_name = /var/lock/LCK..ttyUSB0
 * @param filepath      Path to file to create lockfile for.
 * @param lf_storage    Buffer to store resulting lockfile name.
 * @param lf_len        Max length of lockfile name buffer
 * @return              Pointer to lockfile name, or NULL on failure.
 */
const char * get_lockfile_name(const char * filepath, char * lf_storage, size_t lf_len) {
    char * filename = NULL;

    filename = strrchr(filepath, '/');
    if (filename == NULL) {
        filename = (char *)filepath;
    }

    if (lf_len < (strlen(lockPrefix) + strlen(filename) + 1)) {
#ifdef VERBOSE_DEBUG
        fprintf(stderr, "ERROR: Insufficient space to store lockfile name\n");
#endif
        return NULL;
    }

    /* zero the storage memory */
    memset(lf_storage, 0, lf_len);

    strncat(lf_storage, lockPrefix, lf_len - 1);
    strncat(lf_storage, (filename + 1), lf_len - 1);

    return (const char *)lf_storage;
}

/**
 * Attempt to allocate lockfile if it does not already exist.
 *
 * @param filename
 * @return Zero on success, -1 on failure
 */
int lock_filename(const char * filename) {
    char lock_file_buffer[32];
    const char * lockfile_name = NULL;
    int result = -1;

    lockfile_name = get_lockfile_name(filename, lock_file_buffer, 32);
    if (lockfile_name != NULL) {
        if (exists(lockfile_name) != 0) {
            pid_t pid = getpid(); // get process id
            FILE *fh = NULL;
#ifdef VERBOSE_DEBUG
            fprintf(stderr, "Writing pid %d to lockfile %s\n", pid, lockfile_name);
#endif
            /* Write our process id to the lock file */
            fh = fopen(lockfile_name, "w");
            fprintf(fh, "%10d\n", pid);
            fclose(fh);

            result = 0;
        } else {
#ifdef VERBOSE_DEBUG
            fprintf(stderr, "LOCKFILE exists\n");
#endif
        }
    }

    return result;
}

/**
 * Release the resource by removing the lock file.
 *
 * @param filename
 * @return
 */
int unlock_filename(const char * filename) {
    char lock_file_buffer[32];
    const char *lockfile_name = NULL;
    int result = -1;

    lockfile_name = get_lockfile_name(filename, lock_file_buffer, 32);
    if (lockfile_name != NULL) {
        if (exists(lockfile_name) == 0) {
            if (unlink(lockfile_name) == 0) {
                result = 0;
            }
        } else {
#ifdef VERBOSE_DEBUG
            fprintf(stderr, "LOCKFILE does not exist\n");
#endif
        }
    }

    return result;
}
