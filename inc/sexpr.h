#ifndef sexpr_h_INCLUDED
#define sexpr_h_INCLUDED

#include "tree.h"

tree_t *sexpr(char *str);
void sexpr_free(tree_t *sexpr);

#endif // sexpr_h_INCLUDED

