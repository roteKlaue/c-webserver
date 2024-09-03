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
    table->entries = malloc(sizeof(Entry*) * initial_capacity);
    for (int i = 0; i < initial_capacity; i++) {
        table->entries[i] = NULL;
    }
    table->size = 0;
    table->capacity = initial_capacity;
    return table;
}

Entry* create_entry(const char *key, void *value)
{
    Entry *entry = malloc(sizeof(Entry));
    entry->key = strdup(key);
    entry->value = value;
    entry->next = NULL;
    return entry;
}

void insert(HashTable *table, const char *key, void *value)
{
    const unsigned int bucket = hash(key, table->capacity);
    Entry *entry = table->entries[bucket];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    entry = create_entry(key, value);
    entry->next = table->entries[bucket];
    table->entries[bucket] = entry;
    table->size++;

    if ((float)table->size / (float)table->capacity >= LOAD_FACTOR) {
        resize(table);
    }
}

void *search(const HashTable *table, const char *key)
{
    const unsigned int bucket = hash(key, table->capacity);
    const Entry *entry = table->entries[bucket];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void delete(HashTable *table, const char *key)
{
    const unsigned int bucket = hash(key, table->capacity);
    Entry *entry = table->entries[bucket];
    Entry *prev = NULL;

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
}

void resize(HashTable *table)
{
    const int new_capacity = table->capacity * 2;
    Entry **new_entries = malloc(sizeof(Entry*) * new_capacity);
    for (int i = 0; i < new_capacity; i++) {
        new_entries[i] = NULL;
    }

    for (int i = 0; i < table->capacity; i++) {
        Entry *entry = table->entries[i];
        while (entry != NULL) {
            Entry *next = entry->next;
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
    for (int i = 0; i < table->capacity; i++) {
        Entry *entry = table->entries[i];
        while (entry != NULL) {
            Entry *next = entry->next;
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
    char **key_array = malloc(sizeof(char *) * table->size);
    int count = 0;

    for (int i = 0; i < table->capacity; i++) {
        Entry *entry = table->entries[i];

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
    for (int i = 0; i < key_count; i++) {
        free(keys[i]);
    }
    free(keys);
}