//
// Created by xxrot on 02.09.2024.
//

#ifndef CREST_WEBSERVER_H
#define CREST_WEBSERVER_H

#include <stdbool.h>

#include "StopNotifier.h"
#include "HashTable.h"
#include "Request.h"
#include "Response.h"
#include "Router.h"
#include "export.h"

#define DEFAULT_BUFFER_SIZE 4096
#define DEFAULT_THREAD_COUNT 4
#define DEFAULT_PORT 4096

typedef void (*route_implementation)(const Request *, Response *);

typedef struct {
    HashTable *routes;
    route_implementation not_found;
    void (*internal_server_error)(const Request *, Response *, const char *);
    bool continue_running;
    int port;
    SOCKET socket;
    int buffer_size;
    int thread_count;
    int max_tasks_per_thread;
    StopNotifier *stop_notifier;
} Webserver;

CREST_EXPORT void initialise_webserver_framework();
CREST_EXPORT void clean_up_webserver_framework();
CREST_EXPORT Webserver *create_webserver();
CREST_EXPORT void free_webserver(Webserver *webserver);
CREST_EXPORT bool run_webserver(Webserver *webserver);
CREST_EXPORT void clean_up_webserver(Webserver *webserver);
CREST_EXPORT void add_route(const HashTable *routing_table, enum Method method, const char *route,
                   route_implementation implementation);
CREST_EXPORT void add_router(const HashTable *routing_table, const char *default_route, HashTable *router);
CREST_EXPORT void stop_webserver(Webserver *webserver);

#endif //CREST_WEBSERVER_H
