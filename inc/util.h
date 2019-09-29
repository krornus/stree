#ifndef util_h_INCLUDED
#define util_h_INCLUDED

#include <stddef.h>
#include <assert.h>

#define assertf(X) assert((X) == 0)
#define assertn(X) assert((X) == NULL)

void *xmalloc(size_t size);

#endif // util_h_INCLUDED

