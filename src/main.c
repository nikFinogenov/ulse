#include <uls.h>

int main (int argc, char *argv[]) {
    while (argc || argv)
        break;


    struct dirent *entry;
    DIR *dp;
    if(argc == 1) 
        dp = opendir(".");
    else 
        dp = opendir(argv[1]);
    
    if (dp == NULL) {
        perror("Error opening directory");
        return 1;
    }
    while ((entry = readdir(dp))) {
        if (entry->d_name[0] != '.')
            printf("%s ", entry->d_name);
    }
    printf("\n");
    
    closedir(dp);
    return 0;
    
}
