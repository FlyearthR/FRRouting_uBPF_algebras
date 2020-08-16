#include "minimum.h"

/**
 * σ_1 ≺= σ_2 ⇔ σ_1 ≤ σ_2
 * @param args not used
 * @param met0: a pointer to the signature of a path
 * @param met1: a pointer to the signature of another path
 * @return -1 if σ_1 ≺ σ_2
 *          0 if σ_1 ~ σ_2
 *          1 otherwise
 */
static __attribute__((always_inline)) int prec(__attribute__((unused)) algebra* this, metric met0, metric met1)
{
    const int* a = met0;
    const int* b = met1;
    if (*a < *b)
        return -1;
    if (*a > *b)
        return 1;
    return 0;
}

/**
 * λ ⊕ σ = min(λ, σ)
 * @param args not used
 * @param lambda: a pointer to the label to apply
 * @param sigma: a pointer to the signature on which apply the label
 * @return a pointer to the new signature
 */
static __attribute__((always_inline)) metric lapp(__attribute__((unused)) algebra* this, metric lambda, metric sigma)
{
    int* l = (int*) lambda;
    int* s = (int*) sigma;
    int* r = (int*) cust_malloc(sizeof(int));
    if (*l > *s)
        *r = *s;
    else
        *r = *l;
    return r;
}

static __attribute__((always_inline)) int size_of_str(algebra* this, metric m, __attribute__((unused)) int metric)
{
    int s = *(int*) m;
    int i;
    for (i = 1 ; s/10 > 0 ; i++, s/=10);
    return i;
}

static __attribute__((always_inline)) char* to_str(algebra* this, metric m, int metric)
{
    char* str = cust_calloc(size_of_str(this, m, metric)+1);
    snprintf(str, size_of_str(this, m, metric)+1, "%i", *(int*) m);
    return str;
}

algebra* MIN()
{
    algebra* a = (algebra*) malloc(sizeof(algebra));
    if (!a)
        return NULL;
    a->prec = &prec;
    a->lapp = &lapp;
    a->size_of_str = &size_of_str;
    a->to_str = &to_str;
    a->s_metric = &size_metric_int;
    return a;
}
