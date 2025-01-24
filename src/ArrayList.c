//
// Created by xxrot on 07.09.2024.
//

#include <stdlib.h>
#include "ArrayList.h"

bool resize_arraylist(ArrayList *list)
{
    const int new_capacity = list->capacity * 2;
    void **new_elements = realloc(list->elements, sizeof(void *) * new_capacity);

    if (new_elements == NULL) return false;

    list->elements = new_elements;
    list->capacity = new_capacity;
    return true;
}

ArrayList *create_arraylist(const int init_size)
{
    ArrayList *list = malloc(sizeof(ArrayList));
    if (list == NULL) return NULL;

    list->elements = malloc(sizeof(void *) * init_size);

    if (list->elements == NULL) {
        free(list);
        return NULL;
    }

    list->destroy_element = NULL;
    list->element_equals = NULL;
    list->capacity = init_size;
    list->size = 0;
    return list;
}

ArrayList *create_default_arraylist()
{
    return create_arraylist(ARRAYLIST_DEFAULT_CAPACITY);
}

void *get_arraylist(const ArrayList *list, const int index)
{
    if (list == NULL || index < 0 || index >= list->size) return NULL;
    return list->elements[index];
}

bool is_empty_arraylist(const ArrayList *list)
{
    return list->size == 0;
}

void free_arraylist(ArrayList *list)
{
    if(list == NULL) return;

    if (list->destroy_element) {
        for (int i = 0; i < list->size; ++i) {
            list->destroy_element(list->elements[i]);
        }
    }

    free(list->elements);
    free(list);
}

bool add_arraylist(ArrayList *list, void *element)
{
    if(list == NULL) return false;

    if (list->size == list->capacity) {
        if (!resize_arraylist(list)) return false;
    }

    list->elements[list->size++] = element;
    return true;
}

bool remove_arraylist(ArrayList *list, const int index)
{
    if (list == NULL || index < 0 || index >= list->size) {
        return false;
    }

    for (int i = index; i < list->size - 1; ++i) {
        list->elements[i] = list->elements[i + 1];
    }

    list->size--;
    return true;
}

bool insert_arraylist(ArrayList *list, void *element, const int index)
{
    if (list == NULL || index < 0 || index > list->size) return false;

    if (list->size == list->capacity) {
        if (!resize_arraylist(list)) return false;
    }

    for (int i = list->size; i > index; --i) {
        list->elements[i] = list->elements[i - 1];
    }

    list->elements[index] = element;
    list->size++;
    return true;
}

int get_size_arraylist(const ArrayList *list)
{
    return list->size;
}

int index_of_arraylist(const ArrayList *list, void *element)
{
    if(list == NULL) return -1;

    if (element == NULL || list->element_equals == NULL) {
        for (int i = 0; i < list->size; ++i)
            if (list->elements[i]==element)
                return i;
    } else {
        for (int i = 0; i < list->size; ++i)
            if (list->element_equals(list->elements[i], element))
                return i;
    }
    return -1;
}

int last_index_of_arraylist(const ArrayList *list, void *element) {
    if(list == NULL) return -1;

    if (element == NULL || list->element_equals == NULL) {
        for (int i = list->size-1; i >= 0; --i)
            if (list->elements[i]==element)
                return i;
    } else {
        for (int i = list->size-1; i >= 0; --i)
            if (list->element_equals(list->elements[i], element))
                return i;
    }
    return -1;
}

bool shrink_arraylist(ArrayList *list) {
    if(list == NULL) return false;

    if (list->size > list->capacity * ARRAYLIST_LOAD_FACTOR || list->capacity <= ARRAYLIST_MINIMUM_CAPACITY) return true;

    int new_capacity = list->capacity / 2;
    if (new_capacity < ARRAYLIST_MINIMUM_CAPACITY) new_capacity = ARRAYLIST_MINIMUM_CAPACITY;

    void **new_elements = realloc(list->elements, sizeof(void *) * new_capacity);

    if (new_elements == NULL) return false;

    list->elements = new_elements;
    list->capacity = new_capacity;
    return true;
}

void *find_arraylist(ArrayList *list, bool (*finder)(void *, int, ArrayList *)) {
    if (list == NULL || finder == NULL) return NULL;

    for (int i = 0; i < list->size; ++i) {
        if (finder(list->elements[i], i, list)) return list->elements[i];
    }

    return NULL;
}
