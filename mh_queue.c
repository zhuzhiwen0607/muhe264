#ifndef MH_QUEUE_C
#define MH_QUEUE_C

#include "mh_queue.h"
#include "mh_mm.h"
#include "mh_error.h"

static mh_result_t mh_queue_init(mh_queue_p queue, mh_int32_t capacity);
static mh_result_t mh_queue_deinit(mh_queue_p queue);

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

#endif // MH_QUEUE_C
