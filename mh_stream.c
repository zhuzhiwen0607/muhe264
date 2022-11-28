
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mh_stream.h"
#include "mh_log.h"
#include "mh_semantics.h"
#include "mh_error.h"

static mh_stream_meta_t g_stream_meta;

static mh_stream_meta_p mh_stream_meta = NULL;


static void mh_stream_consumer(const char *streambuf, const int bufsize);
static void mh_stream_nal_unit();
/*
mh_result mh_create_stream_meta(mh_stream_meta_p *meta, mh_int32_t capacity)
{
//    if (meta)
//        return MH_ERROR;

    mh_new_stream_meta(meta);

    mh_init_stream_meta(*meta, capacity);


}

mh_result mh_destroy_stream_meta(mh_stream_meta_p *meta)
{
    mh_deinit_stream_meta(*meta);

    mh_delete_stream_meta(meta);
}

static mh_result mh_new_stream_meta(mh_stream_meta_p *meta)
{
    if (*meta)
        return MH_ERROR;

    *meta = malloc(sizeof(mh_stream_meta_t));
    memset(*meta, 0x00, sizeof(mh_stream_meta_t));

    return MH_OK;
}

static mh_result mh_delete_stream_meta(mh_stream_meta_p *meta)
{
    if (!(*meta))
        return MH_ERROR;

    free(*meta);

    *meta = NULL;

    return MH_OK;
}

mh_result mh_create_stream_meta(mh_stream_meta_p *meta)
{
    if (*meta)
        return MH_ERROR;

    (*meta) = malloc(sizeof(mh_stream_meta_t));
    memset(*meta, 0x00, sizeof(mh_stream_meta_t));

}

mh_result mh_init_stream_meta2(mh_stream_meta_p *meta, mh_int32_t bufsize)
{
    if (*meta)
        return MH_ERROR;

    (*meta) = malloc(sizeof(mh_stream_meta_t));
    memset(*meta, 0x00, sizeof(mh_stream_meta_t));

    (*meta)->bufsize = bufsize;
    (*meta)->buf = malloc(bufsize);
    memset((*meta)->buf, 0x00, bufsize);
}
*/
mh_result_t mh_init_stream_meta(mh_stream_meta_p meta, mh_int32_t size)
{
    if (!meta)
        return MH_ERROR_INVALID_PARAM;

    memset(meta, 0x00, sizeof(mh_stream_meta_t));

    meta->buf = malloc(sizeof(mh_cycle_queue_t));
    memset(meta->buf, 0x00, sizeof(mh_cycle_queue_t));
    mh_cycle_queue_init(meta->buf, size);

    meta->nal_start = NULL;
    meta->nal_end = NULL;
    meta->p = meta->buf->base;

    return MH_OK;

}

mh_result_t mh_deinit_stream_meta(mh_stream_meta_p meta)
{
    if (!meta)
        return MH_ERROR;

    mh_cycle_queue_deinit(meta->buf);
    free(meta->buf);

    meta->buf = NULL;
    meta->nal_start = NULL;
    meta->nal_end = NULL;
    meta->p = NULL;

    return MH_OK;

}

static mh_bool_t mh_reset_streammeta(mh_stream_meta_p meta, mh_int32_t append_bufsize)
{
    if (!meta)
        return mh_false;

//    mh_int32_t new_bufsize = meta->bufsize +
}



static void mh_stream_consumer(const char *streambuf, const int bufsize)
{
    /*
    if (!streambuf)
    {
        mh_error("input param is null");
        return;
    }

    const int buf_offset = g_stream_meta.bufsize;
    const int new_bufsize = bufsize + buf_offset;

    if (g_stream_meta.bufsize > 0 && g_stream_meta.buf)
    {
        realloc(g_stream_meta.buf, new_bufsize);
        memset(g_stream_meta.buf + buf_offset, 0, bufsize);
    }
    else
    {
        g_stream_meta.buf = (char*)malloc(bufsize);
        memset(g_stream_meta.buf, 0, bufsize);
    }

    mh_info("streambuf=%p, bufsize=%d, buf_offset=%d, new_bufsize=%d", streambuf, bufsize, buf_offset, new_bufsize);

    memcpy(g_stream_meta.buf + buf_offset, streambuf, bufsize);
    */

    mh_stream_nal_unit();
}



static void mh_stream_nal_unit()
{
    const mh_uint8_t *base = g_stream_meta.buf;
    mh_uint8_t *p = g_stream_meta.p;
//    mh_int32_t streamsize = g_stream_meta.bufsize;

mh_int32_t streamsize = 0;

    // next_bits( 24 ) != 0x000001 && next_bits( 32 ) != 0x00000001
    while (!next_byte_equal(&g_stream_meta, 3, 0x000001) && !next_byte_equal(p, 4, 0x00000001))
    {
        leading_zero_8bits(p);
        if (p + 4 - base > streamsize)
        {
            mh_error("not a complete nal unit");
            return;
        }
    }

    /*
    int i = 0;
    while (// next_bits( 24 ) != 0x000001 && next_bits( 32 ) != 0x00000001
           (cur[i] != 0x00 || cur[i+1] != 0x00 || cur[i+2] != 0x01)
           && (cur[i] != 0x00 || cur[i+1] != 0x00 || cur[i+2] != 0x00 || cur[i+3] != 0x01))
    {
        ++i;    // leading_zero_8bits

        if (i + 4 > streamsize)
        {
            mh_info("not a complete nal unit");
            return;
        }
    }*/

    if (!next_byte_equal(p, 3, 0x000001))   // next_bits( 24 ) != 0x000001
    {
        zero_byte(p);   // equal to 0x00
    }

    start_code_prefix_one_3bytes(p);    // equal to 0x000001

    g_stream_meta.nal_start = p;

    while (!next_byte_equal(p, 3, 0x000001) && !next_byte_equal(p, 4, 0x00000001))
    {

    }







}

static void mh_nal_unit()
{

}

mh_result_t mh_stream_loop_read(FILE *src, mh_stream_meta_p dst)
{
    if (!src || !dst)
        return MH_ERROR;

    mh_int32_t bufcap = dst->buf->capacity;

    mh_int32_t freebytes = 0;
    mh_int32_t readbytes = 0;
    mh_uint8_t *readbuf = malloc(bufcap);
    mh_bool_t done = mh_false;

    do
    {
        memset(readbuf, 0x00, bufcap);
        freebytes = mh_cycle_queue_notused(dst->buf);

        readbytes = fread(readbuf, 1, freebytes, src);
        if (readbytes < freebytes)
        {
            // eof

            done = mh_true;
        }

        mh_cycle_queue_write(dst->buf, readbuf, readbytes);

        // todo: begin to decode h.264 data readed from file
        mh_stream_nal_unit();

    } while (!done);


}


void mh_stream_main(const char *in)
{
    if (!in)
    {
        mh_error("input param is null");
        return;
    }

    FILE *infile = NULL;
    infile = fopen(in, "r");
    if (!infile)
    {
        mh_error("fopen %s failed", in);
        return;
    }

    mh_stream_meta = malloc(sizeof(mh_stream_meta_t));
    mh_init_stream_meta(&mh_stream_meta, STREAM_BUF_CAPACITY);

    mh_stream_loop_read(infile, &mh_stream_meta);

    fclose(infile);

}


