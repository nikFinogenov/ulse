#include <stdio.h> // убрать потом
#include <uls.h>

void recursive_flag(const char *path, FileEntry *file_entries, int count, s_flags_t *flags) {

    if (flags->l)
        print_longlist(path, file_entries, count, flags);
    else if(flags->m) print_coma(file_entries, count, flags);
    else if(flags->C)
        print_multicolumn(file_entries, count, flags);
    else if (flags->one)
        print_perline(file_entries, count, flags);
    else if(isatty(1))
        print_multicolumn(file_entries, count, flags);
    else
        print_perline(file_entries, count, flags);

        for(int i = 0; i < count; i++) {
            if ((file_entries[i].name[0] == '.' && !flags->a ) 
            || file_entries[i].type != 'd' 
            || (mx_strcmp(file_entries[i].name, ".") == 0
            || mx_strcmp(file_entries[i].name, "..") == 0))
                continue;

            mx_printchar('\n');
            mx_printstr(file_entries[i].path);
            mx_printchar(':');
            mx_printchar('\n');
            int inner_count;
            FileEntry *inner_file_entries = fill_file_entries(file_entries[i].path, &inner_count, flags);
            if(flags->S) custom_qsort(inner_file_entries, inner_count, sizeof(FileEntry), compare_file_entries_size, flags);
            else if(flags->t) custom_qsort(inner_file_entries, inner_count, sizeof(FileEntry), compare_file_entries_date_time, flags);
            else if(!flags->f)custom_qsort(inner_file_entries, inner_count, sizeof(FileEntry), compare_file_entries_name, flags);
            recursive_flag(file_entries[i].path, inner_file_entries, inner_count, flags);
            free(inner_file_entries);
        }
}


int main(int argc, char *argv[]) {
    char *dirname;
    s_flags_t flags;
    init_flags(&flags);
    char **dirs;
    int count_args = parse_args(argc, argv, &flags);
    if(!isatty(1)) flags.G = false;
    int count_dirs = parse_dirs(argc, argv, &dirs);
    while (count_args) {
        break;
    }
    
    if (count_dirs == 0) {
        dirname = ".";
        count_dirs = 1;
        dirs[0] = dirname;
    }
    custom_qsort(dirs, count_dirs, sizeof(char *), compare_names, &flags);
        for(int i = 0; i < count_dirs; i++) {
            dirname = dirs[i];
            if(count_dirs >=2) {
                mx_printstr(dirname);
                mx_printstr(":\n");
            }
            int count_files;
            struct stat sb;
            if (lstat(dirname, &sb) == -1) {
                perror("Cannot get directory information");
            }
            FileEntry *file_entries;
            if (S_ISLNK(sb.st_mode)) {
                file_entries = fill_link_entry(dirname, &flags);
                count_files = 1;
            }
            else file_entries = fill_file_entries(dirname, &count_files, &flags);
            if (file_entries == NULL) {
                return 0;
            }
            
            if(flags.S) custom_qsort(file_entries, count_files, sizeof(FileEntry), compare_file_entries_size, &flags);
            else if(flags.t) custom_qsort(file_entries, count_files, sizeof(FileEntry), compare_file_entries_date_time, &flags);
            else if(!flags.f) custom_qsort(file_entries, count_files, sizeof(FileEntry), compare_file_entries_name, &flags);
            if(flags.R)
                recursive_flag(dirname, file_entries, count_files, &flags);
            else if (flags.l) {
                print_longlist(dirname, file_entries, count_files, &flags);
            }
            else if(flags.m) print_coma(file_entries,count_files, &flags);
            else if (flags.C)
                print_multicolumn(file_entries, count_files, &flags);
            else if (flags.one) {
                print_perline(file_entries, count_files, &flags);
            }
            else if(isatty(1))
                print_multicolumn(file_entries, count_files, &flags);
            else
                print_perline(file_entries, count_files, &flags);
            if (i + 1 != count_dirs) mx_printchar('\n');
        }
    return 0;
}
