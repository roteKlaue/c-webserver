//
// Created by xxrot on 06.09.2024.
//

#include "default-methods.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void default_not_found_function(const Request *request, Response *response)
{
    const char *prefix = "404 not found: ";
    const char *path = request->absolute_path;

    const size_t result_length = strlen(prefix) + strlen(path) + 1;

    char *result = malloc(result_length);

    if (result == NULL) return;

    strcpy(result, prefix);
    strcat(result, path);

    insert_table(response->headers, "path", request->absolute_path);

    set_status_code(response, NOT_FOUND);
    response->auto_clean_up = true;
    send_response(response, result);
}

void default_internal_server_error(const Request *request, Response *response, const char *error)
{
    response->auto_clean_up = true;
    set_status_code(response, INTERNAL_SERVER_ERROR);

    const char *pattern = "internal server error\nError: %s";
    const size_t new_buffer_size = strlen(error) + strlen(pattern) + 1;

    char *new_buffer = malloc(sizeof(char) * new_buffer_size);
    if (new_buffer == NULL) return;

    snprintf(new_buffer, new_buffer_size, pattern, error);
    send_response(response, new_buffer);
}
