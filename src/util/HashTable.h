//
// Created by xxrot on 02.09.2024.
//

#ifndef C_WEBSERVER_HASHTABLE_H
#define C_WEBSERVER_HASHTABLE_H

#define LOAD_FACTOR 0.75

typedef struct Entry
{
    char *key;
    void *value;
    struct Entry *next;
} Entry;

typedef struct
{
    Entry **entries;
    int size;
    int capacity;
} HashTable;

HashTable *create_table(int initial_capacity);
unsigned int hash(const char *key, int capacity);
Entry *create_entry(const char *key, void *value);
void insert(HashTable *table, const char *key, void *value);
void *search(const HashTable *table, const char *key);
void delete(HashTable *table, const char *key);
void resize(HashTable *table);
void free_table(HashTable *table);
char **table_keys(const HashTable *table, int *key_count);
void free_table_keys(char **keys, int key_count);

#endif //C_WEBSERVER_HASHTABLE_H
