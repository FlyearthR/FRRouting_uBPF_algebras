#include "../../include/public_bpf.h"
#include "structures.h"
#include "class_algebra.h"
#include <stdio.h>
#include <stdlib.h>

static __attribute__((always_inline)) char* strncpy(char* destination, const char* source, size_t num)
{
    // return if no memory is allocated to the destination
    if (destination == NULL)
        return NULL;

    // take a pointer pointing to the beginning of destination string
    char* ptr = destination;

    // copy first num characters of C-string pointed by source
    // into the array pointed by destination
    while (*source && num--)
    {
        *destination = *source;
        destination++;
        source++;
    }

    // null terminate destination string
    *destination = '\0';

    // destination is returned by standard strncpy()
    return ptr;
}

#include "cust_malloc.c"
#include "class_algebra.c"
#include "integer_addition.c"

#define METRIC_INFINITY 16
#define IPV6_MAX_BYTELEN    16
#define IPV6_ADDR_SAME(D,S)  (my_super_memcmp ((D), (S), IPV6_MAX_BYTELEN) == 0)

static __attribute__((always_inline)) int my_super_memcmp(const void *s1, const void *s2, size_t n) {
	 const uint8_t *_s1 = s1;
	 const uint8_t *_s2 = s2;

	 while (n-- > 0) {
	     if (*_s1++ != *_s2++) {
	         return _s1[-1] < _s2[-1] ? -1 : 1;
	     }
	 }
	 return 0;
}



extern struct list* cust_agg_node_get(struct cust_agg_node_get_t*, struct triple*);
extern int cust_ripng_route_rte(struct ripng_info*, int);
extern void cust_agg_unlock_node(struct cust_agg_node *node);

uint64_t ism_change_state(bpf_full_args_t *args) {


    ebpf_print("route received\n");
    struct mal* init;
    struct mal* last;
    algebra* a = ADD(0, 64, init, &last);
    init_mal(&init, &last);

    int* ret = bpf_get_args(0, args);
    struct rte* rte = bpf_get_args(1, args);
    uint32_t* if_metric_p = (uint32_t*)bpf_get_args(2, args);
    struct nexthop* nexthop = bpf_get_args(3, args);
    struct sockaddr_in6* from = bpf_get_args(4, args);
    void* table = *(void**) bpf_get_args(5, args);
    bool ecmp = *(bool*) bpf_get_args(6, args);
    struct prefix_ipv6* p = bpf_get_args(7, args);
    void* newinfo = *(void**) bpf_get_args(8, args);

    struct in6_addr *addr_nexthop = NULL;
    struct ret* re = (struct ret*) ctx_malloc(sizeof(struct ret));

    if(!ret || !rte || !if_metric_p || !nexthop || !from || !table || !p || !newinfo) {
        ebpf_print("ret: %p\nrte: %p\nif_metric_p: %p\n", ret, rte, if_metric_p);
        ebpf_print("nexthop: %p\nfrom: %p\ntable: %p\n", nexthop, from, table);
        ebpf_print("p: %p\nnewinfo: %p\n", p, newinfo);
        re->ret_code = 1;
        return (uint64_t) re;
    }

    uint32_t if_metric = *if_metric_p;

    if (!*ret)
        rte->metric += if_metric ? if_metric : 1;

    if (rte->metric > METRIC_INFINITY)
        rte->metric = METRIC_INFINITY;

    /* Set nexthop pointer. */
    if (nexthop->flag == NEXTHOP_ADDRESS)
        addr_nexthop = &nexthop->address;
    else
        addr_nexthop = &from->sin6_addr;



    struct cust_agg_node_get_t* st = (struct cust_agg_node_get_t*) ctx_malloc(sizeof(struct cust_agg_node_get_t));
    st->table = (void*) table;
    st->p = (void*) p;
    st->ni = newinfo;
    st->addr_nexthop = *addr_nexthop;
    st->rte = *rte;
    struct triple* tr = (struct triple*) ctx_malloc(sizeof(struct triple));
    struct list *list = cust_agg_node_get(st, tr);
    struct ripng_info *rinfo = tr->ret;
    re->an = tr->pointer;
    int i;
    for (i = 0 ; i < tr->size ; i++) {
        /* Need to compare with redistributed entry or local
         * entry */
        if (!(rinfo[i].type == ZEBRA_ROUTE_RIPNG && rinfo[i].sub_type == RIPNG_ROUTE_RTE)) {
            break;
        }
        if (IPV6_ADDR_SAME(&rinfo[i].from, &from->sin6_addr)
            && IPV6_ADDR_SAME(&rinfo[i].nexthop, addr_nexthop))
            break;

        if (i == tr->size) {
            /* Not found in the list */

            int pref = preference(a, (metric) &(rte->metric), (metric) &(rinfo[i].metric));
            if (pref > 0) {

                /* New route has a greater metric.
                 * Discard it. */
                cust_agg_unlock_node(tr->pointer);
                ebpf_print("plugin: greater metric");
                re->ret_code = 2;
                free_all(init, &last);
                return (uint64_t) re;
            }

            //if (preference(a, (metric) &(rte->metric), &(rinfo[i].metric)) < 0)
            if (pref < 0) {
                /* New route has a smaller metric.
                 * Replace the ECMP list
                 * with the new one in below. */
                free_all(init, &last);
                break;
            }


            /* Metrics are same. Unless ECMP is disabled,
             * keep "rinfo" null and
             * the new route is added in the ECMP list in
             * below. */
            if (!ecmp)
                break;
        }
    }
    re->rinfo = rinfo + i;
    re->list = list;

    ebpf_print("Getting out of plugin\n");
    close_mal(&init, &last);
    return (uint64_t) re;
}