#include "cust_malloc.h"

struct mal
{
    void* ptr;
    struct mal* next;

    //char str[15];
};

static void __attribute__((always_inline)) init_mal(struct mal** init, struct mal** last)
{
    *init = (struct mal*) ctx_malloc(sizeof(struct mal));
    (*init)->ptr = NULL;
    (*init)->next = NULL;
    *last = *init;

    char str[] = "init";
    //strncpy((*init)->str, str, 14);
}

static void* __attribute__((always_inline)) cust_malloc(size_t s, struct mal* init, struct mal** last)
{
    void* ptr = ctx_malloc(s);
    (*last)->ptr = ptr;
    (*last)->next = (struct mal*) ctx_malloc(sizeof(struct mal));
    *last = (*last)->next;
    (*last)->next = NULL;
    return ptr;
}

#define cust_calloc(s, init, last) \
({ \
uint8_t *ptr8 = (uint8_t *) cust_malloc(s, init, last); \
uint64_t i; \
for (i = 0 ; i < s ; i++) { \
ptr8[i] = '\0'; \
} \
(char *)ptr8;})

static void __attribute__((always_inline)) free_mal(struct mal* cur)
{
    struct mal* cur2;
    while (cur && cur->next)
    {
        cur2 = cur->next;
        ctx_free(cur->ptr);
        ctx_free(cur);
        cur = cur2;
    }
    ctx_free(cur);
}

static __attribute__((always_inline)) void free_all(struct mal* init, struct mal** last)
{
    free_mal(init->next);
    init->next = NULL;
    *last = init;
}

static __attribute__((always_inline)) void close_mal(struct mal** init, struct mal** last)
{
    free_mal(*init);
    *init = NULL;
    *last = NULL;
}