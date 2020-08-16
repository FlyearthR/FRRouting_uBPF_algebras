#ifndef FRR_CLASS_ALGEBRA_H
#define FRR_CLASS_ALGEBRA_H

#include <string.h>
#include <stdint.h>
#include "cust_malloc.h"
#include "seq.h"


typedef struct algebra algebra;
typedef const void* metric;
typedef void* settable_metric;

/**
 * prec: the preference relation
 * lapp: the label application function
 */
struct algebra
{
    int (*prec)(algebra* this, metric met0, metric met1);
    metric (*lapp)(algebra* this, metric lambda, metric sigma);
    int (*equals_p)(algebra* this, metric m);
    metric (*get_p)(algebra* this);
    size_t (*size_p)(algebra* this);
    char* (*to_str)(algebra* this, metric m, int metric);
    int (*size_of_str)(algebra* this, metric m, int metric);
    size_t (*s_metric)(algebra* this, metric m, int metric);
    size_t (*s_packet)(algebra* this, metric m);
    settable_metric (*w_packet)(algebra* this, metric m, settable_metric p);
    metric (*p_packet)(algebra* this, metric packet);
    void (*free_al)(algebra* this);
    void* args[];
};

int preference(algebra* this, metric met0, metric met1);

metric label_application(algebra* this, metric lambda, metric sigma);

int equals_phi(algebra* this, metric m);

metric get_phi(algebra* this);

size_t size_phi(algebra* this);

size_t size_of_string(algebra* this, metric m, int metric);

char* to_string(algebra* this, metric m, int metric);

size_t size_metric(algebra* this, metric m, int metric);

size_t size_packet(algebra* this, metric packet);

settable_metric write_packet(algebra* this, metric m, settable_metric p);

metric parse_packet(algebra* this, metric packet);

void free_algebra(algebra* this);

/**
 * Here we define functions of the pseudo-class BaseAlgebra, implementing the pseudo-interface Algebra.
 */

void free_al_base(algebra* this);

/**
 * Here we define functions of the pseudo-class IntegerAlgebra, inheriting from the pseudo-class BaseAlgebra.
 */

void constructor_algebra_int(algebra* this);

int prec_int(__attribute__((unused)) algebra* this, metric met0, metric met1);

size_t size_p_int(__attribute__((unused)) algebra* this);

int size_of_str_int(__attribute__((unused)) algebra* this, metric m, __attribute__((unused)) int metric);

char* to_str_int(__attribute__((unused))algebra* this, metric m, int metric);

size_t s_metric_int(__attribute__((unused)) algebra* this, __attribute__((unused)) metric m, __attribute__((unused)) int metric);

size_t s_packet_int(__attribute__((unused)) algebra* this, __attribute__((unused)) metric m);

settable_metric w_packet_int(__attribute__((unused)) algebra* this, metric m, settable_metric p);

metric p_packet_int(__attribute__((unused)) algebra* this, metric packet);

/**
 * Here we define functions of the pseudo-class BoundedAlgebra, inheriting from the pseudo-class IntegerAlgebra.
 */

algebra* constructor_algebra_bdd(int n, int m);

int equals_p_bdd(algebra* this, metric m);

metric get_p_bdd(algebra* this);

/**
 * Here we define functions of the pseudo-class MajorisedAlgebra, inheriting from the pseudo-class IntegerAlgebra.
 */

algebra* constructor_algebra_maj(int n);

int equals_p_maj(algebra* this, metric m);

metric get_p_maj(algebra* this);

/**
 * Here we define functions of the pseudo-class SequenceAlgebra, implementing the pseudo-class BaseAlgebra.
 */

void constructor_algebra_seq(algebra* this);

int equals_p_seq(algebra* this, metric m);

metric get_p_seq(algebra* this);

size_t size_p_seq(__attribute__((unused)) algebra* this);

size_t s_packet_seq(__attribute__((unused)) algebra* this, metric m);

settable_metric w_packet_seq(__attribute__((unused)) algebra* this, metric m, settable_metric p);

metric p_packet_seq(__attribute__((unused)) algebra* this, metric packet);

/**
 * Here we define functions of the pseudo-class BinaryOperationAlgebra, implementing the pseudo-interface Algebra.
 */

void constructor_algebra_bop(algebra* this, algebra* a0, algebra* a1);

void free_al_bop(algebra* this);

/**
 * Here we define functions of the pseudo-class BimetricBinaryOperationAlgebra, inheriting from BinaryOperationAlgebra.
 */

algebra* constructor_algebra_bbo(algebra* a0, algebra* a1);

int equals_p_bbo(algebra* this, metric m);

metric get_p_bbo(algebra* this);

size_t size_p_bbo(algebra* this);

size_t s_packet_bbo(algebra* this, metric m);

settable_metric w_packet_bbo(algebra* this, metric m, settable_metric p);

metric p_packet_bbo(algebra* this, metric packet);




#endif // FRR_CLASS_ALGEBRA_H
