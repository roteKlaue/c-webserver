//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_STRINGUTIL_H
#define C_WEBSERVER_STRINGUTIL_H

void copy_until_index(const char *source, char *destination, size_t index);
int find_char_index(const char *src, char find_char);
char *strcpy_until_char(char *dest, const char *src, char stop_char);
char *strcpy_after_char(char *dest, const char *src, char start_char);
void to_uppercase(char *str);
void to_lowercase(char *str);

#endif //C_WEBSERVER_STRINGUTIL_H
