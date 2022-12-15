#include <assert.h>
#include "mh_array.h"
#include "mh_mm.h"
#include "mh_error.h"


static mh_result_t mh_array_init(mh_array_p array, mh_int32_t size);
static mh_result_t mh_array_deinit(mh_array_p array);

mh_result_t mh_array_new(mh_array_p* array, mh_int32_t size)
{
    assert(array);

    mh_result_t ret = mh_malloc(array, size);
    if (MH_OK != ret)
        return ret;

    return mh_array_init(*array, size);

}

mh_result_t mh_array_destroy(mh_array_p* array)
{
    assert(array);

    mh_array_deinit(*array);

    mh_free(array);
}

static mh_result_t mh_array_init(mh_array_p array, mh_int32_t size)
{
    assert(array);

    mh_result_t ret = mh_malloc(&(array->start), size);
    if (MH_OK != ret)
        return ret;

    array->p = array->start;
    array->end = array->start + size + 1;   // *end point to buffer_end+1
    array->offset = 0;
    array->size = size;

    return MH_OK;
}

static mh_result_t mh_array_deinit(mh_array_p array)
{
    assert(array);

    mh_free(&(array->start));

    array->start = NULL;
    array->end = NULL;
    array->p = NULL;
    array->offset = 0;
    array->size = 0;
}
