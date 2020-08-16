#ifndef RIPNG_SEQ_H
#define RIPNG_SEQ_H

struct node {
    struct node* next;
    int i;
};

struct seq {
    int size;
    struct node* head;
    struct node* tail;
};

#define returnPHI       struct seq* PHI = ctx_malloc(sizeof(struct seq)); \
                        PHI->size = ((int*) this->args)[1]+1
#define equalsPHI(x)    x->size == ((int*) this->args)[1]+1

struct seq* copy_seq(struct seq* seq);
void append(struct seq* seq, int i);
struct seq* compare_copy_seq(struct seq* seq, int i);

#endif //RIPNG_SEQ_H
