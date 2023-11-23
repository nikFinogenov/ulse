#include <uls.h>

double custom_round(double value) {
    double result = value + 0.5;
    return (long)result;
}

int compare_names(const void *a, const void *b, bool rev) {
    if (!rev)
        return mx_strcmp(*(const char **)a, *(const char **)b);
    return mx_strcmp(*(const char **)b, *(const char **)a);
}

int compare_file_entries_name(const void *a, const void *b, bool rev) {
    if (!rev)
        return mx_strcmp(((t_file_entry_s *)a)->name, ((t_file_entry_s *)b)->name);
    return mx_strcmp(((t_file_entry_s *)b)->name, ((t_file_entry_s *)a)->name);
}

int compare_file_entries_date_time(const void *a, const void *b, bool rev) {
    if (!rev) {
        if (((t_file_entry_s *)b)->cmptime.tv_sec - ((t_file_entry_s *)a)->cmptime.tv_sec == 0) {
            if (((t_file_entry_s *)b)->cmptime.tv_nsec - ((t_file_entry_s *)a)->cmptime.tv_nsec == 0)
                return mx_strcmp(((t_file_entry_s *)a)->name, ((t_file_entry_s *)b)->name);
            return ((t_file_entry_s *)b)->cmptime.tv_nsec - ((t_file_entry_s *)a)->cmptime.tv_nsec;
        }
        return ((t_file_entry_s *)b)->cmptime.tv_sec - ((t_file_entry_s *)a)->cmptime.tv_sec;
    }
    else {
        if (((t_file_entry_s *)a)->cmptime.tv_sec - ((t_file_entry_s *)b)->cmptime.tv_sec == 0) {
            if (((t_file_entry_s *)a)->cmptime.tv_nsec - ((t_file_entry_s *)b)->cmptime.tv_nsec == 0)
                return mx_strcmp(((t_file_entry_s *)b)->name, ((t_file_entry_s *)a)->name);
            return ((t_file_entry_s *)a)->cmptime.tv_nsec - ((t_file_entry_s *)b)->cmptime.tv_nsec;
        }
        return ((t_file_entry_s *)a)->cmptime.tv_sec - ((t_file_entry_s *)b)->cmptime.tv_sec;
    }
}

int compare_file_entries_size(const void *a, const void *b, bool rev) {
    if (!rev) {
        if (((t_file_entry_s *)b)->size - ((t_file_entry_s *)a)->size == 0)
            return mx_strcmp(((t_file_entry_s *)a)->name, ((t_file_entry_s *)b)->name);
        return ((t_file_entry_s *)b)->size - ((t_file_entry_s *)a)->size;
    }
    else if (((t_file_entry_s *)b)->size - ((t_file_entry_s *)a)->size == 0)
        return mx_strcmp(((t_file_entry_s *)b)->name, ((t_file_entry_s *)a)->name);
    return ((t_file_entry_s *)a)->size - ((t_file_entry_s *)b)->size;
}

bool is_dir(const char *filename) {
    struct stat st;
    if (lstat(filename, &st) == 0) {
        return S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_executable(const char *filename) {
    struct stat st;
    if (lstat(filename, &st) == 0) {
        return st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH) && !S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_fifo(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return S_ISFIFO(st.st_mode) && !S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_link(const char *filename) {
    struct stat st;
    if (lstat(filename, &st) == 0) {
        return S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_socket(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return S_ISSOCK(st.st_mode) && !S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_whiteout(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return S_ISCHR(st.st_mode) && !S_ISLNK(st.st_mode);
    }
    return false;
}

bool is_smth(const char *filename) {
    return is_dir(filename) || is_executable(filename) || is_fifo(filename) || is_link(filename) || is_socket(filename) || is_whiteout(filename);
}

bool is_smth_except_dir(const char *filename) {
    return is_executable(filename) || is_fifo(filename) || is_link(filename) || is_socket(filename) || is_whiteout(filename);
}

void custom_qsort(void *base, size_t num_elements, size_t element_size, int (*comparator)(const void *, const void *, bool), t_flags_s *flags) {
    for (size_t i = 0; i < num_elements - 1; ++i) {
        for (size_t j = i + 1; j < num_elements; ++j) {
            void *element_i = (char *)base + i * element_size;
            void *element_j = (char *)base + j * element_size;
            if (comparator(element_i, element_j, flags->r) > 0) {
                char temp[element_size];
                mx_memcpy(temp, element_i, element_size);
                mx_memcpy(element_i, element_j, element_size);
                mx_memcpy(element_j, temp, element_size);
            }
        }
    }
}

char *format_size(long size) {
    char *suffix[] = {"B", "K", "M", "G", "T"};
    int i;
    double d_size = (double)size;

    for (i = 0; i < 6; i++) {
        if (d_size < 1000) {
            break;
        }
        d_size /= 1024;
    }
    double size_r = custom_round(d_size * 10) / 10;
    char *result = (char *)malloc(20);
    if (size_r >= 10 || i == 0) {
        char *str = mx_lltoa(custom_round(d_size));
        result = mx_strjoin(result, str);
        free(str);
    }
    else {
        char *str = mx_lltoa(size_r);
        result = mx_strjoin(mx_strjoin(result, str), ".");
        free(str);
        str = mx_lltoa((long long)(size_r * 10) % 10);
        result = mx_strjoin(result, str);
        free(str);
    }
    result = mx_strjoin(result, suffix[i]);
    return result;
}

int is_directory_exists(const char *dirname) {
    DIR *dir = opendir(dirname);

    if (dir != NULL) {
        closedir(dir);
        return 1;
    }

    return 0;
}

int is_directory_empty(const char *dirname) {
    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    int file = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (mx_strcmp(entry->d_name, ".") == 0 || mx_strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        file++;
        break;
    }

    closedir(dir);

    return file == 0;
}

int is_file_exists(const char *dirname) {
    struct stat buffer;
    return stat(dirname, &buffer) == 0;
}

void init_flags(t_flags_s *flags) {
    flags->one = false;
    flags->C = false;
    flags->m = false;
    flags->l = false;
    flags->c = false;
    flags->u = false;
    flags->U = false;
    flags->t = false;
    flags->S = false;
    flags->f = false;
    flags->A = false;
    flags->a = false;
    flags->R = false;
    flags->G = false;
    flags->F = false;
    flags->p = false;
    flags->r = false;
    flags->h = false;
    flags->e = false;
    flags->T = false;
    flags->at = false;
    flags->g = false;
    flags->o = false;
    flags->x = false;
    flags->n = false;
    flags->i = false;
}

char **get_xattr(const char *filename) {
    char buffer[1024] = {'\0'};
    ssize_t count = listxattr(filename, buffer, sizeof(buffer), XATTR_NOFOLLOW);
    for (int i = 0; i < count - 1; i++)
        if (buffer[i] == '\0')
            buffer[i] = '|';

    if (count > 0)
        return mx_strsplit(buffer, '|');

    return NULL;
}

t_file_entry_s *fill_entry(const char *name, t_flags_s *flags) {
    struct stat sb;
    if (lstat(name, &sb) == -1) {
        perror("Cannot get directory information");
        return NULL;
    }
    t_file_entry_s *file_entry = malloc(sizeof(t_file_entry_s));
    if (flags->i)
        file_entry->inode = sb.st_ino;
    file_entry->name = mx_strdup(name);
    file_entry->path = mx_strdup(name);
    if (S_ISLNK(sb.st_mode))
        file_entry->type = 'l';
    else if (S_ISSOCK(sb.st_mode))
        file_entry->type = 's';
    else if (S_ISCHR(sb.st_mode))
        file_entry->type = 'c';
    else if (S_ISFIFO(sb.st_mode))
        file_entry->type = 'p';
    else if (S_ISBLK(sb.st_mode))
        file_entry->type = 'b';
    else
        file_entry->type = '-';

    char *new_permissions = NULL;
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IRUSR) ? "r" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWUSR) ? "w" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXUSR) ? "x" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IRGRP) ? "r" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWGRP) ? "w" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXGRP) ? "x" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IROTH) ? "r" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWOTH) ? "w" : "-");
    new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXOTH) ? "x" : "-");
    new_permissions = mx_strjoin(new_permissions, (listxattr(name, NULL, 0, 0x0001) > 0) ? "@" : (acl_get_file(name, ACL_TYPE_EXTENDED)) ? "+"
                                                                                                                                         : " ");

    if (S_ISLNK(sb.st_mode))
        readlink(name, file_entry->symlink, sizeof(file_entry->symlink));
    if ((unsigned long)mx_strlen(new_permissions) < sizeof(file_entry->permissions)) {
        mx_strcpy(file_entry->permissions, new_permissions);
    }
    else {
        perror("Bad permisions");
        exit(1);
    }
    free(new_permissions);
    file_entry->nlinks = (int)sb.st_nlink;

    struct passwd *pw = getpwuid(sb.st_uid);
    struct group *gr = getgrgid(sb.st_gid);
    if (!flags->n) {
        mx_strcpy(file_entry->owner, pw->pw_name);
        mx_strcpy(file_entry->group, gr->gr_name);
    }
    else {
        mx_strcpy(file_entry->owner, mx_itoa(pw->pw_uid));
        mx_strcpy(file_entry->group, mx_itoa(gr->gr_gid));
    }
    if (flags->h) {
        char *new_size = NULL;
        new_size = mx_strjoin(new_size, format_size(sb.st_size));
        mx_strcpy(file_entry->human_size, new_size);
    }
    else {
        file_entry->size = sb.st_size;
    }

    struct timespec timesp = sb.st_mtimespec;
    if (flags->c)
        timesp = sb.st_ctimespec;
    if (flags->u)
        timesp = sb.st_atimespec;
    if (flags->U)
        timesp = sb.st_birthtimespec;
    if (flags->t)
        file_entry->cmptime = timesp;
    char *str = ctime(&timesp.tv_sec);
    char **arr = mx_strsplit(str, ' ');
    time_t now = time(NULL);
    time_t six_months_sec = (365 / 2) * 24 * 60 * 60;
    arr[4][4] = '\0';
    char *new_date_time = NULL;
    if (flags->T) {
        for (int i = 1; arr[i] != NULL; i++) {
            int spaces = 2 - mx_strlen(arr[i]);
            for (int i = 0; i < spaces; i++) {
                new_date_time = mx_strjoin(new_date_time, " ");
            }
            new_date_time = mx_strjoin(new_date_time, arr[i]);

            if (arr[i + 1] != NULL) {
                new_date_time = mx_strjoin(new_date_time, " ");
            }
        }
    }
    else if (timesp.tv_sec + six_months_sec <= now || timesp.tv_sec >= now + six_months_sec) {
        new_date_time = mx_strjoin(new_date_time, arr[1]);
        new_date_time = mx_strjoin(new_date_time, " ");
        int spaces = 2 - mx_strlen(arr[2]);
        for (int i = 0; i < spaces; i++) {
            new_date_time = mx_strjoin(new_date_time, " ");
        }
        new_date_time = mx_strjoin(new_date_time, arr[2]);
        new_date_time = mx_strjoin(new_date_time, "  ");
        new_date_time = mx_strjoin(new_date_time, arr[4]);
    }
    else {
        new_date_time = mx_strjoin(new_date_time, arr[1]);
        new_date_time = mx_strjoin(new_date_time, " ");

        int spaces = 2 - mx_strlen(arr[2]);
        for (int i = 0; i < spaces; i++) {
            new_date_time = mx_strjoin(new_date_time, " ");
        }
        new_date_time = mx_strjoin(new_date_time, arr[2]);

        new_date_time = mx_strjoin(new_date_time, " ");
        char **arr_time = mx_strsplit(arr[3], ':');
        new_date_time = mx_strjoin(new_date_time, arr_time[0]);
        new_date_time = mx_strjoin(new_date_time, ":");
        new_date_time = mx_strjoin(new_date_time, arr_time[1]);
        mx_del_strarr(&arr_time);
    }

    mx_del_strarr(&arr);
    if ((unsigned long)mx_strlen(new_date_time) <= sizeof(file_entry->date_time)) {
        mx_strcpy(file_entry->date_time, new_date_time);
    }
    else {
        perror("Bad time");
        exit(1);
    }
    free(new_date_time);

    if (S_ISLNK(sb.st_mode)) {
        ssize_t len = readlink(name, file_entry->symlink, sizeof(file_entry->symlink) - 1);
        if (len != -1) {
            file_entry->symlink[len] = '\0';
        }
        else {
            perror("readlink");
        }
    }
    else {
        file_entry->symlink[0] = '\0';
    }
    file_entry->xattr_keys = get_xattr(name);

    return file_entry;
}

void switch_strcolor(struct stat sb) {
    switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
        mx_printstr(BLUE_COLOR);
        break;

    case S_IFCHR:
        mx_printstr(BLUE_COLOR);
        break;

    case S_IFDIR:
        mx_printstr(BLUE_COLOR);
        break;

    case S_IFIFO:
        mx_printstr(YELLOW_COLOR);
        break;

    case S_IFLNK:
        mx_printstr(MAGENTA_COLOR);
        break;

    case S_IFSOCK:
        mx_printstr(GREEN_COLOR);
        break;

    default:
        if (sb.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
            mx_printstr(RED_COLOR);
        }
        break;
    }
}

t_file_entry_s *fill_file_entries(const char *dirname, int *count, t_flags_s *flags) {
    DIR *dir;
    struct dirent *entry;
    struct stat sb;
    char *file_path;
    if (!(dir = opendir(dirname))) {
        perror("Cannot open directory");
        return NULL;
    }
    *count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if ((mx_strcmp(entry->d_name, ".") == 0 || mx_strcmp(entry->d_name, "..") == 0) && flags->A)
            continue;
        if (entry->d_name[0] == '.' && !flags->a)
            continue;
        (*count)++;
    }
    closedir(dir);

    t_file_entry_s *file_entries = malloc(*count * sizeof(t_file_entry_s));
    if (file_entries == NULL) {
        perror("malloc");
        return NULL;
    }

    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Cannot open directory");
        free(file_entries);
        return NULL;
    }

    int index = 0;
    while ((entry = readdir(dir)) != NULL) {
        if ((mx_strcmp(entry->d_name, ".") == 0 || mx_strcmp(entry->d_name, "..") == 0) && flags->A)
            continue;
        if (entry->d_name[0] == '.' && !flags->a)
            continue;

        file_path = mx_strjoin(mx_strjoin(dirname, "/"), entry->d_name);

        if (lstat(file_path, &sb) == -1) {
            perror("Cannot get file information");
            continue;
        }
        t_file_entry_s *file_entry = &file_entries[index];
        if (flags->i)
            file_entry->inode = sb.st_ino;
        file_entry->name = mx_strdup(entry->d_name);
        file_entry->path = mx_strdup(file_path);
        if (file_entry->name == NULL) {
            perror("strdup");
            free(file_entries->name);
            closedir(dir);
            return NULL;
        }
        if (S_ISDIR(sb.st_mode))
            file_entry->type = 'd';
        else if (S_ISLNK(sb.st_mode))
            file_entry->type = 'l';
        else if (S_ISSOCK(sb.st_mode))
            file_entry->type = 's';
        else if (S_ISCHR(sb.st_mode))
            file_entry->type = 'c';
        else if (S_ISFIFO(sb.st_mode))
            file_entry->type = 'p';
        else if (S_ISBLK(sb.st_mode))
            file_entry->type = 'b';
        else
            file_entry->type = '-';

        char *new_permissions = NULL;
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IRUSR) ? "r" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWUSR) ? "w" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXUSR) ? "x" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IRGRP) ? "r" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWGRP) ? "w" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXGRP) ? "x" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IROTH) ? "r" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IWOTH) ? "w" : "-");
        new_permissions = mx_strjoin(new_permissions, (sb.st_mode & S_IXOTH) ? "x" : "-");
        new_permissions = mx_strjoin(new_permissions, (listxattr(file_path, NULL, 0, XATTR_NOFOLLOW) > 0) ? "@" : (acl_get_file(file_path, ACL_TYPE_EXTENDED)) ? "+"
                                                                                                                                                               : " ");
        if (S_ISLNK(sb.st_mode))
            readlink(file_path, file_entry->symlink, sizeof(file_entry->symlink));
        if ((unsigned long)mx_strlen(new_permissions) < sizeof(file_entry->permissions)) {
            mx_strcpy(file_entry->permissions, new_permissions);
        }
        else {
            perror("Bad permisions");
            exit(1);
        }
        free(new_permissions);
        file_entry->nlinks = (int)sb.st_nlink;

        struct passwd *pw = getpwuid(sb.st_uid);
        struct group *gr = getgrgid(sb.st_gid);
        if (!flags->n) {
            mx_strcpy(file_entry->owner, pw->pw_name);
            mx_strcpy(file_entry->group, gr->gr_name);
        }
        else {
            mx_strcpy(file_entry->owner, mx_itoa(pw->pw_uid));
            mx_strcpy(file_entry->group, mx_itoa(gr->gr_gid));
        }
        if (flags->h) {
            char *new_size = NULL;
            new_size = mx_strjoin(new_size, format_size(sb.st_size));
            mx_strcpy(file_entry->human_size, new_size);
        }
        else {
            file_entry->size = sb.st_size;
        }

        struct timespec timesp = sb.st_mtimespec;
        if (flags->c)
            timesp = sb.st_ctimespec;
        if (flags->u)
            timesp = sb.st_atimespec;
        if (flags->U)
            timesp = sb.st_birthtimespec;
        if (flags->t)
            file_entry->cmptime = timesp;
        char *str = ctime(&timesp.tv_sec);
        char **arr = mx_strsplit(str, ' ');
        time_t now = time(NULL);
        time_t six_months_sec = (365 / 2) * 24 * 60 * 60;
        arr[4][4] = '\0';
        char *new_date_time = NULL;
        if (flags->T) {

            for (int i = 1; arr[i] != NULL; i++) {

                int spaces = 2 - mx_strlen(arr[i]);
                for (int i = 0; i < spaces; i++) {
                    new_date_time = mx_strjoin(new_date_time, " ");
                }
                new_date_time = mx_strjoin(new_date_time, arr[i]);

                if (arr[i + 1] != NULL) {
                    new_date_time = mx_strjoin(new_date_time, " ");
                }
            }
        }
        else if (timesp.tv_sec + six_months_sec <= now || timesp.tv_sec >= now + six_months_sec) {
            new_date_time = mx_strjoin(new_date_time, arr[1]);
            new_date_time = mx_strjoin(new_date_time, " ");
            int spaces = 2 - mx_strlen(arr[2]);
            for (int i = 0; i < spaces; i++) {
                new_date_time = mx_strjoin(new_date_time, " ");
            }
            new_date_time = mx_strjoin(new_date_time, arr[2]);
            new_date_time = mx_strjoin(new_date_time, "  ");
            new_date_time = mx_strjoin(new_date_time, arr[4]);
        }
        else {
            new_date_time = mx_strjoin(new_date_time, arr[1]);
            new_date_time = mx_strjoin(new_date_time, " ");

            int spaces = 2 - mx_strlen(arr[2]);
            for (int i = 0; i < spaces; i++) {
                new_date_time = mx_strjoin(new_date_time, " ");
            }
            new_date_time = mx_strjoin(new_date_time, arr[2]);

            new_date_time = mx_strjoin(new_date_time, " ");
            char **arr_time = mx_strsplit(arr[3], ':');
            new_date_time = mx_strjoin(new_date_time, arr_time[0]);
            new_date_time = mx_strjoin(new_date_time, ":");
            new_date_time = mx_strjoin(new_date_time, arr_time[1]);
            mx_del_strarr(&arr_time);
        }

        mx_del_strarr(&arr);
        if ((unsigned long)mx_strlen(new_date_time) <= sizeof(file_entry->date_time)) {
            mx_strcpy(file_entry->date_time, new_date_time);
        }
        else {
            perror("Bad time");
            exit(1);
        }
        free(new_date_time);

        if (S_ISLNK(sb.st_mode)) {
            ssize_t len = readlink(file_path, file_entry->symlink, sizeof(file_entry->symlink) - 1);
            if (len != -1) {
                file_entry->symlink[len] = '\0';
            }
            else {
                perror("readlink");
            }
        }
        else {
            file_entry->symlink[0] = '\0';
        }
        index++;

        file_entry->xattr_keys = get_xattr(file_path);
    }

    closedir(dir);

    return file_entries;
}
