#ifndef MH_QUEUE_C
#define MH_QUEUE_C

#include "mh_queue.h"
#include "mh_mm.h"
#include "mh_error.h"


static mh_result_t mh_queue_init(mh_queue_p queue, mh_int32_t capacity);
static mh_result_t mh_queue_deinit(mh_queue_p queue);

static mh_bool_t next_bytes_equal(mh_cycle_queue_p q, mh_int32_t n, mh_uint32_t num);
static mh_result_t leading_zero_8bits(mh_cycle_queue_p q);
static mh_result_t tailing_zero_8bits(mh_cycle_queue_p q);
static mh_result_t zero_byte(mh_cycle_queue_p q);
static mh_result_t start_code_prefix_one_3bytes(mh_cycle_queue_p q);
static mh_bool_t more_data_in_byte_stream(mh_cycle_queue_p q);
static mh_uint8_t mh_cycle_queue_at(mh_cycle_queue_p queue, mh_int32_t i);
static mh_result_t mh_cycle_queue_forward(mh_cycle_queue_p queue, mh_int32_t i);

static mh_bool_t next_bytes_equal(mh_cycle_queue_p q, mh_int32_t n, mh_uint32_t bytes)
{
//    if (!q)
//        return mh_false;

    assert(q);

    mh_bool_t ret = mh_false;
    mh_int32_t i = 0;

    while (i < n)
    {
        mh_uint8_t bits8 = bytes >> (n - i - 1) & 0xFF;
        if (queue_at(q, i) != bits8)
        {
            break;
        }

        ++i;
    }

    if (i >= n)
        ret = mh_true;
    else
        ret = mh_false;

    return ret;
}


static mh_result_t leading_zero_8bits(mh_cycle_queue_p q)
{
    if (!q)
        return MH_ERROR;

//    return mh_cycle_queue_forward(q, 1);
    return queue_forward(q, 1);
}

static mh_result_t tailing_zero_8bits(mh_cycle_queue_p q)
{
    if (!q)
        return MH_ERROR;

//    return mh_cycle_queue_forward(q, 1);
}

static mh_result_t zero_byte(mh_cycle_queue_p q)
{
    if (!q)
        return MH_ERROR;

    if (0x00 == mh_cycle_queue_at(q, 0))
    {
        mh_cycle_queue_forward(q, 1);
        return MH_OK;
    }
    else
        return MH_ERROR;

}

static mh_result_t start_code_prefix_one_3bytes(mh_cycle_queue_p q)
{
    if (!q)
        return MH_ERROR;

    if (mh_cycle_queue_more_bytes(q) < 3)
        return MH_ERROR;

    if (0x00 == mh_cycle_queue_at(q, 0)
        && 0x00 == mh_cycle_queue_at(q, 1)
        && 0x01 == mh_cycle_queue_at(q, 2))
    {
        mh_cycle_queue_forward(q, 3);
    }

    return MH_OK;
}

static mh_bool_t more_data_in_byte_stream(mh_cycle_queue_p q)
{
    if (!q)
        return mh_false;

    if (mh_cycle_queue_more_bytes(q) > 0)
        return mh_true;
    else
        return mh_false;
}







mh_result_t mh_queue_new(mh_queue_p *queue, mh_int32_t capacity)
{
    assert(queue);

//    *queue = mh_malloc(sizeof(mh_queue_t));
//    if (!(*queue))
//        return MH_MM_MALLOC_ERROR;

    mh_malloc(*queue, sizeof(mh_queue_t));

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

//    queue->base = mh_malloc(capacity);
//    if (!(queue->base))
//        return MH_MM_MALLOC_ERROR;

    mh_malloc(queue->base, capacity);

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

    mh_int32_t writebytes = 0;
    mh_int32_t usedbytes = queue_used(queue->start, queue->end, queue->capacity);

    if (usedbytes + size > queue->capacity)
        writebytes = queue->capacity - usedbytes;
    else
        writebytes = size;

    mh_int32_t loopback = 0;
    mh_uint8_t *new_end = queue_pos(queue->base, queue->end, queue->capacity, writebytes, &loopback);

    if (loopback)
    {
        mh_int32_t front_halfsize = queue->base + queue->capacity - queue->end;
        mh_int32_t back_halfsize = writebytes - front_halfsize;
        memcpy(queue->end, src, front_halfsize);
        memcpy(queue->base, src + front_halfsize, back_halfsize);
    }
    else
    {
        memcpy(queue->end, src, writebytes);
    }

    queue->end = new_end;

    return writebytes;

}


mh_result_t mh_queue_read(mh_queue_p queue, mh_uint8_t *dst, mh_int32_t size)
{
    assert(queue);
    assert(dst);

    mh_int32_t readbytes = 0;
    mh_int32_t usedbytes = queue_used(queue->start, queue->end, queue->capacity);
    if (size > usedbytes)
        readbytes = usedbytes;
    else
        readbytes = size;

    mh_int32_t loopback = 0;
    mh_uint8_t *new_start = queue_pos(queue->base, queue->start, queue->capacity, readbytes, &loopback);

    if (loopback)
    {
        mh_int32_t front_halfsize = queue->capacity - queue->start;
        mh_int32_t back_halfsize = readbytes - front_halfsize;
        memcpy(dst, queue->start, front_halfsize);
        memcpy(dst + front_halfsize, queue->base, back_halfsize);
    }
    else
    {
        memcpy(dst, queue->start, readbytes);
    }

    queue->start = new_start;

    return readbytes;

}


mh_int32_t mh_queue_space(mh_queue_p queue)
{
    assert(queue);

    mh_int32_t usedbytes = queue_used(queue->start, queue->end, queue->capacity);

    return (queue->capacity - usedbytes);
}





#endif // MH_QUEUE_C
