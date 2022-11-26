#ifndef MH_SEMANTICS_H
#define MH_SEMANTICS_H

#include <stdlib.h>
#include "mh_type.h"
#include "mh_log.h"
#include "mh_stream.h"

inline mh_bool_t next_byte_equal(const mh_uint8_t *p, mh_int32_t n, mh_uint32_t bitnum)
{
    if (!p)
        return mh_false;

    mh_bool_t ret = mh_false;
    mh_int32_t i = 0;
    while (i < n)
    {
        mh_uint8_t bit8 = bitnum >> (n - i - 1) & 0xFF;
//        printf("bit8[%d] = %x\tp[%d] = %x\n", i, bit8, i, p[i]);

        if (p[i] != bit8)
        {
            break;
        }

        ++i;
    }
//    printf("\n");

    if (i >= n)
        ret = mh_true;
    else
        ret = mh_false;

    return ret;

}

inline void leading_zero_8bits(mh_uint8_t *p)
{
    if (!p)
        return;

    ++p;
}

inline void zero_byte(mh_uint8_t *p)
{
    if (!p)
        return;

    if (*p == 0x00)
        ++p;
}

inline void start_code_prefix_one_3bytes(mh_uint8_t *p)
{
    if (!p)
        return;

    if (0x00 == p[0] && 0x00 == p[1] && 0x01 == p[2])
    {
        p += 3;
    }
}

inline mh_bool_t more_data_in_byte_stream(mh_stream_meta_p meta)
{
    if (!meta)
        return mh_false;


}


#endif // MH_SEMANTICS_H
