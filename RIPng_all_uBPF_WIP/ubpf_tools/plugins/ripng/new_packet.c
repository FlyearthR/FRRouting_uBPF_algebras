#include"structures.h"
/*
extern void log_str(char *str);
extern struct interface* cust_if_lookup_by_index(ifindex_t ifindex, struct ripng* ripng);
extern const char *inet6_ntoa(struct in6_addr addr);
extern void ripng_peer_bad_packet(struct ripng *ripng, struct sockaddr_in6 *from);
extern void ripng_packet_dump(struct ripng_packet *packet, int size, const char *sndrcv);
static void ripng_request_process(struct ripng_packet *packet, int size, struct sockaddr_in6 *from, struct interface *ifp);
static void ripng_response_process(struct ripng_packet *packet, int size, struct sockaddr_in6 *from, struct interface *ifp, int hoplimit);

uint64_t ism_change_state(bpf_full_args_t *data)
{
    bpf_args_t *args = data->args;

    unsigned int len = (unsigned int) args[0].arg;
    struct ripng *ripng = args[1].arg;
    ifindex_t ifindex = (ifindex_t ) args[2].arg;
    struct sockaddr_in6 from = (struct sockaddr_in6) *((struct sockaddr_in6*) args[3].arg);
    int hoplimit = (int) args[4].arg;
    char* vrf = args[5].arg;
    uint8_t version = (uint8_t) args[6].arg;
    unsigned char *rdata = (unsigned char*) args[7].arg;


    struct ripng_packet *packet;
    struct interface *ifp;
    char str[100];

    *//* Check RTE boundary.  RTE size (Packet length - RIPng header size
    (4)) must be multiple size of one RTE size (20).
    if (((len - 4) % 20) != 0) {
        PRINT3(str, "RIPng invalid packet size %d from %s", " (VRF %s)", len, inet6_ntoa(from.sin6_addr), vrf);
        log_str(str);
        ripng_peer_bad_packet(ripng, &from);
        return 0;
    }

    packet = (struct ripng_packet *) (rdata);
    ifp = cust_if_lookup_by_index(ifindex, ripng);

    *//* RIPng packet received.
    PRINT4(str, "RIPng packet received from %s port %d on", " %s (VRF %s)", inet6_ntoa(from.sin6_addr),
            ntohs(from.sin6_port), ifp ? ifp->name : "unknown", vrf);
    log_str(str);

    *//* Logging before packet checking.

    ripng_packet_dump(packet, len, "RECV");

    *//* Packet comes from unknown interface.
    if (ifp == NULL) {
        PRINT(str, "RIPng packet comes from unknown interface %d (VRF %s)", ifindex, vrf);
        log_str(str);
        return 0;
    }

    *//* Packet version mismatch checking.
    if (packet->version != version) {
        PRINT3(str, "RIPng packet version %d doesn't fit to my version %d", " (VRF %s)", packet->version, version, vrf);
        log_str(str);
        ripng_peer_bad_packet(ripng, &from);
        return 0;
    }

    *//* Process RIPng packet.
    switch (packet->command) {
        case RIPNG_REQUEST:
            ripng_request_process(packet, len, &from, ifp);
            break;
        case RIPNG_RESPONSE:
            ripng_response_process(packet, len, &from, ifp, hoplimit);
            break;
        default:
            PRINT(str, "Invalid RIPng command %d (VRF %s)", packet->command, vrf);
            log_str(str);
            ripng_peer_bad_packet(ripng, &from);
            break;
    }
    return 0;
}*/