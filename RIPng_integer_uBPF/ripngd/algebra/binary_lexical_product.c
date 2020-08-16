#include "binary_lexical_product.h"

/**
 * (σ_A, σ_B) ≺= (β_A, β_B) ⇔ σ_A ≺_A β_A ∨ (σ_A ∼_A β_A ∧ σ_B  ≺=_B β_B )
 * @param args: the sub-algebras
 * @param met1: an array of pointers to signatures of sub-algebras {&σ_A, &σ_B}
 * @param met2: an array of pointers to signatures of sub-algebras {&β_A, &β_B}
 * @return -1 if (σ_A, σ_B) ≺= (β_A, β_B)
 *          0 if (σ_A, σ_B) ~ (β_A, β_B)
 *          1 otherwise
 */
static int prec(algebra* this, metric met0, metric met1)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    int r = preference(a0, met0, met1);

    if (r)
        return r;

    size_t sm0 = size_metric(a0, met0, 1);
    size_t sm1 = size_metric(a1, met1, 1);

    return preference(a1, met0+sm0, met1+sm1);
}

/**
 * (λ_A, λ_B) ⊕ (σ_A, σ_B) = (λ_A ⊕_A σ_A, λ_B ⊕_B σ_B)
 * @param args: the sub-algebras
 * @param lambda: an array of pointers to labels of sub-algebras {&λ_A, &λ_B}
 * @param sigma: an array of pointers to signatures of sub-algebras {&σ_A, &σ_B}
 * @return an array of pointers to new signatures {&(λ_A ⊕_A σ_A), &(λ_B ⊕_B σ_B)}
 */
static metric lapp(algebra* this, metric lambda, metric sigma)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    size_t sl0 = size_metric(a0, lambda, 0);
    size_t ss0 = size_metric(a0, sigma, 1);

    metric m0 = label_application(a0, lambda, sigma);
    metric m1 = label_application(a1, lambda+sl0, sigma+ss0);

    size_t sm0 = size_metric(a0, m0, 1);
    size_t sm1 = size_metric(a1, m1, 1);

    metric r = cust_malloc(sm0 + sm1);
    memcpy((settable_metric)r, m0, sm0);
    memcpy((settable_metric)r+sm0, m1, sm1);

    return r;
}

static int size_of_str(algebra* this, metric m, int metric)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];
    int s0 = size_of_string(a0, m, metric);
    int s1 = size_of_string(a1, m+size_metric(a0, m, metric), metric);
    return s0 + s1 + 3;
}

static char* to_str(algebra* this, metric m, int metric)
{
    int s = size_of_string(this, m, metric);
    char* str = cust_calloc((s+1)*sizeof(char));
    str[0] = '(';
    char vir[] = {',', '\0'};
    char par[] = {')', '\0'};
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];
    strcat(str, to_string(a0, m, metric));
    strcat(str, vir);
    strcat(str, to_string(a1, m+size_metric(a0, m, metric), metric));
    strcat(str, par);
    return str;
}

static size_t s_metric(algebra* this, metric m, int metric)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];
    size_t s0 = size_metric(a0, m, metric);
    size_t s1 = size_metric(a1, m+s0, metric);
    return s0+s1;
}


/**
 * create an binary lexical product of algebra a0 and algebra a1
 * @param a0: an algebra
 * @param a1: another algebra
 * @return an algebra which is the cross product of the two others
 */
algebra* CROSS(algebra* a0, algebra* a1)
{
    algebra* this = constructor_algebra_bbo(a0, a1);

    this->prec = &prec;
    this->lapp = &lapp;
    this->size_of_str = &size_of_str;
    this->to_str = &to_str;
    this->s_metric = &s_metric;

    return this;
}
