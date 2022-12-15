#ifndef MH_SEMANTICS_C
#define MH_SEMANTICS_C

#include <assert.h>
#include "mh_semantics.h"
#include "mh_error.h"



#if 0
//------------------------nal unit semantics-----------------
static inline mh_uint8_t get_bit(mh_uint8_t x, mh_int8_t bit)
{
    assert(bit >= 0);

    return ((x & (0x01 << bit)) >> bit);
}


mh_int32_t read_bits(mh_array_p a, mh_int32_t n)
{
    if (!a)
        return -1;

    mh_uint32_t r = 0;
    mh_int32_t i = 0;
    while (i < n)
    {
        ++(a->forward_bits);
        assert(a->forward_bits <= a->bits_size);

        const mh_int8_t offset = a->bits_offset;
        const mh_uint8_t byte = *(a->bits_start);

        mh_uint8_t bitval = get_bit(byte, offset);

        r |= (bitval << i);

        if (++(a->bits_offset) >= 8)
        {
            a->bits_offset = 0;
            ++(a->bits_start);
        }

        ++i;
    }

    mh_info("output: r = %u", r);

    return r;
}

mh_int32_t next_bits(mh_array_p a, mh_int32_t n)
{
    if (!a)
        return -1;

    if (n > a->bits_size - a->forward_bits)
    {
        return -1;
    }

    mh_int8_t offset = a->bits_offset;
//    mh_uint8_t byte = *(a->bits_start);
    mh_uint8_t *byte = a->bits_start;
    mh_uint32_t r = 0;
    mh_int32_t i = 0;
    mh_int32_t nbyte = 0;
    while (i < n)
    {


        mh_uint8_t bitval = get_bit(byte[nbyte], offset);

        r |= bitval << i;

        if (++offset >= 8)
        {
            offset = 0;
            ++nbyte;
        }

        ++i;
    }

    mh_info("r = %u", r);

    return r;
}
#endif



#endif // MH_SEMANTICS_C
