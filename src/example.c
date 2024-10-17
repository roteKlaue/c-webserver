#include <stdio.h>
#include "webserver/webserver.h"
#include "webserver/middleware/static-hosting.h"

Webserver *webserver;

void index_route(const Request *request, Response *response)
{
    send_response(response, "Hello, World!");
}

void shutdown_server(const Request *request, Response *response)
{
    webserver->continue_running = false;
    printf("i did the shutdown");
    send_response(response, "Shutting down");
}

void teapot_route(const Request *request, Response *response)
{
    set_status_code(response, IM_A_TEAPOT);
    send_response(response, "I'm a teapot");
}

void json_test(const Request *request, Response *response)
{
    json_response(response, "\"Test json\"");
}

void html_test(const Request *request, Response *response)
{
    set_content_type(response, HTML);
    send_response(response, "<body> <h1>human</h1><p>sus</p> </body>");
}

void post_test(const Request *request, Response *response)
{
    printf("%s\n", request->body);
    send_response(response, request->body);
}

void header_test(const Request *request, Response *response) {
    insert_table(response->headers, "test", "Test");
    send_response(response, "Hello, World!");
}

void error_test(const Request *request, Response *response) {
    response->error = "request->body;";
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
    add_route(webserver->routes, Get, "/teapot", &teapot_route);
    add_route(webserver->routes, Get, "/json", &json_test);
    add_route(webserver->routes, Get, "/html", &html_test);
    add_route(webserver->routes, Post, "/post", &post_test);
    add_route(webserver->routes, Get, "/header", &header_test);
    add_route(webserver->routes, Get, "/error", &error_test);
    add_route(webserver->routes, Get, "/ERROR", &error_test);
    add_route(webserver->routes, Get, "/redirect", &redirect_test);

    add_router(webserver->routes, "/games", games_router);
    add_route(games_router, Get, "/", &html_test);
    add_route(games_router, Get, "/index", &html_test);
    add_route(games_router, Get, "/index.html", &html_test);
    add_route(games_router, Get, "/post", &post_test);

    run_webserver(webserver);
    clean_up_webserver(webserver);
    clean_up_webserver_framework();

    return 0;
}
