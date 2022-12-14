#ifndef MH_QUEUE_C
#define MH_QUEUE_C

#include "mh_queue.h"
#include "mh_mm.h"
#include "mh_error.h"

static mh_result_t mh_queue_init(mh_queue_p queue, mh_int32_t capacity);
static mh_result_t mh_queue_deinit(mh_queue_p queue);


static inline mh_int32_t queue_size(mh_queue_p queue)
{
    assert(queue);

    mh_int32_t qs = 0;

    if (queue->start <= queue->end)
    {
        qs = queue->end - queue->start;
    }
    else
    {
        mh_int32_t s1 = queue->base + queue->capacity - queue->end;
        mh_int32_t s2 = queue->start - queue->base;
        qs = s1 + s2;
    }

    return qs;
}

mh_result_t mh_queue_new(mh_queue_p *queue, mh_int32_t capacity)
{
    assert(queue);

    *queue = mh_malloc(sizeof(mh_queue_t));
    if (!(*queue))
        return MH_MM_MALLOC_ERROR;

    memset(*queue, 0x00, sizeof(mh_queue_t));

    return mh_queue_init(*queue, capacity);
}

mh_result_t mh_queue_destroy(mh_queue_p *queue)
{
    assert(queue);

    mh_queue_deinit(*queue);

    mh_free(queue);

    return MH_OK;
}

static mh_result_t mh_queue_init(mh_queue_p queue, mh_int32_t capacity)
{
    assert(queue);

    if (0 == capacity)
        return MH_OK;

    queue->base = mh_malloc(capacity);
    if (!(queue->base))
        return MH_MM_MALLOC_ERROR;

    memset(queue->base, 0x00, capacity);

    queue->start = queue->base;
    queue->end = queue->base;
    queue->capacity = capacity;

    return MH_OK;
}

static mh_result_t mh_queue_deinit(mh_queue_p queue)
{
    assert(queue);

    mh_free(queue->base);
    queue->base = NULL;
    queue->start = NULL;
    queue->end = NULL;
    queue->capacity = 0;

    return MH_OK;
}

mh_result_t mh_queue_write(mh_queue_p queue, mh_uint8_t *src, mh_int32_t size)
{
    assert(queue);
    assert(src);

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


mh_result_t mh_queue_read(mh_queue_p queue, mh_uint8_t *dst, mh_int32_t size)
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

#endif // MH_QUEUE_C
