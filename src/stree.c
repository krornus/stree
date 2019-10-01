#include "tree.h"
#include "sexpr.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    char *inf;
    tree_t *query;
    traversal_t *p;

    if (argc != 2) {
        fprintf(stderr, "usage: tq <stree>\n");
        return 1;
    }

    query = sexpr(argv[1]);
    if (!query) {
        return 1;
    }

    p = preorder(query);
    inf = tr(p);
    for (; inf != NULL; inf = (char *)tr(p)) {
        printf("%s\n", inf);
    }

    return 0;
}
