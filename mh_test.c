
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
        mh_result ret = mh_cycle_queue_write(&queue, input, input_len);
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
        mh_result ret = mh_cycle_queue_read(&queue, output, readbytes);
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


void mh_test_main()
{
//    test_semantics();
    test_type();
}
