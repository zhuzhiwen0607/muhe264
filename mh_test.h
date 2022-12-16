#ifndef MH_TEST_H
#define MH_TEST_H

#include "mh_type.h"
#include "mh_log.h"

void mh_test_main();


static inline mh_bool_t SHOULD_EQUAL_INT32(mh_int32_t a, mh_int32_t b)
{
    if (a == b)
    {
        mh_info("result: ok");
        return mh_true;
    }
    else
    {
        mh_error("result: error");
        return mh_false;
    }
}

#endif // MH_TEST_H
