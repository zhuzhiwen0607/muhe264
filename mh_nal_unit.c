
#include <stdlib.h>
#include "mh_nal_unit.h"
#include "mh_error.h"

mh_result_t mh_nal_unit_main(mh_nal_unit_p nalu)
{
    if (!nalu)
        return MH_ERROR;


}



mh_result_t mh_nal_unit_new(mh_int32_t size, mh_nal_unit_p *nalu)
{
    if (nalu == NULL || *nalu != NULL)
        return MH_ERROR;

    // two alloc methods
    // 1. alloc with malloc
    *nalu = malloc(size);
    if (*nalu == NULL)
        return MH_ERROR;


    // 2. alloc from memory pool(mh_mm.c)  --todo

    return MH_OK;

}

mh_result_t mh_nal_unit_init(mh_cycle_queue_p queue, mh_int32_t size, mh_nal_unit_p nalu)
{
    mh_cycle_queue_read(queue, nalu->buf, size);
    nalu->size = size;
}


