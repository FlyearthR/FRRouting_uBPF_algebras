#ifndef FRR_TEST_H
#define FRR_TEST_H

#include <stdarg.h>
#include "integer_addition.h"
#include "integer_product.h"
#include "minimum.h"
#include "maximum.h"
#include "local_preference.h"
#include "origin_preference.h"
#include "sequences.h"
#include "simple_sequences.h"
#include "route_tags.h"
#include "binary_lexical_product.h"
#include "nary_lexical_product.h"
#include "scoped_product.h"
#include "disjunction.h"
#include "cust_malloc.h"

struct link;

struct node
{
    int index;
    struct link* l_pref;
    metric m_pref;
    struct link* links[];
};

struct link
{
    struct node* n[2];
    metric label;
};

struct fifo
{
    int i;
    struct fifo* next;
};

#endif //FRR_TEST_H
