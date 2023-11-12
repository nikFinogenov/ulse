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

void recursive_flag(const char *path, s_flags_t *flags) {
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path))) {
        perror("opendir");
        mx_printstr(path);
        mx_printchar('\n');
        return;
    }

    if(flags->C)
        print_multicolumn(path, flags);
    else if (flags->one)
        print_perline(path, flags);
    else 
        print_multicolumn(path, flags);

    // Then, rewind the directory stream to start from the beginning
    closedir(dir);
    dir = opendir(path);

    // Now, process directories recursively
    // if (flags->one) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] == '.' || entry->d_type != DT_DIR) // Skip hidden files and non-directories
                continue;

            printf("\n");
            char *sub_path;
            if (path[mx_strlen(path) - 1] == '/')
                sub_path = mx_strjoin(path, entry->d_name);
            else {
                sub_path = mx_strjoin(mx_strjoin(path, "/"), entry->d_name);
            }
            mx_printstr(path);
            mx_printchar('/');
            mx_printstr(entry->d_name);
            mx_printchar(':');
            mx_printchar('\n');
            // snprintf(sub_path, sizeof(sub_path), "%s%s", path, entry->d_name);
            recursive_flag(sub_path, flags);
        }
    // }

    closedir(dir);
}


int main(int argc, char *argv[]) {
    const char *dirname;
    s_flags_t flags;
    init_flags(&flags);
    // bool with_flags = false;
    // bool with_dir = false;
    if (argc >= 2 && argv[1][0] == '-') { 
        int count = parse_args(argc, argv, &flags);
        // with_flags = true;
        if (count == argc - 1) dirname = ".";
        else{
            dirname = argv[count + 1];
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
    // if (!with_flags){
    //     print_multicolumn(dirname, &flags);
    // }
    // else{
        if(flags.R)
            recursive_flag(dirname, &flags);
        else if (flags.l) {
                int count;
                FileEntry *file_entries = fill_file_entries(dirname, &count);
                if (file_entries == NULL) {
                    return 1;
                }

                // Сортируем массив по именам
                custom_qsort(file_entries, count, sizeof(FileEntry), compare_file_entries);

                // Выводим отсортированный массив
                print_file_entries(file_entries, count);

                // Освобождаем память для массива
                free(file_entries);
                // print_longlist(dirname, &flags);
        }
        else if (flags.C)
            print_multicolumn(dirname, &flags);
        else if (flags.one) {
            print_perline(dirname, &flags);
        }
        else
            print_multicolumn(dirname, &flags);
    // }
    return 0;
}
