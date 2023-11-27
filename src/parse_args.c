#include "uls.h"

int parse_args(int argc, char *argv[], t_flags_s *flags) {
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
                    if (!flags->a)
                        flags->A = true;
                    flags->a = true;
                    break;
                case 'l':
                    flags->l = true;
                    flags->C = false;
                    flags->one = false;
                    flags->m = false;
                    flags->x = false;
                    break;
                case 'r':
                    if (!flags->f)
                        flags->r = true;
                    break;
                case 'R':
                    flags->R = true;
                    break;
                case 't':
                    if (!flags->f)
                        flags->t = true;
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
                    if (!flags->f)
                        flags->S = true;
                    flags->t = false;
                    break;
                case 'T':
                    flags->T = true;
                    break;
                case 'x':
                    flags->x = true;
                    flags->l = false;
                    flags->C = false;
                    flags->one = false;
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
                    flags->l = true;
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
                    flags->x = false;
                    break;
                case '@':
                    flags->at = true;
                    break;
                case 'C':
                    flags->C = true;
                    flags->one = false;
                    flags->l = false;
                    flags->x = false;
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
                case 'i':
                    flags->i = true;
                    break;
                default:
                    mx_printerr("uls: illegal option -- ");
                    mx_printerr(&argv[i][j]);
                    mx_printerr("\n");
                    mx_printerr("usage: uls [-@ACFGRSTUacefghlmnoprtux1] [file ...]\n");
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

    *dirs = malloc((argc - 1) * sizeof(char *));

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (is_dir(argv[i]) && (is_directory_exists(argv[i]) || is_file_exists(argv[i]))) {
                (*dirs)[dir_count] = mx_strdup(argv[i]);
                dir_count++;
            }
        }
    }
    return dir_count;
}

int parse_err_dirs(int argc, char *argv[], char ***err_dirs) {
    int i, err_dir_count = 0;

    *err_dirs = malloc((argc - 1) * sizeof(char *));

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (!(is_directory_exists(argv[i]) || is_file_exists(argv[i]))) {
                (*err_dirs)[err_dir_count] = mx_strdup(argv[i]);
                err_dir_count++;
            }
        }
    }
    return err_dir_count;
}

int parse_file_input(int argc, char *argv[], char ***file_inputs) {
    int i, file_inputs_count = 0;

    *file_inputs = malloc((argc - 1) * sizeof(char *));

    for (i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            if (!is_dir(argv[i]) && is_file_exists(argv[i])) {
                (*file_inputs)[file_inputs_count] = mx_strdup(argv[i]);
                file_inputs_count++;
            }
        }
    }
    return file_inputs_count;
}
