#include "uls.h"


void print_perline(const char *dirname) {
    struct dirent *dir_entry;
    char **files = NULL;
    int num_files = 0;

    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        mx_printerr("opendir");
        exit(1);
    }
    while ((dir_entry = readdir(dir)) != NULL) {
        if (dir_entry->d_name[0] != '.') {
            files = mx_realloc(files, (num_files + 1) * sizeof(char *));
            files[num_files] = mx_strdup(dir_entry->d_name);
            num_files++;
        }
    }
    if (num_files > 0)
        custom_qsort(files, num_files, sizeof(char *), compare_names);
    for(int i = 0; i < num_files; i++) {
        mx_printstr(files[i]);
        mx_printchar('\n');
    }

    closedir(dir);
}
