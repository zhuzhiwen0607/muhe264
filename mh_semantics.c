#ifndef MH_SEMANTICS_C
#define MH_SEMANTICS_C

#include <assert.h>
#include "mh_semantics.h"
#include "mh_error.h"


//-----------------------stream semantics-------------------

mh_bool_t next_bytes_equal(mh_cycle_queue_p q, mh_int32_t n, mh_uint32_t num)
{
    if (!q)
        return mh_false;

    mh_bool_t ret = mh_false;
    mh_int32_t i = 0;

    while (i < n)
    {
        mh_uint8_t bits8 = num >> (n - i - 1) & 0xFF;
//        printf("bit8[%d] = %x\tp[%d] = %x\n", i, bit8, i, p[i]);

        if (mh_cycle_queue_at(q, i) != bits8)
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


mh_result_t leading_zero_8bits(mh_cycle_queue_p q)
{
    if (!q)
        return MH_ERROR;

    return mh_cycle_queue_forward(q, 1);
}

mh_result_t tailing_zero_8bits(mh_cycle_queue_p q)
{
    if (!q)
        return MH_ERROR;

//    return mh_cycle_queue_forward(q, 1);
}

mh_result_t zero_byte(mh_cycle_queue_p q)
{
    if (!q)
        return MH_ERROR;

    if (0x00 == mh_cycle_queue_at(q, 0))
    {
        mh_cycle_queue_forward(q, 1);
        return MH_OK;
    }
    else
        return MH_ERROR;

}

mh_result_t start_code_prefix_one_3bytes(mh_cycle_queue_p q)
{
    if (!q)
        return MH_ERROR;

    if (mh_cycle_queue_more_bytes(q) < 3)
        return MH_ERROR;

    if (0x00 == mh_cycle_queue_at(q, 0)
        && 0x00 == mh_cycle_queue_at(q, 1)
        && 0x01 == mh_cycle_queue_at(q, 2))
    {
        mh_cycle_queue_forward(q, 3);
    }

    return MH_OK;
}

mh_bool_t more_data_in_byte_stream(mh_cycle_queue_p q)
{
    if (!q)
        return mh_false;

    if (mh_cycle_queue_more_bytes(q) > 0)
        return mh_true;
    else
        return mh_false;
}

//------------------------nal unit semantics-----------------
static inline mh_uint8_t get_bit(mh_uint8_t x, mh_int8_t bit)
{
    assert(bit >= 0);

    return ((x & (0x01 << bit)) >> bit);
}


mh_uint32_t read_bits(mh_array_p a, mh_int32_t n)
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




#endif // MH_SEMANTICS_C
