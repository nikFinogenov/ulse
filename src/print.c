#include "uls.h"
#include "stdlib.h"

static bool is_dir(const char *filename) {
    struct stat st;
    if (lstat(filename, &st) == 0) {
        return S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode);
    }
    return false;
}

static void printint_formatted(int n, int width) {
    int num_width = 0;
    int temp = n;

    while (temp != 0) {
        temp /= 10;
        num_width++;
    }

    for (int i = 0; i < width - num_width; i++)
        mx_printchar(' ');

    mx_printint(n);
}

static void printstr_formatted(char *str, int wid, bool right) {
    if (!right)
        mx_printstr(str);
    for (int i = 0; i < wid - mx_strlen(str); i++)
        mx_printchar(' ');
    if (right)
        mx_printstr(str);
}

static bool is_executable(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH);
    }
    return false;
}

static bool is_fifo(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return S_ISFIFO(st.st_mode);
    }
    return false;
}
static bool is_link(const char *filename) {
    struct stat st;
    if (lstat(filename, &st) == 0) {
        return S_ISLNK(st.st_mode);
    }
    return false;
}
static bool is_socket(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return S_ISSOCK(st.st_mode);
    }
    return false;
}

static bool is_whiteout(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return S_ISCHR(st.st_mode);
    }
    return false;
}

static bool is_smth(const char *filename) {
    return is_dir(filename) || is_executable(filename) 
    || is_fifo(filename) || is_link(filename) 
    || is_socket(filename) || is_whiteout(filename);
}
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
        if ((mx_strcmp(dir_entry->d_name, ".") == 0 || mx_strcmp(dir_entry->d_name, "..") == 0) && flags->A) continue;
        if (dir_entry->d_name[0] == '.' && !flags->a) continue;
            
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
        custom_qsort(files, num_files, sizeof(char *), compare_names, flags);

        int max_name_length = 0;
        for (int i = 0; i < num_files; ++i) {
            int name_length = mx_strlen(files[i]);
            if (flags->p) name_length += 1;
            if (name_length > max_name_length) {
                max_name_length = name_length;
            }
        }
        int tab = 8;
        if(flags->G) tab = 1;
        int width = (max_name_length + tab) & ~(tab - 1);
        int num_columns = terminal_width / width;
        int index = 0;
        int rows = (num_files + num_columns - 1) / num_columns;
        for (int i = 0; i < rows; ++i) {
            index = i;
            for (int j = 0; j < num_columns; j++) {
            if(flags->G) {
                struct stat sb;
                // printf("%d\n", index);
                if (lstat(mx_strjoin(mx_strjoin(dirname, "/"), files[index]), &sb) == -1) {
                    perror("Cannot get file information");
                    continue;
                }   
                switch_strcolor(sb);
            }
                mx_printstr(files[index]);
                if(flags->G) mx_printstr(DEFAULT_COLOR);
                int name_len = mx_strlen(files[index]);
                if(flags->p && is_dir(mx_strjoin(mx_strjoin(dirname, "/"), files[index]))) {
                    mx_printchar('/');
                    name_len += 1;
                }
                else if(flags->F && is_executable(mx_strjoin(mx_strjoin(dirname, "/"), files[index]))) {
                    mx_printchar('*');
                    name_len += 1;
                }
                else if(flags->F && is_link(mx_strjoin(mx_strjoin(dirname, "/"), files[index]))) {
                    mx_printchar('@');
                    name_len += 1;
                }
                else if(flags->F && is_socket(mx_strjoin(mx_strjoin(dirname, "/"), files[index]))) {
                    mx_printchar('=');
                    name_len += 1;
                }
                else if(flags->F && is_whiteout(mx_strjoin(mx_strjoin(dirname, "/"), files[index]))) {
                    mx_printchar('%');
                    name_len += 1;
                }
                else if(flags->F && is_fifo(mx_strjoin(mx_strjoin(dirname, "/"), files[index]))) {
                    mx_printchar('|');
                    name_len += 1;
                }
                
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
void print_xattr(const FileEntry *file_entry, s_flags_t *flags) {
    for (char **ptr = file_entry->xattr_keys; *ptr != NULL; ptr++) {
        mx_printchar('\t');
        mx_printstr(*ptr);
        mx_printchar('\t');
        ssize_t value_size = getxattr(file_entry->path, *ptr, NULL, 0, 0, XATTR_NOFOLLOW);

        if (flags->h) {
            const char *suffixes[] = {"B", "K", "M", "G", "T", "P"};
            double size_f = (double)value_size;
            int i = 0;

            while (size_f >= 1000) {
                size_f /= 1024;
                i++;
            }

            double size_r = custom_round(size_f * 10) / 10;
            // char buf[5] = {'\0'};
            char *result = (char *)malloc(20);

            if (size_r >= 10 || i == 0) {
                char *str = mx_lltoa(custom_round(size_f));
                result = mx_strjoin(result, str);
                free(str);
            }
            else {
                char *str = mx_lltoa(size_r);
                result = mx_strjoin(result, str);
                free(str);
                result = mx_strjoin(result, ".");
                str = mx_lltoa((long long)(size_r * 10) % 10);
                result = mx_strjoin(result, str);
                free(str);
            }

            result = mx_strjoin(result, suffixes[i]);
            // print_aligned_string(buf, width, true);
            printstr_formatted(result, 5, true);
        }
        else {
            printint_formatted(value_size, 5);
        }

        mx_printstr(" \n");
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
        if ((mx_strcmp(dir_entry->d_name, ".") == 0 || mx_strcmp(dir_entry->d_name, "..") == 0) && flags->A) continue;
        if (dir_entry->d_name[0] == '.' && !flags->a) continue;
        files = mx_realloc(files, (num_files + 1) * sizeof(char *));
        files[num_files] = mx_strdup(dir_entry->d_name);
        num_files++;
    }
    if (num_files > 0)
        custom_qsort(files, num_files, sizeof(char *), compare_names, flags);
    for (int i = 0; i < num_files; i++) {
                    if(flags->G) {
                struct stat sb;
                if (lstat(mx_strjoin(mx_strjoin(dirname, "/"), files[i]), &sb) == -1) {
                    perror("Cannot get file information");
                    continue;
                }   
                switch_strcolor(sb);
            }
                mx_printstr(files[i]);
                if(flags->G) mx_printstr(DEFAULT_COLOR);
                if(flags->p && is_dir(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) mx_printchar('/');
                else if(flags->F && is_executable(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) {
                    mx_printchar('*');
                }
                else if(flags->F && is_link(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) {
                    mx_printchar('@');
                }
                else if(flags->F && is_socket(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) {
                    mx_printchar('=');
                }
                else if(flags->F && is_whiteout(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) {
                    mx_printchar('%');
                }
                else if(flags->F && is_fifo(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) {
                    mx_printchar('|');
                }
        mx_printchar('\n');
    }
    for (int i = 0; i < num_files; ++i) {
        free(files[i]);
    }
    free(files);
    closedir(dir);
}

void print_file_entry(const FileEntry *file_entries, int i, t_max_sizes_s mxsize, s_flags_t *flags) {
    mx_printchar(file_entries[i].type);
    for (int j = 0; j < mx_strlen(file_entries[i].permissions); j++) {
        mx_printchar(file_entries[i].permissions[j]);
    }
    mx_printchar(' ');

    if (file_entries[i].nlinks) {
        printint_formatted(file_entries[i].nlinks, mxsize.max_nlinks_len);
    }
    else {
        mx_printstr("  ");
    }

    if (!flags->g) {
        mx_printchar(' ');
        printstr_formatted(mx_char_to_str(file_entries[i].owner), mxsize.max_username_len, true);
    }
    else
        mx_printchar(' ');
    if (!flags->g && !flags->o)
        mx_printstr("  ");
    if (!flags->o) {
        printstr_formatted(mx_char_to_str(file_entries[i].group), mxsize.max_groupname_len, true);
    }
    if (flags->h) {
        mx_printstr("   ");
        if (mxsize.max_h_size_len == 2)
            mx_printstr("  ");
        printstr_formatted(mx_char_to_str(file_entries[i].human_size), mxsize.max_h_size_len, true);
    }
    else {
        (file_entries[i].size == 0) ? mx_printstr(" ") : mx_printstr("  ");
        printint_formatted(file_entries[i].size, mxsize.max_size_len);
    }

    mx_printchar(' ');
    mx_printstr(file_entries[i].modification_time);
    mx_printchar(' ');
                        if(flags->G) {
                struct stat sb;
                if (lstat(file_entries[i].path, &sb) == -1) {
                    perror("Cannot get file information");
                    // continue;
                }   
                switch_strcolor(sb);
            }
                mx_printstr(file_entries[i].name);
                if(flags->G) mx_printstr(DEFAULT_COLOR);
                if(flags->p && is_dir(file_entries[i].path)) mx_printchar('/');
                else if(flags->F && is_executable(file_entries[i].path)) {
                    mx_printchar('*');
                }
                else if(flags->F && is_link(file_entries[i].path)) {
                    mx_printchar('@');
                }
                else if(flags->F && is_socket(file_entries[i].path)) {
                    mx_printchar('=');
                }
                else if(flags->F && is_whiteout(file_entries[i].path)) {
                    mx_printchar('%');
                }
                else if(flags->F && is_fifo(file_entries[i].path)) {
                    mx_printchar('|');
                }

    if (file_entries[i].type == 'l') {
        mx_printstr(" -> ");
        mx_printstr(file_entries[i].symlink);
    }
    mx_printchar('\n');
    if(flags->at && file_entries[i].xattr_keys != NULL) {
        print_xattr(&file_entries[i], flags);
    }
}

void print_longlist(const char *dirname, FileEntry *file_entries, int count, s_flags_t *flags) {
    // while (flags)
    // {
    //     break;
    // }
    int total_blocks = 0;
    struct stat sb;
    char file_path[1024];
    // printf("%s\n", dirname);
    if (lstat(dirname, &sb) == -1) {
        perror("Cannot get directory information");
        exit(1);
    }
    if (!S_ISLNK(sb.st_mode) || dirname[mx_strlen(dirname) - 1] == '/') {
        mx_printstr("total ");

        for (int i = 0; i < count; ++i) {
            // if (file_entries[i].name[0] == '.' && !flags->a)
            //     continue;
            mx_strcpy(file_path, mx_strjoin(mx_strjoin(dirname, "/"), file_entries[i].name));
            if (lstat(file_path, &sb) == -1) {
                perror("Cannot get file information");
                continue;
            }
            total_blocks += (int)(sb.st_blocks);
        }
        mx_printint(total_blocks);
        mx_printchar('\n');
    }
    t_max_sizes_s t_mxsize = {.max_groupname_len = 0, .max_nlinks_len = 0, .max_size_len = 0, .max_h_size_len = 0, .max_username_len = 0};
    for (int i = 0; i < count; ++i) {
        int group_len = mx_strlen(file_entries[i].group);
        int nlinks_len = mx_intlen(file_entries[i].nlinks);
        int size_len = mx_intlen(file_entries[i].size);
        int name_len = mx_strlen(file_entries[i].group);
        int h_size_len = mx_strlen(file_entries[i].human_size);
        if (group_len > t_mxsize.max_groupname_len) {
            t_mxsize.max_groupname_len = group_len;
        }
        if (nlinks_len > t_mxsize.max_nlinks_len) {
            t_mxsize.max_nlinks_len = nlinks_len;
        }
        if (flags->h) {
            if (h_size_len > t_mxsize.max_h_size_len) {
                t_mxsize.max_h_size_len = h_size_len;
            }
        }
        else {
            if (size_len > t_mxsize.max_size_len) {
                t_mxsize.max_size_len = size_len;
            }
        }
        if (name_len > t_mxsize.max_username_len) {
            t_mxsize.max_username_len = name_len;
        }
    }
    for (int i = 0; i < count; ++i) {
        print_file_entry(file_entries, i, t_mxsize, flags);
    }
}

void print_coma(const char *dirname, s_flags_t *flags) {
    struct dirent *dir_entry;
    char **files = NULL;
    int num_files = 0;

    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        mx_printerr("opendir");
        exit(1);
    }
    int terminal_width = 80;
    if (isatty(1)) {
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        terminal_width = w.ws_col;
    }
    int total_width = 0;
    while ((dir_entry = readdir(dir)) != NULL) {
        if ((mx_strcmp(dir_entry->d_name, ".") == 0 || mx_strcmp(dir_entry->d_name, "..") == 0) && flags->A) continue;
        if (dir_entry->d_name[0] == '.' && !flags->a) continue;
        files = mx_realloc(files, (num_files + 1) * sizeof(char *));
        files[num_files] = mx_strdup(dir_entry->d_name);
        num_files++;
    }
    if (num_files > 0)
        if (!flags->f)
            custom_qsort(files, num_files, sizeof(char *), compare_names, flags);
    for (int i = 0; i < num_files; i++) {
        if((flags->p || flags->F) && is_smth(mx_strjoin(mx_strjoin(dirname, "/"), files[i])) 
        && ((total_width + mx_strlen(files[i]) + 2 + 1) >= terminal_width)) {
            mx_printchar('\n');
            total_width = 0;
        }
        else if ((total_width + mx_strlen(files[i]) + 2) >= terminal_width) {
            // mx_printchar('Q');
            mx_printchar('\n');
            total_width = 0;
        }
            if(flags->G) {
                struct stat sb;
                if (lstat(mx_strjoin(mx_strjoin(dirname, "/"), files[i]), &sb) == -1) {
                    perror("Cannot get file information");
                }   
                switch_strcolor(sb);
            }
                mx_printstr(files[i]);
                if(flags->G) mx_printstr(DEFAULT_COLOR);
                if(flags->p && is_dir(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) mx_printchar('/');
                else if(flags->F && is_executable(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) {
                    mx_printchar('*');
                }
                else if(flags->F && is_link(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) {
                    mx_printchar('@');
                }
                else if(flags->F && is_socket(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) {
                    mx_printchar('=');
                }
                else if(flags->F && is_whiteout(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) {
                    mx_printchar('%');
                }
                else if(flags->F && is_fifo(mx_strjoin(mx_strjoin(dirname, "/"), files[i]))) {
                    mx_printchar('|');
                }

        if (i + 1 != num_files) mx_printstr(", ");
        total_width = total_width + mx_strlen(files[i]) + 2;
        if((flags->p || flags->F) && is_smth(mx_strjoin(mx_strjoin(dirname, "/"), files[i])) 
        && ((total_width + mx_strlen(files[i]) + 2 + 1) >= terminal_width)) total_width++;
    }
    mx_printchar('\n');
    for (int i = 0; i < num_files; ++i) {
        free(files[i]);
    }
    free(files);
    closedir(dir);
}
