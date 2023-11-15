#include "uls.h"
#include "stdlib.h"

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
            for (int j = 0; j < num_columns; j++) {
                mx_printstr(files[index]);
                int name_len = mx_strlen(files[index]);
                index = index + rows;
                if (index >= num_files)
                    break;
                int tabs = (width - name_len + tab - 1) / tab;
                for (int i = 0; i < tabs; i++) {
                    if (tab == 1) 
                        mx_printchar(' ');
                    else
                        mx_printchar('\t');
                }                
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

void mx_printint_formatted(int n, int width) {
    int num_width = 0;
    int temp = n;

    // Вычисляем ширину числа
    while (temp != 0) {
        temp /= 10;
        num_width++;
    }

    // Печатаем пробелы перед числом
    for (int i = 0; i < width - num_width; i++) {
        mx_printchar(' ');
    }

    // Печатаем само число
    mx_printint(n);
}

void mx_printstr_formatted(char *str, int wid, bool align) {
    int spaces = wid - mx_strlen(str);

    if (!align) {
        mx_printstr(str);
    }

    for (int i = 0; i < spaces; i++) {
        mx_printchar(' ');
    }

    if (align) {
        mx_printstr(str);
    }
}

void print_file_entry(const FileEntry *file_entries, int i, t_max_sizes_s mxsize) {
    mx_printchar(file_entries[i].type);
    for(int j = 0; j < mx_strlen(file_entries[i].permissions); j++) {
        mx_printchar(file_entries[i].permissions[j]);
    }
    // mx_printstr(file_entries[i].permissions);
    mx_printstr(" ");
    
    if (file_entries[i].nlinks) {
        mx_printint_formatted(file_entries[i].nlinks, mxsize.max_nlinks_len);
    } else {
        mx_printstr("  ");
    }

    mx_printstr(" ");
    char *str_temp = mx_char_to_str(file_entries[i].owner);
    // mx_strcpy(str_temp, file_entries[i].owner);
    mx_printstr_formatted(str_temp, mxsize.max_username_len, true);
    free(str_temp);
    mx_printstr("  ");
    str_temp = mx_char_to_str(file_entries[i].group);
    // mx_strcpy(str_temp, file_entries[i].group);
    mx_printstr_formatted(str_temp, mxsize.max_groupname_len, true);
    free(str_temp);
    (file_entries[i].size == 0) ? mx_printstr(" ") : mx_printstr("  ");
    mx_printint_formatted(file_entries[i].size, mxsize.max_size_len);
    mx_printstr(" ");
    mx_printstr(file_entries[i].modification_time);
    mx_printstr(" ");
    mx_printstr(file_entries[i].name);

    if (file_entries[i].type == 'l') {
        mx_printstr(" -> ");
        mx_printstr(file_entries[i].symlink);
    }

    mx_printstr("\n");
}

void print_longlist(const char *dirname, FileEntry *file_entries, int count, s_flags_t *flags) {
    // Выводим отсортированный массив
    // print_file_entries(file_entries, count);

    // // Освобождаем память для массива
    // free(file_entries);
    // while (flags)
    // {
    //     break;
    // }
    int total_blocks = 0;
    struct stat sb;
    char file_path[1024];
    mx_printstr("total ");
    // Считаем количество файлов в директории
    // while ((entry = readdir(dir)) != NULL) {
    for (int i = 0; i < count; ++i) {
        if (file_entries[i].name[0] == '.' && !flags->a) continue;
        snprintf(file_path, sizeof(file_path), "%s/%s", dirname, file_entries[i].name);
        if (lstat(file_path, &sb) == -1) {
            perror("Cannot get file information");
            continue;
        }
        total_blocks += (int)(sb.st_blocks);
    }
    mx_printint(total_blocks);
    mx_printchar('\n');
    
        int max_ln_len = 0;
    for (int i = 0; i < count; ++i) {
        int x = file_entries[i].nlinks;
        int len = 1;
        while ((x /=10) > 0) len++;
        if (len > max_ln_len) max_ln_len = len; 
    }
    // char *output;
    t_max_sizes_s t_mxsize = {.max_groupname_len = 0, .max_nlinks_len = 0, .max_size_len = 0, .max_username_len = 0};
    for (int i = 0; i < count; ++i) { 
            int group_len = mx_strlen(file_entries[i].group);
            int nlinks_len = mx_intlen(file_entries[i].nlinks);
            int size_len = mx_intlen(file_entries[i].size);
            int name_len = mx_strlen(file_entries[i].group);
            if (group_len > t_mxsize.max_groupname_len) {
                t_mxsize.max_groupname_len = group_len;
            }
            if (nlinks_len > t_mxsize.max_nlinks_len) {
                t_mxsize.max_nlinks_len = nlinks_len;
            }
            if (size_len > t_mxsize.max_size_len) {
                t_mxsize.max_size_len = size_len;
            }
            if (name_len > t_mxsize.max_username_len) {
                t_mxsize.max_username_len = name_len;
            }
    }
    for (int i = 0; i < count; ++i) {
        print_file_entry(file_entries, i, t_mxsize);
        // printf("%c%2s %*d %s  %s%7ld %s %s%s%s\n",
        //        file_entries[i].type,
        //        file_entries[i].permissions,
        //        (file_entries[i].nlinks) ? max_ln_len : 2, // Используйте %*d для динамической ширины поля
        //        file_entries[i].nlinks,
        //        file_entries[i].owner,
        //        file_entries[i].group,
        //        file_entries[i].size, // Используйте явное приведение к long для %ld
        //        file_entries[i].modification_time,
        //        file_entries[i].name,
        //        (file_entries[i].type == 'l' ? " -> " : ""),
        //        (file_entries[i].type == 'l' ? file_entries[i].symlink : ""));

        // free(file_entries[i].name);  // Освобождаем память для каждого имени
    }
    // free(file_entries);
}
