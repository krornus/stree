#include "sexpr.h"
#include "tree.h"
#include "util.h"

#include <ctype.h>
#include <errno.h>
#include <error.h>
#include <stddef.h>
#include <string.h>

static tree_t *atom(char *str, size_t *len);
static tree_t *rsexpr(char *str, size_t *len);

static int isquot(char c)
{
    return c == '"' || c == '\'';
}

/* parse double or single quoted string */
static tree_t *quoted(char *str, size_t *len)
{
    char *val;
    size_t idx, elen;
    int i, j;
    char q;

    /* get quote type - we can assume str[0] is a quote
     * because its a static function called once */
    elen = 0;
    idx = 0;
    q = str[idx];
    idx++;

    /* get the escaped length of the string */
    while (str[idx] != q) {
        if (str[idx] == '\0') {
            error(0, EINVAL, "sexpr: unexpected string end", str[idx]);
            return NULL;
        } else if (str[idx] == '\\') {
            idx++;
            if (str[idx] == '\0') {
                error(0, EINVAL, "sexpr: unexpected string end", str[idx]);
                return NULL;
            } else if (str[idx] != q) {
                error(0, EINVAL, "sexpr: invalid character escape '\\%c'", str[idx]);
                return NULL;
            } else {
                /* skip escaped quote, \' logged as length 1 */
                elen++;
                idx++;
            }
        }

        elen++;
        idx++;
    }

    /* copy the string, but handle escape chars */
    /* the string is also valid at this point */
    val = (char *)xmalloc((elen + 1) * sizeof(char));

    /* i is the source index, j is target escaped index */
    for (i = 1, j = 0; i < idx, j < elen; i++, j++) {
        /* source index + 1 due to start quote */
        if (str[i] == '\\') {
            /* skip escape char */
            i++;
            assert(str[i] != '\0');
        }
        val[j] = str[i];
    }

    /* skip end quote */
    idx++;

    /* consume remaining whitespace */
    while (str[idx] && isspace(str[idx])) {
        idx++;
    }
    *len = idx;

    return tree_new(val);
}

/* parse an atom(ish) - can be double or single quote string */
static tree_t *atom(char *str, size_t *len)
{
    char *val;
    size_t idx = 0;
    *len = 0;

    if (isquot(str[idx])) {
        return quoted(str, len);
    }

    /* not quoted - take everything as is */
    while (str[idx] && !isspace(str[idx]) && str[idx] != '(' && str[idx] != ')') {
        idx++;
    }

    /* allocate result string */
    val = strndup(str, idx);
    if (val == NULL) {
        error(1, errno, "allocation failed");
    }

    /* consume remaining whitespace */
    while (str[idx] && isspace(str[idx])) {
        idx++;
    }
    *len = idx;

    return tree_new(val);
}

static tree_t *rsexpr(char *str, size_t *len)
{
    size_t rlen, idx;
    tree_t *root;

    idx = 1;
    if (str[idx] == '(' || str[idx] == ')') {
        error(0, EINVAL, "unexpected parentheses in sexpr");
        return NULL;
    }

    /* read root value */
    root = atom(&str[idx], &rlen);
    if (root == NULL) {
        return NULL;
    } else {
        /* add length */
        idx += rlen;
    }

    while (str[idx] != ')') {
        tree_t *child;
        size_t clen;

        if (str[idx] == '(') {
            /* there is an sexpr to read */
            child = rsexpr(&str[idx], &clen);
        } else {
            /* there is an atom to read */
            child = atom(&str[idx], &clen);
        }

        if (child == NULL) {
            sexpr_free(root);
            return NULL;
        } else {
            /* add the item to the tree, inc total length */
            tree_add(root, child);
            idx += clen;
        }
    }

    /* skip ')' */
    idx++;

    /* consume remaining whitespace */
    while (isspace(str[idx])) {
        idx++;
    }
    *len = idx;

    return root;
}

/* not quite real sexprs
 * - must be rooted with a value
 * - k-ary instead of binary tree
 */
tree_t *sexpr(char *str)
{
    tree_t *root;
    size_t len, idx;

    if (!str) {
        return NULL;
    }

    idx = 0;
    /* chomp ws */
    while (isspace(str[idx])) {
        idx++;
    }

    if (str[idx] == '(') {
        root = rsexpr(&str[idx], &len);
    } else {
        root = atom(&str[idx], &len);
    }

    if (root) {
        idx += len;
        if (str[idx]) {
            error(0, EINVAL, "sexpr: expected sexpr to end at char %zd (%c)", idx, str[idx]);
            errno = EINVAL;
            return NULL;
        }
    }

    return root;
}

inline void sexpr_free(tree_t *sexpr)
{
    tree_free(sexpr, 1);
}

#ifdef TEST
#include <string.h>
#include <stdio.h>
#include <assert.h>
#define tassert(X)  {tree_t *t = (X); assert(t); sexpr_free(t);}
#define tassertn(X) {tree_t *t = (X); assertn(t);}
#define tstr(X) ((char *)t->data)
void test_sexpr()
{
    tree_t *t;
    tassert(sexpr("a "));
    tassert(sexpr("abc \t"));
    tassert(sexpr(" abc"));
    tassert(sexpr("\t abc "));
    tassert(sexpr("\t 'abc' "));

    t = sexpr("");
    assert(t);
    assert(*tstr(t) == '\0');
    sexpr_free(t);

    t = sexpr("' a\\'bc'");
    assert(t);
    assert(strcmp(tstr(t), " a'bc") == 0);
    sexpr_free(t);

    t = sexpr("'a\\'bc '");
    assert(t);
    assert(strcmp(tstr(t), "a'bc ") == 0);
    sexpr_free(t);

    t = sexpr("' a\\'bc '");
    assert(t);
    assert(strcmp(tstr(t), " a'bc ") == 0);
    sexpr_free(t);

    t = sexpr("' a\\' \tbc '");
    assert(t);
    assert(strcmp(tstr(t), " a' \tbc ") == 0);
    sexpr_free(t);

    t = sexpr("\" a\\\"bc\"");
    assert(t);
    assert(strcmp(tstr(t), " a\"bc") == 0);
    sexpr_free(t);

    t = sexpr("\"a\\\"bc \"");
    assert(t);
    assert(strcmp(tstr(t), "a\"bc ") == 0);
    sexpr_free(t);

    t = sexpr("\" a\\\"bc \"");
    assert(t);
    assert(strcmp(tstr(t), " a\"bc ") == 0);
    sexpr_free(t);

    t = sexpr("\" a\\\" \tbc \"");
    assert(t);
    assert(strcmp(tstr(t), " a\" \tbc ") == 0);
    sexpr_free(t);

    t = sexpr("''");
    assert(t);
    assert(*tstr(t) == '\0');
    sexpr_free(t);

    t = sexpr("\"\"");
    assert(t);
    assert(*tstr(t)  == '\0');
    sexpr_free(t);


    t = sexpr("abc ");
    assert(t);
    assert(strcmp(t->data, "abc") == 0);
    assertn(t->children);
    sexpr_free(t);

    tassert(sexpr("\t (a b c (d e f g) )"));
    tassert(sexpr("(a b c (d e f g)\t) "));
    tassert(sexpr(" (\ta b c (d e f g)) "));
    tassert(sexpr("(  a b c (d e    (ab cd) f g (a b c)   (a b c)  )\t)       "));

    tassertn(sexpr(NULL));
    tassertn(sexpr("()"));
    tassertn(sexpr("((a))"));
    tassertn(sexpr("a a"));
    tassertn(sexpr("()()"));
    tassertn(sexpr("(a))"));
    tassertn(sexpr("((a)"));
    tassertn(sexpr("(a b c (d e f g)))"));
    printf("sexpr: tests suceeeded\n");
}
#endif
