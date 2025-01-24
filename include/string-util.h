//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_STRING_UTIL_H
#define C_WEBSERVER_STRING_UTIL_H

void copy_until_index(const char *src, char *destination, int index, size_t destination_size);
int find_char_index(const char *src, char find_char);
void string_copy_until_char(char *destination, const char *src, char stop_char, size_t destination_size);
void string_copy_after_char(char *destination, const char *src, char start_char, size_t destination_size);
char *to_uppercase(const char *str);
char *to_lowercase(const char *str);
char* substring(const char* str, int start, size_t length);
int string_count_occurrences(const char *str, char key);
char **string_split(const char *str, char key, int *size);
void free_string_parts(char **parts, int size);

#endif //C_WEBSERVER_STRING_UTIL_H
