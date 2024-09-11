//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_STRING_UTIL_H
#define C_WEBSERVER_STRING_UTIL_H

void copy_until_index(const char *src, char *dest, int index);
int find_char_index(const char *src, char find_char);
char *strcpy_until_char(char *dest, const char *src, char stop_char);
char *strcpy_after_char(char *dest, const char *src, char start_char);
char *to_uppercase(const char *str);
char *to_lowercase(const char *str);
char* substring(const char* str, int start, size_t length);
int string_count_occurrences(const char *str, char key);
char **string_split(const char *str, char key, int *size);
void free_string_parts(char **parts, int size);

#endif //C_WEBSERVER_STRING_UTIL_H
