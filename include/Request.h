//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_REQUEST_H
#define C_WEBSERVER_REQUEST_H

#include "HashTable.h"
#include "Method.h"
#include "export.h"

CREST_EXPORT typedef struct {
    int port;
    char *body;
    char **params;
    int paramCount;
    const char *path;
    enum Method method;
    char *absolute_path;
    HashTable *query_params;
} Request;

Request *create_request(int port,
                        const char *path,
                        char *absolute_path,
                        char **params,
                        enum Method method,
                        HashTable *query_params,
                        char *body);

void free_request(Request *request);

#endif //C_WEBSERVER_REQUEST_H
