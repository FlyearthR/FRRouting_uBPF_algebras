#include "disjunction.h"

/**
 * ∀ σ_1, σ_2 ∈ Σ : σ_1 ≺= σ_2 ⇔ (σ_1 ∈ Σ_A ∧ σ_2 ∈ Σ_B) ∨ (∃i ∈ {A, B}|σ_1, σ_2 ∈ Σ_i ∧ σ_1 ≺=_i σ_2 )
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

    struct disjunct* m0 = (struct disjunct*) met0;
    struct disjunct* m1 = (struct disjunct*) met1;

    int r = m0->a - m1->a; //the first algebra is preferred and the PHI is the least preferred
    if(r)
        return r < 0 ? -1 : 1;
    if(m0->a == 3) //both are PHI
        return 0;
    if(m0->a == 0)
        return preference(a0, m0->m, m1->m);
    return preference(a1, m0->m, m1->m);
}

/**
 *                                                λ ⊕_i σ  if i == j
 * ∀i, j ∈ {A, B} : ∀λ ∈ L_i, ∀σ ∈ Σ_j : λ ⊕ σ =
 *                                                φ        otherwise
 * RI ⊕ σ_A = t(σ_A)
 * RI ⊕ σ_B = φ
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
    metric (*map)(metric metric) = (metric) tab[2];

    struct disjunct* l = (struct disjunct*) lambda;
    struct disjunct* s = (struct disjunct*) sigma;
    struct disjunct* r = (struct disjunct*) cust_malloc(sizeof(struct disjunct));

    if(l->a == s->a)
    {
        if(l->a == 0)
            r->m = label_application(a0, l->m, s->m);
        else
            r->m = label_application(a1, l->m, s->m);
        r->a = l->a;
    }
    else if (s->a + l->a == 1) //metric and label are from different algebras
        r->a = 3; //return PHI
    else if (s->a == 3) //sigma == PHI
    {
        r->a = 3;
    }
    else //lambda = RI
    {
        if(s->a == 0)
        {
            if(tab[2] == NULL)
                r->a = 3;
            else
            {
                r->a = 1;
                r->m = map(s->m);
            }
        }
        else
            r->a = 3; //return PHI
    }

    return r;
}

static int equals_p(__attribute__((unused)) algebra* this, metric m)
{
    struct disjunct* d = (struct disjunct*) m;
    return d->a == 3;
}

static metric get_p(algebra* this)
{
    struct disjunct* d = (struct disjunct*) cust_calloc(sizeof(struct disjunct));
    d->a = 3;
    return (metric) d;
}

static size_t size_p(algebra* this)
{
    return sizeof(struct disjunct);
}

static int size_of_str(algebra* this, metric m, int metric)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    struct disjunct* me = (struct disjunct*) m;

    if(me->a == 0)
        return size_of_string(a0, me->m, metric)+2;
    if(me->a == 1)
        return size_of_string(a1, me->m, metric)+2;
    else
        return me->a;
}

static char* to_str(algebra* this, metric m, int metric)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    int s = size_of_string(this, m, metric);
    char* str = cust_calloc((s+1)*sizeof(char));

    struct disjunct* me = (struct disjunct*) m;

    if(me->a == 0)
    {
        str[0] = '0';
        str[1] = '-';
        strcat(str, to_string(a0, me->m, metric));
    }
    else if(me->a == 1)
    {
        str[0] = '1';
        str[1] = '-';
        strcat(str, to_string(a1, me->m, metric));
    }
    else if (me->a == 2)
        strcpy(str, "RI");
    else
        strcpy(str, "PHI");

    return str;
}

static size_t s_metric(algebra* this, metric m, int metric)
{
    return sizeof(struct disjunct);

}


/**
 * create an binary lexical product of algebra a0 and algebra a1
 * @param a0: an algebra
 * @param a1: another algebra
 * @return an algebra which is the disjuction of the two others
 */
algebra* DISJOINT(algebra* a0, algebra* a1, metric (*map)(metric metric))
{
    algebra* this = (algebra*) malloc(sizeof(algebra) + sizeof(algebra*)*2 + sizeof(map));
    if (!this)
        return NULL;
    algebra** tab = (algebra**) this->args;
    tab[0] = a0;
    tab[1] = a1;
    memcpy(&tab[2], &map, sizeof(metric));
    this->prec = &prec;
    this->lapp = &lapp;
    this->equals_p = &equals_p;
    this->get_p = &get_p;
    this->size_p = &size_p;
    this->size_of_str = &size_of_str;
    this->to_str = &to_str;
    this->s_metric = &s_metric;
    this->free_al = &free_al_bop;
    return this;
}
