#ifndef STREAM_H
#define STREAM_H


#include "mh_queue.h"

#include "mh_type.h"

#define STREAM_BUF_SIZE     (1024 * 4)


typedef enum
{
    STREAM_TYPE_FILE,
    STREAM_TYPE_URL
}mh_stream_type_t;


// Byte stream NAL unit semantics

typedef struct
{
    mh_queue_p buf;

}mh_stream_t, *mh_stream_p;

typedef struct
{
//    mh_uint8_t *buf;
//    mh_int32_t bufsize;
    mh_cycle_queue_p buf;
    mh_uint8_t *p;
    mh_uint8_t *nalustart;
//    mh_uint8_t *nal_end;
    mh_int32_t nalusize;
} mh_stream_meta_t, *mh_stream_meta_p;


//mh_result mh_create_stream_meta(mh_stream_meta_p meta, mh_int32_t capacity);
//mh_result mh_destroy_stream_meta(mh_stream_meta_p meta);

//mh_result_t mh_init_stream_meta(mh_stream_meta_p, mh_int32_t);
//mh_result_t mh_deinit_stream_meta(mh_stream_meta_p);

mh_result_t mh_stream_new(mh_stream_p *stream, mh_int32_t capacity);
mh_result_t mh_stream_destroy(mh_stream_p *stream);


mh_result_t mh_stream_meta_init(mh_stream_meta_p meta, mh_int32_t size);
mh_result_t mh_stream_meta_deinit(mh_stream_meta_p meta);
void mh_stream_main(const char *in);


#endif // STREAM_H
