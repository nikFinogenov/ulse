#include <stdio.h>
#include <stdlib.h>
#include <uls.h>

int compare_names(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
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
            files[num_files] = mx_strdup(dir_entry->d_name);
            num_files++;
        }
    }

    closedir(dir);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminal_width = w.ws_col; // Get terminal width using ioctl

    if (num_files > 0) {
        qsort(files, num_files, sizeof(char *), compare_names);

        int max_name_length = 0;
        for (int i = 0; i < num_files; ++i) {
            int name_length = strlen(files[i]);
            if (name_length > max_name_length) {
                max_name_length = name_length;
            }
        }

        int num_columns = terminal_width / (max_name_length + 2); // 2 spaces between columns
        int column_width = (terminal_width + 2) / num_columns - 2; // Calculate column width dynamically

        int rows = (num_files + num_columns - 1) / num_columns;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < num_columns; ++j) {
                int index = i + j * rows;
                if (index < num_files) {
                    printf("%-*s \t", column_width, files[index]);
                }
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
