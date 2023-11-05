#include "../inc/libmx.h"

int mx_sscanf(const char *str, const char *format, char *city1, char *city2, int *distance) {
   int parsed_values = 0;

    if (city1) {
        *city1 = '\0';
    }
    if (city2) {
        *city2 = '\0';
    }

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            if (*format == 's') {
                int i = 0;
                char *city = (parsed_values == 0 && city1) ? city1 : city2;
                while (*str != '\0' && *str != ' ' && *str != ',' && *str != '-') {
                    if (city) {
                        city[i] = *str;
                    }
                    i++;
                    str++;
                }
                if (city) {
                    city[i] = '\0';
                }
                parsed_values++;
            } else if (*format == 'd') {
                *distance = 0;
                while (*str >= '0' && *str <= '9') {
                    *distance = *distance * 10 + (*str - '0');
                    str++;
                }
                parsed_values++;
            }
        } else {
            if (*str != *format) {
                return parsed_values;
            }
            str++;
        }
        format++;
    }

    return parsed_values;
}
