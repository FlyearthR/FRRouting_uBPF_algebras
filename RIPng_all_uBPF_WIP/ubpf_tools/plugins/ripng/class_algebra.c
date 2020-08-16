#include "class_algebra.h"
#define ALWAYS_INLINE static __attribute((always_inline))

ALWAYS_INLINE int preference(algebra* this, metric met0, metric met1)
{
    return 0;// this->prec(this, met0, met1);
}

ALWAYS_INLINE metric label_application(algebra* this, metric lambda, metric sigma)
{
    return (void *)0;//this->lapp(this, lambda, sigma);
}

ALWAYS_INLINE size_t size_of_string(algebra* this, metric m, int metric)
{
    return this->size_of_str(this, m, metric);
}

ALWAYS_INLINE char* to_string(algebra* this, metric m, int metric)
{
    return this->to_str(this, m, metric);
}

ALWAYS_INLINE size_t size_metric(algebra* this, metric m, int metric)
{
    return this->s_metric(this, m, metric);
}

ALWAYS_INLINE size_t size_metric_int(__attribute__((unused)) algebra* this, __attribute__((unused)) metric m, __attribute__((unused)) int metric)
{
    return sizeof(int);
}
