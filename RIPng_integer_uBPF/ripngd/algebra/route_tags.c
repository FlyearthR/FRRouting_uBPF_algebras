#include "route_tags.h"

/**
 * σ_1 ~ σ_2
 * @param args not used
 * @param met0: a pointer to the signature of a path
 * @param met1: a pointer to the signature of another path
 * @return 0
 */
static int prec(__attribute__((unused)) algebra* this, __attribute__((unused)) metric met0, __attribute__((unused)) metric met1)
{
    struct seq* s1 = (struct seq*) met0;
    struct seq* s2 = (struct seq*) met1;
    if (s1->size > s2->size)
        return -1;
    if (s1->size < s2->size)
        return 1;
    return 0;
}

/**
 ∗ i(λ) ⊕ σ = σ ∪ λ
 ∗ d(λ) ⊕ σ = σ \ λ
 ∗ κ ⊕ σ = σ
 * @param args not used
 * @param lambda: a pointer to the label to apply
 * @param sigma: a pointer to the signature on which apply the label
 * @return a pointer to the new signature
 */
static metric lapp(__attribute__((unused)) algebra* this, metric lambda, metric sigma)
{
    const struct change_tags* l = lambda;
    const struct seq* s = sigma;
    struct seq* ret;
    switch (l->action)
    {
    case insert:
        ret = delete_copy_seq(s, &l->tags);
        concatanate(ret, &l->tags);
        break;
    case delete:
        ret = delete_copy_seq(s, &l->tags);
        break;
    default:
        ret = copy_seq(s);
        break;
    }
    return ret;
}

static int size_of_str(algebra* this, metric m, int metric)
{
    if (metric)
    {
        struct seq* seq = (struct seq*) m;
        if (seq->size == 0)
        {
            return 5;
        }
        else
        {
            int s = ((int*) this->args)[0];
            int i;
            for (i = 1 ; s/10 > 0 ; i++, s/=10);
            return (i+1) * seq->size;
        }
    }
    else
    {
        struct change_tags* ct = (struct change_tags*) m;
        if (ct->action == pass)
        {
            return 1;
        }
        else
        {
            int s = ((int*) this->args)[0];
            int i;
            for (i = 1 ; s/10 > 0 ; i++, s/=10);
            return i * ct->tags.size + 3;
        }
    }
}

static char* to_str(algebra* this, metric m, int metric)
{
    char* str;
    if (!metric)
    {
        struct change_tags* ct = (struct change_tags*) m;
        switch (ct->action)
        {
        case pass:
            str = cust_calloc(2*sizeof(char));
            str[0] = 'k';
            return str;
            break;
        case insert:
            str = cust_calloc(size_of_str(this, m, metric));
            str[0] = 'i';
            break;
        case delete:
            str = cust_calloc(size_of_str(this, m, metric));
            str[0] = 'd';
            break;
        }
        strcat(str, "(");
        struct node_seq* cur = ct->tags.head;
        while (cur != NULL)
        {
            int size = size_of_str(this, m, metric);
            char t[size+1];
            for (int i = 0 ; i < size+1 ; t[i] = '\0', i++);
            snprintf(t, size+1, "%i", cur->i);
            strncat(str, t, size);
            cur = cur->next;
            if (cur != NULL)
                strcat(str, ":");
            else
                strcat(str, ")");
        }
        return str;
    }
    else
    {
        struct seq* s = (struct seq*) m;
        if (s->size == 0)
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
                for (int i = 0 ; i < size+1 ; t[i] = '\0', i++);
                snprintf(t, size+1, "%i", cur->i);
                strncat(str, t, size);
                cur = cur->next;
                if (cur != NULL)
                    strcat(str, ":");
            }
        }
    }
    return str;
}

static size_t s_metric(__attribute__((unused)) algebra* this, __attribute__((unused)) metric m, int metric)
{
    if (metric)
        return sizeof(struct seq);
    else
        return sizeof(struct change_tags);
}

algebra* TAGS(int n)
{
    algebra* this = (algebra*) malloc(sizeof(algebra) + sizeof(int));
    if (!this)
        return NULL;
    constructor_algebra_seq(this);
    int* tab = (int*) this->args;
    tab[0] = n;
    this->prec = &prec;
    this->lapp = &lapp;
    this->size_of_str = &size_of_str;
    this->to_str = &to_str;
    this->s_metric = &s_metric;
    this->free_al = &free_al_base;

    return this;
}
