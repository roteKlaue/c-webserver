//
// Created by xxrot on 08.09.2024.
//

#include "LinkedList.h"
#include <stdlib.h>

LinkedList *create_linkedlist()
{
    LinkedList *list = malloc(sizeof(LinkedList));
    if (!list) return NULL;
    list->size = 0;
    list->start_element = NULL;
    list->destroy_element = NULL;
    list->element_equals = NULL;
    return list;
}

void *get_linkedlist(LinkedList *list, int index)
{
    if (index < 0 || index >= list->size || is_empty_linkedlist(list)) return NULL;

    Entry *entry = list->start_element;
    for (int i = 0; i < index; ++i) {
        entry = entry->next;
    }

    return entry->value;
}

bool remove_linkedlist(LinkedList *list, int index)
{
    if (index < 0 || index >= list->size || is_empty_linkedlist(list)) return false;

    Entry *entry = list->start_element;
    for (int i = 0; i < index; ++i) {
        entry = entry->next;
    }

    if (entry->previous) {
        entry->previous->next = entry->next;
    } else {
        list->start_element = entry->next;
    }

    if (entry->next) {
        entry->next->previous = entry->previous;
    }

    free_entry(list, entry);
    list->size--;

    return true;
}

bool add_linkedlist(LinkedList *list, void *element)
{
    Entry *entry = create_entry(element);

    if (entry == NULL) return false;

    if (is_empty_linkedlist(list)) {
        list->start_element = entry;
    } else {
        Entry *temp_entry = list->start_element;
        while (temp_entry->next != NULL) {
            temp_entry = temp_entry->next;
        }
        temp_entry->next = entry;
        entry->previous = temp_entry;
    }

    list->size++;
    return true;
}

void free_entry(LinkedList *list, Entry *entry)
{
    if (list == NULL || entry == NULL) return;
    if (list->destroy_element)
    {
        list->destroy_element(entry->value);
    }
    free(entry);
}

void free_linkedlist(LinkedList *list)
{
    if (list == NULL) return;

    Entry *temp = list->start_element;

    while (temp != NULL) {
        Entry *next = temp->next;
        free_entry(list, temp);
        temp = next;
    }

    free(list);
}

bool is_empty_linkedlist(LinkedList *list)
{
    if (list == NULL) return true;
    return list->size == 0;
}

int index_of_linkedlist(LinkedList *list, void *element)
{
    if (list == NULL || list->start_element == NULL) return -1;

    Entry *entry = list->start_element;
    for (int i = 0; entry!=NULL; i++) {
        if (list->element_equals)
        {
            if (list->element_equals(element, entry))
                return i;
        } else {
            if (entry->value == element)
                return i;
        }
        entry = entry->next;
    }

    return -1;
}
