#include "uls.h"

int parse_args(int argc, char *argv[], s_flags_t *flags) {
    int i, count = 0;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j]; j++) {
                switch (argv[i][j]) {
                    case 'a':
                        flags->a = true;
                        flags->A = false;
                        break;
                    case 'A':
                        if(!flags->a) flags->A = true;
                        flags->a = true;
                        break;
                    case 'l':
                        flags->l = true;
                        flags->C = false;
                        flags->one = false;
                        flags->m = false;
                        break;
                    case 'r':
                        if(!flags->f) flags->r = true;
                        break;
                    case 'R':
                        flags->R = true;
                        break;
                    case 't':
                        if(!flags->f) flags->t = true;
                        flags->S = false;
                        break;
                    case 'u':
                        flags->u = true;
                        flags->c = false;
                        flags->U = false;
                        break;
                    case 'c':
                        flags->c = true;
                        flags->U = false;
                        flags->u = false;
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
                    case 'S':
                        if(!flags->f) flags->S = true;
                        flags->t = false;
                        break;
                    case 'T':
                        flags->T = true;
                        break;
                    case 'x':
                        flags->x = true;
                        break;
                    case 'p':
                        flags->p = true;
                        flags->F = false;
                        break;
                    case 'f':
                        flags->f = true;
                        flags->a = true;
                        flags->S = false;
                        flags->t = false;
                        flags->r = false;
                        break;
                    case 'n':
                        flags->n = true;
                        break;
                    case 'g':
                        flags->g = true;
                        flags->l = true;
                        break;
                    case 'o':
                        flags->o = true;
                        flags->l = true;
                        break;
                    case 'F':
                        flags->F = true;
                        flags->p = true;
                        break;
                    case '1':
                        flags->one = true;
                        flags->C = false;
                        flags->l = false;
                        flags->m = false;
                        break;
                    case '@':
                        flags->at = true;
                        break;
                    case 'C':
                        flags->C = true;
                        flags->one = false;
                        flags->l = false;
                        break;
                    case 'm':
                        flags->m = true;
                        flags->l = false;
                        flags->one = false;
                        break;
                    case 'U':
                        flags->U = true;
                        flags->u = false;
                        flags->c = false;
                        break;
                    case 'q':
                        flags->q = true;
                        break;
                    case 'H':
                        flags->H = true;
                        break;
                    default:
                        fprintf(stderr, "Unrecognized flag: -%c\n", argv[i][j]);
                        exit(1);
                }
            }
            count++;
        }
    }
    return count;
}

int parse_dirs(int argc, char *argv[], char ***dirs) {
    int i, dir_count = 0;

    *dirs = malloc((argc - 1) * sizeof(char *)); // выделяем память для массива указателей

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            (*dirs)[dir_count] = strdup(argv[i]); // используем strdup для копирования строки
            dir_count++;
        }
    }
    return dir_count;
}
