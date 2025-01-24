//
// Created by xxrot on 02.09.2024.
//

#ifndef C_WEBSERVER_HASHTABLE_H
#define C_WEBSERVER_HASHTABLE_H

#define HASTABLE_DEFAULT_SIZE 10
#define HASHTABLE_LOAD_FACTOR 0.75
#define HASHTABLE_SHRINK_THRESHOLD 0.375
#define HASHTABLE_MINIMUM_CAPACITY 8

typedef struct TableEntry
{
    char *key;
    void *value;
    struct TableEntry *next;
} TableEntry;

typedef struct
{
    TableEntry **entries;
    int size;
    int capacity;
} HashTable;

HashTable *create_table(int initial_capacity);
HashTable *create_default_table();
unsigned int hash(const char *key, int capacity);
TableEntry *create_entry(const char *key, void *value);
void insert_table(HashTable *table, const char *key, void *value);
void *search_table(const HashTable *table, const char *key);
void delete_table(HashTable *table, const char *key);
void resize_table(HashTable *table);
void shrink_table(HashTable *table);
void free_table(HashTable *table);
char **table_keys(const HashTable *table, int *key_count);
void free_table_keys(char **keys, int key_count);

#endif //C_WEBSERVER_HASHTABLE_H
