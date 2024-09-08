//
// Created by xxrot on 06.09.2024.
//

#ifndef C_WEBSERVER_DEFAULT_METHODS_H
#define C_WEBSERVER_DEFAULT_METHODS_H

#include "Response.h"
#include "Request.h"

/**
 * @brief Default handler for 404 Not Found errors.
 *
 * This function sets a "404 Not Found" message along with the requested path
 * as the response content.
 *
 * @param request The HTTP request that triggered the error.
 * @param response The HTTP response that will be modified.
 */
void default_not_found_function(Request *request, Response *response);


/**
 * @brief Default handler for 500 Internal Server Error.
 *
 * This function sets the status code to 500 and sends a generic "internal server error" message.
 *
 * @param request The HTTP request that triggered the error.
 * @param response The HTTP response that will be modified.
 * @param error string representation of the error
 */
void default_internal_server_error(Request *request, Response *response, const char *error);

#endif //C_WEBSERVER_DEFAULT_METHODS_H
