#include "my_string.h"
int str_cmp(const char *s1, const char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

char *str_cpy(char *dest, const char *src) {
    char *ret = dest;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return ret;
}

int str_len(const char *s) {
    if (s == 0) {
        return 0;
    } 
    int ret = 0;
    while (*s++) {
        ret++;
    }
    return ret;
}

void remove_newline(char *s) {
    while (*s != '\0') {
        if (*s == '\n') {
            *s = '\0';
            break;
        }
        s++;
    }
}
