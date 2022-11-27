
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mh_stream.h"
#include "mh_log.h"
#include "mh_semantics.h"
#include "mh_error.h"

static mh_stream_meta_t g_stream_meta;


static void mh_stream_consumer(const char *streambuf, const int bufsize);
static void mh_stream_nal_unit();

mh_result mh_create_stream_meta(mh_stream_meta_p *meta, mh_int32_t capacity)
{
//    if (meta)
//        return MH_ERROR;

    mh_new_stream_meta(meta);

    mh_init_stream_meta(*meta, capacity);


}

mh_result mh_destroy_stream_meta(mh_stream_meta_p meta)
{

}

static mh_result mh_new_stream_meta(mh_stream_meta_p *meta)
{
    if (*meta)
        return MH_ERROR;

    *meta = malloc(sizeof(mh_stream_meta_t));
    memset(*meta, 0x00, sizeof(mh_stream_meta_t));

    return MH_OK;
}

/*
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
mh_bool_t mh_init_stream_meta(mh_stream_meta_p meta, mh_int32_t bufsize)
{
    if (!meta)
        return mh_false;

    memset(meta, 0, sizeof(*meta));

    const int bufbytes = sizeof(mh_uint8_t) * bufsize;
    meta->bufsize = bufbytes;
    meta->buf = malloc(bufbytes);
    if (!meta->buf)
        return mh_false;

    memset(meta->buf, 0x00, bufbytes);

    meta->p = meta->buf;

    return mh_true;

}

static mh_bool_t mh_reset_streammeta(mh_stream_meta_p meta, mh_int32_t append_bufsize)
{
    if (!meta)
        return mh_false;

//    mh_int32_t new_bufsize = meta->bufsize +
}



static void mh_stream_consumer(const char *streambuf, const int bufsize)
{
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

    mh_stream_nal_unit();
}



static void mh_stream_nal_unit()
{
    const mh_uint8_t *base = g_stream_meta.buf;
    mh_uint8_t *p = g_stream_meta.p;
    mh_int32_t streamsize = g_stream_meta.bufsize;


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
/*
    char *streambuf = NULL;
    const int bufsize = sizeof(char) * STREAM_BUF_SIZE;
    streambuf = malloc(bufsize);
    if (!streambuf)
    {
        mh_error("malloc streambuf failed");
        return;
    }
*/


    const int bufsize = sizeof(char) * STREAM_BUF_SIZE;
    mh_init_stream_meta(&g_stream_meta, bufsize);

    int readbytes = 0;
    do
    {
//        memset(streambuf, 0, bufsize);

//        readbytes = fread(streambuf, 1, STREAM_BUF_SIZE, infile);
        if (readbytes < STREAM_BUF_SIZE)
        {
            // eof or error

            break;
        }
        else
        {
//            mh_stream_nal_unit();
        }
    } while (readbytes > 0);






//    free(streambuf);

    fclose(infile);

}


