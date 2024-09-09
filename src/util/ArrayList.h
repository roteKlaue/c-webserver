//
// Created by xxrot on 07.09.2024.
//

#ifndef C_WEBSERVER_ARRAYLIST_H
#define C_WEBSERVER_ARRAYLIST_H

#include <stdbool.h>

#define LOAD_FACTOR 0.75
#define DEFAULT_CAPACITY 10
#define MINIMUM_CAPACITY 10

typedef struct {
    int size;
    int capacity;
    void **elements;
    void (*destroy_element)(void *element);
    bool (*element_equals)(void *element1, void *element2);
} ArrayList;

ArrayList *create_arraylist(int init_size);
ArrayList *create_default_arraylist();
void *get_arraylist(ArrayList *list, int index);
bool remove_arraylist(ArrayList *list, int index);
bool add_arraylist(ArrayList *list, void *element);
bool insert_arraylist(ArrayList *list, void *element, int index);
bool is_empty_arraylist(ArrayList *list);
bool resize_arraylist(ArrayList *list);
int index_of_arraylist(ArrayList *list, void *element);
int last_index_of_arraylist(ArrayList *list, void *element);
bool shrink_arraylist(ArrayList *list);
void free_arraylist(ArrayList *list);

#endif //C_WEBSERVER_ARRAYLIST_H
