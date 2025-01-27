//
// Created by xxrot on 18.10.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"
#include "Stack.h"
#include "Method.h"
#include "Router.h"

const enum Method methods[] = {
    Get,
    Delete,
    Post,
    Patch,
    Put,
    Options,
    Head
};

RoutingEntry *create_routing_entry(void *value, const RoutingEntryType type)
{
    RoutingEntry *routing_entry = malloc(sizeof(RoutingEntry));

    if (routing_entry == NULL) {
        perror("Failed to allocate memory for routing entry");
        return NULL;
    }

    routing_entry->type = type;
    routing_entry->data = value;
    return routing_entry;
}

void free_routing_structure(HashTable *routing_table_entry)
{
    if (routing_table_entry == NULL) return;

    Stack *stack = create_default_stack();
    push_stack(stack, routing_table_entry);

    while (!is_stack_empty(stack))
    {
        HashTable *current_table = pop_stack(stack);

        for (int j = 0; j < NUM_METHODS; ++j)
        {
            HashTable *table = search_table(current_table, Method_to_string(methods[j]));

            int keyCount = 0;
            char **keys = table_keys(table, &keyCount);

            for (int i = 0; i < keyCount; ++i) {
                RoutingEntry *entry = search_table(table, keys[i]);

                if (entry != NULL)
                {
                    if (entry->type == ROUTER) push_stack(stack, entry->data);
                    free(entry);
                }
            }

            free_table_keys(keys, keyCount);
            free_table(table);
        }

        HashTable *router_table = search_table(current_table, "ROUTERS");
        int keyCount = 0;
        char **keys = table_keys(router_table, &keyCount);

        for (int i = 0; i < keyCount; ++i) {
            RoutingEntry *entry = search_table(router_table, keys[i]);

            if (entry != NULL)
            {
                if (entry->type == ROUTER) push_stack(stack, entry->data);
                free(entry);
            }
        }

        free_table_keys(keys, keyCount);
        free_table(router_table);

        free_table(current_table);
    }

    free_stack(stack);
}

HashTable *create_routing_table()
{
    HashTable *routing_table = create_table(10);

    if (routing_table == NULL) {
        perror("Failed to create routing table");
        return NULL;
    }

    for (int i = 0; i < NUM_METHODS; ++i)
    {
        insert_table(routing_table,
               Method_to_string(methods[i]),
               create_table(10));
    }
    insert_table(routing_table, "ROUTERS", create_table(10));
    return routing_table;
}
