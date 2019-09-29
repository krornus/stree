#include <error.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void *xmalloc(size_t size)
{
    void *ret;
    ret = malloc(size);
    if (!ret) {
        error(1, errno, "allocation failed");
    } else {
        memset(ret, 0, size);
        return ret;
    }
}
