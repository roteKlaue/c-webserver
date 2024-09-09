//
// Created by xxrot on 08.09.2024.
//

#ifndef C_WEBSERVER_LINKEDLIST_H
#define C_WEBSERVER_LINKEDLIST_H

#include <stdbool.h>

typedef struct ListEntry ListEntry;

struct ListEntry {
    void *value;
    ListEntry *next;
    ListEntry *previous;
};

typedef struct {
    int size;
    ListEntry *start_element;
    void (*destroy_element)(void *element);
    bool (*element_equals)(void *element1, void *element2);
} LinkedList;

LinkedList *create_linkedlist();
ListEntry *create_listentry(void *element);
void *get_linkedlist(LinkedList *list, int index);
bool remove_linkedlist(LinkedList *list, int index);
bool add_linkedlist(LinkedList *list, void *element);
bool insert_linkedlist(LinkedList *list, void *element, int index);
bool is_empty_linkedlist(LinkedList *list);
int index_of_linkedlist(LinkedList *list, void *element);
int last_index_of_linkedlist(LinkedList *list, void *element);
void free_linkedlist(LinkedList *list);
void free_listentry(LinkedList *list, ListEntry *entry);

#endif //C_WEBSERVER_LINKEDLIST_H
