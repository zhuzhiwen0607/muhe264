#ifndef MH_RBSP_H
#define MH_RBSP_H

#include "mh_type.h"
#include "mh_array.h"

typedef struct
{
    mh_array_p buf;
    mh_uint8_t profile_idc;
    mh_uint8_t constraint_set0_flag : 1;
    mh_uint8_t constraint_set1_flag : 1;
    mh_uint8_t constraint_set2_flag : 1;
    mh_uint8_t constraint_set3_flag : 1;
    mh_uint8_t constraint_set4_flag : 1;
    mh_uint8_t constraint_set5_flag : 1;
    mh_uint8_t reserved_zero_2bits : 2;
    mh_uint8_t level_idc;

}mh_rbsp_t, *mh_rbsp_p;

mh_result_t mh_rbsp_new(mh_rbsp_p *rbsp, mh_uint32_t size);
mh_result_t mh_rbsp_destroy(mh_rbsp_p *rbsp);

mh_result_t mh_rbsp_init(mh_rbsp_p rbsp, mh_uint32_t size);
mh_result_t mh_rbsp_deinit(mh_rbsp_p rbsp);


mh_result_t mh_seq_parameter_set_rbsp(mh_rbsp_p rbsp);

#endif // MH_RBSP_H
