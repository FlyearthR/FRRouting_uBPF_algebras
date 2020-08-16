#include "algebra.h"
#include "algebra/class_algebra.h"

#include "algebra/binary_lexical_product.h"
#include "algebra/disjunction.h"
#include "algebra/integer_addition.h"
#include "algebra/integer_product.h"
#include "algebra/local_preference.h"
#include "algebra/maximum.h"
#include "algebra/minimum.h"
#include "algebra/nary_lexical_product.h"
#include "algebra/origin_preference.h"
#include "algebra/route_tags.h"
#include "algebra/scoped_product.h"
#include "algebra/sequences.h"
#include "algebra/simple_sequences.h"

#include "../ubpf_tools/include/public.h"


static algebra* a;

static int arg_check(uint64_t ret_val)
{
    if (ret_val == NULL) {
        return 0;
    }
    return 1;
}

uint64_t plug()
{
    int ret = 0;
    bpf_args_t args[] = {{.arg = &ret, .len = sizeof(int), .kind = kind_primitive, .type = 0}};

    uint64_t r = (uint64_t) ADD(0, 64);

    CALL_ALL(1, args, 1, arg_check, 1, {
        a = (algebra*) r;
        RETURN(0);
    },{
         a = (algebra*) VM_RETURN_VALUE;
         RETURN(0);
     })
}

void initialize_algebra()
{
    if (plug())
        a = ADD(0, 64);
}

inline int pref(metric m0, metric m1)
{
    return preference(a, m0, m1);
}

inline metric label_app(metric label, metric metric)
{
    return label_application(a, label, metric);
}

inline metric g_phi()
{
    return get_phi(a);
}

inline int eq_phi(metric m)
{
    return equals_phi(a, m);
}

inline size_t size_pack(metric packet)
{
    return size_packet(a, packet);
}

inline metric parse_pack(metric packet)
{
    return parse_packet(a, packet);
}

inline void write_pack(metric metric, settable_metric packet)
{
    write_packet(a, metric, packet);
}

inline size_t size_string(metric m, int metric)
{
    return size_of_string(a, m, metric);
}

inline char* to_log(metric m, int metric)
{
    return to_string(a, m, metric);
}