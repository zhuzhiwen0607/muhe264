#ifndef STREAM_H
#define STREAM_H

#include "mh_type.h"

#define STREAM_BUF_SIZE     (1024 * 4)


// Byte stream NAL unit semantics

typedef struct
{
    mh_uint8_t *buf;
    mh_int32_t bufsize;
    mh_uint8_t *p;
    mh_uint8_t *nal_start;
    mh_uint8_t *nal_end;
} mh_stream_meta_t, *mh_stream_meta_p;


//mh_result mh_create_stream_meta(mh_stream_meta_p meta, mh_int32_t capacity);
//mh_result mh_destroy_stream_meta(mh_stream_meta_p meta);

mh_bool_t mh_init_stream_meta(mh_stream_meta_p meta, mh_int32_t bufsize);
mh_result mh_init_stream_meta2(mh_stream_meta_p *meta, mh_int32_t bufsize);
mh_result mh_create_stream_meta(mh_stream_meta_p meta, mh_int32_t capacity);
void mh_stream_main(const char *in);


#endif // STREAM_H
