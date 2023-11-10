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
    }
    return 0;
}
