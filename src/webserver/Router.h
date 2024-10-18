//
// Created by xxrot on 18.10.2024.
//

#ifndef C_WEBSERVER_ROUTER_H
#define C_WEBSERVER_ROUTER_H

typedef enum {
    ROUTER = 0,
    ROUTE = 1,
    // MIDDLE_WARE
} RoutingEntryType;

typedef struct {
    RoutingEntryType type;
    void *data;
} RoutingEntry;

RoutingEntry *create_routing_entry(void *value, RoutingEntryType type);
void free_routing_structure(HashTable *routing_table_entry);
HashTable *create_routing_table();

#endif //C_WEBSERVER_ROUTER_H
