
#include <stdlib.h>
#include "mh_nal_unit.h"
#include "mh_error.h"





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
    array->bits_start = array->base;
    array->bits_offset = 0;
    array->capacity = size;
    array->size = 0;

    // copy bits frome queue to nalu
    mh_cycle_queue_read(queue, array->base, size);
    array->size = size;

    return MH_OK;
}

/*
mh_result_t mh_nal_unit_init(mh_cycle_queue_p queue, mh_int32_t size, mh_nal_unit_p nalu)
{



    mh_cycle_queue_read(queue, nalu->buf, size);
    nalu->size = size;
}
*/



mh_result_t mh_nal_unit_main(mh_nal_unit_p nalu)
{
    if (!nalu)
        return MH_ERROR;


}


