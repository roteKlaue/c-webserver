//
// Created by xxrot on 03.09.2024.
//

#include "Response.h"
#include "StatusCode.h"
#include "webserver_headers.h"
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

void send_request(Response *response, char *content)
{
    if (response == NULL)
    {
        return;
    }

    const char *content_type = ContentType_to_string(response->contentType);
    const char *response_code = StatusCode_to_string(response->status_code);
    size_t page_response_length = strlen(content);
    size_t content_type_length = strlen(content_type);
    size_t response_code_length = strlen(response_code);
    size_t num_digits = digit_count(page_response_length);
    size_t response_code_num_digits = digit_count((int) response->status_code);

    char *str_value = (char *) malloc(sizeof(char) * (num_digits + 1));

    if (str_value == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        response->error = true;
        if(response->auto_clean_up)
        {
            free(content);
        }
        return;
    }

    sprintf(str_value, "%zu", page_response_length);

    size_t response_length = strlen("HTTP/1.1  \r\nContent-Type: \r\nContent-Length: \r\n\r\n") + content_type_length + num_digits + page_response_length + response_code_length + response_code_num_digits + 1;
    char *response_finished = (char *) malloc(response_length);

    if (response_finished == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        free(str_value);
        response->error = true;
        if(response->auto_clean_up)
        {
            free(content);
        }
        return;
    }
    snprintf(response_finished, response_length, "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nContent-Length: %s\r\n\r\n%s", (int) response->status_code, response_code, content_type, str_value, content);

    send(response->socket, response_finished, (int)strlen(response_finished), 0);
    free(str_value);
    free(response_finished);
    if(response->auto_clean_up)
    {
        free(content);
    }
}

void json_request(Response *response, char *content)
{
    response->contentType = JSON;
    send_request(response, content);
}

/*
    send(client_socket, response, (int)strlen(response), 0);
    close(client_socket);

    if(page_response->clean_up)
    {
        free(page_response->content);
    }

    free_route(page_response);
    free_table(params);
    free(response);
    free(buffer);
    free(str_value);
     */