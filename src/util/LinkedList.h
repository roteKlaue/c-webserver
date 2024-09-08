//
// Created by xxrot on 08.09.2024.
//

#ifndef C_WEBSERVER_LINKEDLIST_H
#define C_WEBSERVER_LINKEDLIST_H

#include <stdbool.h>

typedef struct Entry Entry;

struct Entry {
    void *value;
    Entry *next;
    Entry *previous;
};

typedef struct {
    int size;
    Entry *start_element;
    void (*destroy_element)(void *element);
    bool (*element_equals)(void *element1, void *element2);
} LinkedList;

LinkedList *create_linkedlist();
Entry *create_entry(void *element);
void *get_linkedlist(LinkedList *list, int index);
bool remove_linkedlist(LinkedList *list, int index);
bool add_linkedlist(LinkedList *list, void *element);
bool insert_linkedlist(LinkedList *list, void *element, int index);
bool is_empty_linkedlist(LinkedList *list);
int index_of_linkedlist(LinkedList *list, void *element);
int last_index_of_linkedlist(LinkedList *list, void *element);
void free_linkedlist(LinkedList *list);
void free_entry(LinkedList *list, Entry *entry);

#endif //C_WEBSERVER_LINKEDLIST_H
