
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mh_stream.h"
#include "mh_mm.h"
#include "mh_log.h"
#include "mh_semantics.h"
#include "mh_nal_unit.h"
#include "mh_error.h"



//static mh_stream_meta_t g_stream_meta;

//static mh_stream_meta_p mh_stream_meta = NULL;

static mh_stream_p mh_stream = NULL;


//static void mh_stream_consumer(const char *streambuf, const int bufsize);
static void mh_stream_nal_unit();

static mh_stream_type_t mh_stream_check_type(const char *in);
static mh_result_t mh_stream_init(mh_stream_p stream, mh_int32_t capacity);
static mh_result_t mh_stream_deinit(mh_stream_p stream);

mh_result_t mh_stream_new(mh_stream_p *stream, mh_int32_t capacity)
{
    mh_result_t ret = mh_malloc(*stream, sizeof(mh_stream_t));
    if (MH_OK != ret)
        return ret;

    return mh_stream_init(*stream, capacity);
}

mh_result_t mh_stream_destroy(mh_stream_p *stream)
{
    mh_result_t ret = mh_stream_deinit(*stream);
    if (MH_OK != ret)
        return ret;

    mh_free(*stream);
}

static mh_result_t mh_stream_init(mh_stream_p stream, mh_int32_t capacity)
{
    assert(stream);

    mh_result_t ret = mh_malloc(stream->buf, sizeof(mh_queue_t));
    if (MH_OK != ret)
        return ret;

    return MH_OK;

}

static mh_result_t mh_stream_deinit(mh_stream_p stream)
{
    assert(stream);

    mh_free(stream->buf);
    stream->buf = NULL;

    return MH_OK;
}
/*
mh_result_t mh_stream_meta_init(mh_stream_meta_p meta, mh_int32_t size)
{
    if (!meta)
        return MH_ERROR_INVALID_PARAM;

    memset(meta, 0x00, sizeof(mh_stream_meta_t));

    meta->buf = malloc(sizeof(mh_cycle_queue_t));
    memset(meta->buf, 0x00, sizeof(mh_cycle_queue_t));
    mh_cycle_queue_init(meta->buf, size);

    meta->nalustart = NULL;
//    meta->nal_end = NULL;
    meta->nalusize = 0;
    meta->p = meta->buf->base;

    return MH_OK;

}

mh_result_t mh_stream_meta_deinit(mh_stream_meta_p meta)
{
    if (!meta)
        return MH_ERROR;

    mh_cycle_queue_deinit(meta->buf);
    free(meta->buf);

    meta->buf = NULL;
    meta->nalustart = NULL;
    meta->nalusize = 0;
//    meta->nal_end = NULL;
    meta->p = NULL;

    return MH_OK;

}

static mh_bool_t mh_reset_streammeta(mh_stream_meta_p meta, mh_int32_t append_bufsize)
{
    if (!meta)
        return mh_false;

//    mh_int32_t new_bufsize = meta->bufsize +
}
*/

#if 0
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
#endif

//static void mh_stream_set_nal_start()
//{
//    mh_stream_meta->nal_start = mh_stream
//}

//static void mh_stream_set_nal_end()
//{

//}


static void mh_stream_nal_unit()
{
//    mh_cycle_queue_p buf = mh_stream_meta->buf;
    mh_queue_p queue = mh_stream->buf;

    for (;;)
    {
        // next_bits( 24 ) != 0x000001 && next_bits( 32 ) != 0x00000001
        while (!next_bytes_equal(queue, 3, 0x000001) && !next_bytes_equal(queue, 4, 0x00000001))
        {
            leading_zero_8bits(queue);
            if (more_bytes(queue) < 4)
                return;
        }

        if (!next_bytes_equal(queue, 3, 0x000001))   // next_bits( 24 ) != 0x000001
        {
            zero_byte(queue);   // equal to 0x00
        }

        start_code_prefix_one_3bytes(queue);    // equal to 0x000001

        // we must set STREAM_BUF_CAPACITY large enough to restore at least one NAL unit
        // set nal_start
//        mh_stream_meta->nalustart = buf->start;
        mh_stream->nalu_start = queue->start;


        while (more_data_in_byte_stream(queue)
               && !next_bytes_equal(queue, 3, 0x000001)
               && !next_bytes_equal(queue, 4, 0x00000001))
        {
            ++(mh_stream->nalu_size);

            if (more_bytes(queue) < 4)
            {
                // not a compete nal unit, read data from source again
                return;
            }
        }

        mh_int32_t nalu_size = mh_stream->nalu_size;
        mh_stream->nalu_end = mh_stream->nalu_start + nalu_size;


        // parse nal unit
        // copy
        mh_nal_unit_p nal_unit = NULL;
        mh_nal_unit_new(nalu_size, &nal_unit);
        mh_nal_unit_init(buf, ns, nal_unit);


        mh_stream->nalu_size = 0;



        // add nal_unit to mh_stream_meta


        // do parse
        mh_nal_unit_main(nal_unit);



    }

}


mh_result_t mh_stream_read_file(const char *filepath)
{
    if (!filepath)
    {
        mh_error("input %s is null", filepath);
        return MH_INVALID_PARAM;
    }

    FILE *file = NULL;

    file = fopen(filepath, "r");
    if (!file)
    {
        mh_error("fopen %s failed", filepath);
        return;
    }

    mh_int32_t read_buf_size = STREAM_BUF_SIZE;
    mh_uint8_t *readbuf = NULL;
    mh_result_t ret = mh_malloc(&readbuf, read_buf_size);
    if (MH_OK != ret)
        return ret;

    mh_int32_t freebytes = 0;
    mh_int32_t readbytes = 0;
    mh_int32_t writebytes = 0;
    mh_bool_t done = mh_false;
    do
    {
        memset(readbuf, 0x00, read_buf_size);

        freebytes = mh_queue_space(mh_stream->buf);
        readbytes = fread(readbuf, 1, freebytes, infile);
        if (readbytes < freebytes)
        {
            // eof
            done = mh_true;
        }

        writebytes = mh_queue_write(mh_stream->buf, readbuf, readbytes);
        if (writebytes < readbytes)
        {
            mh_error("writebytes < readbytes! writebytes=%d, readbytes=%d", writebytes, readbytes);
        }

        // begin to decode h.264 data readed from file
        mh_stream_nal_unit();

    } while (!done);



    fclose(file);

}

/*
mh_result_t mh_stream_loop_read(FILE *src)
{
    if (!src)
        return MH_ERROR;

    mh_stream_meta_p dst = mh_stream_meta;

    mh_int32_t bufcap = dst->buf->capacity;

    mh_int32_t freebytes = 0;
    mh_int32_t readbytes = 0;
    mh_uint8_t *readbuf = malloc(bufcap);
    mh_bool_t done = mh_false;

    do
    {
        memset(readbuf, 0x00, bufcap);
        freebytes = mh_cycle_queue_free_size(dst->buf);

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
*/
static mh_stream_type_t mh_stream_check_type(const char *in)
{
    return STREAM_TYPE_FILE;
}


void mh_stream_main(const char *in)
{
    if (!in)
    {
        mh_error("input para is null");
        return;
    }

    // todo: check in is filepath or url
    mh_stream_type_t type = mh_stream_check_type(in);

    mh_int32_t stream_buf_size = STREAM_BUF_SIZE;
    mh_stream_new(&mh_stream, stream_buf_size);

    switch (type)
    {
    case STREAM_TYPE_FILE:
        // read h264 from file
        mh_stream_read_file(in);
        break;
    case STREAM_TYPE_URL:
        // todo: read h264 from net stream
        break;
    default:
        break;
    }

    mh_stream_destroy(&mh_stream);
    mh_stream = NULL;


}


