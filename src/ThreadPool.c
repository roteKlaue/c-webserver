//
// Created by xxrot on 25/09/2024.
//

#include "ThreadPool.h"
#include "threads.h"
#include <stdlib.h>

static unsigned int threadpool_worker(void *arg) {
    ThreadPool *pool = arg;

    while (true) {
        pthread_mutex_lock(&pool->lock);

        while (pool->queue_size == 0 && !pool->stop) {
            pthread_cond_wait(&pool->notify, &pool->lock);
        }

        if (pool->stop && pool->queue_size == 0) {
            pthread_mutex_unlock(&pool->lock);
            break;
        }

        Task *task = pool->task_queue_head;
        if (task) {
            pool->task_queue_head = task->next;
            if (pool->task_queue_head == NULL) {
                pool->task_queue_tail = NULL;
            }
            pool->queue_size--;
        }

        pthread_mutex_unlock(&pool->lock);

        if (task) {
            task->taskFunction(task->arg);
            free(task);
        }
    }

    pthread_exit(NULL);
}

ThreadPool *create_threadpool(const int num_threads) {
    if (num_threads <= 0) return NULL;

    ThreadPool *pool = malloc(sizeof(ThreadPool));
    if (pool == NULL) return NULL;

    pool->thread_count = num_threads;
    pool->queue_size = 0;
    pool->task_queue_head = NULL;
    pool->task_queue_tail = NULL;
    pool->stop = false;

    pool->threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    if (pool->threads == NULL) {
        free(pool);
        return NULL;
    }

    if (pthread_mutex_init(&pool->lock, NULL) != 0 ||
        pthread_cond_init(&pool->notify, NULL) != 0) {
        free(pool->threads);
        free(pool);
        return NULL;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, threadpool_worker, (void *)pool);
    }

    return pool;
}

bool threadpool_add_task(ThreadPool *pool, const task_function func, void *arg) {
    if (pool == NULL || func == NULL) return false;

    Task *task = create_task(func, arg);
    if (task == NULL) return false;

    pthread_mutex_lock(&pool->lock);

    if (pool->task_queue_tail) {
        pool->task_queue_tail->next = task;
    } else {
        pool->task_queue_head = task;
    }
    pool->task_queue_tail = task;

    pool->queue_size++;
    pthread_cond_signal(&pool->notify);
    pthread_mutex_unlock(&pool->lock);

    return true;
}

void destroy_threadpool(ThreadPool *pool) {
    if (pool == NULL) return;

    pthread_mutex_lock(&pool->lock);
    pool->stop = true;

    pthread_cond_broadcast(&pool->notify);
    pthread_mutex_unlock(&pool->lock);

    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    while (pool->task_queue_head) {
        Task *task = pool->task_queue_head;
        pool->task_queue_head = task->next;
        free(task);
    }

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->notify);
    free(pool->threads);
    free(pool);
}
