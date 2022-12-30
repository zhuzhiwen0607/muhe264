#if 0
#include <string.h>

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

    ret = next_bytes_equal(p1, n1, bitnum);
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

    ret = next_bytes_equal(p2, n2, bitnum2);
    if (mh_false == ret)
    {
        mh_info("testcase 2: ok");
    }
    else
    {
        mh_error("testcase 2: failed");
    }

}


static void test_read_bits()
{
    mh_info("test_read_bits begin...");


    mh_array_t a = {0};
    a.base = malloc(4);
    a.bits_start = a.base;
    a.bits_offset = 0;
    a.size = 4;
//    a.capacity = 4;

    // 0xFD 45 21 09
    a.base[0] = 0x09;
    a.base[1] = 0x21;
    a.base[2] = 0x45;
    a.base[3] = 0xFD;

    read_bits(&a, 7);    // output bin(0001001) DEC(9)
    read_bits(&a, 3);    // output bin(010) DEC(2)


//    read_bits(&a, 9);     // output bin(100001001)  265

}

static void test_next_bits()
{
    mh_info("test_next_bits begin...");


    mh_array_t a = {0};
    a.base = malloc(4);
    a.bits_start = a.base;
    a.bits_size = 4 * 8;
    a.bits_offset = 0;
    a.size = 4;

    // 0xFD 45 21 09
    a.base[0] = 0x09;
    a.base[1] = 0x21;
    a.base[2] = 0x45;
    a.base[3] = 0xFD;

    next_bits(&a, 9);


}

static void test_semantics()
{
    mh_info("---mh_test_semantics begin---");
//    test_next_byte_equal();
//    test_read_bits();
    test_next_bits();

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
        if (!strcmp(output, expect_output))
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

        if (!strcmp(output, expect_output))
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
    test_semantics();
//    test_type();
//    test_stream();
}
#endif

#include "mh_test.h"
#include "mh_type.h"
#include "mh_log.h"
#include "mh_array.h"
#include "mh_queue.h"
#include "mh_list.h"

static void queue_test();

static void queue_test2()
{
    mh_queue_p q = NULL;
    mh_int32_t cap = 5;
    mh_queue_new(&q, cap);

    mh_uint8_t a[8] = {0};
    a[0] = 0x0A;
    a[1] = 0x00;
    a[2] = 0x03;
    a[3] = 0x04;
    a[4] = 0x10;
    a[5] = 0x11;
    a[6] = 0x12;
    a[7] = 0x13;

    mh_queue_write(q, a, 8);
}

static void queue_test()
{
    mh_queue_p q = NULL;
    mh_int32_t cap = 16;
    mh_queue_new(&q, cap);

    mh_int32_t space = mh_queue_space(q);
    SHOULD_EQUAL_INT(space, cap);

    mh_uint8_t a[8] = {0};
    a[0] = 0x0A;
    a[1] = 0x00;
    a[2] = 0x03;
    a[3] = 0x04;
    mh_queue_write(q, a, 8);    // 0A 00 03 04 00 00 00 00
    SHOULD_EQUAL_INT(q->start[0], 0x0A);
    SHOULD_EQUAL_INT(q->start[7], 0x00);

    space = mh_queue_space(q);
    SHOULD_EQUAL_INT(space, cap - 8);

    mh_uint8_t b[3] = {0};
    b[0] = 0x10;
    b[1] = 0x11;
    b[2] = 0x12;
    mh_queue_write(q, b, 3);    // 0A 00 03 04 00 00 00 00 10 11 12
    SHOULD_EQUAL_INT(q->start[10], 0x12);

    mh_uint8_t c[7] = {0};
    c[4] = 0x20;
    c[5] = 0x21;
    mh_queue_write(q, c, 7);    // 0A 00 03 04 00 00 00 00 10 11 12 00 00 00 00 20 | 21 00 00
    SHOULD_EQUAL_INT(q->start[15], 0x20);
    SHOULD_NOT_EQUAL_INT(q->start[16], 0x21);

    mh_uint8_t d[4] = {0};
    mh_queue_read(q, d, 4);     //  0A 00 03 04 -> d[]
                                //  _ _ _ _ 00 00 00 00 10 11 12 00 00 00 00 20

    SHOULD_EQUAL_INT(d[0], 0x0A);
    SHOULD_EQUAL_INT(d[1], 0x00);
    SHOULD_EQUAL_INT(d[2], 0x03);
    SHOULD_EQUAL_INT(d[3], 0x04);

    mh_uint8_t e[2] = {0};
    e[0] = 0x30;
    e[1] = 0x31;
    mh_queue_write(q, e, 2);    // 30 31 _ _ 00 00 00 00 10 11 12 00 00 00 00 20
    SHOULD_EQUAL_INT(q->start[4], 0x10);
    SHOULD_EQUAL_INT(q->base[0], 0x30);
    SHOULD_EQUAL_INT(q->base[1], 0x31);


    mh_queue_destroy(&q);
}

static void queue_empty_test()
{
    mh_queue_p q = NULL;
    mh_int32_t cap = 4;
    mh_queue_new(&q, cap);

    if (queue_empty(q))
    {
        mh_info("ok");
    }
    else
    {
        mh_error("failed");
    }

    mh_queue_destroy(&q);
}

static void queue_full_test()
{
    mh_queue_p q = NULL;
    mh_int32_t cap = 4;
    mh_queue_new(&q, cap);

    mh_uint8_t a[4];
    a[0] = 0x04;
    a[1] = 0x03;
    a[2] = 0x02;
    a[3] = 0x01;
    mh_queue_write(q, a, 4);

    if (queue_full(q))
    {
        mh_info("ok");
    }
    else
    {
        mh_error("failed");
    }

    mh_queue_destroy(&q);
}

static void queue_at_test()
{
    mh_queue_p q = NULL;
    mh_int32_t cap = 4;
    mh_queue_new(&q, cap);

    mh_uint8_t a[4];
    a[0] = 0x04;
    a[1] = 0x03;
    a[2] = 0x02;
    a[3] = 0x01;
    mh_queue_write(q, a, 4);

    // testcase1
    mh_uint8_t b0 = queue_at(q, 0);
    mh_uint8_t b1 = queue_at(q, 1);
    mh_uint8_t b2 = queue_at(q, 2);
    mh_uint8_t b3 = queue_at(q, 3);
    if (b0 == 0x04 && b1 == 0x03 && b2 == 0x02 && b3 == 0x01)
    {
        mh_info("ok");
    }
    else
    {
        mh_error("failed: %x %x %x %x", b0, b1, b2, b3);
    }

    mh_queue_destroy(&q);
}

static void queue_write_test()
{
    mh_queue_p q = NULL;
    mh_int32_t cap = 4;
    mh_queue_new(&q, cap);

    mh_uint8_t a[2];
    a[0] = 0x10;
    a[1] = 0x11;

    mh_uint8_t b[1];
    b[0] = 0x20;

    mh_uint8_t c[2];
    c[0] = 0x30;
    c[1] = 0x31;

    // testcase 1
    mh_queue_write(q, a, sizeof(a) / sizeof(mh_uint8_t));
    mh_queue_write(q, b, sizeof(b) / sizeof(mh_uint8_t));
    mh_queue_write(q, c, sizeof(c) / sizeof(mh_uint8_t));

    mh_uint8_t x0 = q->base[0];
    mh_uint8_t x1 = q->base[1];
    mh_uint8_t x2 = q->base[2];
    mh_uint8_t x3 = q->base[3];
    if (x0 == a[0] && x1 == a[1] && x2 == b[0] && x3 == c[0])
    {
        mh_info("ok");
    }
    else
    {
        mh_error("failed");
    }

    mh_queue_destroy(&q);

}

static void queue_write_test2()
{
    mh_queue_p q = NULL;
    mh_int32_t cap = 4;
    mh_queue_new(&q, cap);

    mh_uint8_t a[2];
    a[0] = 0x10;
    a[1] = 0x11;

    q->start = q->base;
    q->end = &(q->base[3]);
    mh_queue_write(q, a, sizeof(a) / sizeof(mh_uint8_t));
    if (q->base[0] == 0x00 && q->base[1] == 0x00 && q->base[2] == 0x00 && q->base[3] == a[0] && q->base[4] == 0x00)
    {
        mh_info("ok");
    }
    else
    {
        mh_error("failed: %x %x %x %x", q->base[0], q->base[1], q->base[2], q->base[3]);
    }

    mh_queue_destroy(&q);
}

static void queue_write_test3()
{
    mh_queue_p q = NULL;
    mh_int32_t cap = 4;
    mh_queue_new(&q, cap);

    mh_uint8_t a[6];
    a[0] = 0x10;
    a[1] = 0x11;
    a[2] = 0x12;
    a[3] = 0x13;
    a[4] = 0x14;
    a[5] = 0x15;

    q->start = &(q->base[2]);
    q->end = &(q->base[3]);
    mh_queue_write(q, a, sizeof(a) / sizeof(mh_uint8_t));
    if (q->base[3] == a[0] && q->base[4] == a[1] && q->base[0] == a[2] && q->base[1] == 0x00 && q->base[2] == 0x00)
    {
        if (q->end == &(q->base[1]))
        {
            mh_info("ok");
        }
        else
        {
            mh_error("failed: q->end is error");
        }
    }
    else
    {
        mh_error("failed: q->base(%x %x %x %x %x) is error",
                 q->base[0], q->base[1], q->base[2], q->base[3], q->base[4]);
    }

    mh_queue_destroy(&q);
}

static void queue_read_test()
{
    mh_queue_p q = NULL;
    mh_int32_t cap = 4;
    mh_queue_new(&q, cap);

    mh_uint8_t a[4];
    a[0] = 0x10;
    a[1] = 0x11;
    a[2] = 0x12;
    a[3] = 0x13;

    mh_queue_write(q, a, sizeof(a) / sizeof(mh_uint8_t));

    mh_uint8_t r[4];
    memset(r, 0x00, sizeof(r) / sizeof(mh_uint8_t));

    mh_queue_read(q, r, 4);
    if (r[0] == a[0] && r[1] == a[1] && r[2] == a[2] && r[3] == a[3])
    {
        mh_info("ok");
    }
    else
    {
        mh_error("failed");
    }

    mh_queue_destroy(&q);
}

static void queue_read_test2()
{
    mh_queue_p q = NULL;
    mh_int32_t cap = 4;
    mh_queue_new(&q, cap);

    mh_uint8_t a[4];
    a[0] = 0x10;
    a[1] = 0x11;
    a[2] = 0x12;
    a[3] = 0x13;

    q->start = &(q->base[3]);
    q->end = &(q->base[2]);

    q->base[3] = a[0];
    q->base[4] = a[1];
    q->base[0] = a[2];
    q->base[1] = a[3];

    mh_uint8_t r[4];
    memset(r, 0x00, sizeof(r) / sizeof(mh_uint8_t));
    mh_queue_read(q, r, 4);

    if (r[0] == a[0] && r[1] == a[1] && r[2] == a[2] && r[3] == a[3])
    {
        mh_info("ok");
    }
    else
    {
        mh_error("failed: %x %x %x %x", r[0], r[1], r[2], r[3]);
    }

    mh_queue_destroy(&q);

}

void test_main()
{

    queue_empty_test();
    queue_full_test();
    queue_at_test();
    queue_write_test();
    queue_write_test2();
    queue_write_test3();
    queue_read_test();
    queue_read_test2();

}
