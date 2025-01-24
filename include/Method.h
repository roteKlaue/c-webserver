//
// Created by xxrot on 03.09.2024.
// Description: Defines HTTP methods as an enum and provides utility functions
//              for converting between enum values and their string representations.
//

#ifndef C_WEBSERVER_METHOD_H
#define C_WEBSERVER_METHOD_H

/**
 * Enum: Method
 * Purpose: Enumerates the standard HTTP methods supported by the web server.
 */
enum Method {
    Get,        // HTTP GET method, used to retrieve data.
    Delete,     // HTTP DELETE method, used to delete resources.
    Post,       // HTTP POST method, used to create or submit data.
    Patch,      // HTTP PATCH method, used to partially update resources.
    Put,        // HTTP PUT method, used to fully update or create resources.
    Options,    // HTTP OPTIONS method, used to describe communication options.
    Head,       // HTTP HEAD method, used to retrieve headers without the body.
    NUM_METHODS // Total number of methods. Used as ERROR if invalid Method was passed and for bounds checking
};

/**
 * Function: Method_to_string
 * Purpose: Converts an HTTP method enum value to its string representation.
 *
 * @param method - The HTTP method to convert (from the Method enum).
 * @return A string representing the HTTP method, e.g., "GET" for `Get`.
 *         If the method is invalid, returns "Unknown Method".
 */
const char *Method_to_string(enum Method method);

/**
 * Function: string_to_method
 * Purpose: Converts a string representation of an HTTP method to its enum value.
 *
 * @param method_str - The string representing the HTTP method (case-insensitive).
 *                     Example: "GET" or "get".
 * @return The corresponding Method enum value, or NUM_METHODS if the string
 *         does not match any valid method.
 */
enum Method string_to_method(const char *method_str);

#endif //C_WEBSERVER_METHOD_H
