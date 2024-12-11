#include "my_string.h"
#include <stdlib.h>
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

char *str_tok(char *s, const char *delim) {
    static char *saveptr;
    if (s == 0) {
        s = saveptr;
    }
    if (s == 0 || *s == '\0') {
        return 0;
    }
    char *token = s;
    while (*s != '\0' && str_chr(delim, *s) == 0) {
        s++;
    }
    if (*s != '\0') {
        *s++ = '\0';
    }
    saveptr = s;
    return token;
}

char *str_chr(const char *s, int c) {
    while (*s) {
        if (*s == c) {
            return (char *) s;
        }
        s++;
    }
    return 0;
}

char *str_dup(const char *s) {
    int len = str_len(s) + 1;
    char *dest = (char *) malloc(len);
    if (dest == 0) {
        return 0;
    }
    str_cpy(dest, s);
    return dest;
}