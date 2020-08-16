#include "seq.h"

void append(struct seq* seq, int i)
{
    struct node* end = (struct node*) ctx_malloc(sizeof(struct node));
    seq->size++;
    seq->tail->next = end;
    seq->tail = end;
}

struct node* copy_node(struct node* n)
{
    struct node* new = ctx_malloc(sizeof(struct node));
    new->i = n->i;
    return new;
}

struct seq* copy_seq(struct seq* seq)
{
    struct seq* ret = (struct seq*) ctx_malloc(sizeof(struct seq));
    ret->size = seq->size;
    struct node* next = seq->head;
    struct node* new = NULL;
    if (next == NULL) {
        ret->head = NULL;
        ret->tail = NULL;
        return ret;
    } else {
        new = copy_node(next);
        next = next->next;
        ret->head = new;
    }
    while (next != NULL) {
        new->next = copy_node(next);
        new = new->next;
        next = next->next;
    }
    ret->tail = new;
    new->next = NULL;
    return ret;
}

struct seq* compare_copy_seq(struct seq* seq, int i)
{
    struct seq* ret = (struct seq*) ctx_malloc(sizeof(struct seq));
    ret->size = seq->size;
    struct node* next = seq->head;
    struct node* new = NULL;
    if (next == NULL) {
        ret->head = NULL;
        ret->tail = NULL;
        return ret;
    } else {
        if(next->i == i) return NULL;
        new = copy_node(next);
        next = next->next;
        ret->head = new;
    }
    while (next != NULL) {
        new->next = copy_node(next);
        new = new->next;
        next = next->next;
        if(next->i == i) return NULL;
    }
    ret->tail = new;
    new->next = NULL;
    return ret;
}

/*void set_size(struct seq* seq, int size)
{
    s->size = size;
}

int equalsPHI(struct seq* seq)
{

}*/