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
    flags->a = false;
    flags->A = false;
    flags->l = false;
    flags->r = false;
    flags->R = false;
    flags->t = false;
    flags->u = false;
    flags->c = false;
    flags->G = false;
    flags->h = false;
    flags->e = false;
    flags->i = false;
    flags->S = false;
    flags->T = false;
    flags->x = false;
    flags->p = false;
    flags->d = false;
    flags->f = false;
    flags->n = false;
    flags->g = false;
    flags->o = false;
    flags->L = false;
    flags->F = false;
    flags->one = false;
    flags->C = false;
    flags->B = false;
    flags->s = false;
    flags->X = false;
    flags->v = false;
    flags->w = false;
    flags->D = false;
    flags->P = false;
    flags->Q = false;
    flags->at = false;
}

void free_file_entry(FileEntry *entry) {
    free(entry->name);
}
FileEntry *fill_file_entries(const char *dirname, int *count, s_flags_t *flags) {
    DIR *dir;
    struct dirent *entry;
    struct stat sb;
    char *file_path;
    // printf("")
    if (!(dir = opendir(dirname))) {
        perror("Cannot open directory");
        return NULL;
    }
    // Считаем количество файлов в директории
    *count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && !flags->a) continue;
        (*count)++;
    }
    closedir(dir);
        
    // Выделяем память под массив FileEntry
    FileEntry *file_entries = malloc(*count * sizeof(FileEntry));
    if (file_entries == NULL) {
        perror("malloc");
        return NULL;
    }

    // Заполняем массив FileEntry
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Cannot open directory");
        free(file_entries);
        return NULL;
    }

    int index = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && !flags->a) continue;

        // snprintf(file_path, sizeof(file_path), "%s/%s", dirname, entry->d_name);
        file_path = mx_strjoin(mx_strjoin(dirname, "/"), entry->d_name);

        if (lstat(file_path, &sb) == -1) {
            perror("Cannot get file information");
            continue;
        }

        FileEntry *file_entry = &file_entries[index];

        // Выделение памяти и копирование имени
        file_entry->name = strdup(entry->d_name);
        if (file_entry->name == NULL) {
            perror("strdup");
            free_file_entry(file_entries);
            closedir(dir);
            return NULL;
        }

        // Заполнение остальных полей
        if (S_ISDIR(sb.st_mode)) file_entry->type = 'd';
        else if (S_ISLNK(sb.st_mode)) file_entry->type = 'l';
        else if (S_ISSOCK(sb.st_mode)) file_entry->type = 's';
        else if (S_ISCHR(sb.st_mode)) file_entry->type = 'c';
        else if (S_ISFIFO(sb.st_mode)) file_entry->type = 'p';
        else if (S_ISBLK(sb.st_mode)) file_entry->type = 'b';
        else file_entry->type = '-';

        file_entry->permissions = mx_strjoin(file_entry->permissions, (sb.st_mode & S_IRUSR) ? "r" : "-");
        file_entry->permissions = mx_strjoin(file_entry->permissions, (sb.st_mode & S_IWUSR) ? "w" : "-");
        file_entry->permissions = mx_strjoin(file_entry->permissions, (sb.st_mode & S_IXUSR) ? "x" : "-");
        file_entry->permissions = mx_strjoin(file_entry->permissions, (sb.st_mode & S_IRGRP) ? "r" : "-");
        file_entry->permissions = mx_strjoin(file_entry->permissions, (sb.st_mode & S_IWGRP) ? "w" : "-");
        file_entry->permissions = mx_strjoin(file_entry->permissions, (sb.st_mode & S_IXGRP) ? "x" : "-");
        file_entry->permissions = mx_strjoin(file_entry->permissions, (sb.st_mode & S_IROTH) ? "r" : "-");
        file_entry->permissions = mx_strjoin(file_entry->permissions, (sb.st_mode & S_IWOTH) ? "w" : "-");
        file_entry->permissions = mx_strjoin(file_entry->permissions, (sb.st_mode & S_IXOTH) ? "x" : "-");
        file_entry->permissions = mx_strjoin(file_entry->permissions, (listxattr(file_path, NULL, 0, 0x0001) > 0) ? "@" : (acl_get_file(file_path, ACL_TYPE_EXTENDED)) ? "+" : " ");
        // file_entry->permissions = mx_strjoin(file_entry->permissions, (S_ISLNK(sb.st_mode) && readlink(file_path, file_entry->symlink, sizeof(file_entry->symlink)) != -1) ? "" : "");
        if (S_ISLNK(sb.st_mode)) readlink(file_path, file_entry->symlink, sizeof(file_entry->symlink));

        file_entry->nlinks = (int)sb.st_nlink;

        struct passwd *pw = getpwuid(sb.st_uid);
        struct group *gr = getgrgid(sb.st_gid);
        file_entry->owner = pw->pw_name;
        file_entry->group = gr->gr_name;
        file_entry->size = sb.st_size;


    struct timespec timesp = sb.st_mtimespec;
    char *str = ctime(&timesp.tv_sec);
    char **arr = mx_strsplit(str, ' ');
    time_t now = time(NULL);
    time_t six_months_sec = (365 / 2) * 24 * 60 * 60;
    arr[4][4] = '\0';
    
    if (timesp.tv_sec + six_months_sec <= now
        || timesp.tv_sec >= now + six_months_sec) {
        file_entry->modification_time = mx_strjoin(file_entry->modification_time, arr[1]);
        file_entry->modification_time = mx_strjoin(file_entry->modification_time, " ");
        int spaces = 2 - mx_strlen(arr[2]);
        for (int i = 0; i < spaces; i++) {
            file_entry->modification_time = mx_strjoin(file_entry->modification_time, " ");
        }
        file_entry->modification_time = mx_strjoin(file_entry->modification_time, arr[2]);
        file_entry->modification_time = mx_strjoin(file_entry->modification_time, "  ");
        file_entry->modification_time = mx_strjoin(file_entry->modification_time, arr[4]);
    }
    else {
        file_entry->modification_time = mx_strjoin(file_entry->modification_time, arr[1]);
        file_entry->modification_time = mx_strjoin(file_entry->modification_time, " ");

        int spaces = 2 - mx_strlen(arr[2]);
        for (int i = 0; i < spaces; i++) {
            file_entry->modification_time = mx_strjoin(file_entry->modification_time, " ");
        }
        file_entry->modification_time = mx_strjoin(file_entry->modification_time, arr[2]);

        file_entry->modification_time = mx_strjoin(file_entry->modification_time, " ");
        char **arr_time = mx_strsplit(arr[3], ':');
        file_entry->modification_time = mx_strjoin(file_entry->modification_time, arr_time[0]);
        file_entry->modification_time = mx_strjoin(file_entry->modification_time, ":");
        file_entry->modification_time = mx_strjoin(file_entry->modification_time, arr_time[1]);
        mx_del_strarr(&arr_time);
    }

    mx_del_strarr(&arr);


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
