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

typedef enum {
    ROUTER,
    ROUTE,
    // MIDDLE_WARE
} RoutingEntryType;

typedef struct {
    RoutingEntryType type;
    void *data;
} RoutingEntry;

typedef struct {
    HashTable *routes;
    void (*not_found)(Request *, Response *);
    void (*internal_server_error)(Request *, Response *, const char *);
    bool continue_running;
    int port;
    int socket;
    int buffer_size;
} Webserver;

Webserver *create_webserver();
RoutingEntry *create_routingentry(void *val, RoutingEntryType type);
void free_webserver(Webserver *webserver);
bool run_webserver(Webserver *webserver);
void clean_up_webserver(Webserver *webserver);
HashTable *create_routing_table();
void free_routing_table(HashTable *routing_table);
void add_route(HashTable *routing_table, enum Method method, char *route,
        void (*route_implementation)(Request *, Response *));
void add_router(HashTable *routing_table, char *default_route, HashTable *router);
void free_routingentry(RoutingEntry *entry);

#endif //C_WEBSERVER_WEBSERVER_H
