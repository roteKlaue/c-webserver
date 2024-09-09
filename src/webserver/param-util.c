//
// Created by xxrot on 03.09.2024.
//

#include "param-util.h"
#include <string.h>
#include <stdlib.h>

void parse_url_params(HashTable *table, const char *url)
{
    char *query_start = strchr(url, '?');

    if (!query_start)
    {
        return;
    }

    query_start++;

    char *query = strdup(query_start);
    char *pair, *key, *value;

    pair = strtok(query, "&");

    while (pair != NULL)
    {
        key = strtok(pair, "=");
        value = strtok(NULL, "=");

        if (key && value)
        {
            insert_table(table, key, strdup(value));
        }

        pair = strtok(NULL, "&");
    }

    free(query);
    free(--query_start);
}