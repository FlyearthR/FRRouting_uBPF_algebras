#ifndef FRR_ALGEBRA_H
#define FRR_ALGEBRA_H

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

void initialize_algebra();

int pref(metric m0, metric m1);

metric label_app(metric label, metric metric);

metric g_phi();

int eq_phi(metric m);

size_t size_string(metric m, int metric);

char* to_log(metric m, int metric);

size_t size_pack(metric packet);

metric parse_pack(metric packet);

void write_pack(metric metric, settable_metric packet);

#endif //FRR_ALGEBRA_H
