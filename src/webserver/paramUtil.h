//
// Created by xxrot on 03.09.2024.
//

#ifndef C_WEBSERVER_PARAMUTIL_H
#define C_WEBSERVER_PARAMUTIL_H

#include "../util/HashTable.h"

void parse_url_params(HashTable *table, const char *url);

#endif //C_WEBSERVER_PARAMUTIL_H
