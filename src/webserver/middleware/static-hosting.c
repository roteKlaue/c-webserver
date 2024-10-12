//
// Created by xxrot on 12/09/2024.
// not working currently
//

#include "../../util/ArrayList.h"
#include "static-hosting.h"
#include "../webserver.h"
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

HashTable *route_to_file_table = NULL;

void initialise_static_routing_table()
{
    if (route_to_file_table == NULL)
    {
        route_to_file_table = create_table(30);
    }
}

HashTable *create_static_hosting_router(const char *base_route)
{
    if (!isFolder(base_route)) return NULL;

    HashTable *table = create_routing_table();
    if (table == NULL) return NULL;

    ArrayList *directory_entries = list_directory(base_route);
    if (directory_entries == NULL) return table;

    for (int i = 0; i < directory_entries->size; ++i)
    {
        const char *pathInCurrentDirectory = (char *) directory_entries->elements[i];

        size_t filePathLength = strlen(base_route) + 2 + strlen(pathInCurrentDirectory);
        size_t routerPathLength = 2 + strlen(pathInCurrentDirectory);

        char *filePath = malloc(filePathLength);
        char *routerPath = malloc(routerPathLength);

        if (filePath == NULL || routerPath == NULL)
        {
            perror("Memory allocation failed");
            free(filePath);
            free(routerPath);
            continue;
        }

        sscanf(filePath, "%s/%s", base_route, pathInCurrentDirectory);
        sscanf(routerPath, "/%s", pathInCurrentDirectory);
        filePath[filePathLength] = '\0';
        routerPath[routerPathLength] = '\0';

        if (isFolder(filePath)) {
            add_router(table, routerPath, create_static_hosting_router(filePath));
        } else {
            insert_table(route_to_file_table, routerPath, filePath);
            add_route(table, Get, routerPath, &in_between_function);
        }

        free(routerPath);
        free(filePath);
    }

    return table;
}

ArrayList *list_directory(const char *path)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("Unable to open directory");
        return NULL;
    }

    ArrayList *directory_entry_list = create_default_arraylist();
    if (directory_entry_list == NULL)
    {
        closedir(dir);
        return NULL;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        add_arraylist(directory_entry_list, entry->d_name);
    }

    closedir(dir);
    return directory_entry_list;
}

bool isFolder(const char *path)
{
    DIR *dir = opendir(path);
    if (dir == NULL) return false;

    closedir(dir);
    return true;
}

void in_between_function(const Request *request, Response *response)
{
    const char *requested_path = get_last_path_segment(request->absolute_path);
    const char *file_path = search_table(route_to_file_table, requested_path);

    if (file_path == NULL) {
        set_status_code(response, NOT_FOUND);
        send_response(response, "File Not Found");
        return;
    }

    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        response->error = "Failed to open file.";
        return;
    }

    fseek(file, 0, SEEK_END);
    const long file_size = ftell(file);

    if (file_size < 0) {
        fclose(file);
        response->error = "Failed to open file.";
        return;
    }

    fseek(file, 0, SEEK_SET);

    char *file_data = malloc(file_size + 1);

    if (file_data == NULL) {
        fclose(file);
        response->error = "Failed to open file.";
        return;
    }

    fread(file_data, 1, file_size, file);
    file_data[file_size] = '\0';
    fclose(file);

    const char *file_extension = strrchr(file_path, '.');
    enum ContentType type = UNKNOWN;

    if (file_extension != NULL) {
        if (strcmp(file_extension, ".html") == 0)
            type = HTML;
        else if (strcmp(file_extension, ".css") == 0)
            type = CSS;
        else if (strcmp(file_extension, ".js") == 0)
            type = JS;
        else if (strcmp(file_extension, ".png") == 0)
            type = PNG;
        else if (strcmp(file_extension, ".jpg") == 0 || strcmp(file_extension, ".jpeg") == 0)
            type = JPG;
    }

    set_content_type(response, type);
    send_response(response, file_data);

    free(file_data);
}

const char* get_last_path_segment(const char *requested_path)
{
    const char *last_slash = strrchr(requested_path, '/');
    return (last_slash != NULL) ? last_slash + 1 : requested_path;
}

void cleanup_static_routing_table()
{
    if (route_to_file_table != NULL)
    {
        free(route_to_file_table);
    }
}
