//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_RESPONSE_H
#define C_WEBSERVER_RESPONSE_H

#include <stdbool.h>
#include "HashTable.h"
#include "webserver_headers.h"
#include "ContentType.h"
#include "StatusCode.h"
#include "export.h"

CREST_EXPORT typedef struct Response Response;

CREST_EXPORT struct Response {
    enum ContentType contentType;
    HashTable *headers;
    bool auto_clean_up;
    int status_code;
    const char *error;
    SOCKET socket;
};

CREST_EXPORT void send_response(Response *response, char *content);
CREST_EXPORT void json_response(Response *response, char *content);
CREST_EXPORT void set_content_type(Response *response, enum ContentType contentType);
CREST_EXPORT void set_status_code(Response *response, enum StatusCode statusCode);
CREST_EXPORT void redirect_response(Response *response, const char *redirect_url, bool permanent);
CREST_EXPORT void send_file_response(Response *response, const void *data, size_t data_length);
CREST_EXPORT Response *create_response(SOCKET socket);
CREST_EXPORT void free_response(Response *response);

#endif //C_WEBSERVER_RESPONSE_H
