#ifndef MYTAR_H
# define MYTAR_H

#define UNARCHIVE 1
#define ARCHIVE 2

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <utime.h>
#include <stdio.h>

typedef struct  shead
{
    char        name[100];
    char        mode[8];
    char        uid[8];
    char        gid[8];
    char        size[12];
    char        mtime[12];
    char        checksum[8];
    char        linkflag[1];
    char        linkname[100];
    char        pad[255];
}               thead;


int             farchive(const char *aname, const char *path)
void            funarchive();
#endif