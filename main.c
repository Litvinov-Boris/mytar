#include "mytar.h"


int main(int argc, char **argv)
{
    mode_t flags = 0775;
    char *name_ar = "test.tar";
    char *dir_out = NULL;
    char *path = NULL;
    int mode = 0;
    int opt;

    while ((opt = getopt(argc, argv, "u:o:d") != -1))
    {
        switch (opt)
        {
            case 'o':
                name_ar = optarg;
                dir_out = optarg;
                break;
            case 'u':
                mode = UNARCHIVE;
                path = optarg;
                break;
            case 'd':
                mode = ARCHIVE;
                path = optarg;
                break;
        }
    }
    if (mode == 0 || path == NULL) 
    {
        fprintf(stderr, "неверно введенные данные");
        return (-1);
    }
}