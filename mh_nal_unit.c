
#include <stdlib.h>
#include <string.h>
#include "mh_nal_unit.h"
#include "mh_error.h"
#include "mh_semantics.h"


//static mh_nal_unit_p mh_nal_unit = NULL;

static mh_result_t mh_rbsp_new(mh_uint8_t *rbsp, mh_int32_t size);
static mh_result_t mh_nal_unit(mh_nal_unit_p nalu);
//static mh_result_t mh_seq_parameter_set_rbsp(mh_nal_unit_p nalu);
//static mh_result_t mh_seq_parameter_set_data(mh_nal_unit_p nalu);
//static mh_result_t mh_rbsp_tailing_bits();


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
    mh_array_init(array, size);
//    array->base = malloc(size);
//    memset(array->base, 0x00, size);
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

static mh_result_t mh_rbsp_new(mh_uint8_t *rbsp, mh_int32_t size)
{
    // alloc from system (malloc)
    rbsp = malloc(size);
    memset(rbsp, 0x00, size);

    // alloc from memory pool
}



static mh_result_t mh_nal_unit(mh_nal_unit_p nalu)
{
    if (!nalu)
        return MH_ERROR;

    nalu->forbidden_zero_bit = read_bits(nalu->buf, 1);
    nalu->nal_ref_idc = read_bits(nalu->buf, 2);
    nalu->nal_unit_type = read_bits(nalu->buf, 5);

    mh_int32_t num_bytes_in_rbsp = 0;
    mh_int32_t nal_unit_header_bytes = 1;

    nalu->rbsp = malloc(sizeof(mh_rbsp_t));
    mh_rbsp_init(nalu->rbsp, nalu->buf->size);

    /*
    nalu->rbsp = malloc(sizeof(mh_array_t));
    memset(nalu->rbsp, 0x00, sizeof(mh_array_t));
    mh_array_init(nalu->rbsp, nalu->buf->size);
    nalu->rbsp->bits_offset = 0;
    nalu->rbsp->forward_bits = 0;
    nalu->rbsp->size = 0;
    nalu->rbsp->bits_size = 0;
    */
//    mh_uint8_t *rbsp_byte = nalu->rbsp->base;

//    mh_rbsp_new(nalu->rbsp_byte, nalu->buf->size);

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

    mh_rbsp_p rbsp = nalu->rbsp;

    // for (i = nalUnitHeaderBytes; i < NumBytesInNALunit; i++)
    for (int i = nal_unit_header_bytes; i < nalu->buf->size; i++)
    {
        if (i + 2 < nalu->buf->size && next_bits(nalu->buf, 24) == 0x000003)
        {
//            nalu->rbsp_byte[num_bytes_in_rbsp++] = read_bits(nalu->buf, 8);
//            nalu->rbsp_byte[num_bytes_in_rbsp++] = read_bits(nalu->buf, 8);
//            nalu->rbsp->base[num_bytes_in_rbsp++] = read_bits(nalu->buf, 8);
//            nalu->rbsp->base[num_bytes_in_rbsp++] = read_bits(nalu->buf, 8);
//            nalu->rbsp->size += 2;
//            nalu->rbsp->bits_size += (8 * 2);


            rbsp->buf->base[num_bytes_in_rbsp++] = read_bits(nalu->buf, 8);
            rbsp->buf->base[num_bytes_in_rbsp++] = read_bits(nalu->buf, 8);
            rbsp->buf->size += 2;
            rbsp->buf->bits_size += (8 * 2);
            i += 2;
            // todo emulation_prevention_three_byte
        }
        else
        {
//            nalu->rbsp_byte[num_bytes_in_rbsp++] = read_bits(nalu->buf, 8);
//            nalu->rbsp->base[num_bytes_in_rbsp++] = read_bits(nalu->buf, 8);
//            nalu->rbsp->size += 1;
//            nalu->rbsp->bits_size += 8;

            rbsp->buf->base[num_bytes_in_rbsp++] = read_bits(nalu->buf, 8);
            rbsp->buf->size += 1;
            rbsp->buf->bits_size += 8;
        }
    }

    switch (nalu->nal_unit_type)
    {
        case NAL_UNIT_TYPE_7:
        mh_seq_parameter_set_rbsp(nalu);
        break;
    }

}
/*
static mh_result_t mh_seq_parameter_set_rbsp(mh_nal_unit_p nalu)
{
    if (!nalu)
        return MH_ERROR;

    mh_seq_parameter_set_data(nalu);


}

static mh_result_t mh_seq_parameter_set_data(mh_nal_unit_p nalu)
{
    if (!nalu)
        return MH_ERROR;


}

static mh_result_t mh_rbsp_tailing_bits()
{

}
*/

mh_result_t mh_nal_unit_main(mh_nal_unit_p nalu)
{
    if (!nalu)
        return MH_ERROR;

    mh_nal_unit(nalu);



}


