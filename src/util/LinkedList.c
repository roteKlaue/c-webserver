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
    list->destroy_element = free;
    list->element_equals = NULL;
    return list;
}

ListEntry *create_listentry(void *element)
{
    ListEntry *entry = malloc(sizeof(ListEntry));
    entry->value = element;
    entry->previous = NULL;
    entry->next = NULL;
    return entry;
}

void *get_linkedlist(LinkedList *list, int index)
{
    if (list == NULL || index < 0 || index >= list->size || is_empty_linkedlist(list)) return NULL;

    ListEntry *entry = list->start_element;
    for (int i = 0; i < index; ++i) {
        entry = entry->next;
    }

    return entry->value;
}

bool remove_linkedlist(LinkedList *list, int index)
{
    if (list == NULL || index < 0 || index >= list->size || is_empty_linkedlist(list)) return false;

    ListEntry *entry = list->start_element;
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

    free_listentry(list, entry);
    list->size--;

    return true;
}

bool add_linkedlist(LinkedList *list, void *element)
{
    if (list == NULL) return false;

    ListEntry *entry = create_listentry(element);

    if (entry == NULL) return false;

    if (is_empty_linkedlist(list)) {
        list->start_element = entry;
    } else {
        ListEntry *temp_entry = list->start_element;
        while (temp_entry->next != NULL) {
            temp_entry = temp_entry->next;
        }
        temp_entry->next = entry;
        entry->previous = temp_entry;
    }

    list->size++;
    return true;
}

void free_listentry(LinkedList *list, ListEntry *entry)
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

    ListEntry *temp = list->start_element;

    while (temp != NULL) {
        ListEntry *next = temp->next;
        free_listentry(list, temp);
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

    ListEntry *entry = list->start_element;
    for (int i = 0; entry!=NULL; i++) {
        if (list->element_equals)
        {
            if (list->element_equals(element, entry->value))
                return i;
        } else {
            if (entry->value == element)
                return i;
        }
        entry = entry->next;
    }

    return -1;
}

int last_index_of_linkedlist(LinkedList *list, void *element)
{
    if (list == NULL || list->start_element == NULL) return -1;

    ListEntry *entry = list->start_element;
    int index = -1;
    for (int i = 0; entry != NULL; i++) {
        if (list->element_equals) {
            if (list->element_equals(element, entry->value)) {
                index = i;
            }
        } else {
            if (entry->value == element) {
                index = i;
            }
        }
        entry = entry->next;
    }

    return index;
}

bool insert_linkedlist(LinkedList *list, void *element, int index)
{
    if (list == NULL || index < 0 || index > list->size) return false;

    ListEntry *entry = create_listentry(element);
    if (entry == NULL) return false;

    if (index == 0) {
        if (list->start_element != NULL) {
            entry->next = list->start_element;
            list->start_element->previous = entry;
        }
        list->start_element = entry;
    } else {
        ListEntry *current = list->start_element;
        for (int i = 0; i < index - 1; ++i) {
            current = current->next;
        }
        entry->next = current->next;
        entry->previous = current;
        if (current->next != NULL) {
            current->next->previous = entry;
        }
        current->next = entry;
    }

    list->size++;
    return true;
}
