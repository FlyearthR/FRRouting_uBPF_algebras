#ifndef UBPF_TOOLS_ORIGIN_PREFERENCE_H
#define UBPF_TOOLS_ORIGIN_PREFERENCE_H

#include <stdlib.h>
#include "class_algebra.h"
#include "cust_malloc.h"

/**
 * create an algebra that maintains the label of the origin the smallest
 * @return an algebra
 */
algebra* OP(int n);

#endif //UBPF_TOOLS_ORIGIN_PREFERENCE_H
