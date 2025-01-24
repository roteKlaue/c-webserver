//
// Created by janoo on 25/09/2024.
//

#ifndef C_WEBSERVER_THREADPOOL_H
#define C_WEBSERVER_THREADPOOL_H

#include "Task.h"
#include "threads.h"
#include <stdbool.h>

typedef struct ThreadPool {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    Task *task_queue_head;
    Task *task_queue_tail;
    pthread_t *threads;
    int thread_count;
    int queue_size;
    bool stop;
} ThreadPool;

ThreadPool *create_threadpool(int num_threads);
bool threadpool_add_task(ThreadPool *pool, task_function func, void *arg);
void destroy_threadpool(ThreadPool *pool);

#endif //C_WEBSERVER_THREADPOOL_H
