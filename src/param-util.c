#include "param-util.h"
#include <string.h>
#include <stdlib.h>

void parse_url_params(HashTable *table, const char *url)
{
    // Find the start of the query string
    const char *query_start = strchr(url, '?');

    if (query_start == NULL)
    {
        return;
    }

    query_start++;

    char *query = strdup(query_start);

    if (query == NULL)
    {
        return;
    }

    char *pair = strtok(query, "&");

    while (pair != NULL)
    {
        const char *key = strtok(pair, "=");
        const char *value = strtok(NULL, "");

        if (key && value) {
            insert_table(table, key, strdup(value));
        } else if (key != NULL) {
            insert_table(table, key, strdup(""));
        }

        pair = strtok(NULL, "&");
    }

    free(query);
}
