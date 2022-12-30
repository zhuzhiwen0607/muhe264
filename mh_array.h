#ifndef MH_ARRAY_H
#define MH_ARRAY_H

#include <assert.h>
#include "mh_type.h"

/*
typedef struct
{
    mh_uint8_t *start;       // start pos of array elements
    mh_uint8_t *end;
    mh_uint8_t *cur;         // current base offset
    mh_uint8_t offset;       // bits offset in byte which p point
//    mh_int32_t size;         // bytes of array elements
    mh_int32_t capacity;
}mh_array_t, *mh_array_p;
*/

typedef struct
{
    mh_uint8_t *start;
    mh_uint8_t *end;
    mh_uint8_t *p;
    mh_uint8_t bits_offset;
    mh_uint8_t capacity;
}mh_array_t, *mh_array_p;

/**
 * @brief mh_array_new
 * @param array
 * @param capacity
 * @return
 */
mh_result_t mh_array_new(mh_array_p* array, mh_int32_t capacity);

/**
 * @brief mh_array_destroy
 * @param array
 * @return
 */
mh_result_t mh_array_destroy(mh_array_p* array);

/**
 * @brief mh_array_size: get array size
 * @param array
 * @return
 */
mh_uint32_t mh_array_size(mh_array_p array);

//mh_result_t mh_array_move_end(mh_array_p array, mh_int32_t n);


mh_result_t mh_array_push_back(mh_array_p array, mh_uint8_t byte);

mh_result_t mh_array_write_u1(mh_array_p array, mh_uint8_t byte);
mh_result_t mh_array_write_u(mh_array_p array, mh_uint8_t *src, mh_int32_t n);

//mh_void_t mh_array_write_update(mh_array_p* array, mh_int32_t size);
mh_result_t mh_array_end_forward(mh_array_p array, mh_int32_t i);
mh_result_t mh_array_cur_forward(mh_array_p array, mh_int32_t i);

static inline mh_uint32_t array_size(mh_array_p array);
static inline mh_bool_t eof(mh_array_p array);
static inline mh_uint8_t bit_val(mh_uint8_t byte, mh_uint8_t bit);
static inline mh_uint32_t read_bits_f(mh_array_p array, mh_int32_t n);
static inline mh_uint32_t read_bits_u1(mh_array_p array);
static inline mh_uint32_t read_bits_u(mh_array_p array, mh_int32_t n);
static inline mh_uint32_t next_bits_u(mh_array_p array, mh_int32_t n);
static inline mh_uint32_t next_bits_u1(mh_array_p array);

/*
static inline mh_uint32_t array_size(mh_array_p array)
{
    assert(array);
    assert(array->cur);
    assert(array->end);
    assert(array->end - array->cur >= 0);

    return array->end - array->cur;
}
*/

static inline mh_bool_t eof(mh_array_p array)
{
    assert(array);

    return (array->end - array->p > 0) ? mh_false : mh_true;
}

static inline mh_uint8_t bit_val(mh_uint8_t byte, mh_uint8_t bit)
{
    return ((byte & (1 << bit)) >> bit);
}

static inline mh_uint32_t read_bits_f(mh_array_p array, mh_int32_t n)
{
    assert(array);
    return read_bits_u(array, n);
}

static inline mh_uint32_t read_bits_u1(mh_array_p array)
{
    assert(array);

    mh_uint32_t r = 0;

    if (!eof(array))
    {
        r = bit_val(array->p, array->bits_offset);
    }

    array->p += (array->bits_offset + 1) / 8;
    array->bits_offset = (array->bits_offset + 1) % 8;

    return r;
}

static inline mh_uint32_t read_bits_u(mh_array_p array, mh_int32_t n)
{
    assert(array);

    mh_uint32_t r = 0;

    mh_int32_t i = 0;
    while (i < n)
    {
        r |= read_bits_u1(array) << i;

        ++i;
    }

    return r;
}

static inline mh_uint32_t next_bits_u(mh_array_p array, mh_int32_t n)
{
    assert(array);

    mh_uint32_t r = 0;
    mh_int32_t i = 0;
    while (i < n)
    {
        mh_uint8_t byte = *(array->p + (array->bits_offset + i) / 8);
        mh_uint8_t bit = array->bits_offset + (array->bits_offset + i) % 8;

        r |= bit_val(byte, bit) << i;

        ++i;
    }

    return r;

}

static inline mh_uint32_t next_bits_u1(mh_array_p array)
{
    assert(array);

    return bit_val(array->p, array->bits_offset);

}


/*
static inline mh_int32_t read_bits_n(mh_array_p array, mh_int32_t n)
{
    assert(array);

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
*/
#endif // MH_ARRAY_H
