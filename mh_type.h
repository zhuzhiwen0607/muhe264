#ifndef MH_TYPE_H
#define MH_TYPE_H

// internal type
typedef char            mh_int8_t;
typedef unsigned char   mh_uint8_t;
typedef short           mh_int16_t;
typedef unsigned short  mh_uint16_t;
typedef int             mh_int32_t;
typedef unsigned int    mh_uint32_t;

typedef char            mh_char_t;
typedef unsigned char   mh_uchar_t;


typedef unsigned int    mh_bool_t;
#define mh_true         1
#define mh_false        0

typedef int             mh_result_t;

// basic data struct
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

#endif // MH_TYPE_H
