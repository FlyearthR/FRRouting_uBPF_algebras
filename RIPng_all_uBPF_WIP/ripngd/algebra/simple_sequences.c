#include "simple_sequences.h"

/**
 *          φ       if (σ = [σ_1, σ_2, ..., σ_k] ∧ k = m) ∨ λ ∈ σ
 * λ ⊕ σ =
 *          λ :: σ  otherwise
 * @param args not used
 * @param lambda: a pointer to the label to apply
 * @param sigma: a pointer to the signature on which apply the label
 * @return a pointer to the new signature
 */
static metric lapp(algebra* this, metric lambda, metric sigma)
{
    const int* l = lambda;
    const struct seq* s = sigma;
    if (s->size > ((int*) this->args)[1])
    {
        return get_phi(this);
    }

    struct seq* ret = compare_copy_seq(s, *l);
    if (ret == NULL)
    {
        return get_phi(this);
    }
    append(ret, *l);
    return ret;
}

algebra* SIMSEQ(int n, int m)
{
    algebra* this = SEQ(n, m);
    this->lapp = &lapp;
    return this;
}
