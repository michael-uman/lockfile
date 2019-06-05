/**
 * @file    procutils.c
 * @author  Michael Uman
 * @date    June 4, 2019
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>

#include "siot_procutils.h"

static const char * procFs = "/proc/";

/**
 *
 * @param str
 * @return
 */
static int is_numeric(const char * str) {
    char *sp;

    for (sp = (char *)str ; *sp ; sp++) {
        if (!isdigit(*sp))
            return -1;
    }

    return 0;
}

/**
 * Check if the specified process exists. This is accomplished by iterating
 * through procfs looking for numeric entries.
 * @param id    pid to look for
 * @return Zero if pid exists, -1 otherwise.
 */
int siot_verify_process_id(pid_t id) {
    DIR * dir = NULL;
    struct dirent * dirEntry = NULL;
    int result = -1;

    dir = opendir(procFs);
    if (dir == NULL) {
        perror("Unable to open proc directory");
        return -1;
    }

    while ((dirEntry = readdir(dir))) {
        /* Processes are represented by numeric entries in the /proc/ filesystem */
        if ((dirEntry->d_type == DT_DIR) &&
            (is_numeric(dirEntry->d_name) == 0))
        {
            int this_pid = atoi(dirEntry->d_name);

            if (this_pid == id) {
                result = 0;
                break;
            }
        }
    }

    closedir(dir);

    return result;
}
