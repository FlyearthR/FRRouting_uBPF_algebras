#ifndef RIPNG_SORT_H
#define RIPNG_SORT_H

/*  Inspired from https://jeffreystedfast.blogspot.com/2011/04/optimizing-merge-sort.html */

#include <stdlib.h>

#define MID(lo, hi) (lo + ((hi - lo) >> 1))

#define COPYBY(TYPE, a, b, n) {         \
    long __n = (n) / sizeof (TYPE);     \
    register TYPE *__a = (TYPE *) (a);  \
    register TYPE *__b = (TYPE *) (b);  \
                                        \
    do {                                \
        *__a++ = *__b++;                \
    } while (--__n > 0);                \
}

#define MEMCOPY(dest, src, n) {                 \
    switch (copy_mode) {                        \
    case 1: COPYBY (long, dest, src, n); break; \
    case 2: COPYBY (int, dest, src, n); break;  \
    default: ebpf_ebpf_memcpy (dest, src, n);             \
    }                                           \
}

int MergeSort (void *base, size_t nmemb, size_t size,
               int (* compare) (const void *, const void *));

#endif //RIPNG_SORT_H
