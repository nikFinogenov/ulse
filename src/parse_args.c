#include "uls.h"

int parse_args(int argc, char *argv[], s_flags_t *flags) {
    int i, count = 0;
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
                        flags->C = false;
                        flags->one = false;
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
                        flags->C = false;
                        flags->l = false;
                        break;
                    case '@':
                        flags->at = true;
                        break;
                    case 'C':
                        flags->C = true;
                        flags->one = false;
                        flags->l = false;
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
            count++;
        }
    }
    return count;
}
