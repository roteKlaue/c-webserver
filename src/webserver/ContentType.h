//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_CONTENTTYPE_H
#define C_WEBSERVER_CONTENTTYPE_H

enum ContentType {
    JSON,
    TEXT,
    HTML,
    XML,
    CSS,
    JS
};

const char* ContentType_to_string(enum ContentType content_type);

#endif //C_WEBSERVER_CONTENTTYPE_H
