//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_METHOD_H
#define C_WEBSERVER_METHOD_H

enum Method {
    Get,
    Delete,
    Post,
    Patch,
    Put,
    Options,
    Head,
    NUM_METHODS
};

const char *Method_to_string(enum Method method);

#endif //C_WEBSERVER_METHOD_H
