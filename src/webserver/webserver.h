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
#define DEFAULT_THREAD_COUNT 4
#define DEFAULT_PORT 4096

typedef enum {
    ROUTER = 0,
    ROUTE = 1,
    // MIDDLE_WARE
} RoutingEntryType;

typedef struct {
    RoutingEntryType type;
    void *data;
} RoutingEntry;

typedef void (*route_implementation)(const Request *, Response *);

typedef struct {
    HashTable *routes;
    route_implementation not_found;
    void (*internal_server_error)(const Request *, Response *, const char *);
    bool continue_running;
    int port;
    int socket;
    int buffer_size;
    int thread_count;
    int max_tasks_per_thread;
} Webserver;

void initialise_webserver_framework();
void clean_up_webserver_framework();
Webserver *create_webserver();
RoutingEntry *create_routing_entry(void *value, RoutingEntryType type);
void free_webserver(Webserver *webserver);
bool run_webserver(Webserver *webserver);
void clean_up_webserver(Webserver *webserver);
HashTable *create_routing_table();
void add_route(const HashTable *routing_table, enum Method method, const char *route,
               route_implementation implementation);
void add_router(const HashTable *routing_table, const char *default_route, HashTable *router);
void free_routing_structure(HashTable *routing_table_entry);

#endif //C_WEBSERVER_WEBSERVER_H
