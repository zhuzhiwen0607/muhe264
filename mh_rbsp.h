#ifndef MH_RBSP_H
#define MH_RBSP_H

#include "mh_type.h"

typedef struct
{
    mh_array_p buf;


}mh_rbsp_t, *mh_rbsp_p;

mh_result_t mh_rbsp_init(mh_rbsp_p rbsp, mh_uint32_t size);
mh_result_t mh_rbsp_deinit(mh_rbsp_p rbsp);


mh_result_t mh_seq_parameter_set_rbsp(mh_rbsp_p rbsp);

#endif // MH_RBSP_H
