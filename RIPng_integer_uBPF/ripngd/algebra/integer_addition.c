#include "integer_addition.h"

/**
 *          φ     if λ + σ ∈/ [m]\[n−1]
 * λ ⊕ σ =
 *          λ + σ otherwise
 * @param args: the bounds
 * @param lambda: a pointer to the label to apply
 * @param sigma: a pointer to the signature on which apply the label
 * @return a pointer to the new signature
 */
static metric lapp(algebra* this, metric lambda, metric sigma)
{
    const int* l = lambda;
    const int* s = sigma;
    int rep = *s + *l;
    if (rep > ((int*) this->args)[1])
        rep = ((int*) this->args)[1]+1;
    int* r = (int*) cust_malloc(sizeof(int));
    *r = rep;
    return r;
}

algebra* ADD(int n, int m)
{
    algebra* this = constructor_algebra_bdd(n, m);
    this->lapp = &lapp;

    return this;
}
