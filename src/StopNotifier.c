//
// Created by xxrot on 24.01.2025.
//

#include "StopNotifier.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

static bool create_socket_pair(StopNotifier *notifier);

StopNotifier *create_stop_notifier(void) {
    StopNotifier *notifier = malloc(sizeof(StopNotifier));
    if (notifier == NULL) {
        fprintf(stderr, "Error: could not allocate StopNotifier.\n");
        return NULL;
    }

#ifdef _WIN32
    notifier->fds[0] = INVALID_SOCKET;
    notifier->fds[1] = INVALID_SOCKET;
#else
    notifier->fds[0] = -1;
    notifier->fds[1] = -1;
#endif

    if (!create_socket_pair(notifier)) {
        free(notifier);
        return NULL;
    }

    return notifier;
}

void signal_stop_notifier(const StopNotifier *notifier) {
    if (notifier == NULL) return;

#ifdef _WIN32
    if (send(notifier->fds[1], "x", 1, 0) == SOCKET_ERROR) {
        fprintf(stderr, "signal_stop_notifier: send failed (Win). Error: %d\n", WSAGetLastError());
    }
#else
    ssize_t result = write(notifier->fds[1], "x", 1);
    if (result < 0) {
        fprintf(stderr, "signal_stop_notifier: write failed (Unix). errno: %d\n", errno);
    }
#endif
}

void free_stop_notifier(StopNotifier *notifier) {
    if (notifier == NULL) return;

    if (notifier->fds[0] != INVALID_SOCKET) {
        close_socket(notifier->fds[0]);
    }
    if (notifier->fds[1] != INVALID_SOCKET) {
        close_socket(notifier->fds[1]);
    }

    free(notifier);
}

static bool create_socket_pair(StopNotifier *notifier) {
#ifdef _WIN32
    const SOCKET listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener == INVALID_SOCKET) {
        fprintf(stderr, "create_socket_pair: could not create listener. Error: %d\n", WSAGetLastError());
        return false;
    }

    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port   = 0;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        fprintf(stderr, "create_socket_pair: bind failed. Error: %d\n", WSAGetLastError());
        close_socket(listener);
        return false;
    }

    if (listen(listener, 1) == SOCKET_ERROR) {
        fprintf(stderr, "create_socket_pair: listen failed. Error: %d\n", WSAGetLastError());
        close_socket(listener);
        return false;
    }

    if (getsockname(listener, (struct sockaddr*)&addr, &addrlen) == SOCKET_ERROR) {
        fprintf(stderr, "create_socket_pair: getsockname failed. Error: %d\n", WSAGetLastError());
        close_socket(listener);
        return false;
    }

    notifier->fds[1] = socket(AF_INET, SOCK_STREAM, 0);
    if (notifier->fds[1] == INVALID_SOCKET) {
        fprintf(stderr, "create_socket_pair: client socket creation failed. Error: %d\n", WSAGetLastError());
        close_socket(listener);
        return false;
    }

    if (connect(notifier->fds[1], (struct sockaddr*)&addr, addrlen) == SOCKET_ERROR) {
        fprintf(stderr, "create_socket_pair: connect failed. Error: %d\n", WSAGetLastError());
        close_socket(listener);
        close_socket(notifier->fds[1]);
        return false;
    }

    notifier->fds[0] = accept(listener, NULL, NULL);
    if (notifier->fds[0] == INVALID_SOCKET) {
        fprintf(stderr, "create_socket_pair: accept failed. Error: %d\n", WSAGetLastError());
        close_socket(listener);
        close_socket(notifier->fds[1]);
        return false;
    }

    close_socket(listener);
#else
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, notifier->fds) < 0) {
        perror("create_socket_pair: socketpair failed (Unix)");
        return false;
    }
#endif
    return true;
}
