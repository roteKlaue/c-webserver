//
// Created by xxrot on 03.09.2024.
//
#include "Method.h"

const char *Method_to_string(enum Method method) {
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