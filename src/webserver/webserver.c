//
// Created by xxrot on 02.09.2024.
//

#include <stdio.h>
#include "webserver.h"
#include "Method.h"
#include "../util/StringUtil.h"
#include "paramUtil.h"

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

void default_not_found_function(Request *request, Response *response) {
    const char *prefix = "404 not found: ";
    char *path = request->absolute_path;

    size_t result_length = strlen(prefix) + strlen(path) + 1;

    char *result = (char *)malloc(result_length);

    if (result == NULL) return;

    strcpy(result, prefix);
    strcat(result, path);

    send_request(response, result);
}

Webserver *create_webserver() {
    Webserver *webserver = malloc(sizeof(Webserver));
    webserver->continue_running = true;
    webserver->not_found = &default_not_found_function;
    webserver->routes = create_routing_table();
    return webserver;
}

HashTable *create_routing_table() {
    HashTable *routing_table = create_table(10);
    for (int i = 0; i < NUM_METHODS; ++i) {
        insert(routing_table,
               Method_to_string(methods[i]),
               create_table(10));
    }
    return routing_table;
}

void free_routing_table(HashTable *routing_table) {
    if (routing_table == null) return;
    for (int i = 0; i < NUM_METHODS; ++i) {
        HashTable *entry = (HashTable *) search(routing_table, Method_to_string(methods[i]));
        if (entry == null) continue;
        free_table(entry);
    }
    free_table(routing_table);
}

void handle_client(int client_socket, int buffer_size, Webserver *webserver)
{
    char *buffer = (char *)malloc(sizeof(char) * buffer_size);
    int bytes_read;

    bytes_read = recv(client_socket, buffer, buffer_size - 1, 0);
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
        body += 4; // Skip the double CRLF
    }

    HashTable *query_params = create_table(10);
    parse_url_params(query_params, path);

    HashTable *request_path_map = (HashTable *)search(webserver->routes, method);

    Request *request = create_request(webserver->port, actual_path, NULL, string_to_method(method), query_params, body);
    Response *response = create_response();

    if (request_path_map == NULL)
    {
        webserver->not_found(request, NULL);
        return;
    }

    void (*route)(Request *, Response *) = search(request_path_map, method);

    if (route == NULL)
    {
        webserver->not_found(request, NULL);
        return;
    }

    route(request, NULL);
}

int setup_webserver(int buffer_size, Webserver *webserver)
{
#ifdef _WIN32
    initialize_winsock();
#endif
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_socket = (int) socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(webserver->port);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(server_socket);
        return -1;
    }

    if (listen(server_socket, 10) < 0)
    {
        perror("listen");
        close(server_socket);
        return -1;
    }

    printf("Server is listening on port %d\n", webserver->port);

    while (webserver->continue_running)
    {
        client_socket = (int) accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0)
        {
            perror("accept");
            close(server_socket);
            return -1;
        }

        handle_client(client_socket, buffer_size, webserver);
    }

    return server_socket;
}

void clean_up_webserver(int server_socket) {
    close(server_socket);
#ifdef _WIN32
    cleanup_winsock();
#endif
}
