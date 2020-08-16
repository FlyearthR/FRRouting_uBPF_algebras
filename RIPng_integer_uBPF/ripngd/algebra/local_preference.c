#include "local_preference.h"

/**
 * λ ⊕ σ = λ
 * @param args not used
 * @param lambda: a pointer to the label to apply
 * @param sigma: a pointer to the signature on which apply the label
 * @return a pointer to the new signature
 */
static metric lapp(__attribute__((unused)) algebra* this, __attribute__((unused)) metric lambda, metric sigma)
{
    const int* s = lambda;
    int* r = (int*) cust_malloc(sizeof(int));
    *r = *s;
    return r;
}

algebra* LP(int n)
{
    algebra* this = constructor_algebra_maj(n);
    this->lapp = &lapp;

    return this;
}
