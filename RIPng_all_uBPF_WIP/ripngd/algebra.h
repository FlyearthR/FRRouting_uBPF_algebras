#ifndef FRR_ALGEBRA_H
#define FRR_ALGEBRA_H

#include "algebra/class_algebra.h"
#include "algebra/integer_addition.h"

void initialize_algebra();

int pref(metric m0, metric m1);

metric label_app(metric label, metric metric);

metric g_phi();

int eq_phi(metric m);

size_t size_string(metric m, int metric);

char* to_log(metric m, int metric);

size_t size_pack(metric packet);

metric parse_pack(metric packet);

void write_pack(settable_metric packet, metric metric);

#endif //FRR_ALGEBRA_H
