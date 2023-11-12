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
    while (flags)
    {
        break;
    }
    
    DIR *dir;
    struct dirent *entry;
    struct stat sb;
    char file_path[1024];

    if (!(dir = opendir(dirname))) {
        perror("Cannot open directory");
        return;
    }

    printf("total ");
    
    // Calculate total blocks
    int total_blocks = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        snprintf(file_path, sizeof(file_path), "%s/%s", dirname, entry->d_name);

        if (lstat(file_path, &sb) == -1) {
            perror("Cannot get file information");
            continue;
        }
        total_blocks += (int)(sb.st_blocks / 2);
    }
    closedir(dir);
    printf("%d\n", total_blocks);

    // Reopen directory
    if (!(dir = opendir(dirname))) {
        perror("Cannot open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        snprintf(file_path, sizeof(file_path), "%s/%s", dirname, entry->d_name);

        if (lstat(file_path, &sb) == -1) {
            perror("Cannot get file information");
            continue;
        }

        printf("%c%c%c%c%c%c%c%c%c%c ",
            (S_ISDIR(sb.st_mode)) ? 'd' : '-',
            (sb.st_mode & S_IRUSR) ? 'r' : '-',
            (sb.st_mode & S_IWUSR) ? 'w' : '-',
            (sb.st_mode & S_IXUSR) ? 'x' : '-',
            (sb.st_mode & S_IRGRP) ? 'r' : '-',
            (sb.st_mode & S_IWGRP) ? 'w' : '-',
            (sb.st_mode & S_IXGRP) ? 'x' : '-',
            (sb.st_mode & S_IROTH) ? 'r' : '-',
            (sb.st_mode & S_IWOTH) ? 'w' : '-',
            (sb.st_mode & S_IXOTH) ? 'x' : '-');

        printf("%2d ", (int)sb.st_nlink);

        struct passwd *pw = getpwuid(sb.st_uid);
        struct group *gr = getgrgid(sb.st_gid);

        printf("%s %s ", pw->pw_name, gr->gr_name);

        printf("%7ld ", (long)sb.st_size);

        struct tm *timeinfo;
        timeinfo = localtime(&sb.st_mtime);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%b %d %H:%M", timeinfo);
        printf("%s ", buffer);

        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}
