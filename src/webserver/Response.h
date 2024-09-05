//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_RESPONSE_H
#define C_WEBSERVER_RESPONSE_H

#include <stdbool.h>
#include "Request.h"
#include "ContentType.h"

typedef struct Response Response;

struct Response {
    enum ContentType contentType;
    bool auto_clean_up;
    int status_code;
    bool error;
    int socket;
};

void send_request(Response *response, char *content);
void json_request(Response *response, char *content);
void set_content_type(Response *response, enum ContentType contentType);
void set_status_code(Response *response);
Response *create_response();

#endif //C_WEBSERVER_RESPONSE_H
