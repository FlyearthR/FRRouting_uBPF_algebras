#ifndef UBPF_TOOLS_MAXIMUM_H
#define UBPF_TOOLS_MAXIMUM_H

#include <stdlib.h>
#include "class_algebra.h"
#include "cust_malloc.h"

/**
 * create an algebra that maintains the maximum label along paths and selects the smallest
 * @return an algebra
 */
algebra* MAX();

#endif //UBPF_TOOLS_MAXIMUM_H
