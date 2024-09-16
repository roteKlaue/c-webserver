//
// Created by xxrot on 06.09.2024.
//

#include "default-methods.h"
#include <string.h>
#include <stdlib.h>

void default_not_found_function(Request *request, Response *response)
{
    const char *prefix = "404 not found: ";
    char *path = request->absolute_path;

    size_t result_length = strlen(prefix) + strlen(path) + 1;

    char *result = (char *) malloc(result_length);

    if (result == NULL) return;

    strcpy(result, prefix);
    strcat(result, path);

    send_response(response, result);
    free(result);
}

void default_internal_server_error(Request *request, Response *response, const char *_error)
{
    set_status_code(response, INTERNAL_SERVER_ERROR);
    send_response(response, "internal server error");
}