#ifndef UBPF_TOOLS_ROUTE_TAGS_H
#define UBPF_TOOLS_ROUTE_TAGS_H

#include <stdlib.h>
#include "class_algebra.h"
#include "cust_malloc.h"
#include "seq.h"

enum {insert, delete, pass};

struct change_tags
{
    int action;
    struct seq tags;
};



/**
 * create an algebra the minimum label along paths and selects the smallest
 * @return an algebra
 */
algebra* TAGS(int n);

#endif //UBPF_TOOLS_ROUTE_TAGS_H
