//
// Created by xxrot on 03.09.2024.
//
#include <string.h>
#include <stdlib.h>
#include "Method.h"
#include "../util/string-util.h"

const char *Method_to_string(enum Method method)
{
    switch (method) {
        case Get:
            return "GET";
        case Delete:
            return "DELETE";
        case Post:
            return "POST";
        case Patch:
            return "PATCH";
        case Put:
            return "PUT";
        case Options:
            return "OPTIONS";
        case Head:
            return "HEAD";
        default:
            return "UNKNOWN";
    }
}

enum Method string_to_method(const char *method_str)
{
    char *upper = malloc(strlen(method_str) + 1);
    if (upper == NULL) {
        return NUM_METHODS;
    }

    strcpy(upper, method_str);
    to_uppercase(upper);

    enum Method method;
    if (strcmp(upper, "GET") == 0) {
        method = Get;
    } else if (strcmp(upper, "DELETE") == 0) {
        method = Delete;
    } else if (strcmp(upper, "POST") == 0) {
        method = Post;
    } else if (strcmp(upper, "PATCH") == 0) {
        method = Patch;
    } else if (strcmp(upper, "PUT") == 0) {
        method = Put;
    } else if (strcmp(upper, "OPTIONS") == 0) {
        method = Options;
    } else if (strcmp(upper, "HEAD") == 0) {
        method = Head;
    } else {
        method = NUM_METHODS;
    }

    free(upper);

    return method;
}