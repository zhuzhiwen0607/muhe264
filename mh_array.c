#include <assert.h>
#include "mh_array.h"
#include "mh_mm.h"
#include "mh_error.h"


static mh_result_t mh_array_init(mh_array_p array, mh_int32_t size);
static mh_result_t mh_array_deinit(mh_array_p array);

mh_result_t mh_array_new(mh_array_p* array, mh_int32_t capacity)
{
    assert(array);

    mh_result_t ret = mh_malloc(array, sizeof(mh_array_t));
    if (MH_OK != ret)
        return ret;

    return mh_array_init(*array, capacity);

}

mh_result_t mh_array_destroy(mh_array_p* array)
{
    assert(array);

    mh_array_deinit(*array);

    mh_free(array);
}

mh_result_t mh_array_write_u1(mh_array_p array, mh_uint8_t byte)
{
    assert(array);

    if (array->end - array->start + 1 > array->capacity)
        return MH_ARRAY_OVER_BOUND;

    *(array->end) = byte;
    ++(array->end);

    return MH_OK;
}

mh_result_t mh_array_write_u(mh_array_p array, mh_uint8_t *src, mh_int32_t n)
{
    assert(array);

    if (array->end - array->start + n > array->capacity)
        return MH_ARRAY_OVER_BOUND;

    memcpy(array->end, src, n);
    array->end += n;

    return MH_OK;
}


mh_uint32_t mh_array_size(mh_array_p array)
{
    assert(array);

    return (array->end - array->start);
}

/*
mh_result_t mh_array_move_end(mh_array_p array, mh_int32_t n)
{
    assert(array);

    if (array->end + n - array->start >= array->capacity)
    {
        return MH_ARRAY_OVER_BOUND;
    }
    array->end += array->end + n;

    return MH_OK;
}
*/

mh_result_t mh_array_push_back(mh_array_p array, mh_uint8_t byte)
{
    assert(array);

    *(array->end) = byte;

    return mh_array_end_forward(array, 1);
}

mh_result_t mh_array_end_forward(mh_array_p array, mh_int32_t i)
{
    assert(array);

    if (array->end - array->start + i >= array->capacity)
        return MH_ARRAY_OVER_BOUND;

    array->end += i;

    return MH_OK;
}

mh_result_t mh_array_cur_forward(mh_array_p array, mh_int32_t i)
{
    assert(array);

    if (array->p + i > array->end)
        return MH_ARRAY_OVER_BOUND;

    array->p += i;

    return MH_OK;
}


static mh_result_t mh_array_init(mh_array_p array, mh_int32_t capacity)
{
    assert(array);

    mh_result_t ret = mh_malloc(&(array->start), capacity);
    if (MH_OK != ret)
        return ret;

//    array->cur = array->start;
//    array->end = array->start;   // *end point to buffer_end+1
//    array->offset = 0;
//    array->size = size;
//    array->capacity = capacity;

    array->p = array->start;
    array->end = array->start;
    array->bits_offset = 0;
    array->capacity = capacity;

    return MH_OK;
}

static mh_result_t mh_array_deinit(mh_array_p array)
{
    assert(array);

    mh_free(&(array->start));

    array->start = NULL;
    array->end = NULL;
    array->p = NULL;
    array->bits_offset = 0;
    array->capacity = 0;

//    array->start = NULL;
//    array->end = NULL;
//    array->cur = NULL;
//    array->offset = 0;
//    array->size = 0;
}
