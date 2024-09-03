//
// Created by xxrot on 03.09.2024.
//

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