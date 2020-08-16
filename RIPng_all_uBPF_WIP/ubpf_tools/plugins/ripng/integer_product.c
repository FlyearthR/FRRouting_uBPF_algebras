#include "integer_product.h"

/**
 * σ_1 ≺= σ_2 ⇔ σ_1 ≤ σ_2
 * @param args not used
 * @param met0: a pointer to the signature of a path
 * @param met1: a pointer to the signature of another path
 * @return -1 if σ_1 ≺ σ_2
 *          0 if σ_1 ~ σ_2
 *          1 otherwise
 */
int prec(__attribute__((unused)) algebra* this, metric met0, metric met1)
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
 *          φ     if λ * σ ∈/ [m]\[n−1]
 * λ ⊕ σ =
 *          λ + σ otherwise
 * @param args: the bounds
 * @param lambda: a pointer to the label to apply
 * @param sigma: a pointer to the signature on which apply the label
 * @return a pointer to the new signature
 */
metric lapp(algebra* this, metric lambda, metric sigma)
{
    const int* l = lambda;
    const int* s = sigma;
    int rep = *s * *l;
    if (rep > ((int*) this->args)[1])
        rep = ((int*) this->args)[1]+1;
    int* r = (int*) cust_malloc(sizeof(int), this->init, this->last);
    *r = rep;
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
    char* str = cust_calloc(size_of_str(this, m, metric)+1, this->init, this->last);
    snprintf(str, size_of_str(this, m, metric)+1, "%i", *(int*) m);
    return str;
}

algebra* MULT(int n, int m, struct mal* init, struct mal* last)
{
    algebra* a = (algebra*) malloc(sizeof(algebra) + sizeof(int)*2, this->init, this->last);
    if (!a)
        return NULL;
    int* tab = (int*) a->args;
    tab[0] = n;
    tab[1] = m;
    a->prec = &prec;
    a->lapp = &lapp;
    a->size_of_str = &size_of_str;
    a->to_str = &to_str;
    a->s_metric = &size_metric_int;
    a->init = init;
    a->last = last;
    return a;
}
