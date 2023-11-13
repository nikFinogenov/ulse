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

void recursive_flag(const char *path, FileEntry *file_entries, int count, s_flags_t *flags) {
    DIR *dir;
    // struct dirent *entry;

    if (!(dir = opendir(path))) {
        perror("opendir");
        mx_printstr(path);
        mx_printchar('\n');
        return;
    }
    if (flags->l)
        print_longlist(path, file_entries, count, flags);
    else if(flags->C)
        print_multicolumn(path, flags);
    else if (flags->one)
        print_perline(path, flags);
    else if(isatty(1))
        print_multicolumn(path, flags);
    else
        print_perline(path, flags);

    // Then, rewind the directory stream to start from the beginning
    closedir(dir);
    // dir = opendir(path);

    // Now, process directories recursively
    // if (flags->one) {
        // while ((entry = readdir(dir)) != NULL) {
        for(int i = 0; i < count; i++) {
            if (file_entries[i].name[0] == '.' || file_entries[i].type != 'd')
                continue;


            // printf("\n");
            mx_printchar('\n');
            char *sub_path;
            // if (path[mx_strlen(path) - 1] == '/')
            //     sub_path = mx_strjoin(path, file_entries[i].name);
            // else {
                sub_path = mx_strjoin(mx_strjoin(path, "/"), file_entries[i].name);
            // }
            mx_printstr(path);
            mx_printchar('/');
            mx_printstr(file_entries[i].name);
            mx_printchar(':');
            mx_printchar('\n');
            // snprintf(sub_path, sizeof(sub_path), "%s%s", path, file_entries[i].name);
            int inner_count;
            FileEntry *inner_file_entries = fill_file_entries(sub_path, &inner_count, flags);
            custom_qsort(inner_file_entries, inner_count, sizeof(FileEntry), compare_file_entries);
            // printf("%s -> %d\n", sub_path, 0);
            recursive_flag(sub_path, inner_file_entries, inner_count, flags);
            free(inner_file_entries);
        }
    // }

    // closedir(dir);
}


int main(int argc, char *argv[]) {
    const char *dirname;
    s_flags_t flags;
    init_flags(&flags);
    // bool with_flags = false;
    // bool with_dir = false;
    if (argc >= 2 && argv[1][0] == '-') { 
        int count_args = parse_args(argc, argv, &flags);
        // with_flags = true;
        if (count_args == argc - 1) dirname = ".";
        else{
            dirname = argv[count_args + 1];
            // with_dir = true;
        }
    } 
    else if (argc == 1) {
        dirname = "."; // Use current directory if no argument is provided
    } else if (argc == 2 && argv[1][0] != '-') {
        dirname = argv[1];
    } else {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int count_files;
    FileEntry *file_entries = fill_file_entries(dirname, &count_files, &flags);
    if (file_entries == NULL) {
        return 0;
    }

    // Сортируем массив по именам
    custom_qsort(file_entries, count_files, sizeof(FileEntry), compare_file_entries);
    // if (!with_flags){
    //     print_multicolumn(dirname, &flags);
    // }
    // else{
        if(flags.R)
            recursive_flag(dirname, file_entries, count_files, &flags);
        else if (flags.l) {
            print_longlist(dirname, file_entries, count_files, &flags);
        }
        else if (flags.C)
            print_multicolumn(dirname, &flags);
        else if (flags.one) {
            print_perline(dirname, &flags);
        }
        else if(isatty(1))
            print_multicolumn(dirname, &flags);
        else
            print_perline(dirname, &flags);
    // }
    return 0;
}
