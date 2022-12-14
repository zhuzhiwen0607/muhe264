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

mh_result_t mh_queue_new(mh_queue_p *queue, mh_int32_t capacity);
mh_result_t mh_queue_destroy(mh_queue_p *queue);

mh_result_t mh_queue_write(mh_queue_p queue, mh_uint8_t *src, mh_int32_t size);
mh_result_t mh_queue_read(mh_queue_p queue, mh_uint8_t *dst, mh_int32_t size);


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
