# Crest

# Crest Webserver Framework

Crest is a lightweight, simple-to-use webserver framework for building web applications in C. It supports routing, middleware, static hosting, and more, making it a flexible choice for developers who want control over their web server logic.

## Features

- Simple and customizable routing
- Middleware support
- Static file hosting
- JSON and HTML responses
- Basic error handling
- Redirects

## Example

Here's a simple example of how to set up a web server using Crest. This server demonstrates various routes, including serving static files, handling POST requests, and sending JSON responses.

### Code

```c
#include <stdio.h>
#include "webserver/webserver.h"

Webserver *webserver;

void index_route(const Request *request, Response *response)
{
    send_response(response, "Hello, World!");
}

void shutdown_server(const Request *request, Response *response)
{
    webserver->continue_running = false;
    printf("shutting donw\n");
    send_response(response, "Shutting down");
}

void html_test(const Request *request, Response *response)
{
    set_content_type(response, HTML);
    send_response(response, "<body> <h1>human</h1><p>sus</p> </body>");
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
```

## Building and Running

 1. Clone the repository.
 2. Include the necessary files from the `webserver` directory.
 3. Compile the project:
    `gcc -o webserver main.c webserver/*.c`
 4. Run the webserver `./webserver`
 5. Access the web server at http://localhost:4096 (Default PORT is 4096).

## Contributing

Feel free to fork and submit pull requests to improve the framework. Any contributions are welcome!

## License

This project is licensed under the MIT License.