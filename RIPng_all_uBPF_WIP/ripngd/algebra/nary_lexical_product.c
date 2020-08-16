#include "nary_lexical_product.h"

algebra* naryCROSS(int nb, ...)
{
    algebra* ret;
    va_list argumentPointer;
    va_start(argumentPointer, nb);
    if (nb == 1)
        ret = va_arg(argumentPointer, algebra*);
    else
    {
        algebra* a[nb];

        for (int i = 0 ; i < nb ; i++)
            a[i] = va_arg(argumentPointer, algebra*);

        ret = CROSS(a[nb-2], a[nb-1]);

        for (int i = nb-3 ; i >= 0 ; i--)
            ret = CROSS(a[i], ret);
    }

    va_end(argumentPointer);

    return ret;
}
