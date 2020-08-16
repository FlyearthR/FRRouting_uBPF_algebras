#ifndef FRR_CLASS_ALGEBRA_H
#define FRR_CLASS_ALGEBRA_H



typedef struct algebra algebra;
typedef const void* metric;
typedef void* settable_metric;

metric get_metric(algebra* this, metric m);
/**
 * prec: the preference relation
 * lapp: the label application function
 * set_prefix: a method to recursively set the prefix of this
 */
struct algebra
{
    int (*prec)(algebra* this, metric met0, metric met1);
    metric (*lapp)(algebra* this, metric lambda, metric sigma);
    char* (*to_str)(algebra* this, metric m, int metric);
    int (*size_of_str)(algebra* this, metric m, int metric);
    size_t (*s_metric)(algebra* this, metric m, int metric);
    struct mal* init;
    struct mal** last;
    void* args[];
};

/*metric get_metric(algebra* this, metric m);

int preference(algebra* this, metric met0, metric met1);

metric label_application(algebra* this, metric lambda, metric sigma);

size_t size_of_string(algebra* this, metric m, int metric);

char* to_string(algebra* this, metric m, int metric);

size_t size_metric(algebra* this, metric m, int metric);

size_t size_metric_int(__attribute__((unused)) algebra* this, __attribute__((unused)) metric m, __attribute__((unused)) int metric);*/

#endif // FRR_CLASS_ALGEBRA_H
