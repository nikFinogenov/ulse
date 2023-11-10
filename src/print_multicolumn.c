#include "uls.h"


void print_multicolumn(const char *dirname) {
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

    closedir(dir);

    int terminal_width = 80;
    if (isatty(1)) {
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        terminal_width = w.ws_col;
    }

    if (num_files > 0) {
        custom_qsort(files, num_files, sizeof(char *), compare_names);

        int max_name_length = 0;
        for (int i = 0; i < num_files; ++i) {
            int name_length = mx_strlen(files[i]);
            if (name_length > max_name_length) {
                max_name_length = name_length;
            }
        }
        int tab = 8;
        int width = (max_name_length + tab) & ~(tab - 1);
        int num_columns = terminal_width / width;
        int index = 0;
        int rows = (num_files + num_columns - 1) / num_columns;
        for (int i = 0; i < rows; ++i) {
            index = i;
            for (int j = 0; j < num_columns; ++j) {
                if (index < num_files) {
                    mx_printstr(files[index]);
                    int tabs = (width - mx_strlen(files[index]) + tab - 1) / tab;
                    for (int i = 0; i < tabs; i++) {
                        if (tab == 1) 
                            mx_printchar(' ');
                        else
                            mx_printchar('\t');
                    }
                }
                index = index + rows;
            }
            mx_printchar('\n');
        }

        for (int i = 0; i < num_files; ++i) {
            free(files[i]);
        }
        free(files);
    }
}
