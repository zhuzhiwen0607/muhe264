#ifndef MH_TYPE_H
#define MH_TYPE_H

// internal type
typedef void            mh_void_t;
typedef void*           mh_void_p;

typedef char            mh_int8_t;
typedef unsigned char   mh_uint8_t;
typedef short           mh_int16_t;
typedef unsigned short  mh_uint16_t;
typedef int             mh_int32_t;
typedef unsigned int    mh_uint32_t;

typedef char            mh_char_t;
typedef unsigned char   mh_uchar_t;


typedef enum
{
    mh_false = 0,
    mh_true = 1
}mh_bool_t;

//typedef unsigned int    mh_bool_t;
//#define mh_true         1
//#define mh_false        0

typedef int             mh_result_t;

//-------------------mh_cycle_queue-------------
typedef struct
{
    mh_uint8_t *base;
//    mh_uint8_t *buf;
    mh_uint8_t *start;
    mh_uint8_t *end;

//    mh_int32_t startpos;
//    mh_int32_t endpos;
    mh_int32_t capacity;
    mh_int32_t used;
//    mh_bool_t (*init) (mh_int32_t);
//    mh_bool_t (*deinit) ();
//    mh_bool_t (*read) (mh_int32_t);
//    mh_bool_t (*write) (mh_int32_t);
}mh_cycle_queue_t, *mh_cycle_queue_p;



mh_result_t mh_cycle_queue_init(mh_cycle_queue_p queue, mh_int32_t capacity);
mh_result_t mh_cycle_queue_deinit(mh_cycle_queue_p queue);
mh_result_t mh_cycle_queue_reset(mh_cycle_queue_p queue);
mh_result_t mh_cycle_queue_write(mh_cycle_queue_p queue, mh_uint8_t *src, mh_int32_t wsize);
mh_result_t mh_cycle_queue_read(mh_cycle_queue_p queue, mh_uint8_t *dst, mh_int32_t rsize);
mh_int32_t mh_cycle_queue_more_bytes(mh_cycle_queue_p queue);
mh_int32_t mh_cycle_queue_free_size(mh_cycle_queue_p queue);
mh_uint8_t mh_cycle_queue_at(mh_cycle_queue_p queue, mh_int32_t i);
mh_result_t mh_cycle_queue_forward(mh_cycle_queue_p queue, mh_int32_t i);

//-------------------mh_array-----------------------

typedef struct
{
    mh_uint8_t *base;       // start pos of array elements
    mh_uint8_t *p;          // current base offset
    mh_uint8_t byte_offset; // bits offset in byte which p point
    mh_int32_t bytes_size;  // size of array elements
}mh_array_t, *mh_array_p;

/*
typedef struct
{
    mh_uint8_t *base;
    mh_uint8_t *bits_start;     // always point to a start position of a byte
    mh_int8_t bits_offset;      // bits offset in a byte [bits_start offset]
    mh_int32_t forward_bits;
    mh_int32_t bits_size;       // num bits in nal unit
    mh_int32_t size;            // num bytes in nal unit
//    mh_int32_t capacity;
}mh_array_t, *mh_array_p;
*/

mh_result_t mh_array_new(mh_array_p *array, mh_int32_t capacity);
mh_result_t mh_array_destroy(mh_array_p *array);
//mh_result_t mh_array_init(mh_array_p array, mh_int32_t capacity);
//mh_result_t mh_array_deinit(mh_array_p array);

static inline mh_bool_t eof(mh_array_p array)
{
    assert(array);

    if (array->p > array->base + array->bytes_size)
        return mh_true;
    else
        return mh_false;
}

static inline mh_uint8_t bit_val(mh_uint8_t byte, mh_int8_t bit)
{
    assert(bit >= 0);

    return ((byte & (1 << bit)) >> bit);
}



#endif // MH_TYPE_H
