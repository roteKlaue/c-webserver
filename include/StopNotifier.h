//
// Created by xxrot on 24.01.2025.
//

#ifndef CREST_STOP_NOTIFY_H
#define CREST_STOP_NOTIFY_H

#include "webserver_headers.h"

typedef struct StopNotifier {
    SOCKET fds[2];
} StopNotifier;

StopNotifier *create_stop_notifier(void);
void signal_stop_notifier(StopNotifier *notifier);
void free_stop_notifier(StopNotifier *notifier);

#endif //CREST_STOP_NOTIFY_H
