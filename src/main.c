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

static void printTrueFlags(const s_flags_t *flags) {
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
    // char **directories = malloc(argc * sizeof(char *));
    s_flags_t flags;
    bool with_flags = false;
    if (argc >= 2 && argv[1][0] == '-') { 
        int count = parse_args(argc, argv, &flags);
        printTrueFlags(&flags);
        with_flags = true;
        printf("%d\n", count);
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
        // printf("%x CCCC \n", flags.C);
        // printf("%x 111 \n", flags.one);
        // printf("%x lll \n", flags.l);
        // printf("%x rrr \n", flags.r);
        // printf("%x PPP \n", flags.P);
        // printf("%x QQQ \n", flags.Q);
        // printf("%x DDD \n", flags.D);
        // if(flags.C)
        //         printf("%x CCCC \n", flags.C);
        //     // print_multicolumn(dirname);
        // else if (flags.one)
        //         printf("%x 1111 \n", flags.one);
        //     // print_perline(dirname);
    }
    return 0;
}
