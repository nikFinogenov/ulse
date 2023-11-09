
#ifndef ULS_H

#define ULS_H

#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <sys/acl.h>
#include <sys/ioctl.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <libmx.h>
#include <string.h>

#define MAX_FLAGS 33

typedef struct uls_stat_s {
    unsigned long size;
    unsigned long n_blks;
    unsigned long blocks[32];
} uls_stat_t;


typedef struct s_flags_n{
    bool a;
    bool A;
    bool l;
    bool r;
    bool R;
    bool t;
    bool u;
    bool c;
    bool G;
    bool h;
    bool e;
    bool i;
    bool T;
    bool x;
    bool p;
    bool d;
    bool f;
    bool n;
    bool g;
    bool o;
    bool L;
    bool F;
    bool one;
    bool C;
    bool B;
    bool s;
    bool S;
    bool X;
    bool v;
    bool w;
    bool D;
    bool P;
    bool Q;
} s_flags_t;

typedef struct {
    char flag;
    int order;
} s_flag_order_t;


#endif
