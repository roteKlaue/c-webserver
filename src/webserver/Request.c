//
// Created by xxrot on 03.09.2024.
//

#include <stdlib.h>
#include "Request.h"

Request *create_request(const int port,
                        char *absolute_path,
                        char **params,
                        const enum Method method,
                        HashTable *query_params,
                        char *body)
{
    Request *request = malloc(sizeof(Request));
    request->query_params = query_params;
    request->body = body;
    request->params = params;
    request->method = method;
    request->absolute_path = absolute_path;
    request->port = port;
    return request;
}

void free_request(Request *request)
{
    if (request == NULL) return;

    if (request->body != NULL) {
        free(request->body);
    }

    if (request->params != NULL) {
        for (int i = 0; i < request->paramCount; ++i) {
            if (request->params[i] != NULL) {
                free(request->params[i]);
            }
        }
        free(request->params);
    }

    free(request->absolute_path);
    free_table(request->query_params);
    free(request);
}
