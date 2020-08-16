#ifndef UBPF_TOOLS_LOCAL_PREFERENCE_H
#define UBPF_TOOLS_LOCAL_PREFERENCE_H

#include "class_algebra.h"
#include "cust_malloc.h"

/**
 * create an algebra that prefers the neighbour with the smallest label
 * @return an algebra
 */
algebra* LP(int n);

#endif //UBPF_TOOLS_LOCAL_PREFERENCE_H
