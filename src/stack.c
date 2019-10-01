#include "stack.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

stack_t *stack_new()
{
    stack_t *base;
    base = xmalloc(sizeof(stack_t));
    return base;
}

void stack_push(stack_t *stack, void *data)
{
    /* new item */
    stack_ent_t *item;
    item = xmalloc(sizeof(stack_ent_t));
    item->under = stack->head;
    item->data = data;

    if (!stack->head) {
        stack->tail = item;
    }

    stack->head = item;
}

void stack_enq(stack_t *stack, void *data)
{
    stack_ent_t *item, *tmp;

    if (stack->head == NULL) {
        return stack_push(stack, data);
    }

    /* new item */
    item = xmalloc(sizeof(stack_ent_t));
    item->under = NULL;
    item->data = data;

    /* move head */
    stack->head->under = item;
    stack->head = stack->head->under;
}

void *stack_pop(stack_t *stack)
{
    stack_ent_t *ent;
    void *data;

    if (stack->head) {
        ent = stack->head;
        data = stack->head->data;
        ent->data = NULL;

        stack->head = ent->under;
        ent->under = NULL;

        free(ent);
        ent = NULL;

        return data;
    } else {
        return NULL;
    }
}

void stack_free(stack_t *stack, int free_data)
{
    void *data;
    data = stack_pop(stack);
    while (data) {
        if (free_data) {
            free(data);
            data = NULL;
        }
        data = stack_pop(stack);
    }

    memset(stack, 0, sizeof(stack_t));
    stack->head = NULL;
}
