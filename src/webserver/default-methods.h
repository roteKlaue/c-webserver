//
// Created by xxrot on 06.09.2024.
//

#ifndef C_WEBSERVER_DEFAULT_METHODS_H
#define C_WEBSERVER_DEFAULT_METHODS_H

#include "Response.h"
#include "Request.h"


void default_not_found_function(Request *request, Response *response);
void default_internal_server_error(Request *request, Response *response);

#endif //C_WEBSERVER_DEFAULT_METHODS_H
