//
// Created by xxrot on 05.09.2024.
//

#ifndef C_WEBSERVER_WEBSERVER_HEADERS_H
#define C_WEBSERVER_WEBSERVER_HEADERS_H

#ifdef _WIN32
    #include <winsock2.h>

    #pragma comment(lib, "Ws2_32.lib")
    typedef int socklen_t;
    #define close_socket closesocket
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #define SOCKET int
    #define close_socket close
    #define INVALID_SOCKET -1
#endif

#endif //C_WEBSERVER_WEBSERVER_HEADERS_H
