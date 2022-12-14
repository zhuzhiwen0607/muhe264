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


#endif // MH_QUEUE_H
