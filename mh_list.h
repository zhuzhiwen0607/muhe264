#ifndef MH_LIST_H
#define MH_LIST_H

#include "mh_type.h"



struct mh_list_s
{
    struct mh_list_s *previous;
    struct mh_list_s *next;
};

typedef struct mh_list_s mh_list_t;
typedef struct mh_list_s* mh_list_p;


#endif // MH_LIST_H
