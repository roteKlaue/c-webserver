//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_REQUEST_H
#define C_WEBSERVER_REQUEST_H

#include "../util/HashTable.h"
#include "Method.h"

typedef struct {
    int port;
    char *absolute_path;
    char **params;
    int paramCount;
    enum Method method;
    HashTable *query_params;
    char *body;
} Request;

Request *create_request(int port,
                        char *absolute_path,
                        char **params,
                        enum Method method,
                        HashTable *query_params,
                        char *body);

void free_request(Request *request);

#endif //C_WEBSERVER_REQUEST_H
