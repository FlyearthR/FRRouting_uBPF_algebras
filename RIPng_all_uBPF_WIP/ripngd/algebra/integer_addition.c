#include "integer_addition.h"

/**
 * σ_1 ≺= σ_2 ⇔ σ_1 ≤ σ_2
 * @param args not used
 * @param met0: a pointer to the signature of a path
 * @param met1: a pointer to the signature of another path
 * @return -1 if σ_1 < σ_2
 *          0 if σ_1 ~ σ_2
 *          1 otherwise
 */
static int prec(__attribute__((unused)) algebra* this, metric met0, metric met1)
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
    algebra* this = (algebra*) malloc(sizeof(algebra) + sizeof(int)*2);
    if (!this)
        return NULL;
    constructor_algebra_int(this);
    int* tab = (int*) this->args;
    tab[0] = n;
    tab[1] = m;
    this->prec = &prec;
    this->lapp = &lapp;
    return this;
}
