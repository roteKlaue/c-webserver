//
// Created by roteklaue on 10/9/24.
//

#include "../../util/ArrayList.h"
#include "../../util/HashTable.h"
#include "body-parser.h"
#include <stdlib.h>



BodyData *create_body_data(DataType data)
{
    BodyData *body_data = malloc(sizeof(BodyData));
    if (body_data == NULL) return NULL;

    body_data->data_type = data;
    body_data->data = NULL;

    return body_data;
}

void free_array(ArrayList *list) {
    if (list == NULL) return;
    for (int i = 0; i < list->size; ++i) {
        free_body_data(list->elements[i]);
    }
    free_arraylist(list);
}

void free_object(HashTable *table) {
    int count = 0;
    char **keys = table_keys(table, &count);

    for (int i = 0; i < count; ++i) {
        free_body_data(search_table(table, keys[i]));
    }

    free_table_keys(keys, count);
    free_table(table);
}

void free_body_data(BodyData *data) {
    if (data == NULL) return;

    switch (data->data_type) {
        case Array:
            free_arraylist(data->data);
            break;
        case Object:
            free_object(data->data);
            break;
        case Int:
        case Float:
        case String:
            free(data->data);
    }

    free(data);
}
