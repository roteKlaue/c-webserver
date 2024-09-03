//
// Created by xxrot on 03.09.2024.
//

#include "ContentType.h"

const char *JSON_TYPE = "application/json";
const char *TEXT_TYPE = "text/plain";
const char *HTML_TYPE = "text/html";
const char *XML_TYPE = "application/xml";
const char *CSS_TYPE = "text/css";
const char *JS_TYPE = "application/javascript";

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
            return "unknown";
    }
}