#include "sort.h"
#include "../../include/public_bpf.h"

struct arg_sort {
    void *array;
    void *buf;
    size_t low;
    size_t high;
    size_t size;
    int copy_mode;
    int (* compare) (const void *, const void *);
};

static void msort (struct arg_sort* as)
{
    void *array = as->array;
    void *buf = as->buf;
    size_t low = as->low;
    size_t high = as->high;
    size_t size = as->size;
    int copy_mode = as->copy_mode;
    int (* compare) (const void *, const void *) = as->compare;
    char *a1, *al, *am, *ah, *ls, *hs, *lo, *hi, *b;
    size_t copied = 0;
    size_t mid;

    mid = MID (low, high);

    if (mid + 1 < high) {
        struct arg_sort* as = (struct arg_sort*) ctx_malloc(sizeof(struct arg_sort));
        as->array = array;
        as->buf = buf;
        as->low = mid+1;
        as->high = high;
        as->size = size;
        as->copy_mode = copy_mode;
        as->compare = compare;
        msort(as);
        free(as);
    }
        //msort (array, buf, mid + 1, high, size, copy_mode, compare);

    if (mid > low) {
        struct arg_sort* as = (struct arg_sort*) ctx_malloc(sizeof(struct arg_sort));
        as->array = array;
        as->buf = buf;
        as->low = low;
        as->high = mid;
        as->size = size;
        as->copy_mode = copy_mode;
        as->compare = compare;
        msort(as);
        free(as);
    }
        //msort (array, buf, low, mid, size, copy_mode, compare);

    ah = ((char *) array) + ((high + 1) * size);
    am = ((char *) array) + ((mid + 1) * size);
    a1 = al = ((char *) array) + (low * size);

    b = (char *) buf;
    lo = al;
    hi = am;

    do {
        ls = lo;
        hs = hi;

        if (lo > al || hi > am) {
            /* our last loop already compared lo & hi and found lo <= hi */
            lo += size;
        }

        while (lo < am && compare (lo, hi) <= 0)
            lo += size;

        if (lo < am) {
            if (copied == 0) {
                /* avoid copying the leading items */
                a1 = lo;
                ls = lo;
            }

            /* our last compare tells us hi < lo */
            hi += size;

            while (hi < ah && compare (hi, lo) < 0)
                hi += size;

            if (lo > ls) {
                MEMCOPY (b, ls, lo - ls);
                copied += (lo - ls);
                b += (lo - ls);
            }

            MEMCOPY (b, hs, hi - hs);
            copied += (hi - hs);
            b += (hi - hs);
        } else if (copied) {
            MEMCOPY (b, ls, lo - ls);
            copied += (lo - ls);
            b += (lo - ls);

            /* copy everything we needed to re-order back into array */
            MEMCOPY (a1, buf, copied);
            return;
        } else {
            /* everything already in order */
            return;
        }
    } while (hi < ah);

    if (lo < am) {
        MEMCOPY (b, lo, am - lo);
        copied += (am - lo);
    }

    MEMCOPY (a1, buf, copied);
}

int MergeSort (void *base, size_t nmemb, size_t size,
           int (* compare) (const void *, const void *))
{
    int copy_mode;
    void *tmp;

    if (nmemb < 2)
        return 0;

    if (!(tmp = malloc (nmemb * size))) {
        ebpf_print("Malloc failed\n");
        return -1;
    }

    if ((((char *) base) - ((char *) 0)) % sizeof (long) == 0 && (size % sizeof (long)) == 0)
        copy_mode = 1;
    else if ((((char *) base) - ((char *) 0)) % sizeof (int) == 0 && (size % sizeof (int)) == 0)
        copy_mode = 2;
    else
        copy_mode = 0;

    struct arg_sort* as = (struct arg_sort*) ctx_malloc(sizeof(struct arg_sort));
    as->array = base;
    as->buf = tmp;
    as->low = 0;
    as->high = nmemb-1;
    as->size = size;
    as->copy_mode = copy_mode;
    as->compare = compare;
    msort(as);
    free(as);
    //msort (base, tmp, 0, nmemb - 1, size, copy_mode, compare);

    free (tmp);

    return 0;
}