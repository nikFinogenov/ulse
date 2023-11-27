#include "uls.h"

void printint_formatted(int n, int width) {
    int num_width = 0;
    int temp = n;

    while (temp != 0) {
        temp /= 10;
        num_width++;
    }

    for (int i = 0; i < width - num_width; i++)
        mx_printchar(' ');

    mx_printint(n);
}

void printstr_formatted(char *str, int wid, bool right) {
    if (!right)
        mx_printstr(str);
    for (int i = 0; i < wid - mx_strlen(str); i++)
        mx_printchar(' ');
    if (right)
        mx_printstr(str);
}

void print_multicolumn(t_file_entry_s *file_entries, int count, t_flags_s *flags) {
    int terminal_width = 80;
    if (isatty(1)) {
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        terminal_width = w.ws_col;
    }

    int max_name_length = 0;
    for (int i = 0; i < count; ++i) {
        int name_length = mx_strlen(file_entries[i].name);
        if (flags->i)
            name_length += 9;
        if (flags->p)
            name_length += 1;
        if (name_length > max_name_length) {
            max_name_length = name_length;
        }
    }
    int tab = 8;
    if (flags->G)
        tab = 1;
    int width = (max_name_length + tab) & ~(tab - 1);
    int num_columns = terminal_width / width;
    int index = 0;
    int rows = (count + num_columns - 1) / num_columns;
    for (int i = 0; i < rows; ++i) {
        if (!flags->x)
            index = i;
        for (int j = 0; j < num_columns; j++) {
            if (flags->i) {
                printstr_formatted(mx_ultoa(file_entries[index].inode), 8, true);
                mx_printchar(' ');
            }
            if (flags->G) {
                struct stat sb;

                if (lstat(file_entries[index].path, &sb) == -1) continue;
                switch_strcolor(sb);
            }
            mx_printstr(file_entries[index].name);
            if (flags->G)
                mx_printstr(DEFAULT_COLOR);
            int name_len = mx_strlen(file_entries[index].name);
            if (flags->i)
                name_len += 9;
            if (flags->p && is_dir(file_entries[index].path)) {
                mx_printchar('/');
                name_len += 1;
            }
            else if (flags->F && is_executable(file_entries[index].path)) {
                mx_printchar('*');
                name_len += 1;
            }
            else if (flags->F && is_link(file_entries[index].path)) {
                mx_printchar('@');
                name_len += 1;
            }
            else if (flags->F && is_socket(file_entries[index].path)) {
                mx_printchar('=');
                name_len += 1;
            }
            else if (flags->F && is_whiteout(file_entries[index].path)) {
                mx_printchar('%');
                name_len += 1;
            }
            else if (flags->F && is_fifo(file_entries[index].path)) {
                mx_printchar('|');
                name_len += 1;
            }

            if (!flags->x)
                index = index + rows;
            else
                index++;
            if (index >= count)
                break;
            int tabs = (width - name_len + tab - 1) / tab;
            if ((j == num_columns - 1) && flags->x)
                continue;
            for (int k = 0; k < tabs; k++) {
                if (tab == 1)
                    mx_printchar(' ');
                else
                    mx_printchar('\t');
            }
        }
        mx_printchar('\n');
    }
}

void print_xattr(const t_file_entry_s *file_entry, t_flags_s *flags) {
    for (char **ptr = file_entry->xattr_keys; *ptr != NULL; ptr++) {
        mx_printchar('\t');
        mx_printstr(*ptr);
        mx_printchar('\t');
        ssize_t value_size = getxattr(file_entry->path, *ptr, NULL, 0, 0, XATTR_NOFOLLOW);

        if (flags->h) {
            const char *suffixes[] = {"B", "K", "M", "G", "T", "P"};
            double size_f = (double)value_size;
            int i = 0;

            while (size_f >= 1000) {
                size_f /= 1024;
                i++;
            }

            double size_r = custom_round(size_f * 10) / 10;
            char *result = (char *)malloc(20);

            if (size_r >= 10 || i == 0) {
                char *str = mx_lltoa(custom_round(size_f));
                result = mx_strjoin(result, str);
                free(str);
            }
            else {
                char *str = mx_lltoa(size_r);
                result = mx_strjoin(result, str);
                free(str);
                result = mx_strjoin(result, ".");
                str = mx_lltoa((long long)(size_r * 10) % 10);
                result = mx_strjoin(result, str);
                free(str);
            }

            result = mx_strjoin(result, suffixes[i]);
            printstr_formatted(result, 5, true);
        }
        else {
            printint_formatted(value_size, 5);
        }

        mx_printstr(" \n");
    }
}

void print_perline(t_file_entry_s *file_entries, int count, t_flags_s *flags) {
    for (int i = 0; i < count; i++) {
        if (flags->i) {
            printstr_formatted(mx_ultoa(file_entries[i].inode), 8, true);
            mx_printchar(' ');
        }
        if (flags->G) {
            struct stat sb;
            if (lstat(file_entries[i].path, &sb) == -1) continue;
            switch_strcolor(sb);
        }
        mx_printstr(file_entries[i].name);
        if (flags->G)
            mx_printstr(DEFAULT_COLOR);
        if (flags->p && is_dir(file_entries[i].path))
            mx_printchar('/');
        else if (flags->F && is_executable(file_entries[i].path)) {
            mx_printchar('*');
        }
        else if (flags->F && is_link(file_entries[i].path)) {
            mx_printchar('@');
        }
        else if (flags->F && is_socket(file_entries[i].path)) {
            mx_printchar('=');
        }
        else if (flags->F && is_whiteout(file_entries[i].path)) {
            mx_printchar('%');
        }
        else if (flags->F && is_fifo(file_entries[i].path)) {
            mx_printchar('|');
        }
        mx_printchar('\n');
    }
}

void print_file_entry_s(const t_file_entry_s *file_entries, int i, t_max_sizes_s mxsize, t_flags_s *flags) {
    if (flags->i) {
        printstr_formatted(mx_ultoa(file_entries[i].inode), 8, true);
        mx_printchar(' ');
    }
    mx_printchar(file_entries[i].type);
    for (int j = 0; j < mx_strlen(file_entries[i].permissions); j++) {
        mx_printchar(file_entries[i].permissions[j]);
    }
    mx_printchar(' ');

    if (file_entries[i].nlinks) {
        printint_formatted(file_entries[i].nlinks, mxsize.max_nlinks_len);
    }
    else {
        mx_printstr("  ");
    }

    if (!flags->g) {
        mx_printchar(' ');
        printstr_formatted(mx_char_to_str(file_entries[i].owner), mxsize.max_username_len, true);
    }
    else
        mx_printchar(' ');
    if (!flags->g && !flags->o)
        mx_printstr("  ");
    if (!flags->o) {
        printstr_formatted(mx_char_to_str(file_entries[i].group), mxsize.max_groupname_len, true);
    }
    if (flags->h) {
        mx_printstr("   ");
        if (mxsize.max_h_size_len == 2)
            mx_printstr("  ");
        printstr_formatted(mx_char_to_str(file_entries[i].human_size), mxsize.max_h_size_len, true);
    }
    else {
        (file_entries[i].size == 0) ? mx_printstr(" ") : mx_printstr("  ");
        printint_formatted(file_entries[i].size, mxsize.max_size_len);
    }

    mx_printchar(' ');
    mx_printstr(file_entries[i].date_time);
    mx_printchar(' ');
    if (flags->G) {
        struct stat sb;
        if (lstat(file_entries[i].path, &sb) == -1) {};
        switch_strcolor(sb);
    }
    mx_printstr(file_entries[i].name);
    if (flags->G)
        mx_printstr(DEFAULT_COLOR);
    if (flags->p && is_dir(file_entries[i].path))
        mx_printchar('/');
    else if (flags->F && is_executable(file_entries[i].path)) {
        mx_printchar('*');
    }
    else if (flags->F && is_link(file_entries[i].path)) {
        mx_printchar('@');
    }
    else if (flags->F && is_socket(file_entries[i].path)) {
        mx_printchar('=');
    }
    else if (flags->F && is_whiteout(file_entries[i].path)) {
        mx_printchar('%');
    }
    else if (flags->F && is_fifo(file_entries[i].path)) {
        mx_printchar('|');
    }

    if (file_entries[i].type == 'l') {
        mx_printstr(" -> ");
        mx_printstr(file_entries[i].symlink);
    }
    mx_printchar('\n');
    if (flags->at && file_entries[i].xattr_keys != NULL) {
        print_xattr(&file_entries[i], flags);
    }
}

void print_longlist(const char *dirname, t_file_entry_s *file_entries, int count, t_flags_s *flags) {
    int total_blocks = 0;
    struct stat sb;
    if (lstat(dirname, &sb) == -1) {
        return;
    }
    if ((!S_ISLNK(sb.st_mode) || dirname[mx_strlen(dirname) - 1] == '/') && is_dir(dirname)) {
        mx_printstr("total ");

        for (int i = 0; i < count; ++i) {
            if (lstat(file_entries[i].path, &sb) == -1) continue;
            total_blocks += (int)(sb.st_blocks);
        }
        mx_printint(total_blocks);
        mx_printchar('\n');
    }
    t_max_sizes_s t_mxsize = {.max_groupname_len = 0, .max_nlinks_len = 0, .max_size_len = 0, .max_h_size_len = 0, .max_username_len = 0};
    for (int i = 0; i < count; ++i) {
        int group_len = mx_strlen(file_entries[i].group);
        int nlinks_len = mx_intlen(file_entries[i].nlinks);
        int size_len = mx_intlen(file_entries[i].size);
        int name_len = mx_strlen(file_entries[i].group);
        int h_size_len = mx_strlen(file_entries[i].human_size);
        if (group_len > t_mxsize.max_groupname_len) {
            t_mxsize.max_groupname_len = group_len;
        }
        if (nlinks_len > t_mxsize.max_nlinks_len) {
            t_mxsize.max_nlinks_len = nlinks_len;
        }
        if (flags->h) {
            if (h_size_len > t_mxsize.max_h_size_len) {
                t_mxsize.max_h_size_len = h_size_len;
            }
        }
        else {
            if (size_len > t_mxsize.max_size_len) {
                t_mxsize.max_size_len = size_len;
            }
        }
        if (name_len > t_mxsize.max_username_len) {
            t_mxsize.max_username_len = name_len;
        }
    }
    for (int i = 0; i < count; ++i) {
        print_file_entry_s(file_entries, i, t_mxsize, flags);
        if (acl_get_file(file_entries[i].path, ACL_TYPE_EXTENDED) && flags->e) {
            char **str_split = mx_strsplit(acl_to_text(acl_get_file(file_entries[i].path, ACL_TYPE_EXTENDED), NULL), '\n');
            for (int i = 1; str_split[i] != NULL; i++) {
                char **parts = mx_strsplit(str_split[i], ':');
                mx_printchar(' ');
                mx_printint(i - 1);
                mx_printstr(": ");
                mx_printstr(parts[0]);
                mx_printchar(':');
                mx_printstr(parts[2]);
                mx_printchar(' ');
                mx_printstr(parts[4]);
                mx_printchar(' ');
                mx_printstr(parts[5]);
                mx_printchar('\n');
                mx_del_strarr(&parts);
            }
            mx_del_strarr(&str_split);
        }
    }
}

void print_coma(t_file_entry_s *file_entries, int count, t_flags_s *flags) {
    int terminal_width = 80;
    if (isatty(1)) {
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        terminal_width = w.ws_col;
    }
    int total_width = 0;
    for (int i = 0; i < count; i++) {
        if ((i + 1 == count) && (flags->p || flags->F) && (flags->i) && is_smth(file_entries[i].path) && ((total_width + mx_strlen(file_entries[i].name) + 9 + 1) <= terminal_width)) {
        }
        else if ((flags->p || flags->F) && (flags->i) && is_smth(file_entries[i].path) && ((total_width + mx_strlen(file_entries[i].name) + 2 + 1 + 9) > terminal_width)) {
            mx_printchar('\n');
            total_width = 0;
        }
        else if ((i + 1 == count) && (flags->p || flags->F) && is_smth(file_entries[i].path) && ((total_width + mx_strlen(file_entries[i].name) + 1) <= terminal_width)) {
        }
        else if ((flags->p || flags->F) && is_smth(file_entries[i].path) && ((total_width + mx_strlen(file_entries[i].name) + 2 + 1) > terminal_width)) {
            mx_printchar('\n');
            total_width = 0;
        }
        else if ((i + 1 == count) && ((total_width + mx_strlen(file_entries[i].name)) <= terminal_width)) {
        }
        else if ((total_width + mx_strlen(file_entries[i].name) + 2) >= terminal_width) {
            mx_printchar('\n');
            total_width = 0;
        }
        if (flags->i) {
            printstr_formatted(mx_ultoa(file_entries[i].inode), 8, true);
            mx_printchar(' ');
        }
        if (flags->G) {
            struct stat sb;
            if (lstat(file_entries[i].path, &sb) == -1) {}
            switch_strcolor(sb);
        }
        mx_printstr(file_entries[i].name);
        if (flags->G)
            mx_printstr(DEFAULT_COLOR);
        if (flags->p && is_dir(file_entries[i].path))
            mx_printchar('/');
        else if (flags->F && is_executable(file_entries[i].path)) {
            mx_printchar('*');
        }
        else if (flags->F && is_link(file_entries[i].path)) {
            mx_printchar('@');
        }
        else if (flags->F && is_socket(file_entries[i].path)) {
            mx_printchar('=');
        }
        else if (flags->F && is_whiteout(file_entries[i].path)) {
            mx_printchar('%');
        }
        else if (flags->F && is_fifo(file_entries[i].path)) {
            mx_printchar('|');
        }
        if (i + 1 != count)
            mx_printstr(", ");
        total_width = total_width + mx_strlen(file_entries[i].name) + 2;
        if ((flags->p && is_dir(file_entries[i].path)) || (flags->F && is_smth(file_entries[i].path)))
            total_width++;
        if (flags->i)
            total_width += 9;
    }
    mx_printchar('\n');
}

void print_err(const char *filename) {
    mx_printerr("uls: ");
    mx_printerr(filename);
    mx_printerr(": ");
    mx_printerr(strerror(errno));
    mx_printerr("\n");
}
