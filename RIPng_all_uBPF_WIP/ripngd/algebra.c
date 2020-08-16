#include "algebra.h"
#include "integer_addition.h"

static algebra* a;

void initialize_algebra()
{
    a = ADD(0, 64);
}

inline int pref(metric m0, metric m1)
{
    preference(a, m0, m1);
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
    write_pack(a, metric, packet);
}

inline size_t size_string(metric m, int metric)
{
    return size_of_string(a, m, metric);
}

inline char* to_log(metric m, int metric)
{
    return to_string(a, m, metric);
}