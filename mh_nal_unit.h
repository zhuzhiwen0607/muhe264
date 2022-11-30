#ifndef MH_NAL_UNIT_H
#define MH_NAL_UNIT_H

#include "mh_type.h"


typedef struct
{
    mh_uint8_t *buf;
    mh_int32_t size;
}mh_nal_unit_t, *mh_nal_unit_p;


mh_result_t mh_nal_unit_main(mh_nal_unit_p nalu);
mh_result_t mh_nal_unit_new(mh_int32_t size, mh_nal_unit_p *nalu);
mh_result_t mh_nal_unit_init(mh_cycle_queue_p queue, mh_int32_t size,mh_nal_unit_p nalu);

#endif // MH_NAL_UNIT_H
