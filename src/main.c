#include <uls.h>

void recursive_flag(const char *path, t_file_entry_s *file_entries, int count, t_flags_s *flags) {
    if (flags->l)
        print_longlist(path, file_entries, count, flags);
    else if (flags->m)
        print_coma(file_entries, count, flags);
    else if (flags->C)
        print_multicolumn(file_entries, count, flags);
    else if (flags->one)
        print_perline(file_entries, count, flags);
    else if (isatty(1))
        print_multicolumn(file_entries, count, flags);
    else if (flags->x)
        print_multicolumn(file_entries, count, flags);
    else
        print_perline(file_entries, count, flags);

    for (int i = 0; i < count; i++) {
        if ((file_entries[i].name[0] == '.' && !flags->a) || file_entries[i].type != 'd' || (mx_strcmp(file_entries[i].name, ".") == 0 || mx_strcmp(file_entries[i].name, "..") == 0))
            continue;

        mx_printchar('\n');
        mx_printstr(file_entries[i].path);
        mx_printchar(':');
        mx_printchar('\n');
        int inner_count;
        t_file_entry_s *inner_file_entries = fill_file_entries(file_entries[i].path, &inner_count, flags);
            if(inner_file_entries == NULL && !flags->l) {
                print_err(file_entries[i].path);
                continue;
            }
            if (is_directory_empty(file_entries[i].path) && !flags->a && !flags->l)
                continue;
            else if (is_directory_empty(file_entries[i].path) && flags->l) {
                mx_printstr("total 0\n");
                if(inner_file_entries == NULL) print_err(file_entries[i].path);
                continue;
            }
        if (flags->S)
            custom_qsort(inner_file_entries, inner_count, sizeof(t_file_entry_s), compare_file_entries_size, flags);
        else if (flags->t)
            custom_qsort(inner_file_entries, inner_count, sizeof(t_file_entry_s), compare_file_entries_date_time, flags);
        else if (!flags->f)
            custom_qsort(inner_file_entries, inner_count, sizeof(t_file_entry_s), compare_file_entries_name, flags);
        recursive_flag(file_entries[i].path, inner_file_entries, inner_count, flags);
        free(inner_file_entries);
    }
}

int main(int argc, char *argv[]) {
    char *dirname;
    t_flags_s flags;
    init_flags(&flags);
    char **dirs;
    char **err_dirs;
    char **input_files;
    int count_args = parse_args(argc, argv, &flags);
    if (!isatty(1))
        flags.G = false;
    int count_dirs = parse_dirs(argc, argv, &dirs);
    int count_err_dirs = parse_err_dirs(argc, argv, &err_dirs);
    int count_file_input = parse_file_input(argc, argv, &input_files);
    while (count_args) {
        break;
    }
    int real_count_dirs = count_dirs + count_err_dirs + count_file_input;
    t_file_entry_s *file_entries = malloc(count_file_input * sizeof(t_file_entry_s));
    t_file_entry_s *dir_entries = malloc(count_dirs * sizeof(t_file_entry_s));
    for (int i = 0; i < count_dirs; i++) {
        dirname = dirs[i];
        dir_entries[i] = *fill_entry(dirname, &flags);
    }
    if (real_count_dirs == 0) {
        dirname = ".";
        count_dirs = 1;
        dir_entries[0].name = mx_strdup(dirname);
    }
    if (count_dirs != 0){
        // custom_qsort(dirs, count_dirs, sizeof(char *), compare_names, &flags);
        if (flags.S)
            custom_qsort(dir_entries, count_dirs, sizeof(t_file_entry_s), compare_file_entries_size, &flags);
        else if (flags.t)
            custom_qsort(dir_entries, count_dirs, sizeof(t_file_entry_s), compare_file_entries_date_time, &flags);
        else if (!flags.f)
            custom_qsort(dir_entries, count_dirs, sizeof(t_file_entry_s), compare_file_entries_name, &flags);
    }
    if (count_err_dirs != 0)
        custom_qsort(err_dirs, count_err_dirs, sizeof(char *), compare_names, &flags);
    if (count_file_input != 0)
        custom_qsort(input_files, count_file_input, sizeof(char *), compare_names, &flags);
    
    for (int i = 0; i < count_err_dirs; i++) {
        print_err(err_dirs[i]);
    }
    for (int i = 0; i < count_file_input; i++) {
        dirname = input_files[i];
        file_entries[i] = *fill_entry(dirname, &flags);
    }
    if(count_file_input) {
        // dirname = input_files[i];
        if (flags.S)
            custom_qsort(file_entries, count_file_input, sizeof(t_file_entry_s), compare_file_entries_size, &flags);
        else if (flags.t)
            custom_qsort(file_entries, count_file_input, sizeof(t_file_entry_s), compare_file_entries_date_time, &flags);
        else if (!flags.f)
            custom_qsort(file_entries, count_file_input, sizeof(t_file_entry_s), compare_file_entries_name, &flags);

        if (flags.R)
            recursive_flag(dirname, file_entries, count_file_input, &flags);
        else if (flags.l) {
            print_longlist(dirname, file_entries, count_file_input, &flags);
        }
        else if (flags.m)
            print_coma(file_entries, count_file_input, &flags);
        else if (flags.C)
            print_multicolumn(file_entries, count_file_input, &flags);
        else if (flags.one) {
            print_perline(file_entries, count_file_input, &flags);
        }
        else if (isatty(1))
            print_multicolumn(file_entries, count_file_input, &flags);
        else if (flags.x)
            print_multicolumn(file_entries, count_file_input, &flags);
        else
            print_perline(file_entries, count_file_input, &flags);
    }
    if(count_file_input && (count_dirs || count_err_dirs)) mx_printchar('\n');
    for (int i = 0; i < count_dirs; i++) {
        dirname = dir_entries[i].name;
        if (real_count_dirs >= 2) {
            mx_printstr(dirname);
            mx_printstr(":\n");
        }
        int count_files;
        struct stat sb;
        if (lstat(dirname, &sb) == -1) {
            print_err(dirname);
        }
        t_file_entry_s *file_entries = fill_file_entries(dirname, &count_files, &flags);
        if(file_entries == NULL && !flags.l) {
            print_err(dirname);
            continue;
        }
            if (is_directory_empty(dirname) && count_dirs >= 2 && !flags.l && i+1 != count_dirs)
                mx_printchar('\n');
            if (is_directory_empty(dirname) && !flags.a && !flags.l)
                continue;
            else if (is_directory_empty(dirname) && flags.l) {
                mx_printstr("total 0\n");
                if(file_entries == NULL) print_err(dirname);
                if(count_dirs >= 2 && i + 1 != count_dirs) mx_printchar('\n');
                continue;
            }
            
        if (flags.S)
            custom_qsort(file_entries, count_files, sizeof(t_file_entry_s), compare_file_entries_size, &flags);
        else if (flags.t)
            custom_qsort(file_entries, count_files, sizeof(t_file_entry_s), compare_file_entries_date_time, &flags);
        else if (!flags.f)
            custom_qsort(file_entries, count_files, sizeof(t_file_entry_s), compare_file_entries_name, &flags);

        if (flags.R)
            recursive_flag(dirname, file_entries, count_files, &flags);
        else if (flags.l) {
            print_longlist(dirname, file_entries, count_files, &flags);
        }
        else if (flags.m)
            print_coma(file_entries, count_files, &flags);
        else if (flags.C)
            print_multicolumn(file_entries, count_files, &flags);
        else if (flags.one) {
            print_perline(file_entries, count_files, &flags);
        }
        else if (isatty(1))
            print_multicolumn(file_entries, count_files, &flags);
        else if (flags.x)
            print_multicolumn(file_entries, count_files, &flags);
        else
            print_perline(file_entries, count_files, &flags);
        if (i + 1 != count_dirs)
            mx_printchar('\n');
    }
    return 0;
}
