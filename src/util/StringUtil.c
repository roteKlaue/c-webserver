//
// Created by xxrot on 03.09.2024.
//

#include <ctype.h>
#include <string.h>
#include "StringUtil.h"

void copy_until_index(const char *source, char *destination, size_t index)
{
    if (index == 0 || source == NULL || destination == NULL) return;

    memcpy(destination, source, index);
    destination[index] = '\0';
}

int find_char_index(const char *src, char find_char)
{
    int index = 0;
    while (src[index] != '\0')
    {
        if (src[index] == find_char)
        {
            return index;
        }
        index++;
    }
    return -1;
}

char *strcpy_until_char(char *dest, const char *src, char stop_char)
{
    int i = find_char_index(src, stop_char);

    if (i == -1) {
        strcpy(dest, src);
    } else {
        copy_until_index(src, dest, i);
    }

    return dest;
}


void to_uppercase(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = (char) toupper(str[i]);
    }
}

void to_lowercase(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = (char) tolower(str[i]);
    }
}

char *strcpy_after_char(char *dest, const char *src, char start_char)
{
    int start_index = find_char_index(src, start_char);

    if (start_index != -1) {
        start_index++;
    } else {
        start_index = 0;
    }

    copy_until_index(src + start_index, dest, strlen(src) - start_index);

    return dest;
}
