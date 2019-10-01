#ifndef stack_h_INCLUDED
#define stack_h_INCLUDED

typedef struct stack stack_t;
typedef struct stack_ent stack_ent_t;

struct stack_ent {
    void *data;
    stack_ent_t *under;
};

struct stack {
    stack_ent_t *head;
    stack_ent_t *tail;
};

stack_t *stack_new();
void stack_push(stack_t *stack, void *data);
void *stack_pop(stack_t *stack);
void stack_free(stack_t *stack, int free_data);

#endif // stack_h_INCLUDED
