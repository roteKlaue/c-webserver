//
// Created by xxrot on 18.10.2024.
//

#ifndef C_WEBSERVER_ROUTER_H
#define C_WEBSERVER_ROUTER_H

#include "export.h"

typedef enum {
    ROUTER = 0,
    ROUTE = 1,
    // MIDDLE_WARE
} RoutingEntryType;

CREST_EXPORT typedef struct {
    RoutingEntryType type;
    void *data;
} RoutingEntry;

CREST_EXPORT RoutingEntry *create_routing_entry(void *value, RoutingEntryType type);
CREST_EXPORT void free_routing_structure(HashTable *routing_table_entry);
CREST_EXPORT HashTable *create_routing_table();

#endif //C_WEBSERVER_ROUTER_H
