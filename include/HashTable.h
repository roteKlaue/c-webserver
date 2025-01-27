//
// Created by xxrot on 02.09.2024.
//

#ifndef C_WEBSERVER_HASHTABLE_H
#define C_WEBSERVER_HASHTABLE_H

#include "export.h"

#define HASTABLE_DEFAULT_SIZE 10
#define HASHTABLE_LOAD_FACTOR 0.75
#define HASHTABLE_SHRINK_THRESHOLD 0.375
#define HASHTABLE_MINIMUM_CAPACITY 8

CREST_EXPORT typedef struct TableEntry
{
    char *key;
    void *value;
    struct TableEntry *next;
} TableEntry;

CREST_EXPORT typedef struct
{
    TableEntry **entries;
    int size;
    int capacity;
} HashTable;

CREST_EXPORT HashTable *create_table(int initial_capacity);
CREST_EXPORT HashTable *create_default_table();
CREST_EXPORT unsigned int hash(const char *key, int capacity);
CREST_EXPORT TableEntry *create_entry(const char *key, void *value);
CREST_EXPORT void insert_table(HashTable *table, const char *key, void *value);
CREST_EXPORT void *search_table(const HashTable *table, const char *key);
CREST_EXPORT void delete_table(HashTable *table, const char *key);
CREST_EXPORT void resize_table(HashTable *table);
CREST_EXPORT void shrink_table(HashTable *table);
CREST_EXPORT void free_table(HashTable *table);
CREST_EXPORT char **table_keys(const HashTable *table, int *key_count);
CREST_EXPORT void free_table_keys(char **keys, int key_count);

#endif //C_WEBSERVER_HASHTABLE_H
