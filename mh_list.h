#ifndef MH_LIST_H
#define MH_LIST_H

#include "mh_type.h"



struct mh_list_s
{
    struct mh_list_s *prev;
    struct mh_list_s *next;
};

typedef struct mh_list_s mh_list_t;
typedef struct mh_list_s* mh_list_p;


mh_result_t mh_list_new(mh_list_p list);
mh_result_t mh_list_destroy(mh_list_p list);

mh_result_t mh_list_push_back(mh_list_p list, mh_list_p x);
mh_result_t mh_list_push_front(mh_list_p list, mh_list_p x);
mh_result_t mh_list_insert(mh_list_p list, mh_list_p x, mh_int32_t i);
mh_result_t mh_list_delete(mh_list_p list, mh_int32_t i);
mh_list_p mh_list_pop_back(mh_list_p list);
mh_list_p mh_list_pop_front(mh_list_p list);
mh_list_p mh_list_back(mh_list_p list);
mh_list_p mh_list_front(mh_list_p list);
mh_list_p mh_list_search(mh_list_p list, mh_int32_t i);

static inline mh_bool_t header(mh_list_p list);
static inline mh_bool_t tail(mh_list_p list);
static inline mh_list_p previous(mh_list_p list);
static inline mh_list_p next(mh_list_p list);

static inline mh_bool_t header(mh_list_p list)
{
    assert(list);
    if (!list->prev)
        return mh_true;

    return mh_false;
}

static inline mh_bool_t tail(mh_list_p list)
{
    assert(list);
    if (!list->next)
        return mh_true;

    return mh_false;
}

static inline mh_list_p previous(mh_list_p list)
{
    assert(list);
    return list->prev;
}

static inline mh_list_p next(mh_list_p list)
{
    assert(list);
    return list->next;
}




#endif // MH_LIST_H
