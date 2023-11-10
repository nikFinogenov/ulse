#include "uls.h"

void print_perline(const char *dirname) {
    struct dirent *dir_entry;

    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        mx_printerr("opendir");
        exit(1);
    }

    while ((dir_entry = readdir(dir)) != NULL) {
        if (dir_entry->d_name[0] != '.') {
            mx_printstr(dir_entry->d_name);
            mx_printchar('\n');
        }
    }

    closedir(dir);
}
