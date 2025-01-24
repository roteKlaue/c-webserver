//
// Created by xxrot on 12.10.2024.
//

#ifndef C_WEBSERVER_STACK_H
#define C_WEBSERVER_STACK_H

#include <stdbool.h>

#define STACK_DEFAULT_CAPACITY 10

typedef struct Stack {
    void **stack;
    int top;
    int capacity;
} Stack;

Stack *create_stack(int capacity);
Stack *create_default_stack();
void free_stack(Stack *stack);
bool push_stack(Stack *stack, void *data);
void *pop_stack(Stack *stack);
void *top_stack(const Stack *stack);
bool is_stack_empty(const Stack *stack);
bool push_multiple_stack(Stack *stack, ...);

#endif //C_WEBSERVER_STACK_H
