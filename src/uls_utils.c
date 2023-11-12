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
    char file_path[1024];
    int total_blocks = 0;

    if (!(dir = opendir(dirname))) {
        perror("Cannot open directory");
        return NULL;
    }
    printf("total ");
    // Считаем количество файлов в директории
    *count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && !flags->a) continue;
        snprintf(file_path, sizeof(file_path), "%s/%s", dirname, entry->d_name);
        if (lstat(file_path, &sb) == -1) {
            perror("Cannot get file information");
            continue;
        }
        total_blocks += (int)(sb.st_blocks);
        (*count)++;
    }
    closedir(dir);
    printf("%d\n", total_blocks);
        

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

        snprintf(file_path, sizeof(file_path), "%s/%s", dirname, entry->d_name);

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
        file_entry->type = (S_ISDIR(sb.st_mode)) ? 'd' : '-';
        if (S_ISLNK(sb.st_mode)) {
            file_entry->type = 'l';  // Set type to 'l' for symbolic links
        }
        snprintf(file_entry->permissions, sizeof(file_entry->permissions),
                 "%c%c%c%c%c%c%c%c%c%c%s",
                 (sb.st_mode & S_IRUSR) ? 'r' : '-',
                 (sb.st_mode & S_IWUSR) ? 'w' : '-',
                 (sb.st_mode & S_IXUSR) ? 'x' : '-',
                 (sb.st_mode & S_IRGRP) ? 'r' : '-',
                 (sb.st_mode & S_IWGRP) ? 'w' : '-',
                 (sb.st_mode & S_IXGRP) ? 'x' : '-',
                 (sb.st_mode & S_IROTH) ? 'r' : '-',
                 (sb.st_mode & S_IWOTH) ? 'w' : '-',
                 (sb.st_mode & S_IXOTH) ? 'x' : '-',
                 (char)((listxattr(file_path, NULL, 0, XATTR_NOFOLLOW) > 0) ? '@' : ' '), // Проверка на наличие extended attributes
                (S_ISLNK(sb.st_mode) && readlink(file_path, file_entry->symlink, sizeof(file_entry->symlink)) != -1) ? " -> " : "");

        file_entry->nlinks = (int)sb.st_nlink;

        struct passwd *pw = getpwuid(sb.st_uid);
        struct group *gr = getgrgid(sb.st_gid);

        snprintf(file_entry->owner, sizeof(file_entry->owner), "%s", pw->pw_name);
        snprintf(file_entry->group, sizeof(file_entry->group), "%s", gr->gr_name);

        file_entry->size = sb.st_size;

        struct tm *timeinfo;
        timeinfo = localtime(&sb.st_mtime);
        strftime(file_entry->modification_time, sizeof(file_entry->modification_time), "%b %e %H:%M", timeinfo);
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

void print_file_entries(FileEntry *file_entries, int count) {
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
}
