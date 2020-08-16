#include "test.h"

static int DEBUG = 0;

struct fifo* f_last;

void append_node(int i)
{
    char str[15];
    sprintf(str, "AN%i", i);
    f_last->next = cust_malloc(sizeof(struct fifo));
    f_last = f_last->next;
    f_last->i = i;
    f_last->next = NULL;
}

void debug(char str[], ...)
{
    if (DEBUG)
    {
        va_list vars;
        va_start(vars, str);
        vfprintf(stderr, str, vars);
        va_end(vars);
    }
}

void compute_network(algebra* a, int first_node, int last_node, struct node* network[], int network_size, struct link links[], int router_links[], metric first_metric)
{
    network[first_node]->m_pref = first_metric;

    struct fifo* first = (struct fifo*) cust_malloc(sizeof(struct fifo));
    first->i=first_node;
    first->next=NULL;
    struct fifo* cur = first;
    f_last = first;

    // we propagate the paths to r3
    while (cur != NULL) // while there is a router which just got a better route
    {
        debug("Exploring node %i\n", cur->i);
        debug("Routing table:\n");
        for (int i = 0 ; i < network_size ; i++)
            debug("%i: %s\n", i, to_string(a, network[i]->m_pref, 1));
        debug("\n");
        for (int i = 0 ; i < router_links[cur->i] ; i++) // for all this neighbours
        {
            struct node* nei; // the neighbour
            if (network[cur->i]->links[i]->n[0]->index == cur->i)
                nei = network[cur->i]->links[i]->n[1];
            else
                nei = network[cur->i]->links[i]->n[0];

            debug("label app: %s + %s ==> ", to_string(a, network[cur->i]->links[i]->label, 0), to_string(a, network[cur->i]->m_pref, 1));

            metric nm = label_application(a, network[cur->i]->links[i]->label, network[cur->i]->m_pref); // the cost through this path
            debug("%s\n", to_string(a, nm, 1));
            debug("%s is prefered to %s: ", to_string(a, nm, 1), to_string(a, nei->m_pref, 1));
            if (preference(a, nm, nei->m_pref) < 0) // if it's a better path
            {
                debug("TRUE\n");
                int next_hop;
                if (network[cur->i]->links[i]->n[0]->index == nei->index)
                    next_hop = network[cur->i]->links[i]->n[1]->index;
                else
                    next_hop = network[cur->i]->links[i]->n[0]->index;
                int last_hop;
                if (nei->l_pref->n[0]->index == nei->index)
                    last_hop = nei->l_pref->n[1]->index;
                else
                    last_hop = nei->l_pref->n[0]->index;

                debug("Node %i prefers node %i with cost %s than node %i with cost %s\n\n", nei->index, next_hop, to_string(a, nm, 1), last_hop, to_string(a, nei->m_pref, 1));
                append_node(nei->index); // this router has to advertise it
                nei->l_pref = network[cur->i]->links[i]; // update best next hop
                nei->m_pref = nm; // update best cost
            }
            else
            {
                debug("FALSE\n\n");
            }
        }
        debug("\n");
        cur = cur->next;
    }

    printf("%s\n", to_string(a, network[last_node]->m_pref, 1));

}

void reset(struct node* network[], int network_size, metric infinity, struct link links[], int links_no_lo)
{
    for (int i = 0 ; i < network_size ; i++)
    {
        network[i]->l_pref = &links[i+links_no_lo];
        network[i]->m_pref = infinity;
    }
}

/**
 * Set, Launch, Clean & Compare
 */
void slc(char* should_be, algebra* a, int first_node, int last_node, struct node* network[], int network_size, struct link links[], int router_links[], metric first_metric, metric infinity, int links_no_lo)
{
    printf("should be\t%s\tand is\t", should_be);
    reset(network, network_size, infinity, links, links_no_lo);
    compute_network(a, first_node, last_node, network, network_size, links, router_links, first_metric);
    free_all();
}

metric same(metric m)
{
    return m;
}

metric plus32(metric m)
{
    int* ret = cust_malloc(sizeof(int));
    *ret = 32 + *(int*)m;
    return ret;
}

int main(int argc, char* argv[])
{
    const int zero = 0;
    const int one = 1;
    const int two = 2;
    const int three = 3;
    const int four = 4;
    const int thirtythree = 33;
    const int thirtyfour = 34;
    const int sixtyfive = 65;

    init_mal();

    /** Netwok 1a
     *                  +-----+  5  +-----+
     *                  | r0  +-----+ r1  |
     *                  +--+--+     +--+--+
     *               2/    |           |   2\
     *        +-----+      |           |      +-----+
     *        | r2  |     1|          1|      | r3  |
     *        +-----+      |           |      +-----+
     *               1\    |           |   4/
     *                  +-----+  1  +--+--+
     *                  | r4  +-----+ r5  |
     *                  +-----+     +-----+
     */

    struct node* n1[6]; // 0    1    2    3    4    5    6    7    8    9    10   11   12   13
    struct link l1[14]; // l01, l02, l04, l15, l13, l24, l35, l45, l00, l11, l22, l33, l44, l55

    int n1lrr[] = {0, 1, 2, 0, 3, 4, 1, 5, 4, 6, 2, 5, 7, 3, 6, 7}; // links of each router
    int n1lrs[6] = {3, 3, 2, 2, 3, 3}; // number of link of each router
    int* n1lr[6] = {n1lrr}; // = {0, 1, 2}, {0, 3, 4}, {1, 5}, {4, 6}, {2, 5, 7}, {3, 6, 7}}
    for (int i = 1 ; i < 6 ; i++)
        n1lr[i] = n1lr[i-1] + n1lrs[i-1];

    for (int i = 0 ; i < 6; i++) // initialization of the routers
    {
        n1[i] = (struct node*) malloc(sizeof(struct node)+ (n1lrs[i]+1)*sizeof(struct link*));
        for (int j = 0 ; j < n1lrs[i] ; j++)
            n1[i]->links[j] = &l1[n1lr[i][j]];
        n1[i]->links[n1lrs[i]] = NULL;
        n1[i]->index = i;
    }

    struct node* ln[14][2] = {{n1[0], n1[1]}, {n1[0], n1[2]}, {n1[0], n1[4]}, {n1[1], n1[5]}, {n1[1], n1[3]},
        {n1[2], n1[4]}, {n1[3], n1[5]}, {n1[4], n1[5]}, {n1[0], n1[0]}, {n1[1], n1[1]},
        {n1[2], n1[2]}, {n1[3], n1[3]}, {n1[4], n1[4]}, {n1[5], n1[5]}
    };
    int ll1[14] = {5, 2, 1, 1, 2, 1, 4, 1, 0, 0, 0, 0, 0, 0}; // labels
    for (int i = 0 ; i < 14 ; i++)
    {
        l1[i].label = &ll1[i];
        l1[i].n[0] = ln[i][0];
        l1[i].n[1] = ln[i][1];
    }

    DEBUG = 0;

    printf("ADD:\n");
    algebra* a = ADD(0, 64);
    int infinity = 65;
    int fi_met = 0;

    slc("5", a, 3, 2, n1, 6, l1, n1lrs, &fi_met, &infinity, 8);
    slc("5", a, 2, 3, n1, 6, l1, n1lrs, &fi_met, &infinity, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nMULT:\n");
    a = MULT(1, 64);
    fi_met = 1;

    slc("2", a, 3, 2, n1, 6, l1, n1lrs, &fi_met, &infinity, 8);
    slc("2", a, 2, 3, n1, 6, l1, n1lrs, &fi_met, &infinity, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nMIN:\n");
    a = MIN();

    slc("1", a, 3, 2, n1, 6, l1, n1lrs, &infinity, &infinity, 8);
    slc("1", a, 2, 3, n1, 6, l1, n1lrs, &infinity, &infinity, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nMAX:\n");
    a = MAX();
    fi_met = 0;

    slc("2", a, 3, 2, n1, 6, l1, n1lrs, &fi_met, &infinity, 8);
    slc("2", a, 2, 3, n1, 6, l1, n1lrs, &fi_met, &infinity, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nLP:\n");
    a = LP(64);

    slc("1", a, 3, 2, n1, 6, l1, n1lrs, &infinity, &infinity, 8);
    slc("2", a, 2, 3, n1, 6, l1, n1lrs, &infinity, &infinity, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nOP:\n");
    a = OP(64);
    fi_met = 42;

    slc("42", a, 3, 2, n1, 6, l1, n1lrs, &fi_met, &infinity, 8);
    slc("42", a, 2, 3, n1, 6, l1, n1lrs, &fi_met, &infinity, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nSEQ:\n");
    a = SEQ(64, 64);
    struct seq s = {.size = 0, .head = NULL, .tail = NULL};
    struct seq PHI = {.size = 65, .head = NULL, .tail = NULL};

    slc("sequence of size 3", a, 3, 2, n1, 6, l1, n1lrs, &s, &PHI, 8);
    slc("sequence of size 3", a, 2, 3, n1, 6, l1, n1lrs, &s, &PHI, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nSIMSEQ:\n");
    a = SIMSEQ(64, 64);

    slc("PHI", a, 3, 2, n1, 6, l1, n1lrs, &s, &PHI, 8);
    slc("2:5:1:4", a, 2, 3, n1, 6, l1, n1lrs, &s, &PHI, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nnCROSS(SIMSEQ):\n");
    a = nCROSS(SIMSEQ(64, 64));

    slc("PHI", a, 3, 2, n1, 6, l1, n1lrs, &s, &PHI, 8);
    slc("2:5:1:4", a, 2, 3, n1, 6, l1, n1lrs, &s, &PHI, 8);

    free_algebra(a);


    /** Netwok 1b
     *                  +-----+ i(3)+-----+
     *                  | r0  +-----+ r1  |
     *                  +--+--+     +--+--+
     *          i(1,2)/    |           | d(1)\
     *        +-----+      |           |      +-----+
     *        | r2  |     k|          k|      | r3  |
     *        +-----+      |           |      +-----+
     *               k\    |           | i(1)/
     *                  +-----+ d(1)+--+--+
     *                  | r4  +-----+ r5  |
     *                  +-----+     +-----+
     */

    //  0    1    2    3    4    5    6    7    8    9    10   11   12   13
    // l01, l02, l04, l15, l13, l24, l35, l45, l00, l11, l22, l33, l44, l55

    struct change_tags i01 = {.action = insert};
    struct node_seq n013 = {.next = NULL, .i = 3};
    i01.tags.size = 1;
    i01.tags.head = &n013;
    i01.tags.tail = &n013;

    struct change_tags i02 = {.action = insert};
    struct node_seq n021 = {.next = NULL, .i = 1};
    struct node_seq n022 = {.next = &n021, .i = 2};
    i02.tags.size = 2;
    i02.tags.head = &n022;
    i02.tags.tail = &n021;

    struct change_tags i13 = {.action = delete};
    struct node_seq n131 = {.next = NULL, .i = 1};
    i13.tags.size = 1;
    i13.tags.head = &n131;
    i13.tags.tail = &n131;

    struct change_tags i35 = {.action = insert};
    struct node_seq n351 = {.next = NULL, .i = 1};
    i35.tags.size = 1;
    i35.tags.head = &n351;
    i35.tags.tail = &n351;

    struct change_tags kappa = {.action = pass};
    for (int i = 0 ; i < 14 ; i++)
    {
        switch (i)
        {
        case 0:
            l1[i].label = &i01;
            break;
        case 1:
            l1[i].label = &i02;
            break;
        case 4:
            l1[i].label = &i13;
            break;
        case 6:
            l1[i].label = &i35;
            break;
        case 7:
            l1[i].label = &i13;
            break;
        default:
            l1[i].label = &kappa;
        }
    }

    DEBUG = 0;

    printf("\nTAGS:\n");
    a = TAGS(64);

    slc("whatever", a, 3, 2, n1, 6, l1, n1lrs, &s, &s, 8);
    slc("whatever", a, 2, 3, n1, 6, l1, n1lrs, &s, &s, 8);

    free_algebra(a);


    /** Netwok 1c
     *                  +-----+(1,3)+-----+
     *                  | r0  +-----+ r1  |
     *                  +--+--+     +--+--+
     *           (2,4)/    |           |(1,1)\
     *        +-----+      |           |      +-----+
     *        | r2  | (1,2)|      (1,5)|      | r3  |
     *        +-----+      |           |      +-----+
     *           (1,1)\    |           |(1,2)/
     *                  +-----+(4,1)+--+--+
     *                  | r4  +-----+ r5  |
     *                  +-----+     +-----+
     */

    //  0    1    2    3    4    5    6    7    8    9    10   11   12   13
    // l01, l02, l04, l15, l13, l24, l35, l45, l00, l11, l22, l33, l44, l55

    int ll1c[14][2] = {{1,3}, {2,4}, {1,2}, {1,5}, {1,1}, {1,1}, {1,2}, {4,1}, {0, 5}, {0, 5}, {0, 5}, {0, 5}, {0, 5}, {0, 5}}; // labels
    for (int i = 0 ; i < 14 ; i++)
        l1[i].label = &ll1c[i];

    DEBUG = 0;

    printf("\nCROSS(ADD, ADD):\n");
    algebra* a0 = ADD(0, 64);
    algebra* a1 = ADD(0, 64);
    a = CROSS(a0, a1);

    int fmAX[2] = {0,0};
    int infAX[2] = {65, 65};

    slc("(4,7)", a, 3, 2, n1, 6, l1, n1lrs, fmAX, infAX, 8);
    slc("(4,7)", a, 2, 3, n1, 6, l1, n1lrs, fmAX, infAX, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nCROSS(ADD, MULT):\n");
    a0 = ADD(0, 64);
    a1 = MULT(1, 64);
    a = CROSS(a0, a1);

    int fmAM[2] = {1,1};

    slc("(5,6)", a, 3, 2, n1, 6, l1, n1lrs, fmAM, infAX, 8);
    slc("(5,6)", a, 2, 3, n1, 6, l1, n1lrs, fmAM, infAX, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nCROSS(ADD, MIN):\n");
    a0 = ADD(0, 64);
    a1 = MIN();
    a = CROSS(a0, a1);
    int fmAMi[2] = {0, 65};

    slc("(4,1)", a, 3, 2, n1, 6, l1, n1lrs, fmAMi, infAX, 8);
    slc("(4,1)", a, 2, 3, n1, 6, l1, n1lrs, fmAMi, infAX, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nCROSS(ADD, MAX):\n");
    a0 = ADD(0, 64);
    a1 = MAX();
    a = CROSS(a0, a1);

    slc("(4,3)", a, 3, 2, n1, 6, l1, n1lrs, fmAX, infAX, 8);
    slc("(4,3)", a, 2, 3, n1, 6, l1, n1lrs, fmAX, infAX, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nCROSS(ADD, LP):\n");
    a0 = ADD(0, 64);
    a1 = LP(64);
    a = CROSS(a0, a1);

    slc("(4,1)", a, 3, 2, n1, 6, l1, n1lrs, fmAX, infAX, 8);
    slc("(4,1)", a, 2, 3, n1, 6, l1, n1lrs, fmAX, infAX, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nCROSS(ADD, OP):\n");
    a0 = ADD(0, 64);
    a1 = OP(64);
    a = CROSS(a0, a1);

    slc("(4,0)", a, 3, 2, n1, 6, l1, n1lrs, fmAX, infAX, 8);
    slc("(4,0)", a, 2, 3, n1, 6, l1, n1lrs, fmAX, infAX, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nCROSS(ADD, SEQ):\n");
    a0 = ADD(0, 64);
    a1 = SEQ(64, 64);
    a = CROSS(a0, a1);

    int8_t fmAS[sizeof(int)+sizeof(struct seq)];
    int8_t infAS[sizeof(int)+sizeof(struct seq)];
    memcpy(fmAS, &zero, sizeof(int));
    memcpy(infAS, &sixtyfive, sizeof(int));
    memcpy(fmAS+sizeof(int), &s, sizeof(struct seq));
    memcpy(infAS+sizeof(int), &PHI, sizeof(struct seq));

    slc("(4,1:3:4)", a, 3, 2, n1, 6, l1, n1lrs, fmAS, infAS, 8);
    slc("(4,4:3:1)", a, 2, 3, n1, 6, l1, n1lrs, fmAS, infAS, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nCROSS(ADD, SIMSEQ):\n");
    a0 = ADD(0, 64);
    a1 = SIMSEQ(64, 64);
    a = CROSS(a0, a1);

    slc("(4,1:3:4)", a, 3, 2, n1, 6, l1, n1lrs, fmAS, infAS, 8);
    slc("(4,4:3:1)", a, 2, 3, n1, 6, l1, n1lrs, fmAS, infAS, 8);

    free_algebra(a);


    /** Netwok 1d
     *        (1,i(1,2))+-----+(1,i(3))+-----+ (1,d(1))
     *        +---------+ r0  +--------+ r1  +---------+
     *        |         +--+--+        +--+--+         |
     *        |            |              |            |
     *     +--+--+         |              |         +--+--+
     *     | r2  |    (1,k)|         (1,k)|         | r3  |
     *     +--+--+         |              |         +--+--+
     *        |            |              |            |
     *        |  (1,k)  +-----+(4,d(1))+--+--+ (1,i(1))|
     *        +---------+ r4  +--------+ r5  +---------+
     *                  +-----+        +-----+
     */

    //  0    1    2    3    4    5    6    7    8    9    10   11   12   13
    // l01, l02, l04, l15, l13, l24, l35, l45, l00, l11, l22, l33, l44, l55
    //  1    1    1    1    1    1    1    4    0    0    0    0    0    0
    //i(3) i(1,2) k    k  d(1)   k  i(1) d(1)   k    k    k    k    k    k
    int8_t linksAT[14][sizeof(int)+sizeof(struct change_tags)];

    for (int i = 0 ; i < 7 ; i++)
        memcpy(linksAT[i], &one, sizeof(int));
    memcpy(linksAT[7], &four, sizeof(int));
    for (int i = 8 ; i < 14 ; i++)
        memcpy(linksAT[i], &zero, sizeof(int));

    for (int i = 0 ; i < 14 ; i++)
        memcpy(linksAT[i]+sizeof(int), &kappa, sizeof(struct change_tags)); //kappa everywhere
    memcpy(linksAT[0]+sizeof(int), &i01, sizeof(struct change_tags));
    memcpy(linksAT[1]+sizeof(int), &i02, sizeof(struct change_tags));
    memcpy(linksAT[4]+sizeof(int), &i13, sizeof(struct change_tags));
    memcpy(linksAT[6]+sizeof(int), &i35, sizeof(struct change_tags));
    memcpy(linksAT[7]+sizeof(int), &i13, sizeof(struct change_tags));

    for (int i = 0 ; i < 14 ; i++)
        l1[i].label = linksAT[i];


    DEBUG = 0;

    printf("\nCROSS(ADD, TAGS):\n");
    a0 = ADD (0, 64);
    a1 = TAGS(64);
    a = CROSS(a0, a1);

    slc("(3,3:2:1)", a, 3, 2, n1, 6, l1, n1lrs, fmAS, infAS, 8);
    slc("(3,2:3)", a, 2, 3, n1, 6, l1, n1lrs, fmAS, infAS, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nnCROSS(ADD, TAGS):\n");
    a0 = ADD (0, 64);
    a1 = TAGS(64);
    a = nCROSS(a0, a1);

    slc("(3,3:2:1)", a, 3, 2, n1, 6, l1, n1lrs, fmAS, infAS, 8);
    slc("(3,2:3)", a, 2, 3, n1, 6, l1, n1lrs, fmAS, infAS, 8);

    free_algebra(a);




    /** Netwok 1e
     *         (0,1,1)+-----+(1,1,1)+-----+(2,2,1)
     *        +-------+ r0  +-------+ r1  +-------+
     *        |       +--+--+       +--+--+       |
     *        |          |             |          |
     *     +--+--+       |             |       +--+--+
     *     | r2  |(1,2,1)|      (2,0,1)|       | r3  |
     *     +--+--+       |             |       +--+--+
     *        |          |             |          |
     *        |(1,2,1)+-----+(1,1,2)+--+--+(1,1,1)|
     *        +-------+ r4  +-------+ r5  +-------+
     *                +-----+       +-----+
     */

    //  0    1    2    3    4    5    6    7    8    9    10   11   12   13
    // l01, l02, l04, l15, l13, l24, l35, l45, l00, l11, l22, l33, l44, l55

    int ll1e[14][3] = {{1,1,1}, {0,1,1}, {1,2,1}, {2,0,1}, {2,2,1}, {1,2,1}, {1,1,1}, {1,1,2}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}}; // labels
    for (int i = 0 ; i < 14 ; i++)
        l1[i].label = &ll1e[i];

    DEBUG = 0;

    printf("\nnCROSS(ADD, ADD, ADD):\n");
    a0 = ADD(0, 64);
    a1 = ADD(0, 64);
    algebra* a2 = ADD(0, 64);
    a = nCROSS(a0, a1, a2);

    int fmAAA[3] = {0,0,0};
    int infAAA[3] = {65, 65, 65};

    slc("(3,(4,3))", a, 3, 2, n1, 6, l1, n1lrs, fmAAA, infAAA, 8);
    slc("(3,(4,3))", a, 2, 3, n1, 6, l1, n1lrs, fmAAA, infAAA, 8);

    free_algebra(a);


    /** Netwok 1f
     *                  +-----+(2,1)+-----+
     *                  | r0  +-----+ r1  |
     *                  +--+--+     +--+--+
     *           (.,1)/    |           |(.,2)\
     *        +-----+      |           |      +-----+
     *        | r2  | (.,1)|      (.,1)|      | r3  |
     *        +-----+      |           |      +-----+
     *           (.,3)\    |           |(.,4)/
     *                  +-----+(1,2)+--+--+
     *                  | r4  +-----+ r5  |
     *                  +-----+     +-----+
     */

    //  0    1    2    3    4    5    6    7    8    9    10   11   12   13
    // l01, l02, l04, l15, l13, l24, l35, l45, l00, l11, l22, l33, l44, l55

    struct zone_label dot0 = {.new_zone = 0, .label = &zero};
    struct zone_label dot1 = {.new_zone = 0, .label = &one};
    struct zone_label dot2 = {.new_zone = 0, .label = &two};
    struct zone_label dot3 = {.new_zone = 0, .label = &three};
    struct zone_label dot4 = {.new_zone = 0, .label = &four};
    int onetwo[2] = {1, 2};
    int twoone[2] = {2, 1};
    struct zone_label ot = {.new_zone = 1, .label = onetwo};
    struct zone_label to = {.new_zone = 1, .label = twoone};

    l1[0].label = (metric) &to;
    l1[1].label = (metric) &dot1;
    l1[2].label = (metric) &dot1;
    l1[3].label = (metric) &dot1;
    l1[4].label = (metric) &dot2;
    l1[5].label = (metric) &dot3;
    l1[6].label = (metric) &dot4;
    l1[7].label = (metric) &ot;
    for (int i = 8 ; i < 14 ; i++)
        l1[i].label = (metric) &dot0;

    DEBUG = 0;

    printf("\nSCOPE(ADD, ADD):\n");
    a0 = ADD(0, 64);
    a1 = ADD(0, 64);
    a = SCOPE(a0, a1);

    slc("(1,4)", a, 3, 2, n1, 6, l1, n1lrs, fmAX, infAX, 8);
    slc("(1,5)", a, 2, 3, n1, 6, l1, n1lrs, fmAX, infAX, 8);

    free_algebra(a);


    /** Netwok 1g
     *                  +-----+  RI +-----+
     *                  | r0  +-----+ r1  |
     *                  +--+--+     +--+--+
     *            1-2 /    |           | 0-3 \
     *        +-----+      |           |      +-----+
     *        | r2  |  0-1 |       1-1 |      | r3  |
     *        +-----+      |           |      +-----+
     *            1-1 \    |           |  RI /
     *                  +-----+  RI +--+--+
     *                  | r4  +-----+ r5  |
     *                  +-----+     +-----+
     */

    //  0    1    2    3    4    5    6    7    8    9    10   11   12   13
    // l01, l02, l04, l15, l13, l24, l35, l45, l00, l11, l22, l33, l44, l55

    struct disjunct dzz = {.a = 0, .m = &zero};
    struct disjunct dzo = {.a = 0, .m = &one};
    struct disjunct doo = {.a = 1, .m = &one};
    struct disjunct dot = {.a = 1, .m = &two};
    struct disjunct dzt = {.a = 0, .m = &three};
    struct disjunct dRI = {.a = 2, .m = &one};
    struct disjunct dPHI = {.a = 3};

    l1[0].label = (metric) &dRI;
    l1[1].label = (metric) &dot;
    l1[2].label = (metric) &dzo;
    l1[3].label = (metric) &doo;
    l1[4].label = (metric) &dzt;
    l1[5].label = (metric) &doo;
    l1[6].label = (metric) &dRI;
    l1[7].label = (metric) &dRI;
    for (int i = 8 ; i < 14 ; i++)
        l1[i].label = (metric) &dzz;

    DEBUG = 0;

    printf("\nDISJOINT(ADD, ADD, same):\n");
    a0 = ADD(0, 64);
    a1 = ADD(0, 64);
    a = DISJOINT(a0, a1, &same);

    slc("1-5", a, 3, 2, n1, 6, l1, n1lrs, &dzz, &dPHI, 8);
    slc("PHI", a, 2, 3, n1, 6, l1, n1lrs, &dzz, &dPHI, 8);

    free_algebra(a);

    DEBUG = 0;

    printf("\nDISJOINT(ADD, ADD, NULL):\n");
    a0 = ADD(0, 64);
    a1 = ADD(0, 64);
    a = DISJOINT(a0, a1, NULL);

    slc("PHI", a, 3, 2, n1, 6, l1, n1lrs, &dzz, &dPHI, 8);
    slc("PHI", a, 2, 3, n1, 6, l1, n1lrs, &dzz, &dPHI, 8);

    free_algebra(a);

    /** Netwok 1h
     *                  +-----+  RI +-----+
     *                  | r0  +-----+ r1  |
     *                  +--+--+     +--+--+
     *            1-34/    |           | 0-3 \
     *        +-----+      |           |      +-----+
     *        | r2  |  0-1 |       1-33|      | r3  |
     *        +-----+      |           |      +-----+
     *            1-33\    |           |  RI /
     *                  +-----+  RI +--+--+
     *                  | r4  +-----+ r5  |
     *                  +-----+     +-----+
     */


    struct disjunct dott = {.a = 1, .m = &thirtythree};
    struct disjunct dotf = {.a = 1, .m = &thirtyfour};

    l1[1].label = (metric) &dotf;
    l1[3].label = (metric) &dott;
    l1[5].label = (metric) &dott;

    DEBUG = 0;

    printf("\nDISJOINT(ADD, ADD, +32):\n");
    a0 = ADD(0, 32);
    a1 = ADD(32, 128);
    a = DISJOINT(a0, a1, &plus32);

    slc("1-69", a, 3, 2, n1, 6, l1, n1lrs, &dzz, &dPHI, 8);
    slc("PHI", a, 2, 3, n1, 6, l1, n1lrs, &dzz, &dPHI, 8);

    free_algebra(a);

    close_mal();

    for (int i = 0 ; i < 6; i++) // suppression of the routers
        free(n1[i]);

    return 0;
}
