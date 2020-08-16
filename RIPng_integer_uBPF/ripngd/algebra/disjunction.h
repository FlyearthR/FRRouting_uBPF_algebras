#ifndef DISJUNCTION_H_INCLUDED
#define DISJUNCTION_H_INCLUDED

#include "class_algebra.h"

struct disjunct
{
    uint8_t a; //0: algebra0, 1: algebra1, 2: label is RI, 3: metric is PHI
    metric m;
};

algebra* DISJOINT(algebra* a0, algebra* a1, metric (*map)(metric metric));

#endif // DISJUNCTION_H_INCLUDED
