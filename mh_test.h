#ifndef MH_TEST_H
#define MH_TEST_H

#include <stdio.h>
#include <string.h>
#include "mh_type.h"
#include "mh_log.h"

void mh_test_main();

#define SHOULD_EQUAL_INT(a, b) \
    do { \
        if (a == b) { \
            mh_info("ok"); \
        } else { \
            mh_error("error"); \
        } \
    } while (0);


static inline mh_void_t SHOULD_EQUAL_INT32(mh_int32_t a, mh_int32_t b, mh_char_t *text)
{
    mh_int32_t len = strlen(text);
    mh_char_t *buffer = calloc(1, len + 16);

    memcpy(buffer, text, len);

    if (a == b)
    {
//        mh_info("ok");
//        strcat(buffer, "ok");
    }
    else
    {
//        mh_error("error");
//        strcat(buffer, "error");
    }


}

static inline mh_bool_t SHOULD_EQUAL_UINT8(mh_uint8_t a, mh_uint8_t b)
{
    if (a == b)
    {
        mh_info("ok");
        return mh_true;
    }
    else
    {
        mh_error("error");
        return mh_false;
    }
}

#endif // MH_TEST_H
