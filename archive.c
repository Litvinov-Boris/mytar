#include "archive.h"
#include <dirent.h>
# define DT_REG 8
# define DT_DIR 4
int archive_dir(const char *path, const int to)
{
	DIR *dir;
	struct dirent *info_dir;

	dir = opendir(path);
	if (dir == NULL) {
		perror("Dir error");
		return -1;
	}

	struct stat buf_info;
    stat(path, &buf_info);
    struct Header info;
    strcpy(&(info.path), path);
    info.st_size = buf_info.st_size;
    info.st_ino = buf_info.st_mode;
    info.st_mode = buf_info.st_mode;
    info.st_uid = buf_info.st_uid;
    info.st_gid = buf_info.st_gid;
    //info.st_atime = buf_info.st_atime;
    //info.st_mtime = buf_info.st_mtime;
    //info.st_ctime = buf_info.st_ctime;
    if (write(to, &info, sizeof(struct Header)) == -1) {
		perror("I/O error");
		return -1;
	}

	while ((info_dir = readdir(dir)) != NULL) {
		char *next = (char*)malloc(strlen(path) + strlen(info_dir->d_name) + 2);
		if (next == NULL) {
			perror("Memory error");
			return -1;
		}
		sprintf(next, "%s/%s", path, info_dir->d_name);
		if (info_dir->d_type == DT_REG && archive_file(next, to, info_dir->d_type) == -1)
			return -1;
		else if (info_dir->d_type == DT_DIR && strcmp(info_dir->d_name, ".")
			&& strcmp(info_dir->d_name, "..") && (archive_dir(next, to) == -1)){
			    return -1;
        }
		free(next);
        }
	closedir(dir);
	return 0;
}

int archive_file(const char *curr_path, const int to, unsigned char type)
{
    struct stat buf_info;
	struct Header info;
	int file;

    if (strlen(curr_path) >= PATH_SIZE) {
		fprintf(stderr, "The path or name is too long\n");
		return -1;
	}

    file = open(curr_path, O_RDONLY);
	if (file == -1 || fstat(file, &buf_info) == -1) {
		perror("I/O error");
		return -1;
	}

    //curr_size += buf_info.st_size;

    strcpy(&(info.path), curr_path);
    info.st_size = buf_info.st_size;
    info.st_ino = buf_info.st_mode;
    info.st_mode = buf_info.st_mode;
    info.st_uid = buf_info.st_uid;
    info.st_gid = buf_info.st_gid;
    //info.st_atime = buf_info.st_atime;
    //info.st_mtime = buf_info.st_mtime;
    //info.st_ctime = buf_info.st_ctime;

    if (write(to, &info, sizeof(struct Header)) == -1) {
		perror("I/O error");
		close(file);
		return -1;
	}

    if (move_file(file, to, info.st_size) == -1) {
		close(file);
		return -1;
	}

	close(file);
	return 0;
 }

int move_file(const int from, const int to, int size_bytes)
{
	int n_bytes;
	int size_buf = 512;

	char *tmp;

	if (size_bytes < -1) {
		fprintf(stderr, "Invalid argument for copy file\n");
		return -1;
	}
	if (size_bytes == -1) {
		struct stat info;

		fstat(from, &info);
		size_bytes = info.st_size;
	}

	char *buf = malloc(size_buf);

	if (buf == NULL) {
		perror("Memory Error");
		return -1;
	}
	tmp = buf;

	int mod;

	do {
		if (size_bytes == 0)
			break;

		mod = size_bytes % size_buf;
		n_bytes = mod ? mod : size_buf;
		size_bytes -= n_bytes;
		n_bytes = read(from, buf, n_bytes);

		if (n_bytes == -1 || write(to, buf, n_bytes) == -1) {
			perror("I/O error");
			free(buf);
			return -1;
		}
	} while (n_bytes);

	free(buf);
	return 0;
}

int unpack(const int archive_file)
{
    struct Header info;
    int reader;
    int out_file;
    while ((reader = read(archive_file, &info, sizeof(struct Header))) > 0)
    {
        if (S_ISDIR(info.st_mode)){
            if (mkdir(info.path, info.st_mode) != 0)
                if (errno != 17) // directory exists
                    goto IO_error;
        }else{
            out_file = open(info.path, O_WRONLY | O_CREAT, info.st_mode);
            if (out_file == -1)
			goto IO_error;

            if (move_file(archive_file, out_file, info.st_size) == -1)
			    return -1;
        }
    }
    if (reader < 0)
        goto IO_error;
    return 0;

IO_error:
		perror("I/O error");
		return -1;
}