#include "seq.h"

void append(struct seq* seq, const int i)
{
    struct node_seq* end = (struct node_seq*) cust_malloc(sizeof(struct node_seq));
    if (seq->size == 0)
    {
        seq->size++;
        seq->tail = end;
        seq->head = end;
        end->i = i;
        end->next = NULL;
    }
    else
    {
        seq->size++;
        seq->tail->next = end;
        seq->tail = end;
        end->i = i;
        end->next = NULL;
    }
}

struct node_seq* copy_node(const struct node_seq* n)
{
    struct node_seq* new = cust_malloc(sizeof(struct node_seq));
    new->next = NULL;
    new->i = n->i;
    return new;
}

struct seq* copy_seq(const struct seq* seq)
{
    struct seq* ret = (struct seq*) cust_calloc(sizeof(struct seq));
    ret->size = seq->size;
    struct node_seq* next = seq->head;
    struct node_seq* new = NULL;
    if (next == NULL)
    {
        ret->head = NULL;
        ret->tail = NULL;
        return ret;
    }
    else
    {
        new = copy_node(next);
        next = next->next;
        ret->head = new;
    }
    while (next != NULL)
    {
        new->next = copy_node(next);
        new = new->next;
        next = next->next;
    }
    ret->tail = new;
    new->next = NULL;
    return ret;
}

struct seq* compare_copy_seq(const struct seq* seq, const int i)
{
    struct seq* ret = (struct seq*) cust_malloc(sizeof(struct seq));
    ret->size = seq->size;
    struct node_seq* next = seq->head;
    struct node_seq* new = NULL;
    if (next == NULL)
    {
        ret->head = NULL;
        ret->tail = NULL;
        return ret;
    }
    else
    {
        if(next->i == i)
            return NULL;
        new = copy_node(next);
        next = next->next;
        ret->head = new;
    }
    while (next != NULL)
    {
        if(next->i == i)
            return NULL;
        new->next = copy_node(next);
        new = new->next;
        next = next->next;
    }
    ret->tail = new;
    new->next = NULL;
    return ret;
}

int in (const int i, const struct seq* seq)
{
    for (struct node_seq* next = seq->head ; next != NULL ; next = next->next)
        if(next->i == i)
            return 1;
    return 0;
}

struct seq* delete_copy_seq(const struct seq* seq, const struct seq* to_del)
{
    struct seq* ret = (struct seq*) cust_calloc(sizeof(struct seq));
    if (seq->size == 0)
        return ret;

    struct node_seq* next;
    for (next = seq->head ; next != NULL && in(next->i, to_del) ; next = next->next); // pass the first if they have to be deleted

    while (next != NULL)
    {
        if(!in(next->i, to_del))
        {
            append(ret, next->i);
        }
        next = next->next;
    }
    return ret;
};

void concatanate(struct seq* seq, const struct seq* end)
{
    struct node_seq* next;
    for (next = end->head ; next != NULL ; append(seq, next->i), next = next->next);
}

/*void set_size(struct seq* seq, int size)
{
    s->size = size;
}

int equalsPHI(struct seq* seq)
{

}*/
