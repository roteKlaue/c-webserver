//
// Created by xxrot on 02.09.2024.
//

#include <stdio.h>
#include "../util/StringUtil.h"
#include "webserver_headers.h"
#include "default-methods.h"
#include "paramUtil.h"
#include "webserver.h"
#include "Method.h"

#define null NULL

#ifdef _WIN32
void initialize_winsock()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

void cleanup_winsock()
{
    WSACleanup();
}
#endif

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
        insert(routing_table,
               Method_to_string(methods[i]),
               create_table(10));
    }
    return routing_table;
}

void free_routing_table(HashTable *routing_table)
{
    if (routing_table == null) return;
    for (int i = 0; i < NUM_METHODS; ++i)
    {
        HashTable *entry = (HashTable *) search(routing_table, Method_to_string(methods[i]));
        if (entry == null) continue;
        free_table(entry);
    }
    free_table(routing_table);
}

void handle_client(int client_socket, int socket, Webserver *webserver)
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
    char actual_path[256];
    strcpy_until_char(actual_path, path, '?');

    char *body = strstr(buffer, "\r\n\r\n");
    if (body)
    {
        body += 4;
    }

    HashTable *query_params = create_table(10);
    parse_url_params(query_params, path);

    HashTable *request_path_map = (HashTable *)search(webserver->routes, method);

    Request *request = create_request(webserver->port, actual_path, NULL, string_to_method(method), query_params, body);
    Response *response = create_response(socket);

    if (request_path_map == NULL)
    {
        printf("Method not found");
        webserver->not_found(request, response);
        return;
    }

    void (*route)(Request *, Response *) = search(request_path_map, actual_path);

    if (route == NULL)
    {
        printf("Route not found");
        webserver->not_found(request, response);
        return;
    }

    route(request, response);
    free_request(request);
    free_response(response);
}

bool run_webserver(Webserver *webserver)
{
#ifdef _WIN32
    initialize_winsock();
#endif
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

        handle_client(client_socket, client_socket, webserver);
    }

    return true;
}

void clean_up_webserver(Webserver *webserver)
{
    close(webserver->socket);
    free_webserver(webserver);
#ifdef _WIN32
    cleanup_winsock();
#endif
}

void free_webserver(Webserver *webserver)
{
    free_routing_table(webserver->routes);
    free(webserver);
}
