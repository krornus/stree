#include <error.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct tree tree_t;
typedef struct tree_list tree_list_t;

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

void *xmalloc(size_t size);

tree_t *tree_new(void *data);
tree_list_t *tree_list_new(tree_t *tree);
int tree_add(tree_t *anc, tree_t *node);

int main(int argc, char *argv[]);

void *xmalloc(size_t size)
{
    void *ret;
    ret = malloc(size);
    if (!ret) {
        error(1, ENOMEM, "allocation failed");
    } else {
        memset(ret, 0, size);
        return ret;
    }
}

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

int main(int argc, char *argv[])
{

}
