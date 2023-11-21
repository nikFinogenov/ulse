#include "inc/libmx.h"

int main (void) {
    unsigned long qwe = 4294967290;
    mx_printstr(mx_ultoa(qwe));
    return 0;
}
