//
// Created by xxrot on 02.09.2024.
//

#ifndef C_WEBSERVER_WEBSERVER_H
#define C_WEBSERVER_WEBSERVER_H
#include <stdbool.h>
#include "../util/HashTable.h"
#include "Request.h"
#include "Response.h"

#define DEFAULT_BUFFER_SIZE 4096
#define DEFAULT_PORT 4096

typedef struct {
    HashTable *routes;
    void (*not_found)(Request *, Response *);
    void (* internal_server_error)(Request *, Response *);
    bool continue_running;
    int port;
    int socket;
    int buffer_size;
} Webserver;

Webserver *create_webserver();
void free_webserver(Webserver *webserver);
bool run_webserver(Webserver *webserver);
void clean_up_webserver(Webserver *webserver);
HashTable *create_routing_table();
void free_routing_table(HashTable *routing_table);

#endif //C_WEBSERVER_WEBSERVER_H
