#ifndef MH_QUEUE_C
#define MH_QUEUE_C

#include "mh_queue.h"
#include "mh_mm.h"
#include "mh_error.h"
#include "mh_log.h"

static mh_result_t mh_queue_init(mh_queue_p queue, mh_int32_t capacity);
static mh_result_t mh_queue_deinit(mh_queue_p queue);


/*
static mh_result_t tailing_zero_8bits(mh_cycle_queue_p q)
{
    if (!q)
        return MH_ERROR;

//    return mh_cycle_queue_forward(q, 1);
}

*/



mh_result_t mh_queue_new(mh_queue_p *queue, mh_int32_t capacity)
{
    assert(queue);

//    *queue = mh_malloc(sizeof(mh_queue_t));
//    if (!(*queue))
//        return MH_MM_MALLOC_ERROR;

    mh_malloc(queue, sizeof(mh_queue_t));

    return mh_queue_init(*queue, capacity);
}

mh_result_t mh_queue_destroy(mh_queue_p *queue)
{
    assert(queue);
    assert(*queue);

    mh_queue_deinit(*queue);

    mh_free(queue);

    return MH_OK;
}

mh_result_t mh_queue_clear(mh_queue_p queue)
{
    assert(queue);

    queue->start = queue->base;
    queue->end = queue->base;

    memset(queue->base, 0x00, queue->realsize);

    return MH_OK;
}

static mh_result_t mh_queue_init(mh_queue_p queue, mh_int32_t capacity)
{
    assert(queue);

    if (0 == capacity)
        return MH_OK;

//    queue->base = mh_malloc(capacity);
//    if (!(queue->base))
//        return MH_MM_MALLOC_ERROR;

    mh_int32_t size = capacity + 1;

    mh_malloc(&(queue->base), size);

    memset(queue->base, 0x00, size);

    queue->start = queue->base;
    queue->end = queue->base;
    queue->capacity = capacity;
    queue->realsize = size;

    return MH_OK;
}

static mh_result_t mh_queue_deinit(mh_queue_p queue)
{
    assert(queue);

    mh_free(&(queue->base));
    queue->base = NULL;
    queue->start = NULL;
    queue->end = NULL;
    queue->capacity = 0;
    queue->realsize = 0;

    return MH_OK;
}


mh_result_t mh_queue_write(mh_queue_p queue, mh_uint8_t *src, mh_int32_t size)
{
    assert(queue);
    assert(src);

    mh_int32_t writebytes = 0;
    mh_int32_t usedbytes = queue_used(queue);

    if (usedbytes + size > queue->capacity)
        writebytes = queue->capacity - usedbytes;
    else
        writebytes = size;

//    mh_int32_t start_idx = queue_start_index(queue);
    mh_int32_t end_idx = queue_end_index(queue);

    mh_int32_t new_end_idx = (end_idx + writebytes) % queue->realsize;
    mh_bool_t loopback = (end_idx + writebytes) / queue->realsize;


//    mh_bool_t loopback = mh_false;
//    mh_uint8_t *new_end = queue_new_end(queue, writebytes, &loopback);

    if (loopback)
    {
//        mh_int32_t front_halfsize = queue->base + queue->realsize - queue->end;
        mh_int32_t front_halfsize = queue->realsize - end_idx;
        mh_int32_t back_halfsize = writebytes - front_halfsize;
        memcpy(queue->end, src, front_halfsize);
        memcpy(queue->base, src + front_halfsize, back_halfsize);
    }
    else
    {
        memcpy(queue->end, src, writebytes);
    }

    queue->end = queue->base + new_end_idx;

//    mh_int32_t endidx = queue_end_index(queue);
//    mh_info("queue end index=%d", queue_end_index(queue));

    return writebytes;

}


mh_result_t mh_queue_read(mh_queue_p queue, mh_uint8_t *dst, mh_int32_t size)
{
    assert(queue);
    assert(dst);

    mh_int32_t readbytes = 0;
    mh_int32_t usedbytes = queue_used(queue);
    if (size > usedbytes)
        readbytes = usedbytes;
    else
        readbytes = size;

//    mh_int32_t loopback = 0;
//    mh_uint8_t *new_start = dequeue_update_pos(queue, readbytes, &loopback);

    mh_int32_t start_idx = queue_start_index(queue);
    mh_int32_t new_start_idx = (start_idx + readbytes) % queue->realsize;
    mh_bool_t loopback = (start_idx + readbytes) / queue->realsize;

//    mh_bool_t loopback = mh_false;
//    mh_uint8_t *new_start = queue_new_start(queue, readbytes, &loopback);

    if (loopback)
    {
        mh_int32_t front_halfsize = queue->realsize - start_idx;
//        mh_int32_t front_halfsize = queue->base + queue->capacity - queue->start;
        mh_int32_t back_halfsize = readbytes - front_halfsize;
        memcpy(dst, queue->start, front_halfsize);
        memcpy(dst + front_halfsize, queue->base, back_halfsize);
    }
    else
    {
        memcpy(dst, queue->start, readbytes);
    }

//    queue->start = new_start;
    queue->start = queue->base + new_start_idx;

    return readbytes;

}


mh_int32_t mh_queue_space(mh_queue_p queue)
{
    assert(queue);

    mh_int32_t usedbytes = queue_used(queue);

    return (queue->capacity - usedbytes);
}





#endif // MH_QUEUE_C
