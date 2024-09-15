//
// Created by xxrot on 12/09/2024.
//

#include "../../util/ArrayList.h"
#include "static-hosting.h"
#include "../webserver.h"
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

HashTable *create_static_hosting_router(const char *base_route)
{
    if (!isFolder(base_route)) return NULL;
    HashTable *table = create_routing_table();
    if (table == NULL) return NULL;

    ArrayList *directory_entries = list_directory(base_route);

    return table;
}

ArrayList *list_directory(const char *path) {
    if (!isFolder(path)) {
        perror("Unable to open directory");
        return NULL;
    }

    DIR *dir = opendir(path);

    ArrayList *directory_entry_list = create_default_arraylist();

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        add_arraylist(directory_entry_list, entry->d_name);
    }

    closedir(dir);

    return directory_entry_list;
}

bool isFolder(const char *path)
{
    DIR *dir = opendir(path);
    if (!dir) return false;

    closedir(dir);
    return true;
}

int main() {
    const char *path = "..";  // Current directory
    list_directory(path);
    return 0;
}
