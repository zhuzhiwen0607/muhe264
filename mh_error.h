#ifndef MH_ERROR_H
#define MH_ERROR_H


#define MH_OK                         0
#define MH_ERROR                      -1

// common
#define MH_COMMON_BEGIN               -100
#define MH_INVALID_PARAM              MH_COMMON_BEGIN - 0
#define MH_INPUT_PTR_NULL             MH_COMMON_BEGIN - 1


#define MH_COMMON_END                 MH_COMMON_BEGIN - 99


// mm
#define MH_MM_BEGIN                   -200
#define MH_MM_MALLOC_ERROR            MH_MM_BASE - 0

#define MH_MM_END                     MH_MM_MALLOC_ERROR - 99

// queue
#define MH_QUEUE_BEGIN                -300

#define MH_QUEUE_OVER_BOUND           MH_QUEUE_BEGIN - 0

//#define MH_ERROR_QUEUE_OVER_WRITE     MH_QUEUE_BEGIN - 0
//#define MH_ERROR_QUEUE_OVER_READ      MH_QUEUE_BEGIN - 1
//#define MH_ERROR_QUEUE_CAP_NOT_ENOUGH MH_QUEUE_BEGIN - 2


#define MH_QUEUE_END                  MH_QUEUE_BEGIN - 99

// array
#define MH_ARRAY_BEGIN                -400

#define MH_ARRAY_END                  MH_ARRAY_BEGIN - 99

#endif // MH_ERROR_H
