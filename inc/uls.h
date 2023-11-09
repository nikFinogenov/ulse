
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


typedef struct uls_stat_s {
    unsigned long size;
    unsigned long n_blks;
    unsigned long blocks[32];
} uls_stat_t;




#endif
