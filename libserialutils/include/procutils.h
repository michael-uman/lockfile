/**
 * @file    procutils.h
 * @author  Michael Uman
 * @date    June 4, 2019
 */

#ifndef LOCKFILE_PROCUTILS_H
#define LOCKFILE_PROCUTILS_H

#include <sys/types.h>
#include <unistd.h>

int verify_process_id(pid_t id);

#endif //LOCKFILE_PROCUTILS_H
