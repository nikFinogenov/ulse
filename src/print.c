#include "uls.h"

void print_multicolumn(const char *dirname, s_flags_t *flags) {
    struct dirent *dir_entry;
    char **files = NULL;
    int num_files = 0;

    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        mx_printerr("opendir");
        exit(1);
    }

    while ((dir_entry = readdir(dir)) != NULL) {
        if (dir_entry->d_name[0] == '.' && !flags->a)
            continue;
        files = mx_realloc(files, (num_files + 1) * sizeof(char *));
        files[num_files] = mx_strdup(dir_entry->d_name);
        num_files++;
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

void print_perline(const char *dirname, s_flags_t *flags) {
    struct dirent *dir_entry;
    char **files = NULL;
    int num_files = 0;

    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        mx_printerr("opendir");
        exit(1);
    }
    while ((dir_entry = readdir(dir)) != NULL) {
        if (dir_entry->d_name[0] == '.' && !flags->a)
            continue;
        files = mx_realloc(files, (num_files + 1) * sizeof(char *));
        files[num_files] = mx_strdup(dir_entry->d_name);
        num_files++;
    }
    if (num_files > 0)
        custom_qsort(files, num_files, sizeof(char *), compare_names);
    for(int i = 0; i < num_files; i++) {
        mx_printstr(files[i]);
        mx_printchar('\n');
    }
    for (int i = 0; i < num_files; ++i) {
        free(files[i]);
    }
    free(files);
    closedir(dir);
}

void print_longlist(const char *dirname, s_flags_t *flags) {
    int count;
    FileEntry *file_entries = fill_file_entries(dirname, &count, flags);
    if (file_entries == NULL) {
        return;
    }

    // Сортируем массив по именам
    custom_qsort(file_entries, count, sizeof(FileEntry), compare_file_entries);

    // Выводим отсортированный массив
    // print_file_entries(file_entries, count);

    // // Освобождаем память для массива
    // free(file_entries);
    while (flags)
    {
        break;
    }
    
        int max_ln_len = 0;
    for (int i = 0; i < count; ++i) {
        int x = file_entries[i].nlinks;
        int len = 1;
        while ((x /=10) > 0) len++;
        if (len > max_ln_len) max_ln_len = len; 
    }
    for (int i = 0; i < count; ++i) {
        printf("%c%2s %*d %s  %s%7ld %s %s%s%s\n",
               file_entries[i].type,
               file_entries[i].permissions,
               (file_entries[i].nlinks) ? max_ln_len : 2, // Используйте %*d для динамической ширины поля
               file_entries[i].nlinks,
               file_entries[i].owner,
               file_entries[i].group,
               file_entries[i].size, // Используйте явное приведение к long для %ld
               file_entries[i].modification_time,
               file_entries[i].name,
               (file_entries[i].type == 'l' ? " -> " : ""),
               (file_entries[i].type == 'l' ? file_entries[i].symlink : ""));

        free(file_entries[i].name);  // Освобождаем память для каждого имени
    }
    free(file_entries);
}
