#ifndef RIPNG_SEQ_H
#define RIPNG_SEQ_H

#include <stdlib.h>
#include "cust_malloc.h"

struct node_seq
{
    struct node_seq* next;
    int i;
};

struct seq
{
    int size;
    struct node_seq* head;
    struct node_seq* tail;
};

struct seq* copy_seq(const struct seq* seq);
void append(struct seq* seq, const int i);
struct seq* compare_copy_seq(const struct seq* seq, const int i);
struct seq* delete_copy_seq(const struct seq* seq, const struct seq* to_del);
void concatanate(struct seq* seq, const struct seq* end);

#endif //RIPNG_SEQ_H
