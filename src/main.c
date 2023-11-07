#include "../inc/uls.h"

void usage() {
    fprintf(stderr, "usage: uls [-l] [file ...]\n");
    exit(1);
}

void error(char *msg) {
    perror(msg);
    exit(1);
}

void list_file(char *filename, int long_format) {
    struct stat buf;
    if (stat(filename, &buf) == -1) {
        fprintf(stderr, "uls: cannot access %s: No such file or directory\n", filename);
        return;
    }

    if (long_format) {
        struct passwd *pwd;
        struct group *grp;
        char timebuf[100];

        pwd = getpwuid(buf.st_uid);
        grp = getgrgid(buf.st_gid);

        strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M", localtime(&buf.st_mtime));

        printf("%c", (buf.st_mode & S_IFDIR) ? 'd' : '-');
        printf("%c", (buf.st_mode & S_IRUSR) ? 'r' : '-');
        printf("%c", (buf.st_mode & S_IWUSR) ? 'w' : '-');
        printf("%c", (buf.st_mode & S_IXUSR) ? 'x' : '-');
        printf("%c", (buf.st_mode & S_IRGRP) ? 'r' : '-');
        printf("%c", (buf.st_mode & S_IWGRP) ? 'w' : '-');
        printf("%c", (buf.st_mode & S_IXGRP) ? 'x' : '-');
        printf("%c", (buf.st_mode & S_IROTH) ? 'r' : '-');
        printf("%c", (buf.st_mode & S_IWOTH) ? 'w' : '-');
        printf("%c", (buf.st_mode & S_IXOTH) ? 'x' : '-');

        printf(" %ld ", (long)buf.st_nlink);
        printf("%s ", pwd->pw_name);
        printf("%s ", grp->gr_name);
        printf("%lld ", (long long)buf.st_size);
        printf("%s ", timebuf);
    }

    printf("%s\n", filename);
}

int main(int argc, char *argv[]) {
    int opt;
    int long_format = 0;

    while ((opt = getopt(argc, argv, "l")) != -1) {
        switch (opt) {
            case 'l':
                long_format = 1;
                break;
            default:
                usage();
        }
    }

    if (optind >= argc) {
        list_file(".", long_format);
    } else {
        for (int i = optind; i < argc; i++) {
            list_file(argv[i], long_format);
        }
    }

    return 0;
}