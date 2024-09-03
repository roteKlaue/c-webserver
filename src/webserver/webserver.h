//
// Created by xxrot on 02.09.2024.
//

#ifndef C_WEBSERVER_WEBSERVER_H
#define C_WEBSERVER_WEBSERVER_H
#include <stdbool.h>
#include "../util/HashTable.h"
#include "Request.h"
#include "Response.h"

#ifdef _WIN32
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")
typedef int socklen_t;
#define close closesocket
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

typedef struct {
    HashTable *routes;
    void (*not_found)(Request *, Response *);
    void (* internal_server_error)(Request *, Response *);
    bool continue_running;
} Webserver;

HashTable *create_routing_table();
Webserver *create_webserver();
void free_routing_table(HashTable *routing_table);

#endif //C_WEBSERVER_WEBSERVER_H
