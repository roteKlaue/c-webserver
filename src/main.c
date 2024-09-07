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


void json_test(Request *request, Response *response)
{
   json_request(response, "\"Test json\"");
}

void html_test(Request *request, Response *response)
{
    set_content_type(response, HTML);
    send_request(response, "<body> <h1>human</h1><p>sus</p> </body>");
}

int main()
{
    webserver = create_webserver();

    insert(search(webserver->routes, "GET"), "/", &index_route);
    insert(search(webserver->routes, "GET"), "/shutdown", &shutdown);
    insert(search(webserver->routes, "GET"), "/json", &json_test);
    insert(search(webserver->routes, "GET"), "/html", &html_test);

    run_webserver(webserver);
    clean_up_webserver(webserver);

    return 0;
}