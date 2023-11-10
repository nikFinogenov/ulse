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
void recursive_flag(char *path, s_flags_t *flags) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path))) {
        perror("opendir");
        return;
    }


    // First, list all files and directories in the current directory
    if (flags->one) {
        printf("%s:\n", path);
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] == '.') // Skip hidden files
                continue;

            printf("%s\n", entry->d_name);
        }
    }

    // Then, rewind the directory stream to start from the beginning
    rewinddir(dir);

    // Now, process directories recursively
    if (flags->one) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] == '.' || entry->d_type != DT_DIR) // Skip hidden files and non-directories
                continue;

            printf("\n");
            char sub_path[1024];
            snprintf(sub_path, sizeof(sub_path), "%s/%s", path, entry->d_name);
            recursive_flag(sub_path, flags);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *dirname;
    s_flags_t flags;
    initFalseFlags(&flags);
    bool with_flags = false;
    if (argc >= 2 && argv[1][0] == '-') { 
        int count = parse_args(argc, argv, &flags);
        with_flags = true;
        dirname = argv[count + 1];
    } 
    else if (argc == 1) {
        dirname = "."; // Use current directory if no argument is provided
    } else if (argc == 2 && argv[1][0] != '-') {
        dirname = argv[1];
    } else {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (!with_flags){
        print_multicolumn(dirname);
    }
    else{
        if(flags.C)
            print_multicolumn(dirname);
        else if (flags.one)
            print_perline(dirname);
        else if (flags.R) {
            recursive_flag(dirname, &flags);
        }
    }
    return 0;
}
