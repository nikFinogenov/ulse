#include <uls.h>

int compare_names(const void *a, const void *b) {
    return mx_strcmp(*(const char **)a, *(const char **)b);
}

int compare_file_entries(const void *a, const void *b) {
    return strcmp(((FileEntry *)a)->name, ((FileEntry *)b)->name);
}

void custom_qsort(void *base, size_t num_elements, size_t element_size, int (*comparator)(const void *, const void *)) {
    for (size_t i = 0; i < num_elements - 1; ++i) {
        for (size_t j = i + 1; j < num_elements; ++j) {
            void *element_i = (char *)base + i * element_size;
            void *element_j = (char *)base + j * element_size;
            if (comparator(element_i, element_j) > 0) {
                char temp[element_size];
                mx_memcpy(temp, element_i, element_size);
                mx_memcpy(element_i, element_j, element_size);
                mx_memcpy(element_j, temp, element_size);
            }
        }
    }
}

void init_flags(s_flags_t *flags) {
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
    flags->g= false;
    flags->o = false;
    flags->x = false;
    flags->n = false;
    flags->q = false;
    flags->H = false;
}

FileEntry *fill_link_entry(const char *linkname, s_flags_t *flags) {
    while(flags) break;
    struct stat sb;
    if (lstat(linkname, &sb) == -1) {
        perror("Cannot get directory information");
        return NULL;
    }
    FileEntry *file_entry = malloc(sizeof(FileEntry));
    file_entry->name = mx_strdup(linkname);
    file_entry->type = 'l';
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
        new_permissions = mx_strjoin(new_permissions, (listxattr(linkname, NULL, 0, 0x0001) > 0) ? "@" : (acl_get_file(linkname, ACL_TYPE_EXTENDED)) ? "+" : " ");
        if (S_ISLNK(sb.st_mode)) readlink(linkname, file_entry->symlink, sizeof(file_entry->symlink));
        if ((unsigned long)mx_strlen(new_permissions) < sizeof(file_entry->permissions)) {
        mx_strcpy(file_entry->permissions, new_permissions);
        } else {
            perror("Bad permisions");
            exit(1);
        }
        free(new_permissions);
        file_entry->nlinks = (int)sb.st_nlink;

        struct passwd *pw = getpwuid(sb.st_uid);
        struct group *gr = getgrgid(sb.st_gid);
        mx_strcpy(file_entry->owner, pw->pw_name);
        mx_strcpy(file_entry->group, gr->gr_name);
        file_entry->size = sb.st_size;


    struct timespec timesp = sb.st_mtimespec;
    char *str = ctime(&timesp.tv_sec);
    char **arr = mx_strsplit(str, ' ');
    time_t now = time(NULL);
    time_t six_months_sec = (365 / 2) * 24 * 60 * 60;
    arr[4][4] = '\0';
    char *new_modification_time = NULL;
    if (timesp.tv_sec + six_months_sec <= now
        || timesp.tv_sec >= now + six_months_sec) {
        new_modification_time = mx_strjoin(new_modification_time, arr[1]);
        new_modification_time = mx_strjoin(new_modification_time, " ");
        int spaces = 2 - mx_strlen(arr[2]);
        for (int i = 0; i < spaces; i++) {
            new_modification_time = mx_strjoin(new_modification_time, " ");
        }
        new_modification_time = mx_strjoin(new_modification_time, arr[2]);
        new_modification_time = mx_strjoin(new_modification_time, "  ");
        new_modification_time = mx_strjoin(new_modification_time, arr[4]);
    }
    else {
        new_modification_time = mx_strjoin(new_modification_time, arr[1]);
        new_modification_time = mx_strjoin(new_modification_time, " ");

        int spaces = 2 - mx_strlen(arr[2]);
        for (int i = 0; i < spaces; i++) {
            new_modification_time = mx_strjoin(new_modification_time, " ");
        }
        new_modification_time = mx_strjoin(new_modification_time, arr[2]);

        new_modification_time = mx_strjoin(new_modification_time, " ");
        char **arr_time = mx_strsplit(arr[3], ':');
        new_modification_time = mx_strjoin(new_modification_time, arr_time[0]);
        new_modification_time = mx_strjoin(new_modification_time, ":");
        new_modification_time = mx_strjoin(new_modification_time, arr_time[1]);
        mx_del_strarr(&arr_time);
    }

    mx_del_strarr(&arr);
    if (strlen(new_modification_time) < sizeof(file_entry->modification_time)) {
    strcpy(file_entry->modification_time, new_modification_time);
    } else {
        perror("Bad time");
        exit(1);
    }
    free(new_modification_time);

        if (S_ISLNK(sb.st_mode)) {
            ssize_t len = readlink(linkname, file_entry->symlink, sizeof(file_entry->symlink) - 1);
            if (len != -1) {
                file_entry->symlink[len] = '\0';
            } else {
                perror("readlink");
            }
        } else {
            file_entry->symlink[0] = '\0';
        }

    // closedir(dir);

    return file_entry;
}

FileEntry *fill_file_entries(const char *dirname, int *count, s_flags_t *flags) {
    // struct stat dir_sb;
    // if (lstat(dirname, &dir_sb) == -1) {
    //     perror("Cannot get directory information");
    //     return NULL;
    // }
    DIR *dir;
    struct dirent *entry;
    struct stat sb;
    char *file_path;
    if (!(dir = opendir(dirname))) {
        perror("Cannot open directory");
        return NULL;
    }
    // if (S_ISLNK(dir_sb.st_mode))  {
    // }
    *count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && !flags->a) continue;
        (*count)++;
    }
    closedir(dir);
        
    FileEntry *file_entries = malloc(*count * sizeof(FileEntry));
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
        if (entry->d_name[0] == '.' && !flags->a) continue;

        file_path = mx_strjoin(mx_strjoin(dirname, "/"), entry->d_name);

        if (lstat(file_path, &sb) == -1) {
            perror("Cannot get file information");
            continue;
        }

        FileEntry *file_entry = &file_entries[index];

        file_entry->name = strdup(entry->d_name);
        if (file_entry->name == NULL) {
            perror("strdup");
            free(file_entries->name);
            closedir(dir);
            return NULL;
        }

        if (S_ISDIR(sb.st_mode)) file_entry->type = 'd';
        else if (S_ISLNK(sb.st_mode)) file_entry->type = 'l';
        else if (S_ISSOCK(sb.st_mode)) file_entry->type = 's';
        else if (S_ISCHR(sb.st_mode)) file_entry->type = 'c';
        else if (S_ISFIFO(sb.st_mode)) file_entry->type = 'p';
        else if (S_ISBLK(sb.st_mode)) file_entry->type = 'b';
        else file_entry->type = '-';

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
        new_permissions = mx_strjoin(new_permissions, (listxattr(file_path, NULL, 0, 0x0001) > 0) ? "@" : (acl_get_file(file_path, ACL_TYPE_EXTENDED)) ? "+" : " ");
        if (S_ISLNK(sb.st_mode)) readlink(file_path, file_entry->symlink, sizeof(file_entry->symlink));
        if ((unsigned long)mx_strlen(new_permissions) < sizeof(file_entry->permissions)) {
        mx_strcpy(file_entry->permissions, new_permissions);
        } else {
            perror("Bad permisions");
            exit(1);
        }
        free(new_permissions);
        file_entry->nlinks = (int)sb.st_nlink;

        struct passwd *pw = getpwuid(sb.st_uid);
        struct group *gr = getgrgid(sb.st_gid);
        mx_strcpy(file_entry->owner, pw->pw_name);
        mx_strcpy(file_entry->group, gr->gr_name);
        file_entry->size = sb.st_size;


    struct timespec timesp = sb.st_mtimespec;
    char *str = ctime(&timesp.tv_sec);
    char **arr = mx_strsplit(str, ' ');
    time_t now = time(NULL);
    time_t six_months_sec = (365 / 2) * 24 * 60 * 60;
    arr[4][4] = '\0';
    char *new_modification_time = NULL;
    if (timesp.tv_sec + six_months_sec <= now
        || timesp.tv_sec >= now + six_months_sec) {
        new_modification_time = mx_strjoin(new_modification_time, arr[1]);
        new_modification_time = mx_strjoin(new_modification_time, " ");
        int spaces = 2 - mx_strlen(arr[2]);
        for (int i = 0; i < spaces; i++) {
            new_modification_time = mx_strjoin(new_modification_time, " ");
        }
        new_modification_time = mx_strjoin(new_modification_time, arr[2]);
        new_modification_time = mx_strjoin(new_modification_time, "  ");
        new_modification_time = mx_strjoin(new_modification_time, arr[4]);
    }
    else {
        new_modification_time = mx_strjoin(new_modification_time, arr[1]);
        new_modification_time = mx_strjoin(new_modification_time, " ");

        int spaces = 2 - mx_strlen(arr[2]);
        for (int i = 0; i < spaces; i++) {
            new_modification_time = mx_strjoin(new_modification_time, " ");
        }
        new_modification_time = mx_strjoin(new_modification_time, arr[2]);

        new_modification_time = mx_strjoin(new_modification_time, " ");
        char **arr_time = mx_strsplit(arr[3], ':');
        new_modification_time = mx_strjoin(new_modification_time, arr_time[0]);
        new_modification_time = mx_strjoin(new_modification_time, ":");
        new_modification_time = mx_strjoin(new_modification_time, arr_time[1]);
        mx_del_strarr(&arr_time);
    }

    mx_del_strarr(&arr);
    if (strlen(new_modification_time) < sizeof(file_entry->modification_time)) {
    strcpy(file_entry->modification_time, new_modification_time);
    } else {
        perror("Bad time");
        exit(1);
    }
    free(new_modification_time);

        if (S_ISLNK(sb.st_mode)) {
            ssize_t len = readlink(file_path, file_entry->symlink, sizeof(file_entry->symlink) - 1);
            if (len != -1) {
                file_entry->symlink[len] = '\0';
            } else {
                perror("readlink");
            }
        } else {
            file_entry->symlink[0] = '\0';
        }
        index++;
    }

    closedir(dir);

    return file_entries;
}
