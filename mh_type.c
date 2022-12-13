
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mh_type.h"
#include "mh_error.h"
#include "mh_log.h"

//----------------mh_cycle_queue----------------
static inline mh_int32_t start_index(mh_cycle_queue_p queue)
{
    if (!queue)
        return -1;

    return (queue->start - queue->base);
}

static inline mh_int32_t end_index(mh_cycle_queue_p queue)
{
    if (!queue)
        return -1;

    return (queue->end - queue->base);
}


mh_result_t mh_cycle_queue_init(mh_cycle_queue_p queue, mh_int32_t capacity)
{
    if (!queue)
        return MH_ERROR;

//    int sizebytes = queuesize * sizeof(mh_uint8_t);
    queue->base = malloc(capacity);
    if (!queue->base)
        return MH_ERROR;

    memset(queue->base, 0x00, capacity);
    queue->start = queue->base;
    queue->end = queue->base;
//    queue->startpos = 0;
//    queue->endpos = 0;
    queue->capacity = capacity;
    queue->used = 0;

    mh_info("capacity=%d, used=%d, base=%p, start=%p, end=%p",
            queue->capacity, queue->used, queue->base, queue->start, queue->end);

    return MH_OK;
}

mh_result_t mh_cycle_queue_deinit(mh_cycle_queue_p queue)
{
    if (!queue)
        return MH_ERROR;

    free(queue->base);
    queue->base = NULL;
    queue->start = NULL;
    queue->end = NULL;
    queue->capacity = 0;
    queue->used = 0;


    return MH_OK;
}

mh_result_t mh_cycle_queue_reset(mh_cycle_queue_p queue)
{
    if (!queue)
        return MH_ERROR;

    memset(queue->base, 0x00, queue->capacity);
    queue->start = queue->base;
    queue->end = queue->base;
    queue->used = 0;

    return MH_OK;
}

mh_result_t mh_cycle_queue_write(mh_cycle_queue_p queue, mh_uint8_t *src, mh_int32_t wsize)
{
    if (!queue || !src)
    {
        return MH_ERROR_INVALID_PARAM;
    }


    int qs = queue->capacity;
    int us = queue->used;

    if (us + wsize > qs)
    {
        // error: can not write completely
        mh_error("can not write completely");
        return MH_ERROR_QUEUE_OVER_WRITE;
    }

    int start_idx = queue->start - queue->base;
    int end_idx = queue->end - queue->base;
    int backwind = (end_idx + wsize) / qs;

    mh_info("before queue operation: base=%p:0, start=%p:%d, end=%p:%d, capacity=%d, used=%d",
            queue->base, queue->start, start_idx, queue->end, end_idx, queue->capacity, queue->used);

    if (!backwind)
    {
        memcpy(queue->end, src, wsize);
    }
    else
    {
        int front_halfsize = queue->capacity - end_idx;
        int back_halfsize = wsize - front_halfsize;
        memcpy(queue->end, src, front_halfsize);
        memcpy(queue->base, src + front_halfsize, back_halfsize);
    }

    int new_end_idx = (end_idx + wsize) % qs;
    queue->end = queue->base + new_end_idx;
    queue->used += wsize;

    mh_info("write size is %d", wsize);

    mh_info("after queue operation: base=%p:0, start=%p:%d, end=%p:%d, capacity=%d, used=%d",
            queue->base, queue->start, start_idx, queue->end, new_end_idx, queue->capacity, queue->used);

    assert(queue->capacity > queue->used);

    return MH_OK;

}

mh_result_t mh_cycle_queue_read(mh_cycle_queue_p queue, mh_uint8_t *dst, mh_int32_t rsize)
{
    if (!queue || !dst)
        return MH_ERROR_INVALID_PARAM;

    int qs = queue->capacity;
    int us = queue->used;

    if (rsize > us)
    {
        mh_error("over read");
        return MH_ERROR_QUEUE_OVER_READ;
    }

    int start_idx = queue->start - queue->base;
    int end_idx = queue->end - queue->base;
    int backwind = (start_idx + rsize) / qs;

    mh_info("before queue operation: base=%p:0, start=%p:%d, end=%p:%d, capacity=%d, used=%d",
            queue->base, queue->start, start_idx, queue->end, end_idx, queue->capacity, queue->used);

    if (!backwind)
    {
        memcpy(dst, queue->start, rsize);
    }
    else
    {
        int front_halfsize = queue->capacity - start_idx;
        int back_halfsize = rsize - front_halfsize;
        memcpy(dst, queue->start, front_halfsize);
        memcpy(dst + front_halfsize, queue->base, back_halfsize);
    }

    int new_start_idx = (start_idx + rsize) % qs;
    queue->start = queue->base + new_start_idx;
    queue->used -= rsize;

    mh_info("read size is %d", rsize);

    mh_info("after queue operation: base=%p:0, start=%p:%d, end=%p:%d, capacity=%d, used=%d",
            queue->base, queue->start, new_start_idx, queue->end, end_idx, queue->capacity, queue->used);

    assert(queue->used >= 0);

    return MH_OK;
}

mh_int32_t mh_cycle_queue_more_bytes(mh_cycle_queue_p queue)
{
    if (!queue)
        return -1;

//    assert(queue->capacity >= queue->used);

//    return (queue->capacity - queue->used);

    return (queue->used);
}

mh_int32_t mh_cycle_queue_free_size(mh_cycle_queue_p queue)
{
    if (!queue)
        return -1;

    assert(queue->capacity >= queue->used);

    return (queue->capacity - queue->used);
}



mh_uint8_t mh_cycle_queue_at(mh_cycle_queue_p queue, mh_int32_t i)
{

    if (!queue)
        return -1;

    if (i > queue->used)
        return -1;

    const mh_int32_t pos = (start_index(queue) + i) % queue->capacity;

    return queue->base[pos];
}

mh_result_t mh_cycle_queue_forward(mh_cycle_queue_p queue, mh_int32_t i)
{
    if (!queue)
        return MH_ERROR;

    if (i > queue->used)
        return MH_ERROR_QUEUE_OVER_READ;

    const mh_int32_t pos = (start_index(queue) + i) % queue->capacity;

    queue->start = queue->base + pos;

    queue->used -= i;

    return MH_OK;
}

//--------------------mh_array_t-------------------
mh_result_t mh_array_init(mh_array_p array, mh_int32_t capacity)
{
    memset(array, 0x00, sizeof(mh_array_t));
    array->base = malloc(capacity);
    memset(array->base, 0x00, capacity);
    array->bits_start = array->base;
}
mh_result_t mh_array_deinit(mh_array_p array)
{
    free(array->base);
    array->base = NULL;
    array->bits_offset = 0;
    array->bits_size = 0;
    array->bits_start = NULL;
    array->forward_bits = 0;
    array->size = 0;
}
