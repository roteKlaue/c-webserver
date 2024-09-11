//
// Created by xxrot on 02.09.2024.
//

#include <stdio.h>
#include "../util/string-util.h"
#include "webserver_headers.h"
#include "../util/ArrayList.h"
#include "default-methods.h"
#include "param-util.h"
#include "webserver.h"
#include "Method.h"

#define null NULL

HashTable *used_ports;

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

bool initialised = false;

void initialise_webserver_framework()
{ /* if later required add more initialization here */
    if (initialised) return;

    used_ports = create_table(10);

#ifdef _WIN32
    initialize_winsock();
#endif
    initialised = true;
}

void clean_up_webserver_framework()
{ /* if later required add more cleanup here */
    if (!initialised) return;

    free_table(used_ports);

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

void handle_client(int client_socket, Webserver *webserver)
{
    char *buffer = (char *)malloc(sizeof(char) * webserver->buffer_size);
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

    char *body = strstr(buffer, "\r\n\r\n");
    if (body)
    {
        body += 4;
    }

    HashTable *query_params = create_table(10);
    parse_url_params(query_params, path);

    HashTable *request_path_map = (HashTable *)search_table(webserver->routes, method);

    Request *request = create_request(webserver->port, path, absolute_path,
                                      NULL, string_to_method(method),
                                      query_params, body);
    Response *response = create_response(client_socket);

    if (request_path_map == NULL)
    {
        webserver->not_found(request, response);
        return;
    }

    HashTable *routers = search_table(request_path_map, "ROUTERS");
    ArrayList *list = create_default_arraylist();

    RoutingEntry *entry = search_table(request_path_map, absolute_path);

    if (entry == NULL)
    {
        webserver->not_found(request, response);
        return;
    }

    ((void (*)(Request *, Response *)) entry->data)(request, response);

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

    int client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    webserver->socket = (int) socket(AF_INET, SOCK_STREAM, 0);
    if (webserver->socket < 0)
    {
        perror("socket");
        return false;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(webserver->port);
    if (bind(webserver->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
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
        client_socket = (int) accept(webserver->socket, (struct sockaddr *)&client_addr, &client_addr_len);
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
    RoutingEntry *routing_entry = malloc(sizeof(RoutingEntry *));
    routing_entry->type = type;
    routing_entry->data = val;
    return routing_entry;
}

void insert_helper(HashTable *routing_table, const char *method, const char *route, void *val, RoutingEntryType type)
{
    char *lower_route = to_lowercase(route);
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
    if (entry->type == ROUTER)
    {
        free_routing_table(entry->data);
    }
    free(entry);
}
