cmake_minimum_required(VERSION 3.15)
project(crest C)

set(CMAKE_C_STANDARD 17)

add_library(crest
        src/Task.c
        src/ArrayList.c
        src/ContentType.c
        src/default-methods.c
        src/HashTable.c
        src/LinkedList.c
        src/Method.c
        src/param-util.c
        src/Request.c
        src/Response.c
        src/Router.c
        src/Stack.c
        src/StatusCode.c
        src/string-util.c
        src/ThreadPool.c
        src/webserver.c
        src/StopNotifier.c
)

target_include_directories(crest PUBLIC include )

set_target_properties(crest PROPERTIES
        PUBLIC_HEADER
        "include/ArrayList.h;include/ContentType.h;include/default-methods.h;include/HashTable.h;include/LinkedList.h;include/Method.h;include/param-util.h;include/Request.h;include/Response.h;include/Router.h;include/Stack.h;include/StatusCode.h;include/string-util.h;include/Task.h;include/ThreadPool.h;include/threads.h;include/webserver.h;include/webserver_headers.h;include/StopNotifier.h;include/export.h"
)

install(TARGETS crest
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include
)
