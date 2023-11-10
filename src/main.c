#include <stdio.h>
#include <stdlib.h>
#include <uls.h>

static int compare_names(const void *a, const void *b) {
    return mx_strcmp(*(const char **)a, *(const char **)b);
}

static void custom_qsort(void *base, size_t num_elements, size_t element_size, int (*comparator)(const void *, const void *)) {
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

static void print_multicolumn(const char *dirname) {
    struct dirent *dir_entry;
    char **files = NULL;
    int num_files = 0;

    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        mx_printerr("opendir");
        exit(1);
    }

    while ((dir_entry = readdir(dir)) != NULL) {
        if (dir_entry->d_name[0] != '.') {
            files = mx_realloc(files, (num_files + 1) * sizeof(char *));
            files[num_files] = mx_strdup(dir_entry->d_name);
            num_files++;
        }
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

int parse_args(int argc, char *argv[], s_flags_t *flags) {
    int i;
    int count = 0;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j]; j++) {
                switch (argv[i][j]) {
                    case 'a':
                        flags->a = true;
                        break;
                    case 'A':
                        flags->A = true;
                        break;
                    case 'l':
                        flags->l = true;
                        break;
                    case 'r':
                        flags->r = true;
                        break;
                    case 'R':
                        flags->R = true;
                        break;
                    case 't':
                        flags->t = true;
                        break;
                    case 'u':
                        flags->u = true;
                        break;
                    case 'c':
                        flags->c = true;
                        break;
                    case 'G':
                        flags->G = true;
                        break;
                    case 'h':
                        flags->h = true;
                        break;
                    case 'e':
                        flags->e = true;
                        break;
                    case 'i':
                        flags->i = true;
                        break;
                    case 'S':
                        flags->S = true;
                        break;
                    case 'T':
                        flags->T = true;
                        break;
                    case 'x':
                        flags->x = true;
                        break;
                    case 'p':
                        flags->p = true;
                        break;
                    case 'd':
                        flags->d = true;
                        break;
                    case 'f':
                        flags->f = true;
                        break;
                    case 'n':
                        flags->n = true;
                        break;
                    case 'g':
                        flags->g = true;
                        break;
                    case 'o':
                        flags->o = true;
                        break;
                    case 'L':
                        flags->L = true;
                        break;
                    case 'F':
                        flags->F = true;
                        break;
                    case '1':
                        flags->one = true;
                        break;
                    case '@':
                        flags->at = true;
                        break;
                    case 'C':
                        flags->C = true;
                        break;
                    case 'B':
                        flags->B = true;
                        break;
                    case 's':
                        flags->s = true;
                        break;
                    case 'X':
                        flags->X = true;
                        break;
                    case 'v':
                        flags->v = true;
                        break;
                    case 'w':
                        flags->w = true;
                        break;
                    case 'D':
                        flags->D = true;
                        break;
                    case 'P':
                        flags->P = true;
                        break;
                    case 'Q':
                        flags->Q = true;
                        break;
                    default:
                        fprintf(stderr, "Unrecognized flag: -%c\n", argv[i][j]);
                        exit(1);
                }
            }
        } else {
            break;
        }
        count++;
    }
    return count;
}

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

void recursive_flag(const char *path, s_flags_t *flags) {
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

void printTrueFlags(const s_flags_t *flags) {
    if (flags->a) printf("a ");
    if (flags->A) printf("A ");
    if (flags->l) printf("l ");
    if (flags->r) printf("r ");
    if (flags->R) printf("R ");
    if (flags->t) printf("t ");
    if (flags->u) printf("u ");
    if (flags->c) printf("c ");
    if (flags->G) printf("G ");
    if (flags->h) printf("h ");
    if (flags->e) printf("e ");
    if (flags->i) printf("i ");
    if (flags->S) printf("S ");
    if (flags->T) printf("T ");
    if (flags->x) printf("x ");
    if (flags->p) printf("p ");
    if (flags->d) printf("d ");
    if (flags->f) printf("f ");
    if (flags->n) printf("n ");
    if (flags->g) printf("g ");
    if (flags->o) printf("o ");
    if (flags->L) printf("L ");
    if (flags->F) printf("F ");
    if (flags->one) printf("1 ");
    if (flags->at) printf("@ ");
    if (flags->C) printf("C ");
    if (flags->B) printf("B ");
    if (flags->s) printf("s ");
    if (flags->X) printf("X ");
    if (flags->v) printf("v ");
    if (flags->w) printf("w ");
    if (flags->D) printf("D ");
    if (flags->P) printf("P ");
    if (flags->Q) printf("Q ");
    printf("\n");
}


int main(int argc, char *argv[]) {
    const char *dirname;
    s_flags_t flags;
    bool with_flags = false;
    int count = 0;
    if (argc >= 2 && argv[1][0] == '-') { 
        count = parse_args(argc, argv, &flags);
        with_flags = true;
        dirname = argv[count + 1];
        if (flags.R) {
            recursive_flag(dirname, &flags);
        }
        
    } 

    if (!with_flags){
        if (argc == 1) {
            dirname = "."; // Use current directory if no argument is provided
        } else if (argc == 2) {
            dirname = argv[1];
        } else {
            fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        print_multicolumn(dirname);
    }


    

    return 0;
}
