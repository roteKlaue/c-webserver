//
// Created by xxrot on 03.09.2024.
//

#include <ctype.h>
#include "StringUtil.h"

char *strcpy_until_char(char *dest, const char *src, char stop_char)
{
    int i = 0;
    while (src[i] != '\0') {
        if (src[i] == stop_char) {
            break;
        }
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

void to_uppercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char) toupper(str[i]);
    }
}

void to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char) tolower(str[i]);
    }
}
