
#include <stdlib.h>
#include <string.h>
#include "mh_rbsp.h"
#include "mh_error.h"
#include "mh_type.h"


static mh_result_t mh_seq_parameter_set_data(mh_rbsp_p rbsp);

mh_result_t mh_rbsp_init(mh_rbsp_p rbsp, mh_uint32_t size)
{
    if (!rbsp)
        return MH_ERROR;

//    *rbsp = malloc(sizeof(mh_rbsp_t));
//    memset(*rbsp, 0x00, sizeof(mh_rbsp_t));

    memset(rbsp, 0x00, sizeof(mh_rbsp_t));

    rbsp->buf = malloc(sizeof(mh_array_t));

    mh_array_init(rbsp->buf, size);

    return MH_OK;

}

mh_result_t mh_rbsp_deinit(mh_rbsp_p rbsp)
{
    if (!rbsp)
        return MH_ERROR;

    // todo

    return MH_OK;
}

mh_result_t mh_seq_parameter_set_rbsp(mh_rbsp_p rbsp)
{

    return MH_OK;
}

static mh_result_t mh_seq_parameter_set_data(mh_rbsp_p rbsp)
{
    return MH_OK;
}
