#ifndef MH_LOG_H
#define MH_LOG_H

#define MAX_LOG_LEN 1024

#define TARGET_CONSOLE 0
#define TARGET_LOGFILE 1

void mh_log_info(int target, const char *file, const int line, const char *func, const char *format, ...);
void mh_log_error(int target, const char *file, const int line, const char *func, const char *format, ...);

#define mh_info(format, ...) \
    { \
        mh_log_info(TARGET_CONSOLE, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__); \
    }

#define mh_error(format, ...) \
    { \
        mh_log_error(TARGET_CONSOLE, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__); \
    }


//#define test_info(format, ...) \
//    { \
//        mh_log_info(TARGET_CONSOLE, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__); \
//    }

//#define test_error(format, ...) \
//    { \
//        mh_log_error(TARGET_CONSOLE, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__); \
//    }

//#define mh_info(format, ...) mh_log_info(TARGET_CONSOLE, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__);
//#define mh_error(format, ...) mh_log_error(TARGET_CONSOLE, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__);


#endif // MH_LOG_H
