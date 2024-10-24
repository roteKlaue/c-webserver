cmake_minimum_required(VERSION 3.15)
project(crest C)

set(CMAKE_C_STANDARD 17)

add_library(crest STATIC
    src/webserver/webserver.c
    src/webserver/Request.c
    src/webserver/Response.c
    src/webserver/Method.c
    src/webserver/ContentType.c
    src/webserver/param-util.c
    src/webserver/StatusCode.c
    src/webserver/Router.c
    src/webserver/default-methods.c
    src/webserver/middleware/body-parser.c
    src/util/HashTable.c
    src/util/string-util.c
    src/util/ArrayList.c
    src/util/LinkedList.c
    src/util/Stack.c
    src/multithreading/Task.c
    src/multithreading/ThreadPool.c
)

target_include_directories(crest PUBLIC src)
target_include_directories(crest PUBLIC include)

if (WIN32)
    target_link_libraries(crest PRIVATE ws2_32)
endif()

install(TARGETS crest)
