#include <stdio.h>
#include <stdlib.h>
#include <uls.h>

int compare_names(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}
void print_tab_aligned_columns(int width, int tab_width) {
    int tabs = (width + tab_width - 1) / tab_width;

    for (int i = 0; i < tabs; i++) {
        if (tab_width == 1) {
            mx_printchar(' ');
        } else {
            mx_printchar('\t');
        }
    }
}
void print_multicolumn(const char *dirname) {
    struct dirent *dir_entry;
    char **files = NULL;
    int num_files = 0;

    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((dir_entry = readdir(dir)) != NULL) {
        if (dir_entry->d_name[0] != '.') {
            files = realloc(files, (num_files + 1) * sizeof(char *));
            files[num_files] = strdup(dir_entry->d_name);
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
        qsort(files, num_files, sizeof(char *), compare_names);

        int max_name_length = 0;
        for (int i = 0; i < num_files; ++i) {
            int name_length = strlen(files[i]);
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
            printf("\n");
        }

        for (int i = 0; i < num_files; ++i) {
            free(files[i]);
        }
        free(files);
    }
}

int main(int argc, char *argv[]) {
    const char *dirname;
    if (argc == 1) {
        dirname = "."; // Use current directory if no argument is provided
    } else if (argc == 2) {
        dirname = argv[1];
    } else {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    print_multicolumn(dirname);

    return 0;
}
