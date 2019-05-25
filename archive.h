#ifndef ARCHIVE_H
#define ARCHIVE_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <ftw.h>
#include <math.h>
#include <utime.h>
#include <getopt.h>

#define UNPACK 1
#define PACK 2
#define PATH_SIZE 200

extern unsigned int total_size;
extern unsigned int curr_size;

struct Header {
	char    path[PATH_SIZE];
    off_t st_size;
    ino_t st_ino;
    mode_t st_mode;
    uid_t st_uid;
    gid_t st_gid;
    //time_t st_atime;
    //time_t st_mtime;
    //time_t st_ctime;
};

int archive_dir(const char *path, const int to);
int archive_file(const char *curr_path, const int to, unsigned char type);
int move_file(const int from, const int to, int size_bytes);
int unpack(const int archive_file);

#endif

