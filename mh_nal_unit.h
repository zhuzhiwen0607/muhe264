#ifndef MH_NAL_UNIT_H
#define MH_NAL_UNIT_H

#include "mh_type.h"

typedef enum
{
    NAL_UNIT_TYPE_0,
    NAL_UNIT_TYPE_1,
    NAL_UNIT_TYPE_2,
    NAL_UNIT_TYPE_3,
    NAL_UNIT_TYPE_4,
    NAL_UNIT_TYPE_5,
    NAL_UNIT_TYPE_6,
    NAL_UNIT_TYPE_7,
    NAL_UNIT_TYPE_8,
    NAL_UNIT_TYPE_9,
    NAL_UNIT_TYPE_10,
    NAL_UNIT_TYPE_11,
    NAL_UNIT_TYPE_12,
    NAL_UNIT_TYPE_13,
    NAL_UNIT_TYPE_14,
    NAL_UNIT_TYPE_15,
    NAL_UNIT_TYPE_16,
    NAL_UNIT_TYPE_17,
    NAL_UNIT_TYPE_18,
    NAL_UNIT_TYPE_19,
    NAL_UNIT_TYPE_20,
    NAL_UNIT_TYPE_21,
    NAL_UNIT_TYPE_22,
    NAL_UNIT_TYPE_23,
    NAL_UNIT_TYPE_24,
    NAL_UNIT_TYPE_25,
    NAL_UNIT_TYPE_26,
    NAL_UNIT_TYPE_27,
    NAL_UNIT_TYPE_28,
    NAL_UNIT_TYPE_29,
    NAL_UNIT_TYPE_30,
    NAL_UNIT_TYPE_31,
}mh_nal_unit_type_e;


typedef struct
{
//    mh_uint8_t *buf;
    mh_array_p buf;

//    mh_int32_t size;
    mh_uint8_t forbidden_zero_bit : 1;
    mh_uint8_t nal_ref_idc : 2;
    mh_uint8_t nal_unit_type : 5;
    mh_uint8_t svc_extension_flag : 1;
    mh_uint8_t avc_3d_extension_flag : 1;

    mh_array_p rbsp;
    mh_uint8_t *rbsp_byte;
}mh_nal_unit_t, *mh_nal_unit_p;



mh_result_t mh_nal_unit_new(mh_int32_t size, mh_nal_unit_p *nalu);
mh_result_t mh_nal_unit_init(mh_cycle_queue_p queue, mh_int32_t size,mh_nal_unit_p nalu);
mh_result_t mh_nal_unit_main(mh_nal_unit_p nalu);

#endif // MH_NAL_UNIT_H
