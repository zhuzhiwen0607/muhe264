#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "mh_log.h"



#define mh_log_core(buf, file, line, func, format) \
    do \
    { \
        snprintf(buf, sizeof(buf), "%s:%d %s $ ", file, line, func); \
        int offset = strlen(buf); \
        va_list arglist; \
        va_start(arglist, format); \
        vsnprintf(buf + offset, sizeof(buf) - offset, format, arglist); \
        va_end(arglist); \
    } while (0);




void mh_log_info(int target, const char *file, const int line, const char *func, const char *format, ...)
{
    char buf[MAX_LOG_LEN] = {0};

    mh_log_core(buf, file, line, func, format);

    /*
    do
    {
        snprintf(buf, sizeof(buf), "%s:%d %s# ", file, line, func);
        int offset = strlen(buf);
        va_list arglist;
        va_start(arglist, format);
        vsnprintf(buf + offset, sizeof(buf) - offset, format, arglist);
        va_end(arglist);
    } while (0);
    */

    if (target == TARGET_CONSOLE)
        printf("INFO %s\n", buf);
    else
        // todo
        ;
}

void mh_log_error(int target, const char *file, const int line, const char *func, const char *format, ...)
{
    char buf[MAX_LOG_LEN] = {0};

//    va_list arglist;
//    va_start(arglist, format);
//    vsnprintf(buf, sizeof(buf), format, arglist);
//    va_end(arglist);
    mh_log_core(buf, file, line, func, format);


    if (target == TARGET_CONSOLE)
    {
        printf("ERROR %s\n", buf);
//        fflush()
    }
    else
        // todo
        ;
}

