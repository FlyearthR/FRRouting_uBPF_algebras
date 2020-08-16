#include "sequences.h"

/**
 * σ_1 ≺= σ_2 ⇔ σ_1 = [σ_{1,1}, σ_{1,2}, ..., σ_{1,k1} ] ∧ σ_2 = [σ_{2,1}, σ_{2,2}, ..., σ_{2,k2}] ∧ k1 ≤ k2
 * @param args not used
 * @param met0: a pointer to the signature of a path
 * @param met1: a pointer to the signature of another path
 * @return -1 if σ_1 ≺ σ_2
 *          0 if σ_1 ~ σ_2
 *          1 otherwise
 */
static int prec(__attribute__((unused)) algebra* this, metric met0, metric met1)
{
    const struct seq* a = met0;
    const struct seq* b = met1;
    if (equals_phi(this, met0) && equals_phi(this, met1))
        return 0;
    if (equals_phi(this, met0))
        return 1;
    if (equals_phi(this, met1))
        return -1;
    if (a->size < b->size)
        return -1;
    if (a->size > b->size)
        return 1;
    return 0;
}

/**
 *          φ       if σ = [σ_1, σ_2, ..., σ_k] ∧ k = m
 * λ ⊕ σ =
 *          λ :: σ  otherwise
 * @param args not used
 * @param lambda: a pointer to the label to apply
 * @param sigma: a pointer to the signature on which apply the label
 * @return a pointer to the new signature
 */
static metric lapp(algebra* this, metric lambda, metric sigma)
{
    const int* l = lambda;
    const struct seq* s = sigma;
    if (s->size > ((int*) this->args)[1])
    {
        return get_phi(this);
    }

    struct seq* ret = copy_seq(s);
    append(ret, *l);
    return ret;
}

static int size_of_str(algebra* this, metric m, int metric)
{
    if (metric)
    {
        struct seq* s = (struct seq*) m;
        if (equals_phi(this, m))
        {
            return 3;
        }
        else if (s->size == 0)
        {
            return 5;
        }
        else
        {
            int s = ((int*) this->args)[1];
            int i;
            for (i = 1 ; s/10 > 0 ; i++, s/=10);
            return i * ((struct seq*) m)->size;
        }
    }
    else
    {
        int s = *(int*) m;
        int i;
        for (i = 1 ; s/10 > 0 ; i++, s/=10);
        return i;
    }
}

static char* to_str(algebra* this, metric m, int metric)
{
    char* str;
    struct seq* s = (struct seq*) m;
    if (!metric)
    {
        str = (char*) cust_calloc((size_of_str(this, m, metric)+1)*sizeof(char));
        snprintf(str, size_of_str(this, m, metric), "%i", *(int*) m);
    }
    else if (equals_phi(this, m))
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

static size_t s_metric(__attribute__((unused)) algebra* this, __attribute__((unused)) metric m, int metric)
{
    if (metric)
        return sizeof(struct seq);
    else
        return sizeof(int);
}

static size_t s_packet(algebra* this, metric m)
{
    if (equals_phi(this, m))
        return sizeof(int);
    return s_packet_seq(this, m);
}

static settable_metric w_packet(algebra* this, metric m, settable_metric p)
{
    if (equals_phi(this, m))
    {
        int* pa = (int*) p;
        *pa = -1;
        return (settable_metric) ++pa;
    }
    return w_packet_seq(this, m, p);
}

static metric p_packet(algebra* this, metric packet)
{
    metric m = p_packet_seq(this, packet);
    if (m == NULL)
    {
        return get_phi(this);
    }
    return m;
}

algebra* SEQ(int n, int m)
{
    algebra* this = (algebra*) malloc(sizeof(algebra) + sizeof(int)*2);
    if (!this)
        return NULL;
    constructor_algebra_seq(this);
    int* tab = (int*) this->args;
    tab[0] = n;
    tab[1] = m;
    this->prec = &prec;
    this->lapp = &lapp;
    this->size_of_str = &size_of_str;
    this->to_str = &to_str;
    this->s_metric = &s_metric;
    this->s_packet = &s_packet;
    this->w_packet = &w_packet;
    this->p_packet = &p_packet;
    this->free_al = &free_al_base;

    return this;
}
