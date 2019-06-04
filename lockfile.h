//
// Created by developer on 6/3/19.
//

#ifndef LOCKFILE_LOCKFILE_H
#define LOCKFILE_LOCKFILE_H

#include <stdio.h>

const char *    get_lockfile_name(const char * filename, char * lf_storage, size_t lf_len);
int             lock_filename(const char * filename);
int             unlock_filename(const char * filename);

#endif //LOCKFILE_LOCKFILE_H
