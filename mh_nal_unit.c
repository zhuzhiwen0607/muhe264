
#include <stdlib.h>
#include "mh_nal_unit.h"
#include "mh_error.h"
#include "mh_semantics.h"


//static mh_nal_unit_p mh_nal_unit = NULL;


mh_result_t mh_nal_unit_new(mh_int32_t size, mh_nal_unit_p *nalu)
{
    if (nalu == NULL || *nalu != NULL)
        return MH_ERROR;

    // two alloc methods
    // 1. alloc with malloc
    *nalu = malloc(sizeof(mh_nal_unit_t));
    if (*nalu == NULL)
        return MH_ERROR;

    memset(*nalu, 0x00, sizeof(mh_nal_unit_t));

//    (*nalu)->buf = malloc(size);
//    memset((*nalu)->buf, 0x00, size);

//    (*nalu)->buf


    // 2. alloc from memory pool(mh_mm.c)  --todo

    return MH_OK;

}

mh_result_t mh_nal_unit_init(mh_cycle_queue_p queue, mh_int32_t size, mh_nal_unit_p nalu)
{
    if (!nalu)
        return MH_ERROR;

    nalu->buf = malloc(sizeof(mh_array_t));
    memset(nalu->buf, 0x00, sizeof(mh_array_t));

    mh_array_p array = nalu->buf;
    array->base = malloc(size);
    memset(array->base, 0x00, size);
//    array->bits_start = array->base;
//    array->bits_offset = 0;
//    array->capacity = size;
//    array->size = 0;

    // copy bits frome queue to nalu
    mh_cycle_queue_read(queue, array->base, size);
    array->bits_start = array->base;
    array->bits_offset = 0;
    array->forward_bits = 0;
    array->size = size;
    array->bits_size = size * sizeof(mh_uint8_t);

    return MH_OK;
}

/*
mh_result_t mh_nal_unit_init(mh_cycle_queue_p queue, mh_int32_t size, mh_nal_unit_p nalu)
{



    mh_cycle_queue_read(queue, nalu->buf, size);
    nalu->size = size;
}
*/

static mh_result_t mh_nal_uint(mh_nal_unit_p nalu)
{
    if (!nalu)
        return MH_ERROR;

    nalu->forbidden_zero_bit = read_bits(nalu->buf, 1);
    nalu->nal_ref_idc = read_bits(nalu->buf, 2);
    nalu->nal_unit_type = read_bits(nalu->buf, 5);

    mh_int32_t num_bytes_in_rbsp = 0;
    mh_int32_t nal_unit_header_bytes = 1;

    if (NAL_UNIT_TYPE_14 == nalu->nal_unit_type
            || NAL_UNIT_TYPE_20 == nalu->nal_unit_type
            || NAL_UNIT_TYPE_21 == nalu->nal_unit_type)
    {
        if (NAL_UNIT_TYPE_21 != nalu->nal_unit_type)
        {
            nalu->svc_extension_flag = read_bits(nalu->buf, 1);
        }
        else
        {
            nalu->avc_3d_extension_flag = read_bits(nalu->buf, 1);
        }

        if (nalu->svc_extension_flag)
        {
            // todo: nal_unit_header_svc_extension( )
            nal_unit_header_bytes += 3;
        }
        else if (nalu->avc_3d_extension_flag)
        {
            // todo: nal_unit_header_3davc_extension( )
            nal_unit_header_bytes += 2;
        }
        else
        {
            // todo: nal_unit_header_mvc_extension( )
            nal_unit_header_bytes += 3;
        }
    }

    for (int i = 0; i < nal_unit_header_bytes; i++)
    {

    }

}


mh_result_t mh_nal_unit_main(mh_nal_unit_p nalu)
{
    if (!nalu)
        return MH_ERROR;

    mh_nal_uint(nalu);



}


