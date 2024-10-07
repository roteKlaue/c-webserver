//
// Created by xxrot on 02.09.2024.
//

#include "HashTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOAD_FACTOR 0.75

unsigned int hash(const char *key, const int capacity)
{
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char) *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % capacity;
}

HashTable* create_table(const int initial_capacity)
{
    HashTable *table = malloc(sizeof(HashTable));
    if (table == NULL) return NULL;

    table->entries = malloc(sizeof(TableEntry*) * initial_capacity);
    if (table->entries == NULL) {
        free(table);
        return NULL;
    }

    for (int i = 0; i < initial_capacity; ++i) {
        table->entries[i] = NULL;
    }

    table->size = 0;
    table->capacity = initial_capacity;
    return table;
}

TableEntry* create_entry(const char *key, void *value)
{
    TableEntry *entry = malloc(sizeof(TableEntry));
    if (entry == NULL) return NULL;

    entry->key = strdup(key);

    if (entry->key == NULL) {
        free(entry);
        return NULL;
    }

    entry->value = value;
    entry->next = NULL;
    return entry;
}

void insert_table(HashTable *table, const char *key, void *value)
{
    if (table == NULL || key == NULL) return;

    const unsigned int bucket = hash(key, table->capacity);
    TableEntry *entry = table->entries[bucket];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    entry = create_entry(key, value);
    if (entry == NULL) return;

    entry->next = table->entries[bucket];
    table->entries[bucket] = entry;
    table->size++;

    if ((float)table->size / (float)table->capacity >= LOAD_FACTOR) {
        resize_table(table);
    }
}

void *search_table(const HashTable *table, const char *key)
{
    if (table == NULL || key == NULL) return NULL;

    const unsigned int bucket = hash(key, table->capacity);
    const TableEntry *entry = table->entries[bucket];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void delete_table(HashTable *table, const char *key)
{
    if (table == NULL || key == NULL) return;

    const unsigned int bucket = hash(key, table->capacity);
    TableEntry *entry = table->entries[bucket];
    TableEntry *prev = NULL;

    while (entry != NULL && strcmp(entry->key, key) != 0) {
        prev = entry;
        entry = entry->next;
    }

    if (entry == NULL) {
        return;
    }

    if (prev == NULL) {
        table->entries[bucket] = entry->next;
    } else {
        prev->next = entry->next;
    }

    free(entry->key);
    free(entry);
    table->size--;

    if ((float)table->size / (float)table->capacity <= SHRINK_THRESHOLD) {
        shrink_table(table);
    }
}

void resize_table(HashTable *table)
{
    if (table == NULL) return;

    const int new_capacity = table->capacity * 2;
    TableEntry **new_entries = malloc(sizeof(TableEntry*) * new_capacity);
    for (int i = 0; i < new_capacity; ++i) {
        new_entries[i] = NULL;
    }

    for (int i = 0; i < table->capacity; ++i) {
        TableEntry *entry = table->entries[i];
        while (entry != NULL) {
            TableEntry *next = entry->next;
            const unsigned int bucket = hash(entry->key, new_capacity);
            entry->next = new_entries[bucket];
            new_entries[bucket] = entry;
            entry = next;
        }
    }

    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
}

void free_table(HashTable *table)
{
    if (table == NULL) return;

    for (int i = 0; i < table->capacity; ++i) {
        TableEntry *entry = table->entries[i];
        while (entry != NULL) {
            TableEntry *next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    free(table->entries);
    free(table);
}

char **table_keys(const HashTable *table, int *key_count)
{
    if (table == NULL || key_count == NULL) return NULL;

    char **key_array = malloc(sizeof(char *) * table->size);
    int count = 0;

    for (int i = 0; i < table->capacity; ++i) {
        TableEntry *entry = table->entries[i];

        while (entry != NULL) {
            key_array[count] = strdup(entry->key);
            count++;
            entry = entry->next;
        }
    }

    *key_count = count;
    return key_array;
}

void free_table_keys(char **keys, int key_count)
{
    if (keys == NULL) return;

    for (int i = 0; i < key_count; ++i) {
        free(keys[i]);
    }

    free(keys);
}

void shrink_table(HashTable *table)
{
    if (table == NULL || table->capacity <= MINIMUM_CAPACITY) return;

    const int new_capacity = table->capacity / 2;
    if (new_capacity < MINIMUM_CAPACITY) return;

    TableEntry **new_entries = malloc(sizeof(TableEntry*) * new_capacity);
    if (new_entries == NULL) return;

    for (int i = 0; i < new_capacity; ++i) {
        new_entries[i] = NULL;
    }

    for (int i = 0; i < table->capacity; ++i) {
        TableEntry *entry = table->entries[i];
        while (entry != NULL) {
            TableEntry *next = entry->next;
            const unsigned int bucket = hash(entry->key, new_capacity);
            entry->next = new_entries[bucket];
            new_entries[bucket] = entry;
            entry = next;
        }
    }

    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
}
