#ifndef UBPF_TOOLS_SIMPLE_SEQUENCES_H
#define UBPF_TOOLS_SIMPLE_SEQUENCES_H

#include <stdlib.h>
#include "class_algebra.h"
#include "seq.h"
#include "cust_malloc.h"

/**
 * create an algebra that maintains growing lists of label along paths and selects the shortest
 * @return an algebra
 */
algebra* SIMSEQ(int n, int m);

#endif //UBPF_TOOLS_SIMPLE_SEQUENCES_H
