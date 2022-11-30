#ifndef MH_ERROR_H
#define MH_ERROR_H


#define MH_OK                         0
#define MH_ERROR                      -1

// common error
#define MH_ERROR_COMMON_BASE          -100
#define MH_ERROR_INVALID_PARAM        MH_ERROR_COMMON_BASE - 0


#define MH_ERROR_COMMON_END           MH_ERROR_COMMON_BASE - 99

// mh_type error
#define MH_ERROR_TYPE_BASE            -200
#define MH_ERROR_QUEUE_OVER_WRITE     MH_ERROR_TYPE_BASE - 0
#define MH_ERROR_QUEUE_OVER_READ      MH_ERROR_TYPE_BASE - 1
#define MH_ERROR_QUEUE_CAP_NOT_ENOUGH MH_ERROR_TYPE_BASE - 2



#define MH_ERROR_TYPE_END             MH_ERROR_TYPE_BASE - 99

#endif // MH_ERROR_H
