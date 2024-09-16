//
// Created by xxrot on 12/09/2024.
//

#ifndef C_WEBSERVER_STATIC_HOSTING_H
#define C_WEBSERVER_STATIC_HOSTING_H

#include <stdbool.h>
#include "../../util/HashTable.h"
#include "../Response.h"
#include "../Request.h"

HashTable *create_static_hosting_router(const char *base_route);
ArrayList *list_directory(const char *path);
bool isFolder(const char *path);
void in_between_function(Request *request, Response *response);
const char* get_last_path_segment(const char *requested_path);

#endif //C_WEBSERVER_STATIC_HOSTING_H
