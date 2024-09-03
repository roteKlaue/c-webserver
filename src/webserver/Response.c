//
// Created by xxrot on 03.09.2024.
//

#include "Response.h"

void json_request(Request *request, Response *response, char *content)
{
    response->contentType = JSON;
    send_request(request, response, content);
}