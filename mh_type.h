#ifndef MH_TYPE_H
#define MH_TYPE_H

// internal type

typedef void            mh_void_t;
typedef void*           mh_void_p;

typedef char            mh_int8_t;
typedef unsigned char   mh_uint8_t;
typedef short           mh_int16_t;
typedef unsigned short  mh_uint16_t;
typedef int             mh_int32_t;
typedef unsigned int    mh_uint32_t;

typedef char            mh_char_t;
typedef unsigned char   mh_uchar_t;

typedef unsigned int    mh_bool_t;
#define mh_true         1
#define mh_false        0

typedef int             mh_result_t;    // enums in mh_error.h




#endif // MH_TYPE_H
