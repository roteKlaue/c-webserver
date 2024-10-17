//
// Created by xxrot on 03.09.2024.
//

#include "webserver_headers.h"
#include "Response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t digit_count(size_t number)
{
    if (number == 0)
        return 1;

    size_t count = 0;
    while (number != 0)
    {
        number /= 10;
        count++;
    }
    return count;
}

void send_response(Response *response, char *content)
{
    if (response == NULL || content == NULL)
    {
        return;
    }

    const char *content_type = ContentType_to_string(response->contentType);
    const char *response_code = StatusCode_to_string(response->status_code);
    const size_t page_response_length = strlen(content);
    const size_t content_type_length = strlen(content_type);
    const size_t response_code_length = strlen(response_code);
    const size_t num_digits = digit_count(page_response_length);
    const size_t response_code_num_digits = digit_count(response->status_code);

    char *str_value = malloc(sizeof(char) * (num_digits + 1));

    if (str_value == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        response->error = "Memory allocation failed";
        if(response->auto_clean_up)
        {
            free(content);
        }
        return;
    }

    sprintf(str_value, "%zu", page_response_length);

    size_t headers_length = 0;
    int header_count = 0;
    char **header_keys = table_keys(response->headers, &header_count);

    for (int i = 0; i < header_count; i++)
    {
        const char *key = header_keys[i];
        const char *value = search_table(response->headers, key);
        if (key != NULL && value != NULL)
        {
            headers_length += strlen(key) + strlen(value) + strlen(": \r\n");
        }
    }

    const size_t response_length = strlen("HTTP/1.1  \r\nContent-Type: \r\nContent-Length: \r\n\r\n") +
        content_type_length + num_digits + page_response_length +
        response_code_length + response_code_num_digits + headers_length  + 1;
    char *response_finished = malloc(response_length);

    if (response_finished == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        free(str_value);
        response->error = "Memory allocation failed";
        if(response->auto_clean_up)
        {
            free(content);
        }
        free_table_keys(header_keys, header_count);
        return;
    }

    snprintf(response_finished, response_length,
             "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nContent-Length: %s\r\n",
             response->status_code, response_code, content_type, str_value);

    size_t offset = strlen(response_finished);
    for (int i = 0; i < header_count; i++)
    {
        const char *key = header_keys[i];
        const char *value = search_table(response->headers, key);
        if (key != NULL && value != NULL)
        {
            snprintf(response_finished + offset, response_length - offset, "%s: %s\r\n", key, value);
            offset += strlen(key) + strlen(value) + strlen(": \r\n");
        }
    }

    snprintf(response_finished + offset, response_length - offset, "\r\n%s", content);

    send(response->socket, response_finished, (int) strlen(response_finished), 0);
    close(response->socket);

    free(str_value);
    free(response_finished);
    free_table_keys(header_keys, header_count);
    if(response->auto_clean_up)
    {
        free(content);
    }
}

void send_file_response(Response *response, const void *data, const size_t data_length)
{
    if (response == NULL || data == NULL)
    {
        return;
    }

    const char *content_type = ContentType_to_string(response->contentType);
    const char *response_code = StatusCode_to_string(response->status_code);
    const size_t content_type_length = strlen(content_type);
    const size_t response_code_length = strlen(response_code);
    const size_t num_digits = digit_count(data_length);

    char str_value[20];
    snprintf(str_value, sizeof(str_value), "%zu", data_length);

    size_t headers_length = 0;
    int header_count = 0;
    char **header_keys = table_keys(response->headers, &header_count);

    for (int i = 0; i < header_count; i++)
    {
        const char *key = header_keys[i];
        const char *value = search_table(response->headers, key);
        if (key != NULL && value != NULL)
        {
            headers_length += strlen(key) + strlen(value) + strlen(": \r\n");
        }
    }

    const size_t response_length = strlen("HTTP/1.1  \r\nContent-Type: \r\nContent-Length: \r\n\r\n") +
        content_type_length + num_digits +
        response_code_length + headers_length + 1;
    char *response_finished = malloc(response_length);

    if (response_finished == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        response->error = "Memory allocation failed";
        free_table_keys(header_keys, header_count);
        return;
    }

    snprintf(response_finished, response_length,
             "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nContent-Length: %s\r\n",
             response->status_code, response_code, content_type, str_value);

    size_t offset = strlen(response_finished);
    for (int i = 0; i < header_count; i++)
    {
        const char *key = header_keys[i];
        const char *value = search_table(response->headers, key);
        if (key != NULL && value != NULL)
        {
            snprintf(response_finished + offset, response_length - offset, "%s: %s\r\n", key, value);
            offset += strlen(key) + strlen(value) + strlen(": \r\n");
        }
    }

    snprintf(response_finished + offset, response_length - offset, "\r\n");

    send(response->socket, response_finished, (int) strlen(response_finished), 0);
    send(response->socket, data, data_length, 0);

    free(response_finished);
    free_table_keys(header_keys, header_count);
    close(response->socket);
}

void redirect_response(Response *response, const char *redirect_url, const bool permanent)
{
    if (response == NULL || redirect_url == NULL)
    {
        return;
    }

    response->status_code = permanent ? PERMANENT_REDIRECT : TEMPORARY_REDIRECT;

    const char *response_code = StatusCode_to_string(response->status_code);
    const size_t response_code_length = strlen(response_code);
    const size_t redirect_url_length = strlen(redirect_url);

    const char *location_header = "Location: ";
    const size_t location_header_length = strlen(location_header);

    const size_t response_length = strlen("HTTP/1.1  \r\n\r\n") +
                                   response_code_length +
                                   location_header_length + redirect_url_length + strlen("\r\n") + 1;

    char *response_finished = malloc(response_length);

    if (response_finished == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        response->error = "Memory allocation failed";
        return;
    }

    snprintf(response_finished, response_length,
             "HTTP/1.1 %d %s\r\n%s%s\r\n\r\n",
             response->status_code, response_code, location_header, redirect_url);

    send(response->socket, response_finished, (int)strlen(response_finished), 0);

    free(response_finished);
    close(response->socket);
}


void json_response(Response *response, char *content)
{
    response->contentType = JSON;
    send_response(response, content);
}

void set_content_type(Response *response, const enum ContentType contentType)
{
    response->contentType = contentType;
}

void set_status_code(Response *response, const enum StatusCode statusCode)
{
    response->status_code = statusCode;
}

Response *create_response(const int socket)
{
    Response *response = malloc(sizeof(Response));

    if (response == NULL) return NULL;

    response->headers = create_table(10);
    response->auto_clean_up = false;
    response->contentType = TEXT;
    response->status_code = OK;
    response->socket = socket;
    response->error = NULL;
    return response;
}

void free_response(Response *response)
{
    free_table(response->headers);
    free(response);
}
