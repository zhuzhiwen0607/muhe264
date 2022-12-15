#ifndef MH_ARRAY_H
#define MH_ARRAY_H

#include "mh_type.h"

typedef struct
{
    mh_uint8_t *start;       // start pos of array elements
    mh_uint8_t *end;
    mh_uint8_t *p;          // current base offset
    mh_uint8_t byte_offset; // bits offset in byte which p point
//    mh_int32_t size;  // size of array elements
}mh_array_t, *mh_array_p;

mh_result_t mh_array_new(mh_array_p* array, size);
mh_result_t mh_array_destroy(mh_array_p* array);

#endif // MH_ARRAY_H
