#include <assert.h>
#include "mh_list.h"
#include "mh_mm.h"
#include "mh_type.h"
#include "mh_error.h"
#include "mh_log.h"

mh_result_t mh_list_new(mh_list_p *list)
{
    mh_result_t ret = mh_malloc(list, sizeof(mh_list_t));
    if (MH_OK != ret)
        return ret;

    (*list)->prev = NULL;
    (*list)->next = NULL;

    return MH_OK;
}

mh_result_t mh_list_destroy(mh_list_p *list)
{
    assert(*list);

    if ((*list)->prev || (*list)->next)
        mh_error("list->previous or list->next is not NULL, current list maybe broken!");

    mh_free(list);

    return MH_OK;
}

mh_result_t mh_list_push_back(mh_list_p list, mh_list_p x)
{
    assert(list);

    mh_list_p p = list;

    while (!tail(p))
    {
        p = next(p);
    }

    p->next = x;
    x->prev = p;
    x->next = NULL;

    return MH_OK;
}

mh_result_t mh_list_push_front(mh_list_p list, mh_list_p x)
{
    assert(list);

    mh_list_p p = list;

    while (!header(p))
    {
        p = previous(p);
    }

    x->next = p;
    p->prev = x;
    x->prev = NULL;

    return MH_OK;
}

mh_result_t mh_list_insert(mh_list_p list, mh_list_p x, mh_int32_t i)
{

}

mh_result_t mh_list_delete(mh_list_p list, mh_int32_t i)
{

}

mh_list_p mh_list_pop_back(mh_list_p list)
{

}

mh_list_p mh_list_pop_front(mh_list_p list)
{

}

mh_list_p mh_list_back(mh_list_p list)
{

}

mh_list_p mh_list_front(mh_list_p list)
{

}

mh_list_p mh_list_search(mh_list_p list, mh_int32_t i)
{

}
