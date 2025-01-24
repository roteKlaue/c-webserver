#include "webserver.h"
#include <stdio.h>
#include <stdlib.h>

Webserver *webserver;

void index_route(const Request *request, Response *response)
{
    send_response(response, "Hello, World!");
}

void shutdown_server(const Request *request, Response *response)
{
    printf("Shutting down\n");
    send_response(response, "Shutting down");
    stop_webserver(webserver);
}

void html_test(const Request *request, Response *response)
{
    set_content_type(response, HTML);
    send_response(response, "<!DOCTYPE html><head></head><body><h1>Test HTML</h1><p>Paragraph</p></body>");
}

void redirect_test(const Request *request, Response *response) {
    redirect_response(response, "/", true);
}

int main()
{
    initialise_webserver_framework();
    webserver = create_webserver();
    HashTable *games_router = create_routing_table();

    add_route(webserver->routes, Get, "/", &index_route);
    add_route(webserver->routes, Get, "/shutdown", &shutdown_server);
    add_route(webserver->routes, Get, "/redirect", &redirect_test);
    add_router(webserver->routes, "/games", games_router);

    add_route(games_router, Get, "/", &html_test);
    add_route(games_router, Get, "/index", &html_test);
    add_route(games_router, Get, "/index.html", &html_test);

    run_webserver(webserver);
    clean_up_webserver(webserver);
    clean_up_webserver_framework();

    return 0;
}
