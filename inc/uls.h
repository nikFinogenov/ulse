#ifndef ULS_H
#define ULS_H

#include "libmx.h"
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
#include <string.h>

#define MAX_FLAGS 34

typedef struct uls_stat_s {
    unsigned long size;
    unsigned long n_blks;
    unsigned long blocks[32];
} uls_stat_t;


typedef struct s_flags_n{
    bool a; // Flag for option 'a': Usually stands for "all", showing hidden files.
    bool A; // Flag for option 'A': Lists all entries except for '.' and '..'.
    bool l; // Flag for option 'l': Long format, showing detailed file information.
    bool r; // Flag for option 'r': Reverse the order of listing.
    bool R; // Flag for option 'R': Recursively list subdirectories.
    bool t; // Flag for option 't': Sort by modification time, showing the newest first.
    bool u; // Flag for option 'u': Sort by access time, showing the newest first.
    bool c; // Flag for option 'c': Sort by change time, showing the newest first.
    bool G; // Flag for option 'G': Enable colored output.
    bool h; // Flag for option 'h': Human-readable file sizes (e.g., in KB, MB).
    bool e; // Flag for option 'e': Display the escape sequences for control characters.
    bool i; // Flag for option 'i': Show the file's inode number.
    bool S; // Flag for option 'S': Sort by file size, showing the largest first.
    bool T; // Flag for option 'T': Display full time information.
    bool x; // Flag for option 'x': List entries by lines, rather than columns.
    bool p; // Flag for option 'p': Add a trailing '/' to directory names.
    bool d; // Flag for option 'd': List the directory itself, rather than its contents.
    bool f; // Flag for option 'f': Disable sorting, and list entries in the order they appear.
    bool n; // Flag for option 'n': List numeric user and group IDs instead of names.
    bool g; // Flag for option 'g': Like 'l', but excludes the owner.
    bool o; // Flag for option 'o': Like 'l', but excludes the group.
    bool L; // Flag for option 'L': Follow symbolic links when listing.
    bool F; // Flag for option 'F': Append a character to entries to indicate their type.
    bool one; // Flag for option '1': List one entry per line.
    bool at; //Flag for option '@': Extended attribute keys.
    bool C; // Flag for option 'C': Force multi-column output.
    bool B; // Flag for option 'B': Disable line wrapping, useful for long filenames.
    bool s; // Flag for option 's': Show the size of each file in blocks.
    bool X; // Flag for option 'X': Sort alphabetically by entry extension.
    bool v; // Flag for option 'v': Show version information.
    bool w; // Flag for option 'w': List in wide format, with more details.
    bool D; // Flag for option 'D': Show the time in the format specified by the environment variable.
    bool P; // Flag for option 'P': Show permissions of the listed files.
    bool Q; // Flag for option 'Q': Quote file names, useful for special characters in names.
} s_flags_t;

typedef struct {
    char flag;
    int order;
} s_flag_order_t;


int parse_args(int argc, char *argv[], s_flags_t *flags);
void print_multicolumn(const char *dirname, s_flags_t *flags);
void print_perline(const char *dirname, s_flags_t *flags);
int compare_names(const void *a, const void *b);
void custom_qsort(void *base, size_t num_elements, size_t element_size, int (*comparator)(const void *, const void *));

#endif
