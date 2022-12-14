#ifndef MH_MEM_H
#define MH_MEM_H

#include <assert.h>
#include <stdlib.h>
#include "mh_type.h"

#define USE_POSIX


static inline mh_void_p mh_malloc(mh_int32_t size)
{
#ifdef USE_POSIX
    return malloc(size);
#else
    // todo
#endif
}

static inline mh_void_p mh_realloc(mh_void_p p, mh_int32_t size)
{
    assert(p);

#ifdef USE_POSIX
    return realloc(p, size);
#else
    // todo
#endif
}

static inline mh_void_t mh_free(mh_void_t *p)
{
    assert(p);

#ifdef USE_POSIX
    free(p);
#elif
    // todo
#endif
}



#endif // MH_MEM_H
