#include "maximum.h"
#undef MAX

/**
 * λ ⊕ σ = max(λ, σ)
 * @param args not used
 * @param lambda: a pointer to the label to apply
 * @param sigma: a pointer to the signature on which apply the label
 * @return a pointer to the new signature
 */
static metric lapp(__attribute__((unused)) algebra* this, metric lambda, metric sigma)
{
    int* l = (int*) lambda;
    int* s = (int*) sigma;
    int* r = (int*) cust_malloc(sizeof(int));
    if (*l < *s)
        *r = *s;
    else
        *r = *l;
    return r;
}

algebra* MAX(int n)
{
    algebra* this = constructor_algebra_maj(n);
    this->lapp = &lapp;

    return this;
}

#define MAX(x,y) ((x)<=(y)?(y):(x))
