
#include <stdlib.h>
#include <assert.h>
#include "mh_type.h"
#include "mh_error.h"
#include "mh_log.h"

//static inline void cycle_queue_forward(mh_cycle_queue_p queue)
//{
//    if (!p)
//        return;

//    p = (p + 1) % qs;
//}

// mh_cycle_queue
mh_bool_t mh_cycle_queue_init(mh_cycle_queue_p queue, mh_int32_t queuesize)
{
    if (!queue)
        return mh_false;

    int sizebytes = queuesize * sizeof(mh_uint8_t);
    queue->buf = malloc(sizebytes);
    if (!queue->buf)
        return mh_false;

    memset(queue->buf, 0x00, sizebytes);
    queue->start = queue->buf;
    queue->end = queue->buf;
    queue->queuesize = sizebytes;
    queue->usedsize = 0;

    mh_info("queuesize=%d, usedsize=%d, buf=%p, start=%p, end=%p",
            queue->queuesize, queue->usedsize, queue->buf, queue->start, queue->end);

    return mh_true;
}

mh_bool_t mh_cycle_queue_deinit(mh_cycle_queue_p queue)
{
    if (!queue)
        return mh_false;

    free(queue->buf);
    queue->buf = NULL;
    queue->start = NULL;
    queue->end = NULL;
    queue->queuesize = 0;
    queue->usedsize = 0;


    return mh_true;
}

mh_bool_t mh_cycle_queue_reset(mh_cycle_queue_p queue)
{
    if (!queue)
        return mh_false;

    memset(queue->buf, 0x00, queue->queuesize);
    queue->start = queue->buf;
    queue->end = queue->buf;
    queue->usedsize = 0;

    return mh_true;
}

mh_result mh_cycle_queue_write(mh_cycle_queue_p queue, mh_uint8_t *src, mh_int32_t wsize)
{
    if (!queue || !src)
    {
        return MH_ERROR_INVALID_PARAM;
    }


    int qs = queue->queuesize;
    int us = queue->usedsize;

    if (us + wsize > qs)
    {
        // error: can not write completely
        mh_error("can not write completely");
        return MH_ERROR_QUEUE_OVER_WRITE;
    }

    int start_idx = queue->start - queue->buf;
    int end_idx = queue->end - queue->buf;
    int backwind = (end_idx + wsize) / qs;

    mh_info("before queue operation: buf=%p:0, start=%p:%d, end=%p:%d, queuesize=%d, usedsize=%d",
            queue->buf, queue->start, start_idx, queue->end, end_idx, queue->queuesize, queue->usedsize);

    if (!backwind)
    {
        memcpy(queue->end, src, wsize);
    }
    else
    {
        int front_halfsize = queue->queuesize - end_idx;
        int back_halfsize = wsize - front_halfsize;
        memcpy(queue->end, src, front_halfsize);
        memcpy(queue->buf, src + front_halfsize, back_halfsize);
    }

    int new_end_idx = (end_idx + wsize) % qs;
    queue->end = queue->buf + new_end_idx;
    queue->usedsize += wsize;

    mh_info("write size is %d", wsize);

    mh_info("after queue operation: buf=%p:0, start=%p:%d, end=%p:%d, queuesize=%d, usedsize=%d",
            queue->buf, queue->start, start_idx, queue->end, new_end_idx, queue->queuesize, queue->usedsize);

    assert(queue->queuesize > queue->usedsize);

    return MH_OK;

}

mh_result mh_cycle_queue_read(mh_cycle_queue_p queue, mh_uint8_t *dst, mh_int32_t rsize)
{
    if (!queue || !dst)
        return MH_ERROR_INVALID_PARAM;

    int qs = queue->queuesize;
    int us = queue->usedsize;

    if (rsize > us)
    {
        mh_error("over read");
        return MH_ERROR_QUEUE_OVER_READ;
    }

    int start_idx = queue->start - queue->buf;
    int end_idx = queue->end - queue->buf;
    int backwind = (start_idx + rsize) / qs;

    mh_info("before queue operation: buf=%p:0, start=%p:%d, end=%p:%d, queuesize=%d, usedsize=%d",
            queue->buf, queue->start, start_idx, queue->end, end_idx, queue->queuesize, queue->usedsize);

    if (!backwind)
    {
        memcpy(dst, queue->start, rsize);
    }
    else
    {
        int front_halfsize = queue->queuesize - start_idx;
        int back_halfsize = rsize - front_halfsize;
        memcpy(dst, queue->start, front_halfsize);
        memcpy(dst + front_halfsize, queue->buf, back_halfsize);
    }

    int new_start_idx = (start_idx + rsize) % qs;
    queue->start = queue->buf + new_start_idx;
    queue->usedsize -= rsize;

    mh_info("read size is %d", rsize);

    mh_info("after queue operation: buf=%p:0, start=%p:%d, end=%p:%d, queuesize=%d, usedsize=%d",
            queue->buf, queue->start, new_start_idx, queue->end, end_idx, queue->queuesize, queue->usedsize);

    assert(queue->usedsize >= 0);

    return MH_OK;
}
