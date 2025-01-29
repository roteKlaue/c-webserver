//
// Created by xxrot on 02.09.2024.
//

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ArrayList.h"
#include "ThreadPool.h"
#include "string-util.h"
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
    if (initialised)
        return;

    // initialise_static_routing_table();

#ifdef _WIN32
    initialize_winsock();
#endif

    initialised = true;
}

void clean_up_webserver_framework()
{
    // If not initialized, no need to clean up
    if (!initialised)
        return;

    // cleanup_static_routing_table();

#ifdef _WIN32
    cleanup_winsock();
#endif

    initialised = false;
}

Webserver *create_webserver()
{
    Webserver *webserver = malloc(sizeof(Webserver));
    if (webserver == null)
    {
        perror("Failed to allocate memory for webserver");
        return NULL;
    }

    webserver->internal_server_error = &default_internal_server_error;
    webserver->not_found = &default_not_found_function;
    webserver->thread_count = DEFAULT_THREAD_COUNT;
    webserver->buffer_size = DEFAULT_BUFFER_SIZE;
    webserver->routes = create_routing_table();
    webserver->max_tasks_per_thread = 0;
    webserver->continue_running = true;
    webserver->port = DEFAULT_PORT;
    webserver->socket = -1;

    webserver->stop_notifier = create_stop_notifier();
    if (webserver->stop_notifier == NULL)
    {
        free(webserver);
        fprintf(stderr, "Failed to create StopNotifier.\n");
        return NULL;
    }

    return webserver;
}

ArrayList *param_options(const char *total_path)
{
    ArrayList *list = create_default_arraylist();

    int partsCount;
    char **urlParts = string_split(total_path, '/', &partsCount);

    for (int i = 1; i < partsCount; ++i)
    {
        int total_length = 0;
        for (int j = 0; j <= i; ++j)
        {
            total_length += (int)strlen(urlParts[j]) + 1;
        }

        char *part = malloc(total_length + 1);
        if (part == null)
        {
            free_string_parts(urlParts, partsCount);
            return NULL;
        }

        part[0] = '\0';
        for (int j = 0; j <= i; ++j)
        {
            strncat(part, urlParts[j], total_length - strlen(part) - 1);
            if (j != i)
            {
                strncat(part, "/", total_length - strlen(part) - 1);
            }
        }

        add_arraylist(list, part);
    }

    free_string_parts(urlParts, partsCount);

    return list;
}

bool search_in_routing_table(const HashTable *table, Request *request, Response *response, const char *path)
{
    if (table == NULL || path == NULL)
    {
        return false;
    }
    const HashTable *method_table = search_table(table, Method_to_string(request->method));

    if (method_table != NULL)
    {
        const RoutingEntry *entry = search_table(method_table, path);
        if (entry != NULL)
        {
            ((route_implementation)entry->data)(request, response);
            return true;
        }
    }

    const HashTable *routers = search_table(table, "ROUTERS");
    if (routers == null)
        return false;

    ArrayList *list = param_options(path);
    if (list == null)
        return false;

    bool found = false;
    for (int i = 0; i < list->size; ++i)
    {
        const char *key = list->elements[i];
        const size_t partLength = strlen(key);
        char *sub = substring(path, (int)partLength, strlen(path));

        if (sub == NULL)
        {
            sub = malloc(sizeof(char) * 2);
            sub[0] = '/';
            sub[1] = '\0';
        }

        const RoutingEntry *routing_entry = search_table(routers, key);

        if (routing_entry != null)
        {
            found = search_in_routing_table(routing_entry->data, request, response, sub);
        }

        free(sub);

        if (found)
            break;
    }

    free_arraylist(list);
    return found;
}

void handle_client(const SOCKET client_socket, const Webserver *webserver)
{
    char *buffer = malloc(sizeof(char) * webserver->buffer_size);
    if (buffer == null)
    {
        perror("Failed to allocate memory for buffer");
        close_socket(client_socket);
        return;
    }

    const int bytes_read = recv(client_socket, buffer, webserver->buffer_size - 1, 0);

    if (bytes_read < 0)
    {
        perror("recv");
        close_socket(client_socket);
        free(buffer);
        return;
    }
    buffer[bytes_read] = '\0';

    char method[16], path[256], version[16];
    sscanf(buffer, "%s %s %s", method, path, version);

    char *absolute_path = malloc(sizeof(char) * (strlen(path) + 1));
    string_copy_until_char(absolute_path, path, '?', strlen(path) + 1);

    if (absolute_path == null)
    {
        close_socket(client_socket);
        free(buffer);
        return;
    }

    char *body = strstr(buffer, "\r\n\r\n");
    if (body)
        body += 4;

    HashTable *query_params = create_table(10);
    parse_url_params(query_params, path);

    Request *request = create_request(webserver->port, path, absolute_path,
                                        NULL, string_to_method(method),
                                        query_params, body);
    Response *response = create_response(client_socket, webserver->not_found);

    const bool found = search_in_routing_table(webserver->routes, request, response, absolute_path);

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

struct inter_holder
{
    SOCKET client_socket;
    Webserver *webserver;
};

void inter_helper(void *arg)
{
    struct inter_holder *data = arg;
    handle_client(data->client_socket, data->webserver);
    free(data);
}

bool run_webserver(Webserver *webserver)
{
    if (!initialised)
    {
        fprintf(stderr, "Not initialised. Run `initialise_webserver_framework()` before calling `run_webserver()`\n");
        return false;
    }

    if (webserver == NULL)
        return false;

    struct sockaddr_in socket_address = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(webserver->port)};

    webserver->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (webserver->socket < 0)
    {
        perror("socket");
        return false;
    }

    if (bind(webserver->socket, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0)
    {
        if (errno == EADDRINUSE)
        {
            fprintf(stderr, "Error: Port %d already in use.", webserver->port);
        }
        else
        {
            perror("bind");
        }

        close_socket(webserver->socket);
        return false;
    }

    if (listen(webserver->socket, 10) < 0)
    {
        perror("listen");
        close_socket(webserver->socket);
        return false;
    }

    printf("Server is listening on port %d\n", webserver->port);

    ThreadPool *pool = null;
    if (webserver->thread_count > 0)
    {
        pool = create_threadpool(webserver->thread_count);
    }

    SOCKET stop_fd = webserver->stop_notifier ? webserver->stop_notifier->fds[0] : INVALID_SOCKET;

    while (webserver->continue_running)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(webserver->socket, &read_fds);

        if (stop_fd != INVALID_SOCKET)
        {
            FD_SET(stop_fd, &read_fds);
        }

        const SOCKET max_fd = webserver->socket > stop_fd ? webserver->socket : stop_fd;

        const int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0)
        {
#ifdef _WIN32
            const int wsaErr = WSAGetLastError();
            if (wsaErr == WSAEINTR)
            {
                continue;
            }
            fprintf(stderr, "select() error: %d\n", wsaErr);
#else
            if (errno == EINTR)
            {
                continue;
            }
            perror("select() error");
#endif
            break;
        }

        if (stop_fd != INVALID_SOCKET && FD_ISSET(stop_fd, &read_fds))
        {
#ifdef _WIN32
            char dummy[1];
            recv(stop_fd, dummy, 1, 0);
#else
            char dummy;
            read(stop_fd, &dummy, 1);
#endif
            break;
        }

        if (FD_ISSET(webserver->socket, &read_fds))
        {
            const SOCKET client_socket = accept(webserver->socket, NULL, NULL);
            if (client_socket == INVALID_SOCKET)
            {
                perror("accept");
                continue;
            }

            struct inter_holder *data = malloc(sizeof(struct inter_holder));
            if (!data)
            {
                close_socket(client_socket);
                continue;
            }
            data->client_socket = client_socket;
            data->webserver = webserver;

            if (pool)
            {
                const bool task_added = threadpool_add_task(pool, inter_helper, data);
                if (!task_added)
                {
                    inter_helper(data);
                }
            }
            else
            {
                inter_helper(data);
            }
        }
    }

    if (pool != null)
    {
        destroy_threadpool(pool);
    }

    return true;
}

void stop_webserver(Webserver *webserver)
{
    if (webserver == NULL)
        return;
    webserver->continue_running = false;

    if (webserver->stop_notifier)
    {
        signal_stop_notifier(webserver->stop_notifier);
    }
}

void clean_up_webserver(Webserver *webserver)
{
    close_socket(webserver->socket);
    free_webserver(webserver);
}

void free_webserver(Webserver *webserver)
{
    if (webserver == NULL)
        return;

    free_routing_structure(webserver->routes);

    if (webserver->stop_notifier)
    {
        free_stop_notifier(webserver->stop_notifier);
        webserver->stop_notifier = NULL;
    }

    free(webserver);
}

void add_route(const HashTable *routing_table, const enum Method method, const char *route,
               const route_implementation implementation)
{
    insert_table(search_table(routing_table, Method_to_string(method)), route,
                 create_routing_entry(implementation, ROUTE));
}

void add_router(const HashTable *routing_table, const char *default_route, HashTable *router)
{
    insert_table(search_table(routing_table, "ROUTERS"), default_route,
                 create_routing_entry(router, ROUTER));
}
