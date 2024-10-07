//
// Created by xxrot on 25/09/2024.
//

#include <stdlib.h>
#include "Task.h"

Task* create_task(void (*pFunction)(void *), void *pArgument)
{
    Task *task = malloc(sizeof(Task));

    if (task == NULL) return NULL;

    task->arg = pArgument;
    task->taskFunction = pFunction;
    task->next = NULL;

    return task;
}

void free_task(Task *task)
{
    free(task);
}