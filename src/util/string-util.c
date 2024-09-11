//
// Created by xxrot on 03.09.2024.
//

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "string-util.h"

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

void copy_until_index(const char *src, char *dest, int index)
{
    for (int i = 0; i < index; ++i) {
        dest[i] = src[i];
    }
    dest[index] = '\0';
}

char *strcpy_until_char(char *dest, const char *src, char stop_char)
{
    int index = find_char_index(src, stop_char);

    if (index == -1) {
        strcpy(dest, src);
    } else {
        copy_until_index(src, dest, index);
    }

    return dest;
}

char *to_uppercase(const char *str)
{
    char *upper = strdup(str);
    for (int i = 0; str[i] != '\0'; i++)
    {
        upper[i] = (char) toupper(upper[i]);
    }
    return upper;
}

char *to_lowercase(const char *str)
{
    char *lower = strdup(str);
    for (int i = 0; str[i] != '\0'; i++)
    {
        lower[i] = (char) tolower(lower[i]);
    }
    return lower;
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

char* substring(const char* str, int start, size_t length) {
    if (str == NULL || start < 0 || length <= 0 || start >= strlen(str)) {
        return NULL;
    }

    size_t str_length = strlen(str);
    if (start + length > str_length) {
        length = str_length - start;
    }

    char* substr = (char*)malloc((length + 1) * sizeof(char));

    if (substr == NULL) {
        return NULL;
    }

    strncpy(substr, str + start, length);
    substr[length] = '\0';

    return substr;
}

int string_count_occurrences(const char *str, const char key)
{
    if (str == NULL) return -1;
    int count = 0;
    for (int i = 0; i < strlen(str); ++i) {
        if (str[i] == key) count++;
    }
    return count;
}

char **string_split(const char *str, const char key, int *size)
{
    if (str == NULL) return NULL;

    int part_count = string_count_occurrences(str, key) + 1;

    char **parts = malloc(sizeof(char *) * part_count);
    if (parts == NULL) return NULL;

    int start = 0, part_index = 0;

    for (int i = 0; i <= strlen(str); ++i)
    {
        if (str[i] == key || str[i] == '\0')
        {
            int length = i - start;

            parts[part_index] = malloc(sizeof(char) * (length + 1));

            if (parts[part_index] == NULL)
            {
                for (int j = 0; j < part_index; j++)
                {
                    free(parts[j]);
                }
                free(parts);
                return NULL;
            }

            strncpy(parts[part_index], str + start, length);
            parts[part_index][length] = '\0';

            part_index++;
            start = i + 1;
        }
    }

    *size = part_count;

    return parts;
}

void free_string_parts(char **parts, const int size) {
    if (parts == NULL) return;

    for (int i = 0; i < size; i++) {
        free(parts[i]);
    }
    free(parts);
}
