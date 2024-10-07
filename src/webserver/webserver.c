//
// Created by xxrot on 02.09.2024.
//

#include <stdio.h>
#include "./middleware/static-hosting.h"
#include "../util/string-util.h"
#include "webserver_headers.h"
#include "default-methods.h"
#include "param-util.h"
#include "webserver.h"
#include "Method.h"

#define null NULL

#ifdef _WIN32
void initialize_winsock()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        fprintf(stderr, "Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

void cleanup_winsock()
{
    WSACleanup();
}
#endif

static bool initialised = false;

void initialise_webserver_framework()
{
    // If already initialized, return early
    if (initialised) return;

    initialise_static_routing_table();

#ifdef _WIN32
    initialize_winsock();
#endif

    initialised = true;
}

void clean_up_webserver_framework()
{
    // If not initialized, no need to clean up
    if (!initialised) return;

    cleanup_static_routing_table();

#ifdef _WIN32
    cleanup_winsock();
#endif

    initialised = false;
}



const enum Method methods[] = {
        Get,
        Delete,
        Post,
        Patch,
        Put,
        Options,
        Head
};

Webserver *create_webserver()
{
    Webserver *webserver = malloc(sizeof(Webserver));
    if (!webserver) {
        perror("Failed to allocate memory for webserver");
        return NULL;
    }

    webserver->internal_server_error = &default_internal_server_error;
    webserver->not_found = &default_not_found_function;
    webserver->buffer_size = DEFAULT_BUFFER_SIZE;
    webserver->routes = create_routing_table();
    webserver->continue_running = true;
    webserver->port = DEFAULT_PORT;
    webserver->socket = -1;

    return webserver;
}

HashTable *create_routing_table()
{
    HashTable *routing_table = create_table(10);

    if (!routing_table) {
        perror("Failed to create routing table");
        return NULL;
    }

    for (int i = 0; i < NUM_METHODS; ++i)
    {
        insert_table(routing_table,
               Method_to_string(methods[i]),
               create_table(10));
    }
    insert_table(routing_table, "ROUTERS", create_table(10));
    return routing_table;
}

void free_entry(HashTable *routing_table_entry)
{
    if (routing_table_entry == null) return;

    int keyCount;
    char **keys = table_keys(routing_table_entry, &keyCount);

    for (int j = 0; j < keyCount; ++j)
    {
        free_routingentry(search_table(routing_table_entry, keys[j]));
    }

    free_table_keys(keys, keyCount);

    free_table(routing_table_entry);
}

void free_routing_table(HashTable *routing_table)
{
    if (routing_table == null) return;

    for (int i = 0; i < NUM_METHODS; ++i)
    {
        free_entry(search_table(routing_table, Method_to_string(methods[i])));
    }

    free_entry(search_table(routing_table, "ROUTERS"));
    free_table(routing_table);
}

ArrayList *param_options(const char *total_path)
{
    ArrayList *list = create_default_arraylist();

    int partsCount;
    char **urlParts = string_split(total_path, '/', &partsCount);

    for (int i = 1; i < partsCount; ++i) {
        int total_length = 0;
        for (int j = 0; j <= i; ++j) {
            total_length += strlen(urlParts[j]) + 1;
        }

        char *part = malloc(total_length);
        if (!part) {
            free_string_parts(urlParts, partsCount);
            return NULL;
        }

        part[0] = '\0';
        for (int j = 0; j <= i; ++j) {
            strcat(part, urlParts[j]);
            if (j != i) {
                strcat(part, "/");
            }
        }

        add_arraylist(list, part);
    }

    free_string_parts(urlParts, partsCount);

    return list;
}

bool search_in_routing_table(HashTable *table, Request *request, Response *response, const char* path)
{
    if (table == NULL || path == NULL)
    {
        return false;
    }
    HashTable *method_table = search_table(table, Method_to_string(request->method));

    if (method_table != NULL)
    {
        RoutingEntry *entry = search_table(method_table, path);
        if (entry != NULL)
        {
            ((void (*)(Request *, Response *)) entry->data)(request, response);
            return true;
        }
    }

    HashTable *routers = search_table(table, "ROUTERS");
    if (!routers) return false;

    ArrayList *list = param_options(path);
    if (!list) return false;

    bool found = false;
    for (int i = 0; i < list->size; ++i) {
        char *key = list->elements[i];
        size_t partLength = strlen(key);
        char *sub = substring(path, partLength, strlen(path));

        if (sub == NULL)
        {
            sub = malloc(sizeof(char) * 2);
            sub[0] = '/';
            sub[1] = '\0';
        }

        found = search_in_routing_table(((RoutingEntry *) search_table(routers, key))->data, request, response, sub);
        free(sub);

        if (found) break;
    }

    free_arraylist(list);
    return found;
}

void handle_client(int client_socket, Webserver *webserver)
{
    char *buffer = (char *)malloc(sizeof(char) * webserver->buffer_size);
    if (!buffer)
    {
        perror("Failed to allocate memory for buffer");
        close(client_socket);
        return;
    }

    int bytes_read;

    bytes_read = recv(client_socket, buffer, webserver->buffer_size - 1, 0);
    if (bytes_read < 0)
    {
        perror("recv");
        close(client_socket);
        free(buffer);
        return;
    }
    buffer[bytes_read] = '\0';

    char method[16], path[256], version[16];
    sscanf(buffer, "%s %s %s", method, path, version);

    char *absolute_path = strcpy_until_char(malloc(sizeof(char) * (strlen(path) + 1)), path, '?');
    if (!absolute_path)
    {
        close(client_socket);
        free(buffer);
        return;
    }

    char *body = strstr(buffer, "\r\n\r\n");
    if (body) body += 4;

    HashTable *query_params = create_table(10);
    parse_url_params(query_params, path);

    Request *request = create_request(webserver->port, path, absolute_path,
                                      NULL, string_to_method(method),
                                      query_params, body);
    Response *response = create_response(client_socket);

    bool found = search_in_routing_table(webserver->routes, request, response, absolute_path);

    if (!found)
    {
        webserver->not_found(request, response);
    }

    if (response->error != null)
    {
        webserver->internal_server_error(request, response, response->error);
    }

    free(absolute_path);
    free_response(response);
    free_request(request);
    free(buffer);
}

bool run_webserver(Webserver *webserver)
{
    if (!initialised)
    {
        fprintf(stderr, "Not initialised. Run `initialise_webserver_framework()` before calling `run_webserver()`\n");
        return false;
    }

    struct sockaddr_in server_addr = {
            .sin_family = AF_INET,
            .sin_addr.s_addr = INADDR_ANY,
            .sin_port = htons(webserver->port)
    };

    webserver->socket = (int) socket(AF_INET, SOCK_STREAM, 0);
    if (webserver->socket < 0)
    {
        perror("socket");
        return false;
    }

    if (bind(webserver->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        if (errno == EADDRINUSE) {
            fprintf(stderr, "Error: Port %d already in use.", webserver->port);
        } else {
            perror("bind");
        }

        close(webserver->socket);
        return false;
    }

    if (listen(webserver->socket, 10) < 0)
    {
        perror("listen");
        close(webserver->socket);
        return false;
    }

    printf("Server is listening on port %d\n", webserver->port);

    while (webserver->continue_running)
    {
        int client_socket = accept(webserver->socket, NULL, NULL);
        if (client_socket < 0)
        {
            perror("accept");
            close(webserver->socket);
            return false;
        }

        handle_client(client_socket, webserver);
    }

    return true;
}

void clean_up_webserver(Webserver *webserver)
{
    close(webserver->socket);
    free_webserver(webserver);
}

void free_webserver(Webserver *webserver)
{
    free_routing_table(webserver->routes);
    free(webserver);
}

RoutingEntry *create_routingentry(void *val, RoutingEntryType type)
{
    RoutingEntry *routing_entry = malloc(sizeof(RoutingEntry));

    if (!routing_entry) {
        perror("Failed to allocate memory for routing entry");
        return NULL;
    }

    routing_entry->type = type;
    routing_entry->data = val;
    return routing_entry;
}

void insert_helper(HashTable *routing_table, const char *method, const char *route, void *val, RoutingEntryType type)
{
    char *lower_route = to_lowercase(route);
    if (!lower_route) return;

    insert_table(search_table(routing_table, method), lower_route, create_routingentry(val, type));
    free(lower_route);
}

void add_route(HashTable *routing_table, enum Method method, const char *route,
               void (*route_implementation)(Request *, Response *))
{
    insert_helper(routing_table, Method_to_string(method), route, route_implementation, ROUTE);
}

void add_router(HashTable *routing_table, const char *default_route, HashTable *router)
{
    insert_helper(routing_table, "ROUTERS", default_route, router, ROUTER);
}

void free_routingentry(RoutingEntry *entry)
{
    if (!entry) return;

    if (entry->type == ROUTER) {
        free_routing_table(entry->data);
    }

    free(entry);
}
