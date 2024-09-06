#include "webserver/webserver.h"

Webserver *webserver;

void sussy(Request *request, Response *response)
{
    send_request(response, "Hello, World!");
}

void shutdown(Request *request, Response *response)
{
    webserver->continue_running = false;
    send_request(response, "Shutting down");
}

int main()
{
    webserver = create_webserver();
    insert(search(webserver->routes, "GET"), "/", &sussy);
    insert(search(webserver->routes, "GET"), "/shutdown", &shutdown);
    run_webserver(webserver);
    clean_up_webserver(webserver);
}