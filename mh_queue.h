#ifndef MH_QUEUE_H
#define MH_QUEUE_H

#include <assert.h>
#include <stddef.h>
#include "mh_type.h"
#include "mh_error.h"


typedef struct
{
    mh_uint8_t *base;
    mh_uint8_t *start;
    mh_uint8_t *end;
//    mh_int32_t start_idx;
//    mh_int32_t end_idx;

    mh_int32_t capacity;
    mh_int32_t realsize;          // realsize = capacity + 1
//    mh_int32_t realcap;     // realcap = capacity + 1
//    mh_int32_t used;
}mh_queue_t, *mh_queue_p;


/**
 * @brief mh_queue_new: alloc mh_queue_t space to restore queue meta data and
 * alloc the queue buffer to restore bytes
 * @param queue: the address of queue pointer which will alloc memory
 * @param capacity: the capacity of queue->base that user can write or read, but the real buffer size
 * of queue->base is capacity + 1
 * @return MH_OK if success, otherwise not success
 */
mh_result_t mh_queue_new(mh_queue_p *queue, mh_int32_t capacity);

/**
 * @brief mh_queue_destroy: destory the space of queue totally
 * @param queue: the address of queue pointer
 * @return MH_OK if success, otherwise not success
 */
mh_result_t mh_queue_destroy(mh_queue_p *queue);

/**
 * @brief mh_queue_clear
 * @param queue
 * @return MH_OK if success, otherwise not success
 */
mh_result_t mh_queue_clear(mh_queue_p queue);

/**
 * @brief mh_queue_write: write bytes from src to queue, the write bytes size is
 * according to param size
 * @param queue
 * @param src
 * @param size
 * @return MH_OK if success, otherwise not success
 */
mh_result_t mh_queue_write(mh_queue_p queue, mh_uint8_t *src, mh_int32_t size);

/**
 * @brief mh_queue_read: read bytes from queue to dst, the read bytes size if
 * according to param size
 * @param queue
 * @param dst
 * @param size
 * @return MH_OK if success, otherwise not success
 */
mh_result_t mh_queue_read(mh_queue_p queue, mh_uint8_t *dst, mh_int32_t size);

/**
 * @brief mh_queue_space: get the free space of queue
 * @param queue
 * @return size of free space
 */
mh_int32_t mh_queue_space(mh_queue_p queue);






static inline mh_bool_t next_bytes_equal(mh_queue_p q, mh_int32_t n, mh_uint32_t bytes);
static inline mh_result_t leading_zero_8bits(mh_queue_p q);
static inline mh_int32_t more_bytes(mh_queue_p queue);
static inline mh_result_t zero_byte(mh_queue_p queue);
static inline mh_result_t start_code_prefix_one_3bytes(mh_queue_p queue);
static inline mh_bool_t more_data_in_byte_stream(mh_queue_p queue);

/**
 * @brief queue_empty
 * @param queue
 * @return true if queue is empty, otherwise return false
 */
static inline mh_bool_t queue_empty(mh_queue_p queue);

/**
 * @brief queue_full
 * @param queue
 * @return true if queue is empty, otherwise return false
 */
static inline mh_bool_t queue_full(mh_queue_p queue);
//static inline mh_int32_t queue_used(mh_uint8_t *start, mh_uint8_t *end, mh_int32_t capacity);

/**
 * @brief queue_used
 * @param queue
 * @return
 */
static inline mh_int32_t queue_used(mh_queue_p queue);



static inline mh_uint8_t* queue_new_start(mh_queue_p queue, mh_int32_t step, mh_bool_t *loopback);
static inline mh_uint8_t* queue_new_end(mh_queue_p queue, mh_int32_t step, mh_bool_t *loopback);
//static inline mh_uint8_t* queue_pos(mh_uint8_t *base, mh_uint8_t *pos, mh_int32_t capacity, mh_int32_t size, mh_int32_t *loopback);
static inline mh_result_t enqueue_update_pos(mh_queue_p queue, mh_int32_t size, mh_int32_t *loopback);
static inline mh_result_t dequeue_update_pos(mh_queue_p queue, mh_int32_t size, mh_int32_t *loopback);

/**
 * @brief queue_at: look at a byte in the offset i of queue buffer
 * @param queue
 * @param i
 * @return a mh_uint8_t[byte] value
 */
static inline mh_uint8_t queue_at(mh_queue_p queue, mh_int32_t i);
static inline mh_result_t queue_forward(mh_queue_p queue, mh_int32_t i);


/**
 * @brief queue_start_index
 * @param queue
 * @return index of queue->start according to the queue->base
 */
static inline mh_int32_t queue_start_index(mh_queue_p queue);

/**
 * @brief queue_end_index
 * @param queue
 * @return index of queue->end according to the queue->base
 */
static inline mh_int32_t queue_end_index(mh_queue_p queue);


//static inline mh_int32_t queue_index();



static inline mh_bool_t next_bytes_equal(mh_queue_p queue, mh_int32_t n, mh_uint32_t bytes)
{
    assert(queue);

    mh_bool_t ret = mh_false;
    mh_int32_t i = 0;

    while (i < n)
    {
        mh_uint8_t bits8 = bytes >> (n - i - 1) & 0xFF;
        if (queue_at(queue, i) != bits8)
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

static inline mh_result_t leading_zero_8bits(mh_queue_p queue)
{
    assert(queue);

    return queue_forward(queue, 1);
}

static inline mh_int32_t more_bytes(mh_queue_p queue)
{
    assert(queue);

    return queue_used(queue);
}

static inline mh_result_t zero_byte(mh_queue_p queue)
{
    assert(queue);

    if (0x00 == queue_at(queue, 0))
    {
        queue_forward(queue, 1);
        return MH_OK;
    }
    else
        return MH_ERROR;

}

static inline mh_result_t start_code_prefix_one_3bytes(mh_queue_p queue)
{
    assert(queue);

    if (more_bytes(queue) < 3)
        return MH_QUEUE_OVER_BOUND;

    if (0x00 == queue_at(queue, 0)
        && 0x00 == queue_at(queue, 1)
        && 0x01 == queue_at(queue, 2))
    {
        queue_forward(queue, 3);
    }

    return MH_OK;
}

static inline mh_bool_t more_data_in_byte_stream(mh_queue_p queue)
{
    assert(queue);

    if (more_bytes(queue) > 0)
        return mh_true;
    else
        return mh_false;
}


static inline mh_bool_t queue_empty(mh_queue_p queue)
{
    assert(queue);

//    if (queue->start == queue->end)
//        return mh_true;
//    else
//        return mh_false;

    return (queue->start == queue->end);
}

static inline mh_bool_t queue_full(mh_queue_p queue)
{
    assert(queue);

//    return ((queue->end + 1) % queue->realcap) == queue->start;
}


static inline mh_int32_t queue_used(mh_queue_p queue)
{
    assert(queue);

    return (queue->start <= queue->end) ? (queue->end - queue->start) : (queue->realsize - (queue->start - queue->end));
}

static inline mh_uint8_t* queue_new_start(mh_queue_p queue, mh_int32_t step, mh_bool_t *loopback)
{
    assert(queue);

    if (step > queue_used(queue))
        return MH_QUEUE_OVER_BOUND;

    mh_int32_t idx = queue->start - queue->base;

    if (loopback)
        *loopback = (idx + step) / (queue->capacity + 1);

    idx = (idx + step) % (queue->capacity + 1);

    return (queue->base + idx);
}

static inline mh_uint8_t* queue_new_end(mh_queue_p queue, mh_int32_t step, mh_bool_t *loopback)
{
    assert(queue);

    if (step > mh_queue_space(queue))
        return MH_QUEUE_OVER_BOUND;

    mh_int32_t idx = queue->end - queue->base;

    if (loopback)
        *loopback = (idx + step) / (queue->capacity + 1);

    idx = (idx + step) % (queue->capacity + 1);

    return (queue->base + idx);
}


static inline mh_result_t enqueue_update_pos(mh_queue_p queue, mh_int32_t size, mh_int32_t *loopback)
{
    assert(queue);

    if (size > mh_queue_space(queue))
        return MH_QUEUE_OVER_BOUND;

    mh_int32_t idx = queue->end - queue->base;

    if (loopback)
        *loopback = (idx + size) / queue->capacity;

    idx = (idx + size) % queue->capacity;

    queue->end += idx;

    return MH_OK;
}

static inline mh_result_t dequeue_update_pos(mh_queue_p queue, mh_int32_t size, mh_int32_t *loopback)
{
    assert(queue);

    if (size > queue_used(queue))
        return MH_QUEUE_OVER_BOUND;

    mh_int32_t idx = queue->start - queue->base;

    if (loopback)
        *loopback = (idx + size) / (queue->capacity + 1);

    idx = (idx + size) % (queue->capacity + 1);

    queue->start += idx;

    return MH_OK;
}


/*
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
*/

static inline mh_uint8_t queue_at(mh_queue_p queue, mh_int32_t i)
{
    assert(queue);
    assert(i >= 0);

    mh_int32_t usedbytes = queue_used(queue);
    assert(i < usedbytes);


//    mh_int32_t start_idx = queue_start_index(queue);
    mh_int32_t idx = (queue_start_index(queue) + i) % queue->realsize;

//    mh_int32_t idx = queue->start - queue->base;
//    mh_int32_t usedbytes = queue_used(queue->start, queue->end, queue->capacity);


//    idx = (idx + i) % queue->capacity;

    return queue->base[idx];
}


static inline mh_result_t queue_forward(mh_queue_p queue, mh_int32_t i)
{
    assert(queue);

//    mh_int32_t usedbytes = queue_used(queue->start, queue->end, queue->capacity);
    mh_int32_t usedbytes = queue_used(queue);

    if (i >= usedbytes)
    {
        return MH_ERROR;
    }

//    queue->start = queue_pos(queue->base, queue->start, queue->capacity, i, NULL);

    dequeue_update_pos(queue, i, NULL);

    return MH_OK;
}

static inline mh_int32_t queue_start_index(mh_queue_p queue)
{
    assert(queue);
    return (queue->start - queue->base) % (queue->realsize);
}

static inline mh_int32_t queue_end_index(mh_queue_p queue)
{
    assert(queue);
    return (queue->end - queue->base) % (queue->realsize);
}





#endif // MH_QUEUE_H
