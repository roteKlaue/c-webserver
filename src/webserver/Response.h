//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_RESPONSE_H
#define C_WEBSERVER_RESPONSE_H

#include <stdbool.h>
#include "Request.h"
#include "ContentType.h"
#include "StatusCode.h"

typedef struct Response Response;

struct Response {
    enum ContentType contentType;
    HashTable *headers;
    bool auto_clean_up;
    int status_code;
    const char *error;
    int socket;
};

void send_response(Response *response, char *content);
void json_response(Response *response, char *content);
void set_content_type(Response *response, enum ContentType contentType);
void set_status_code(Response *response, enum StatusCode statusCode);
Response *create_response(int socket);
void free_response(Response *response);

#endif //C_WEBSERVER_RESPONSE_H
