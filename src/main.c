#include <stdio.h> // убрать потом
#include <uls.h>
bool separate_flags(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        for (int j = 0; argv[i][j]; j++) {
            if (argv[i][j] == '-') {
                return true;
            }
        }
    }
    return false;
}

void initFalseFlags(s_flags_t *flags) {
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
void recursive_flag(const char *path, s_flags_t *flags) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path))) {
        perror("opendir");
        mx_printstr(path);
        mx_printchar('\n');
        return;
    }

    if(flags->C)
        print_multicolumn(path, flags);
    else if (flags->one)
        print_perline(path, flags);
    else 
        print_multicolumn(path, flags);

    // Then, rewind the directory stream to start from the beginning
    closedir(dir);
    dir = opendir(path);

    // Now, process directories recursively
    // if (flags->one) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] == '.' || entry->d_type != DT_DIR) // Skip hidden files and non-directories
                continue;

            printf("\n");
            char *sub_path;
            if (path[mx_strlen(path) - 1] == '/')
                sub_path = mx_strjoin(path, entry->d_name);
            else {
                sub_path = mx_strjoin(mx_strjoin(path, "/"), entry->d_name);
            }
            mx_printstr(path);
            mx_printchar('/');
            mx_printstr(entry->d_name);
            mx_printchar(':');
            mx_printchar('\n');
            // snprintf(sub_path, sizeof(sub_path), "%s%s", path, entry->d_name);
            recursive_flag(sub_path, flags);
        }
    // }

    closedir(dir);
}

void ls_l(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat sb;
    char file_path[1024];

    if (!(dir = opendir(path))) {
        perror("Cannot open directory");
        return;
    }

    printf("total ");
    
    // Calculate total blocks
    int total_blocks = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);

        if (lstat(file_path, &sb) == -1) {
            perror("Cannot get file information");
            continue;
        }
        total_blocks += (int)(sb.st_blocks / 2);
    }
    closedir(dir);
    printf("%d\n", total_blocks);

    // Reopen directory
    if (!(dir = opendir(path))) {
        perror("Cannot open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);

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


int main(int argc, char *argv[]) {
    const char *dirname;
    s_flags_t flags;
    initFalseFlags(&flags);
    // bool with_flags = false;
    // bool with_dir = false;
    if (argc >= 2 && argv[1][0] == '-') { 
        int count = parse_args(argc, argv, &flags);
        // with_flags = true;
        if (count == argc - 1) dirname = ".";
        else{
            dirname = argv[count + 1];
            // with_dir = true;
        }
    } 
    else if (argc == 1) {
        dirname = "."; // Use current directory if no argument is provided
    } else if (argc == 2 && argv[1][0] != '-') {
        dirname = argv[1];
    } else {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // if (!with_flags){
    //     print_multicolumn(dirname, &flags);
    // }
    // else{
        if(flags.R)
            recursive_flag(dirname, &flags);
        else if (flags.l)
            ls_l(dirname);
        else if (flags.C)
            print_multicolumn(dirname, &flags);
        else if (flags.one) {
            print_perline(dirname, &flags);
        }
        else
            print_multicolumn(dirname, &flags);
    // }
    return 0;
}
