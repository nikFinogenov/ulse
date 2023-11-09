#include <stdio.h>
#include <stdlib.h>
#include <uls.h>

int compare_names(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}
void print_tab_aligned_columns(int width, int tab_width) {
    int tabs = (width + tab_width - 1) / tab_width;

    for (int i = 0; i < tabs; i++) {
        if (tab_width == 1) {
            mx_printchar(' ');
        } else {
            mx_printchar('\t');
        }
    }
}
void print_multicolumn(const char *dirname) {
    struct dirent *dir_entry;
    char **files = NULL;
    int num_files = 0;

    DIR *dir = opendir(dirname);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((dir_entry = readdir(dir)) != NULL) {
        if (dir_entry->d_name[0] != '.') {
            files = realloc(files, (num_files + 1) * sizeof(char *));
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
        qsort(files, num_files, sizeof(char *), compare_names);

        int max_name_length = 0;
        for (int i = 0; i < num_files; ++i) {
            int name_length = strlen(files[i]);
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
            printf("\n");
        }

        for (int i = 0; i < num_files; ++i) {
            free(files[i]);
        }
        free(files);
    }
}

void parse_args(int argc, char *argv[], s_flags_t *flags) {
    int i;
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
            // assume the rest of the arguments are filenames
            break;
        }
    }
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



int main(int argc, char *argv[]) {
    const char *dirname;
    s_flags_t flags;
    bool with_flags = false;
    if (argc == 2 && argv[1][0] == '-') { 
        parse_args(argc, argv, &flags);
        with_flags = true;
        
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
