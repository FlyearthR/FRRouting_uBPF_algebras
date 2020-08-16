#ifndef SCOPED_PRODUCT_H_INCLUDED
#define SCOPED_PRODUCT_H_INCLUDED

#include "class_algebra.h"

struct zone_label
{
    int new_zone;
    metric label;
};

algebra* SCOPE(algebra* a0, algebra* a1);

#endif // SCOPED_PRODUCT_H_INCLUDED
