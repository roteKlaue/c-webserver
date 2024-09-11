//
// Created by xxrot on 03.09.2024.
//

#include <stdlib.h>
#include <string.h>
#include "ContentType.h"

const char *JSON_TYPE = "application/json";
const char *TEXT_TYPE = "text/plain";
const char *HTML_TYPE = "text/html";
const char *XML_TYPE = "application/xml";
const char *CSS_TYPE = "text/css";
const char *JS_TYPE = "application/javascript";
const char *UNKNOWN_TYPE = "unknown";

const char* ContentType_to_string(enum ContentType content_type)
{
    switch (content_type) {
        case TEXT:
            return TEXT_TYPE;
        case JSON:
            return JSON_TYPE;
        case JS:
            return JS_TYPE;
        case HTML:
            return HTML_TYPE;
        case XML:
            return XML_TYPE;
        case CSS:
            return CSS_TYPE;
        default:
            return UNKNOWN_TYPE;
    }
}

enum ContentType string_to_ContentType(const char *content_type_str)
{
    if (content_type_str == NULL)
    {
        return UNKNOWN;
    }

    if (strcmp(content_type_str, TEXT_TYPE) == 0) {
        return TEXT;
    } else if (strcmp(content_type_str, JSON_TYPE) == 0) {
        return JSON;
    } else if (strcmp(content_type_str, JS_TYPE) == 0) {
        return JS;
    } else if (strcmp(content_type_str, HTML_TYPE) == 0) {
        return HTML;
    } else if (strcmp(content_type_str, XML_TYPE) == 0) {
        return XML;
    } else if (strcmp(content_type_str, CSS_TYPE) == 0) {
        return CSS;
    } else {
        return UNKNOWN;
    }
}
