#ifndef UBPF_TOOLS_SEQUENCES_H
#define UBPF_TOOLS_SEQUENCES_H

#include <stdlib.h>
#include "class_algebra.h"
#include "seq.h"
#include "cust_malloc.h"

/**
 * create an algebra that maintains growing lists of labels along paths and selects the shortest
 * @param n: maximum value of a label
 * @param m: maximum size of a list
 * @return an algebra
 */
algebra* SEQ(int n, int m);

#endif //UBPF_TOOLS_SEQUENCES_H
