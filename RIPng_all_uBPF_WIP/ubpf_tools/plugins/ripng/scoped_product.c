#include "scoped_product.h"

/**
 * (σ_A, σ_B) ≺= (β_A, β_B) ⇔ σ_A ≺_A β_A ∨ (σ_A ∼_A β_A ∧ σ_B  ≺=_B β_B )
 * @param args: the sub-algebras
 * @param met1: an array of pointers to signatures of sub-algebras {&σ_A, &σ_B}
 * @param met2: an array of pointers to signatures of sub-algebras {&β_A, &β_B}
 * @return -1 if (σ_A, σ_B) ≺= (β_A, β_B)
 *          0 if (σ_A, σ_B) ~ (β_A, β_B)
 *          1 otherwise
 */
static __attribute__((always_inline)) int prec(algebra* this, metric met0, metric met1)
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
 * (λ_A, σ̂_B) ⊕ (σ_A, σ_B) = (λ_A ⊕_A σ_A, σ̂_B)         between two zones
 *       λ_B  ⊕ (σ_A, σ_B) = (σ_A        , λ_B ⊕_B σ_B) inside a zone
 * @param args: the sub-algebras
 * @param lambda: an array of pointers to labels of sub-algebras {&λ_A, &λ_B}
 * @param sigma: an array of pointers to signatures of sub-algebras {&σ_A, &σ_B}
 * @return an array of pointers to new signatures {&(λ_A ⊕_A σ_A), &(λ_B ⊕_B σ_B)}
 */
static __attribute__((always_inline)) metric lapp(algebra* this, metric lambda, metric sigma)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    struct zone_label* l = (struct zone_label*) lambda;
    metric m0, m1;
    if (l->new_zone)
    {
        m0 = label_application(a0, l->label, sigma);
        m1 = l->label + size_metric(a0, l->label, 0);
    }
    else
    {
        size_t sm = size_metric(a0, sigma, 1);
        m0 = sigma;
        m1 = label_application(a1, l->label, sigma+sm);
    }
    //TODO free m0
    size_t sm0 = size_metric(a0, m0, 1);
    size_t sm1 = size_metric(a1, m1, 1);
    metric r = cust_malloc(sm0+sm1);
    ebpf_memcpy((settable_metric)r, m0, sm0);
    ebpf_memcpy((settable_metric)r+sm0, m1, sm1);

    return r;
}

static __attribute__((always_inline)) int size_of_str(algebra* this, metric m, int metric)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];
    int s0, s1;
    if (metric)
    {
        s0 = size_of_string(a0, m, metric);
        s1 = size_of_string(a1, m+size_metric(a0, m, metric), metric);
    }
    else
    {
        struct zone_label* l = (struct zone_label*) m;
        if (l->new_zone)
        {
            s0 = size_metric(a0, l->label, 0);
            s1 = size_metric(a1, l->label+s0, 1);
        }
        else
        {
            s0 = 1;
            s1 = size_metric(a1, l->label, 0);
        }
    }

    return s0 + s1 + 3;
}

static __attribute__((always_inline)) char* to_str(algebra* this, metric m, int metric)
{
    int s = size_of_string(this, m, metric);
    char* str = cust_calloc((s+1)*sizeof(char));
    str[0] = '(';
    char vir[] = {',', '\0'};
    char par[] = {')', '\0'};
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];
    char* str0;
    char* str1;

    if (metric)
    {
        str0 = to_string(a0, m, metric);
        str1 = to_string(a1, m+size_metric(a0, m, metric), metric);
    }
    else
    {
        struct zone_label* l = (struct zone_label*) m;
        if (l->new_zone)
        {
            size_t s0 = size_metric(a0, l->label, 0);
            str0 = to_string(a0, l->label, 0);
            str1 = to_string(a1, l->label+s0, 1);
        }
        else
        {
            str0 = ".";
            str1 = to_string(a1, l->label, 0);
        }
    }

    strcat(str, str0);
    strcat(str, vir);
    strcat(str, str1);
    strcat(str, par);

    return str;
}

static __attribute__((always_inline)) size_t s_metric(algebra* this, metric m, int metric)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];
    if (metric)
    {
        size_t s0 = size_metric(a0, m, metric);
        size_t s1 = size_metric(a1, m+s0, metric);
        return s0+s1;
    }
    else
    {
        return sizeof(struct zone_label);
    }
}


/**
 * create an binary lexical product of algebra a0 and algebra a1
 * @param a0: an algebra
 * @param a1: another algebra
 * @return an algebra which is the cross product of the two others
 */
algebra* SCOPE(algebra* a0, algebra* a1)
{
    algebra* a = (algebra*) malloc(sizeof(algebra) + sizeof(algebra*)*2);
    if (!a)
        return NULL;
    algebra** tab = (algebra**) a->args;
    tab[0] = a0;
    tab[1] = a1;
    a->prec = &prec;
    a->lapp = &lapp;
    a->size_of_str = &size_of_str;
    a->to_str = &to_str;
    a->s_metric = &s_metric;
    return a;
}
