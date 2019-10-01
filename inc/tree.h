#ifndef tree_h_INCLUDED
#define tree_h_INCLUDED

#include "stack.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct tree tree_t;
typedef struct tree_list tree_list_t;

typedef enum order order_t;
typedef struct traversal traversal_t;

enum order {
    ORDER_PRE,
};

struct traversal {
    order_t order;
    stack_t *stack;
};

struct tree {
    void *data;
    tree_list_t *children;
    tree_list_t *tail;
    size_t len;
};

struct tree_list {
    tree_t *ent;
    tree_list_t *next;
};

tree_t *tree_new(void *data);
tree_list_t *tree_list_new(tree_t *tree);
int tree_add(tree_t *anc, tree_t *node);

void tree_free(tree_t *tree, int free_data);
void tree_list_free(tree_list_t *list, int free_data);

traversal_t *preorder(tree_t *root);
void *tr(traversal_t *iter);

#endif // tree_h_INCLUDED
