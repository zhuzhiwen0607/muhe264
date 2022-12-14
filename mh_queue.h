#ifndef MH_QUEUE_H
#define MH_QUEUE_H

#include "mh_type.h"

typedef struct
{
    mh_uint8_t *base;
    mh_uint8_t *start;
    mh_uint8_t *end;
    mh_int32_t capacity;
//    mh_int32_t used;
}mh_queue_t, *mh_queue_p;


static inline mh_int32_t queue_used(mh_uint8_t *start, mh_uint8_t *end, mh_int32_t capacity)
{
    assert(start);
    assert(end);

    return (start <= end) ? (end - start) : (capacity - (start - end));
}

static inline mh_uint8_t* queue_pos(mh_uint8_t *base, mh_uint8_t *pos, mh_int32_t capacity, mh_int32_t size, mh_int32_t *loopback)
{
    assert(base);
    assert(pos);

    mh_int32_t idx = pos - base;

    if (loopback)
        *loopback = (idx + size) / capacity;    // 0 or 1

    idx = (idx + size) % capacity;

    return (base + idx);
}


static inline mh_uint8_t queue_at(mh_queue_p queue, mh_int32_t i)
{
    assert(queue);
    assert(i >= 0);

    mh_int32_t idx = queue->start - queue->base;
    mh_int32_t usedbytes = queue_used(queue->start, queue->end, queue->capacity);

    assert(i < usedbytes);

    idx = (idx + i) % queue->capacity;

    return queue->base[idx];
}


static inline mh_result_t queue_forward(mh_cycle_queue_p queue, mh_int32_t i)
{
    assert(queue);

    mh_int32_t usedbytes = queue_used(queue->start, queue->end, queue->capacity);

    if (i >= usedbytes)
    {
        return MH_ERROR;
    }

    const mh_int32_t pos = (start_index(queue) + i) % queue->capacity;

    queue->start = queue_pos(queue->base, queue->start, queue->capacity, i, NULL);


    return MH_OK;
}







mh_result_t mh_queue_new(mh_queue_p *queue, mh_int32_t capacity);
mh_result_t mh_queue_destroy(mh_queue_p *queue);

mh_result_t mh_queue_write(mh_queue_p queue, mh_uint8_t *src, mh_int32_t size);
mh_result_t mh_queue_read(mh_queue_p queue, mh_uint8_t *dst, mh_int32_t size);
mh_int32_t mh_queue_space(mh_queue_p queue);

//mh_result_t mh_cycle_queue_init(mh_cycle_queue_p queue, mh_int32_t capacity);
//mh_result_t mh_cycle_queue_deinit(mh_cycle_queue_p queue);
//mh_result_t mh_cycle_queue_reset(mh_cycle_queue_p queue);
//mh_result_t mh_cycle_queue_write(mh_cycle_queue_p queue, mh_uint8_t *src, mh_int32_t wsize);
//mh_result_t mh_cycle_queue_read(mh_cycle_queue_p queue, mh_uint8_t *dst, mh_int32_t rsize);
//mh_int32_t mh_cycle_queue_more_bytes(mh_cycle_queue_p queue);
//mh_int32_t mh_cycle_queue_free_size(mh_cycle_queue_p queue);
//mh_uint8_t mh_cycle_queue_at(mh_cycle_queue_p queue, mh_int32_t i);
//mh_result_t mh_cycle_queue_forward(mh_cycle_queue_p queue, mh_int32_t i);



#endif // MH_QUEUE_H
