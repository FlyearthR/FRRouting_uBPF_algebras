#include "real_product.h"

#define PHI ((float*) this->args)[0]-1

/**
 * σ_1 ≺= σ_2 ⇔ σ_1 ≤ σ_2
 * @param args not used
 * @param met0: a pointer to the signature of a path
 * @param met1: a pointer to the signature of another path
 * @return -1 if σ_1 < σ_2
 *          0 if σ_1 ~ σ_2
 *          1 otherwise
 */
int prec(__attribute__((unused)) algebra* this, metric met0, metric met1)
{
    float* a = (float*) met0;
    float* b = (float*) met1;
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
    float* l = lambda;
    float* s = sigma;
    *s *= *l;
    if (*s < ((float*) this->args)[0] || *s > ((float*) this->args)[1])
        return NULL;
    float* r = (float*) ctx_malloc(sizeof(float));
    *r = *s;
    return (metric) r;
}

/**
 * create an algebra that multiplies real metrics along paths and selects the smallest
 * @param n: the minimum value of a signature
 * @param m: the maximum value of a signature
 * @return an algebra
 */
algebra* MULTr(float n, float m)
{
    algebra* a = (algebra*) ctx_malloc(sizeof(algebra*) + sizeof(float)*2);
    if (!a)
        return PHI;
    float* tab = a->args[0];
    tab[0] = n;
    tab[1] = m;
    a->prec = &prec;
    a->lapp = &lapp;
    return a;
}