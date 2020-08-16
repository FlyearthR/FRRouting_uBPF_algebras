#ifndef FRR_MINIMUM_H
#define FRR_MINIMUM_H
#undef MIN

#include <stdlib.h>
#include "class_algebra.h"
#include "cust_malloc.h"

/**
 * create an algebra that maintains the minimum label along paths and selects the smallest
 * @return an algebra
 */
algebra* MIN(int n);

#define MIN(x,y) ((x)<=(y)?(x):(y))
#endif // FRR_MINIMUM_H
