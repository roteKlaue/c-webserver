//
// Created by xxrot on 12.10.2024.
//

#include "Stack.h"

#include <stdarg.h>
#include <stdlib.h>

Stack *create_stack(const int capacity) {
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) return NULL;

    stack->stack = malloc(capacity * sizeof(void *));
    if (stack->stack == NULL) {
        free(stack);
        return NULL;
    }

    stack->capacity = capacity;
    stack->top = -1;
    return stack;
}

Stack *create_default_stack() {
    return create_stack(STACK_DEFAULT_CAPACITY);
}

void free_stack(Stack *stack) {
    if (stack == NULL) return;
    free(stack->stack);
    free(stack);
}

bool push_stack(Stack *stack, void *data) {
    if (stack == NULL) return false;

    if (stack->top == stack->capacity - 1) {
        const int new_capacity = stack->capacity * 2;
        void **new_stack = realloc(stack->stack, new_capacity * sizeof(void *));

        if (new_stack == NULL) return false;

        stack->stack = new_stack;
        stack->capacity = new_capacity;
    }

    stack->stack[++stack->top] = data;
    return true;
}

void *pop_stack(Stack *stack) {
    if (stack == NULL || stack->top == -1) return NULL;
    return stack->stack[stack->top--];
}

void *top_stack(const Stack *stack) {
    if (stack == NULL || stack->top == -1) return NULL;
    return stack->stack[stack->top];
}

bool is_stack_empty(const Stack *stack) {
    return stack == NULL || stack->top == -1;
}

bool push_multiple_stack(Stack *stack, ...) {
    if (stack == NULL) return false;

    va_list args;
    va_start(args, stack);

    while (true) {
        void *value = va_arg(args, void *);
        if (value == NULL) break;

        if (!push_stack(stack, value)) {
            va_end(args);
            return false;
        }
    }

    va_end(args);
    return true;
}
