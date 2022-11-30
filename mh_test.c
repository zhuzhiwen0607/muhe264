
#include "mh_test.h"
#include "mh_log.h"
#include "mh_error.h"

//-----------------sementics test--------------------------
#include "mh_semantics.h"

static mh_bool_t test_next_byte_equal()
{
    mh_info("test_next_byte_equal begin...");

    mh_bool_t ret;

    // testcase 1
    // expect: mh_true
    mh_uint8_t p1[4] = {0x00, 0x00, 0x00, 0x01};
    mh_int32_t n1 = 4;
    mh_uint32_t bitnum = 0x00000001;

    ret = next_byte_equal(p1, n1, bitnum);
    if (mh_true == ret)
    {
        mh_info("testcase 1: ok");
    }
    else
    {
        mh_error("testcase 1: failed");
    }

    // testcase 2
    // expect: mh_false
    mh_uint8_t p2[3] = {0x01, 0x00, 0x00};
    mh_int32_t n2 = 3;
    mh_uint32_t bitnum2 = 0x000001;

    ret = next_byte_equal(p2, n2, bitnum2);
    if (mh_false == ret)
    {
        mh_info("testcase 2: ok");
    }
    else
    {
        mh_error("testcase 2: failed");
    }

}

static void test_semantics()
{
    mh_info("---mh_test_semantics begin---");
    test_next_byte_equal();

    mh_info("---mh_test_semantics end-----");

}


//--------------------------type test----------------------------
#include "mh_type.h"

static void test_cycle_queue()
{
#define QUEUE_SIZE 16

    mh_cycle_queue_t queue;
//    mh_bool_t ret;

    char output[QUEUE_SIZE];
    char input[] = "hello, muhe.";   // 12 bytes
    int input_len = strlen(input);

    char input2[] = "enjoy it.";    // 9 bytes
    int input2_len = strlen(input2);

    int queuesize = QUEUE_SIZE;

    mh_cycle_queue_init(&queue, queuesize);

    // testcase 1:
    // expect:
    {
        memset(output, 0x00, QUEUE_SIZE);
        mh_cycle_queue_write(&queue, input, input_len);
        mh_cycle_queue_read(&queue, output, input_len);

        if (!strncmp(input, output, input_len))
        {
            mh_info("testcase 1: ok");
        }
        else
        {
            mh_error("testcase 1: failed. output: %s | expect: %s", output, input);
        }
    }

    // testcase 2:
    // expect:
    {
        memset(output, 0x00, QUEUE_SIZE);
        mh_cycle_queue_reset(&queue);
        mh_cycle_queue_write(&queue, input, input_len);
        mh_cycle_queue_read(&queue, output, 9);     // output: 'hello, mu'
        char expect_output[] = "hello, mu";
        if (!strncmp(output, expect_output))
        {
            mh_info("testcase 2: ok");
        }
        else
        {
            mh_error("testcase 2: failed. output: %s | expect: %s", output, expect_output);
        }
    }

    // testcase 3: queue backwind
    {
        int readbytes = 9;
        memset(output, 0x00, QUEUE_SIZE);
        mh_cycle_queue_reset(&queue);
        mh_cycle_queue_write(&queue, input, input_len);
        mh_cycle_queue_read(&queue, output, readbytes);     // output: 'hello, mu'

        mh_cycle_queue_write(&queue, input2, input2_len);
        int readbytes2 = input_len - readbytes + input2_len;    // 12 bytes
        mh_cycle_queue_read(&queue, output, readbytes2);

        char expect_output[] = "he.enjoy it.";

        if (!strncmp(output, expect_output))
        {
            mh_info("testcase 3: ok");
        }
        else
        {
            mh_error("testcase 3: failed. output: %s | expect: %s", output, expect_output);
        }
    }

    // testcase 4: over write
    {
        memset(output, 0x00, QUEUE_SIZE);
        mh_cycle_queue_reset(&queue);

        mh_cycle_queue_write(&queue, input, input_len);
        mh_result_t ret = mh_cycle_queue_write(&queue, input, input_len);
        if (ret == MH_ERROR_QUEUE_OVER_WRITE)
        {
            mh_info("testcase 4: ok");
        }
        else
        {
            mh_error("testcase 4: failed. expect over write");
        }
    }

    // testcase 5: over read
    {
        memset(output, 0x00, QUEUE_SIZE);
        mh_cycle_queue_reset(&queue);
        int readbytes = 5;
        mh_result_t ret = mh_cycle_queue_read(&queue, output, readbytes);
        if (ret == MH_ERROR_QUEUE_OVER_READ)
        {
            mh_info("testcase 5: ok");
        }
        else
        {
            mh_error("testcase 5: failed. expect over read");
        }


    }






    mh_cycle_queue_deinit(&queue);


}

static void test_type()
{
    mh_info("---mh_test_type begin---");

    test_cycle_queue();

    mh_info("---mh_test_type end-----");
}

//--------------------------stream test----------------------------
#include "mh_stream.h"
static void test_init_stream_meta()
{
//    mh_stream_meta_p meta = NULL;

//    mh_create_stream_meta(meta, 16);

//    mh_destroy_stream_meta(meta);

//    mh_stream_meta_t meta;
//    mh_init_stream_meta(&meta, 33);

//    mh_info("meta.cap=%d", meta.bufsize);


    mh_stream_meta_p meta = malloc(sizeof(mh_stream_meta_t));

    mh_stream_meta_init(meta, 1024);

    mh_info("meta.cap=%d", meta->buf->capacity);

    mh_stream_meta_deinit(meta);

//    mh_stream_meta_p meta3 = NULL;
//    mh_create_stream_meta(&meta3);
//    mh_init_stream_meta(meta3, 11);
//    mh_info("meta3.cap=%d", meta3->bufsize);

//    mh_stream_meta_p meta4 = NULL;
//    mh_init_stream_meta2(&meta4, 17);
//    mh_info("meta4.cap=%d", meta4->bufsize);

//    mh_stream_meta_p meta5 = NULL;
//    mh_create_stream_meta(&meta5, 9);
//    mh_info("meta5.cap=%d", meta5->bufsize);

//    mh_destroy_stream_meta(&meta5);

}


static void test_stream()
{
    mh_info("---test_stream begin---");
    test_init_stream_meta();

    mh_info("---test_stream end-----");
}


//----------------------------test main-------------------------------
void mh_test_main()
{
//    test_semantics();
//    test_type();
    test_stream();
}
