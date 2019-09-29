#include "util.h"
#include "tree.h"

#include <stddef.h>

tree_t *tree_new(void *data)
{
    tree_t *root;
    root = xmalloc(sizeof(tree_t));
    root->data = data;
    return root;
}

tree_list_t *tree_list_new(tree_t *tree)
{
    tree_list_t *list;
    if (tree == NULL) {
        return NULL;
    }
    list = xmalloc(sizeof(tree_list_t));
    list->ent = tree;
    return list;
}

int tree_add(tree_t *anc, tree_t *node)
{
    if (anc == NULL || node == NULL) {
        return 1;
    }

    if (anc->len == 0) {
        anc->children = tree_list_new(node);
        anc->tail = anc->children;
    } else {
        anc->tail->next = tree_list_new(node);
        anc->tail = anc->tail->next;
    }

    return 0;
}

void tree_free(tree_t *tree, int free_data)
{
    if (!tree) {
        return;
    }

    if (free_data && tree->data) {
        free(tree->data);
    }

    tree->data = NULL;
    tree->tail = NULL;
    tree->len = 0;
    tree_list_free(tree->children, free_data);
    free(tree);
}

void tree_list_free(tree_list_t *list, int free_data)
{
    tree_list_t *head;
    head = list;

    while (list) {
        tree_list_t *next;

        tree_free(list->ent, free_data);
        list->ent = NULL;

        next = list->next;
        list->next = NULL;

        free(list);
        list = next;
    }
}
