#include "cust_malloc.h"


struct mal* init;
static struct mal* last;

struct mal
{
    void* ptr;
    struct mal* next;
};

void init_mal()
{
    init = (struct mal*) malloc(sizeof(struct mal));
    init->ptr = NULL;
    init->next = NULL;
    last = init;
}

void* cust_malloc(size_t s)
{
    void* ptr = malloc(s);
    last->ptr = ptr;
    last->next = (struct mal*) calloc(1, sizeof(struct mal));
    last = last->next;
    return ptr;
}

void* cust_calloc(size_t s)
{
    void* ptr = cust_malloc(s);
    memset(ptr, '\0', s);
    return ptr;
}

void free_mal(struct mal* cur)
{
    struct mal* cur2;
    while (cur != NULL && cur->next != NULL)
    {
        cur2 = cur->next;
        free(cur->ptr);
        free(cur);
        cur = cur2;
    }
    free(cur);
}

void free_all()
{
    free(init->ptr);
    free_mal(init->next);
    init->next = NULL;
    init->ptr = NULL;
    last = init;
}

void close_mal()
{
    free_mal(init);
    init = NULL;
    last = NULL;
}



