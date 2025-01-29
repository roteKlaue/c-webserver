//
// Created by xxrot on 03.09.2024.
//

#include <stdlib.h>
#include <string.h>
#include "ContentType.h"
#include "string-util.h"

const char *JSON_TYPE = "application/json";
const char *TEXT_TYPE = "text/plain";
const char *HTML_TYPE = "text/html";
const char *XML_TYPE = "application/xml";
const char *CSS_TYPE = "text/css";
const char *JS_TYPE = "application/javascript";
const char *JPG_TYPE = "image/jpeg";
const char *PNG_TYPE = "image/png";
const char *GIF_TYPE = "image/gif";
const char *UNKNOWN_TYPE = "application/octet-stream";

const char *ContentType_to_string(const enum ContentType content_type)
{
    switch (content_type)
    {
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
    case JPG:
        return JPG_TYPE;
    case PNG:
        return PNG_TYPE;
    case GIF:
        return GIF_TYPE;
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

    if (strcmp(content_type_str, TEXT_TYPE))
    {
        return TEXT;
    }
    if (strcmp(content_type_str, JSON_TYPE))
    {
        return JSON;
    }
    if (strcmp(content_type_str, JS_TYPE))
    {
        return JS;
    }
    if (strcmp(content_type_str, HTML_TYPE))
    {
        return HTML;
    }
    if (strcmp(content_type_str, XML_TYPE))
    {
        return XML;
    }
    if (strcmp(content_type_str, CSS_TYPE))
    {
        return CSS;
    }
    if (strcmp(content_type_str, CSS_TYPE))
    {
        return CSS;
    }
    return UNKNOWN;
}

enum ContentType guess_content_type(const char *file_path)
{
    if (file_path == NULL) return UNKNOWN;

    const char *dot = strrchr(file_path, '.');
    if (!dot) return UNKNOWN;

    if (string_equals_ignore_case(dot, ".html") || string_equals_ignore_case(dot, ".htm"))
        return HTML;
    if (string_equals_ignore_case(dot, ".css"))
        return CSS;
    if (string_equals_ignore_case(dot, ".js"))
        return JS;
    if (string_equals_ignore_case(dot, ".json"))
        return JSON;
    if (string_equals_ignore_case(dot, ".png"))
        return PNG;
    if (string_equals_ignore_case(dot, ".jpg") || string_equals_ignore_case(dot, ".jpeg"))
        return JPG;
    if (string_equals_ignore_case(dot, ".gif"))
        return GIF;

    return UNKNOWN;
}
