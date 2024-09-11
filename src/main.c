#include <stdio.h>
#include "webserver/webserver.h"

Webserver *webserver;

void index_route(Request *request, Response *response)
{
    send_request(response, "Hello, World!");
}

void shutdown(Request *request, Response *response)
{
    webserver->continue_running = false;
    send_request(response, "Shutting down");
}

void teapot_route(Request *request, Response *response)
{
    set_status_code(response, IM_A_TEAPOT);
    send_request(response, "I'm a teapot");
}

void json_test(Request *request, Response *response)
{
   json_request(response, "\"Test json\"");
}

void html_test(Request *request, Response *response)
{
    set_content_type(response, HTML);
    send_request(response, "<body> <h1>human</h1><p>sus</p> </body>");
}

void post_test(Request *request, Response response)
{
    printf("%s\n", request->body);
    send_request(&response, request->body);
}

int main()
{
    initialise_webserver_framework();
    webserver = create_webserver();

    add_route(webserver->routes, Get, "/", &index_route);
    add_route(webserver->routes, Get, "/shutdown", &shutdown);

    run_webserver(webserver);
    clean_up_webserver(webserver);
    clean_up_webserver_framework();

    return 0;
}
