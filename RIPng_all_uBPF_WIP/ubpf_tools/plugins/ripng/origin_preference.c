#include "origin_preference.h"

/**
 * σ_1 ≺= σ_2 ⇔ σ_1 ≤ σ_2
 * @param args not used
 * @param met0: a pointer to the signature of a path
 * @param met1: a pointer to the signature of another path
 * @return -1 if σ_1 < σ_2
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
 * λ ⊕ σ = σ
 * @param args not used
 * @param lambda: a pointer to the label to apply
 * @param sigma: a pointer to the signature on which apply the label
 * @return a pointer to the new signature
 */
static __attribute__((always_inline)) metric lapp(__attribute__((unused)) algebra* this, __attribute__((unused)) metric lambda, metric sigma)
{
    const int* s = sigma;
    static __attribute__((always_inline)) int I = 0;
    I++;
    char str[15];
    sprintf(str, "lo:%i:%i+%i", I, *(int*)lambda, *(int*)sigma);
    int* r = (int*) cust_malloc(sizeof(int));
    *r = *s;
    return r;
}

static __attribute__((always_inline)) int size_of_str(algebra* this, metric m, int metric)
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

algebra* OP(int n)
{
    algebra* a = (algebra*) malloc(sizeof(algebra)+sizeof(int));
    if (!a)
        return NULL;
    int* tab = (int*) a->args;
    tab[0] = n;
    a->prec = &prec;
    a->lapp = &lapp;
    a->size_of_str = &size_of_str;
    a->to_str = &to_str;
    a->s_metric = &size_metric_int;
    return a;
}
