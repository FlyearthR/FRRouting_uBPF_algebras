#ifndef UBPF_TOOLS_INTEGER_PRODUCT_H
#define UBPF_TOOLS_INTEGER_PRODUCT_H

#include <stdlib.h>
#include "class_algebra.h"
#include "cust_malloc.h"

/**
 * create an algebra that multiplies metrics along paths and selects the smallest
 * @param n: the minimum value of a signature
 * @param m: the maximum value of a signature
 * @return an algebra
 */
algebra* MULT(int n, int m);

#endif //UBPF_TOOLS_INTEGER_PRODUCT_H
