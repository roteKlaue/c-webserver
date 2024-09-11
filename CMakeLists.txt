cmake_minimum_required(VERSION 3.15)
project(C-Webserver C)

add_executable(${PROJECT_NAME} src/main.c
        src/webserver/webserver.c
        src/webserver/webserver.h
        src/util/HashTable.c
        src/util/HashTable.h
        src/webserver/Request.c
        src/webserver/Request.h
        src/webserver/Response.c
        src/webserver/Response.h
        src/webserver/ContentType.h
        src/webserver/Method.h
        src/webserver/Method.c
        src/webserver/ContentType.c
        src/webserver/param-util.c
        src/webserver/param-util.h
        src/util/string-util.c
        src/util/string-util.h
        src/webserver/StatusCode.c
        src/webserver/StatusCode.h
        src/webserver/webserver_headers.h
        src/webserver/default-methods.c
        src/webserver/default-methods.h
        src/util/ArrayList.c
        src/util/ArrayList.h
        src/util/LinkedList.c
        src/util/LinkedList.h
        src/multithreading/threads.h)

target_link_libraries(C-Webserver PRIVATE ws2_32)
