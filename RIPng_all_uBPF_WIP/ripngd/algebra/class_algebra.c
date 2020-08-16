#include "class_algebra.h"

inline int preference(algebra* this, metric met0, metric met1)
{
    return this->prec(this, met0, met1);
}

inline metric label_application(algebra* this, metric lambda, metric sigma)
{
    return this->lapp(this, lambda, sigma);
}

inline int equals_phi(algebra* this, metric m)
{
    return this->equals_p(this, m);
}

inline metric get_phi(algebra* this)
{
    return this->get_p(this);
}

inline size_t size_phi(algebra* this)
{
    return this->size_p(this);
}

inline size_t size_of_string(algebra* this, metric m, int metric)
{
    return this->size_of_str(this, m, metric);
}

inline char* to_string(algebra* this, metric m, int metric)
{
    return this->to_str(this, m, metric);
}

inline size_t size_metric(algebra* this, metric m, int metric)
{
    return this->s_metric(this, m, metric);
}

inline size_t size_packet(algebra* this, metric m)
{
    return this->s_packet(this, m);
}

inline settable_metric write_packet(algebra* this, metric m, settable_metric p)
{
    return this->w_packet(this, m, p);
}

inline metric parse_packet(algebra* this, metric packet)
{
    return this->p_packet(this, packet);
}

inline void free_algebra(algebra* this)
{
    this->free_al(this);
}

/**********************************************************************************************************************/

void free_al_base(algebra* this)
{
    free(this);
}

/**********************************************************************************************************************/

void constructor_algebra_int(algebra* this)
{
    this->size_p = &size_p_int;
    this->size_of_str = &size_of_str_int;
    this->to_str = &to_str_int;
    this->s_metric = &s_metric_int;
    this->s_packet = &s_packet_int;
    this->w_packet = &w_packet_int;
    this->p_packet = &p_packet_int;
    this->free_al = &free_al_base;
}

size_t size_p_int(__attribute__((unused)) algebra* this)
{
    return sizeof(int);
}

int size_of_str_int(__attribute__((unused)) algebra* this, metric m, __attribute__((unused)) int metric)
{
    int s = *(int*) m;
    int i;
    for (i = 1 ; s/10 > 0 ; i++, s/=10);
    return i;
}

char* to_str_int(__attribute__((unused))algebra* this, metric m, int metric)
{
    char* str = cust_calloc(size_of_str_int(this, m, metric)+1);
    snprintf(str, size_of_str_int(this, m, metric)+1, "%i", *(int*) m);
    return str;
}

size_t s_metric_int(__attribute__((unused)) algebra* this, __attribute__((unused)) metric m, __attribute__((unused)) int metric)
{
    return sizeof(int);
}

size_t s_packet_int(__attribute__((unused)) algebra* this, __attribute__((unused)) metric m)
{
    return sizeof(int);
}

settable_metric w_packet_int(__attribute__((unused)) algebra* this, metric m, settable_metric p)
{
    int* to_write = (int*) p;
    *to_write = *(int*) m;
    return ++to_write;
}

metric p_packet_int(__attribute__((unused)) algebra* this, metric packet)
{
    int* ret = (int*) cust_malloc(sizeof(int));
    *ret = *(int*) packet;
    return ret;
}

/**********************************************************************************************************************/

void constructor_algebra_seq(algebra* this)
{
    this->equals_p = &equals_p_seq;
    this->get_p = &get_p_seq;
    this->size_p = &size_p_seq;
}

int equals_p_seq(algebra* this, metric m)
{
    struct seq* seq = (struct seq*) m;
    return seq->size == ((int*) this->args)[1]+1;
}

metric get_p_seq(algebra* this)
{
    struct seq* PHI = cust_calloc(sizeof(struct seq));
    PHI->size = ((int*) this->args)[1]+1;
    return PHI;
}

size_t size_p_seq(__attribute__((unused)) algebra* this)
{
    return sizeof(struct seq);
}

size_t s_packet_seq(__attribute__((unused)) algebra* this, metric m)
{
    struct seq* s = (struct seq*) m;
    return (s->size+1)*sizeof(int);
}

settable_metric w_packet_seq(__attribute__((unused)) algebra* this, metric m, settable_metric p)
{
    struct seq* s = (struct seq*) m;
    struct node_seq* cur = s->head;
    int* c = (int*) p;
    *c = s->size;
    while(cur != NULL)
    {
        c++;
        *c = cur->i;
        cur = cur->next;
    }
    return c;
}

metric p_packet_seq(__attribute__((unused)) algebra* this, metric packet)
{
    int s = *(int*) packet;
    if (s < 0)
        return NULL;
    int* p = (int*) packet;
    struct seq* seq = (struct seq*) cust_calloc (sizeof(struct seq));
    for (int i = 0 ; i < s ; i++, p++)
        append(seq, *p);
    return (metric) seq;
}

/**********************************************************************************************************************/

void constructor_algebra_bop(algebra* this, algebra* a0, algebra* a1)
{
    algebra** tab = (algebra**) this->args;
    tab[0] = a0;
    tab[1] = a1;
    this->free_al = &free_al_bop;
}

void free_al_bop(algebra* this)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    free_algebra(a0);
    free_algebra(a1);
    free(this);
}

/**********************************************************************************************************************/

algebra* constructor_algebra_bbo(algebra* a0, algebra* a1)
{
    algebra* this = (algebra*) malloc(sizeof(algebra) + sizeof(algebra*)*2);
    constructor_algebra_bop(this, a0, a1);
    this->s_packet = &s_packet_bbo;
    this->w_packet = &w_packet_bbo;
    this->p_packet = &p_packet_bbo;

    return this;
}

int equals_p_bbo(algebra* this, metric m)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    if (!equals_phi(a0, m))
        return 0;

    size_t sm0 = size_metric(a0, m, 1);
    return equals_phi(a1, ((uint8_t) m)+sm0);
}

metric get_p_bbo(algebra* this)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    size_t sp0 = size_phi(a0);
    size_t sp1 = size_phi(a1);
    settable_metric phi = cust_malloc(sp0 + sp1);

    memcpy(phi, get_phi(a0), sp0);
    memcpy(((uint8_t) phi)+sp0, get_phi(a1), sp1);

    return (metric) phi;
}

size_t size_p_bbo(algebra* this)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    size_t sp0 = size_phi(a0);
    size_t sp1 = size_phi(a1);

    return sp0+sp1;
}

size_t s_packet_bbo(algebra* this, metric m)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    size_t sm0 = size_metric(a0, m, 1);
    return size_packet(a0, m) + size_packet(a1, ((uint8_t) m)+sm0);

}

settable_metric w_packet_bbo(algebra* this, metric m, settable_metric p)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    settable_metric p1 = write_packet(a0, m, p);
    size_t sm0 = size_metric(a0, m, 1);
    return write_packet(a1, ((uint8_t) m)+sm0, p1);
}

metric p_packet_bbo(algebra* this, metric packet)
{
    algebra** tab = (algebra**) this->args;
    algebra* a0 = tab[0];
    algebra* a1 = tab[1];

    size_t sp0 = size_packet(a0, packet);
    metric m0 = parse_packet(a0, packet);
    metric m1 = parse_packet(a1, ((uint8_t) packet)+sp0);

    size_t sm0 = size_metric(a0, m0, 1);
    size_t sm1 = size_metric(a1, m1, 1);

    settable_metric m = cust_malloc(sm0+sm1);
    memcpy(m, m0, sm0);
    memcpy(((uint8_t) m)+sm0, m1, sm1);

    return (metric) m;
}
