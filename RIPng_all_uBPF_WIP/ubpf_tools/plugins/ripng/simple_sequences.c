#include "simple_sequences.h"

/**
 * σ_1 ≺= σ_2 ⇔ σ_1 = [σ_{1,1}, σ_{1,2}, ..., σ_{1,k1} ] ∧ σ_2 = [σ_{2,1}, σ_{2,2}, ..., σ_{2,k2}] ∧ k1 ≤ k2
 * @param args not used
 * @param met0: a pointer to the signature of a path
 * @param met1: a pointer to the signature of another path
 * @return -1 if σ_1 < σ_2
 *          0 if σ_1 ~ σ_2
 *          1 otherwise
 */
static __attribute__((always_inline)) int prec(__attribute__((unused)) algebra* this, metric met0, metric met1)
{
    const struct seq* a = met0;
    const struct seq* b = met1;
    if (equalsPHI(a) && equalsPHI(b))
        return 0;
    if (equalsPHI(a))
        return 1;
    if (equalsPHI(b))
        return -1;
    if (a->size < b->size)
        return -1;
    if (a->size > b->size)
        return 1;
    return 0;
}

/**
 *          φ       if (σ = [σ_1, σ_2, ..., σ_k] ∧ k = m) ∨ λ ∈ σ
 * λ ⊕ σ =
 *          λ :: σ  otherwise
 * @param args not used
 * @param lambda: a pointer to the label to apply
 * @param sigma: a pointer to the signature on which apply the label
 * @return a pointer to the new signature
 */
static __attribute__((always_inline)) metric lapp(algebra* this, metric lambda, metric sigma)
{
    const int* l = lambda;
    const struct seq* s = sigma;
    if (s->size > ((int*) this->args)[1])
    {
        returnPHI;
    }

    struct seq* ret = compare_copy_seq(s, *l);
    if (ret == NULL)
    {
        returnPHI;
    }
    append(ret, *l);
    return ret;
}

static __attribute__((always_inline)) int size_of_str(algebra* this, metric m, int metric)
{
    if (metric)
    {
        struct seq* s = (struct seq*) m;
        return equalsPHI(s) ? 3 : s->size == 0 ? 5 : ((((int*) this->args)[1])/10 + 1) * s->size;
    }
    else
    {
        int s = *(int*) m;
        int i;
        for (i = 1 ; s/10 > 0 ; i++, s/=10);
        return i;
    }
}

static __attribute__((always_inline)) char* to_str(algebra* this, metric m, int metric)
{
    char* str;
    struct seq* s = (struct seq*) m;
    if (!metric)
    {
        str = (char*) cust_calloc(size_of_str(this, m, metric)*sizeof(char));
        snprintf(str, size_of_str(this, m, metric)+1, "%i", *(int*) m);
    }
    else if (equalsPHI(s))
    {
        str = (char*) cust_calloc(4*sizeof(char));
        char empty[6] = "PHI";
        strncpy(str, empty, 4);
    }
    else if (s->size == 0)
    {
        str = (char*) cust_calloc(6*sizeof(char));
        char empty[6] = "empty";
        strncpy(str, empty, 6);
    }
    else
    {
        int size = size_of_str(this, m, metric);
        str = (char*) cust_calloc(size * s->size * sizeof(char));
        struct node_seq* cur = s->head;
        while (cur != NULL)
        {
            char t[size+1];
            snprintf(t, size, "%i", cur->i);
            strncat(str, t, size);
            cur = cur->next;
            if (cur != NULL)
                strcat(str, ":");
        }
    }
    return str;
}

static __attribute__((always_inline)) size_t s_metric(__attribute__((unused)) algebra* this, __attribute__((unused)) metric m, int metric)
{
    if (metric)
        return sizeof(struct seq);
    else
        return sizeof(int);
}

algebra* SIMSEQ(int n, int m)
{
    algebra* a = (algebra*) malloc(sizeof(algebra) + sizeof(int)*2);
    if (!a)
        return NULL;
    int* tab = (int*) a->args;
    tab[0] = n;
    tab[1] = m;
    a->prec = &prec;
    a->lapp = &lapp;
    a->size_of_str = &size_of_str;
    a->to_str = &to_str;
    a->s_metric = &s_metric;
    return a;
}
