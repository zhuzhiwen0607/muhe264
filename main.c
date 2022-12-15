#include <stdio.h>

#include "mh_log.h"
#include "mh_error.h"
#include "mh_test.h"

static void show_usage()
{
    printf("usage:\n");
    printf("exe h264file\n");
}


int main(int argc, const char *argv[])
{

//    mh_test_main();
    return 0;

    if (argc != 2)
        show_usage();

    const char *infile = NULL;
    infile = argv[1];
    if (!infile)
    {
        show_usage();
        return MH_INVALID_PARAM;
    }


    return 0;
}
