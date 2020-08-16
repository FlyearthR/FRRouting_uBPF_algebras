#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../include/plugin_arguments.h"
#include "route_types.h"


#ifndef FRR_STRUCTURES_H
#define FRR_STRUCTURES_H

/* RIPng commands. */
//#define RIPNG_REQUEST                    1
//#define RIPNG_RESPONSE                   2


#define RIPNG_ROUTE_RTE                  0
#define listnextnode(X) ((X) ? ((X)->next) : NULL)
#define listhead(X) ((X) ? ((X)->head) : NULL)
#define listgetdata(X) (X)->data
#define ALL_LIST_ELEMENTS_RO(list, node, data)                                 \
	(node) = listhead(list), ((data) = NULL);                              \
	(node) != NULL && ((data) = listgetdata(node));                        \
	(node) = listnextnode(node), ((data) = NULL)

#define table_rdonly(x)		x

typedef signed int ifindex_t;
typedef uint32_t vrf_id_t;

struct triple {
    struct ripng_info* ret;
    int size;
    void* pointer;
};

struct prefix {
    uint8_t family;
    uint16_t prefixlen;
    uint8_t u[20];
};

struct interface {
    char name[20];
};

struct listnode {
    struct listnode *next;
    struct listnode *prev;

    /* private member, use getdata() to retrieve, do not access directly */
    void *data;
};

struct list {
    struct listnode *head;
    struct listnode *tail;

    /* invariant: count is the number of listnodes in the list */
    unsigned int count;

    /*
     * Returns -1 if val1 < val2, 0 if equal?, 1 if val1 > val2.
     * Used as definition of sorted for listnode_add_sort
     */
    int (*cmp)(void *val1, void *val2);

    /* callback to free user-owned data when listnode is deleted. supplying
     * this callback is very much encouraged!
     */
    void (*del)(void *val);
};

/* Routing Table Entry */
struct rte {
    uint16_t tag;	 /* RIPng tag */
    uint8_t metric;       /* Metric of the RIPng route */
    /* The nexthop is stored by the structure
     * ripng_nexthop within ripngd.c */
};

struct ripng_info {
    /* This route's type.  Static, ripng or aggregate. */
    uint8_t type;

    /* Sub type for static route. */
    uint8_t sub_type;

    /* Metric of this route.  */
    uint8_t metric;

    struct in6_addr nexthop;
    struct in6_addr from;

    /* Tag field of RIPng packet.*/
    uint16_t tag;

    struct cust_agg_node *rp;
};

struct nexthop {
    enum nexthop_type {
        NEXTHOP_UNSPEC,
        NEXTHOP_ADDRESS
    } flag;
    struct in6_addr address;
};

struct thash_head {
    struct thash_item **entries;
    uint32_t count;

    uint8_t tabshift;
    uint8_t minshift, maxshift;
};

struct thash_item {
    struct thash_item *next;
    uint32_t hashval;
};

struct rn_hash_node_head {
    struct thash_head hh;
};

struct rn_hash_node_item {
    struct thash_item hi;
};


struct route_node {
    struct prefix p;

	/* Tree link. */
	struct route_table* table_rdonly(table);
	struct route_node* table_rdonly(parent);
	struct route_node* table_rdonly(link[2]);

	/* Lock of this radix */
	unsigned int table_rdonly(lock);

	struct rn_hash_node_item nodehash;
	/* Each node of route. */
	void *info;

#define l_left   link[0]
#define l_right  link[1]
};

typedef const struct route_table_delegate_t_ route_table_delegate_t;

struct route_table {
    struct route_node *top;
    struct rn_hash_node_head hash;

    /*
     * Delegate that performs certain functions for this table.
     */
    route_table_delegate_t *delegate;
    void (*cleanup)(struct route_table *, struct route_node *);

    unsigned long count;

    /*
     * User data.
     */
    void *info;
};

struct agg_table {
    struct route_table *route_table;

    void *info;
};

struct RIB {
    /* Routing information base. */
    void *table;

    /* ECMP flag */
    bool ecmp;
};

struct cust_agg_node {
    /*
     * Caution these must be the very first fields
     * @see agg_node_to_rnode
     * @see agg_node_from_rnode
     */
    /* Actual prefix of this radix. */
	struct prefix p;

	/* Tree link. */
	struct route_table* table;
	struct route_node* parent;
	struct route_node* link[2];

	/* Lock of this radix */
	unsigned int lock;

	/* Each node of route. */
	void *info;

    /* Aggregation. */
    void *aggregate;
};

struct ripng_packet {
    uint8_t command;
    uint8_t version;
    uint16_t zero;
    struct rte rte[1];
};

struct ret {
	struct ripng_info* rinfo;
	struct list* list;
	struct cust_agg_node* an;
	int ret_code;
};

struct cust_agg_node_get_t {
    void *table;
    void *p;
    void *ni;
    struct in6_addr addr_nexthop;
    struct rte rte;
};


#endif //FRR_STRUCTURES_H
