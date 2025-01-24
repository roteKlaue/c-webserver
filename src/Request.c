//
// Created by xxrot on 03.09.2024.
//

#include <stdlib.h>
#include <string.h>
#include "Request.h"

Request *create_request(const int port,
                        const char *path,
                        char *absolute_path,
                        char **params,
                        const enum Method method,
                        HashTable *query_params,
                        char *body)
{
    Request *request = malloc(sizeof(Request));

    if (request == NULL)
    {
        return NULL;
    }

    request->absolute_path = strdup(absolute_path);
    request->query_params = query_params;
    request->params = params;
    request->method = method;
    request->body = body;
    request->port = port;
    request->path = path;
    return request;
}

void free_request(Request *request)
{
    if (request == NULL) return;

    if (request->params != NULL)
    {
        for (int i = 0; i < request->paramCount; ++i)
        {
            if (request->params[i] != NULL)
            {
                free(request->params[i]);
            }
        }
        free(request->params);
    }

    free_table(request->query_params);
    free(request->absolute_path);
    free(request);
}
