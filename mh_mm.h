#ifndef MH_MEM_H
#define MH_MEM_H

#include <assert.h>
#include <stdlib.h>
#include "mh_type.h"
#include "mh_error.h"

#define USE_POSIX


static inline mh_result_t mh_malloc(mh_void_p *p, mh_int32_t size)
{
#ifdef USE_POSIX
    *p = calloc(size, 1);
    if (!(*p))
        return MH_MM_MALLOC_ERROR;

    return MH_OK;
#else
    // todo
#endif
}

static inline mh_result_t mh_realloc(mh_void_p *p, mh_int32_t size)
{
    assert(p);

#ifdef USE_POSIX
    *p = realloc(*p, size);
    if (!(*p))
        return MH_MM_MALLOC_ERROR;

    return MH_OK;
#else
    // todo
#endif
}

static inline mh_result_t mh_free(mh_void_p *p)
{
    assert(p);

#ifdef USE_POSIX
    free(*p);

    return MH_OK;
#elif
    // todo
#endif
}



#endif // MH_MEM_H
