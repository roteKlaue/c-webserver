//
// Created by xxrot on 25/09/2024.
//

#ifndef C_WEBSERVER_TASK_H
#define C_WEBSERVER_TASK_H

typedef void (*task_function)(void *);
typedef struct Task Task;

struct Task {
    task_function taskFunction;
    void *arg;
    Task *next;
};

Task* create_task(task_function, void *pArgument);
void free_task(Task *);

#endif //C_WEBSERVER_TASK_H
