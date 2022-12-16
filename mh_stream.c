
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mh_stream.h"
#include "mh_mm.h"
#include "mh_log.h"
#include "mh_semantics.h"
#include "mh_nal_unit.h"
#include "mh_error.h"


static mh_stream_p mh_stream = NULL;


static void mh_stream_nal_unit();

static mh_stream_type_t mh_stream_check_type(const char *in);
static mh_result_t mh_stream_init(mh_stream_p stream, mh_int32_t capacity);
static mh_result_t mh_stream_deinit(mh_stream_p stream);
static mh_void_t mh_stream_add_nalu(mh_stream_p stream, mh_nal_unit_p nalu);


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

//    mh_result_t ret = mh_malloc(stream->buf, sizeof(mh_queue_t));
//    if (MH_OK != ret)
//        return ret;

    mh_result_t ret = mh_array_new(&(stream->buf), capacity);
    if (MH_OK != ret)
        return ret;

    ret = mh_list_new(&(stream->list));
    if (MH_OK != ret)
        return ret;

    stream->nalu_start = NULL;
    stream->nalu_end = NULL;
    stream->nalu_size = 0;


    return MH_OK;

}

static mh_result_t mh_stream_deinit(mh_stream_p stream)
{
    assert(stream);

    mh_free(stream->buf);
    stream->buf = NULL;

    return MH_OK;
}

static mh_void_t mh_stream_add_nalu(mh_nal_unit_p nalu)
{
    assert(mh_stream);
    assert(nalu);

    mh_list_push_back(mh_stream->list, nalu->list);
}


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

        // new nalu_unit
        mh_nal_unit_p nal_unit = NULL;
        mh_nal_unit_new(mh_stream->buf, &nal_unit, nalu_size);

        // add nal_unit to mh_stream_meta
        mh_stream_add_nalu(nal_unit);

        mh_stream->nalu_size = 0;

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
        readbytes = fread(readbuf, 1, freebytes, file);
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


