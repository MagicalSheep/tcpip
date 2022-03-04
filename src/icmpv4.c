#include <icmpv4.h>
#include <stdlib.h>
#include <string.h>

void buf_to_icmpv4(struct skbuff *const buf, struct icmp_v4 *const packet)
{
    packet->type = buf->head[0];
    packet->code = buf->head[1];
    packet->check_sum = ((buf->head[2]) << 8) & 0xff00;
    packet->check_sum |= buf->head[3] & 0xff;
    packet->data = &buf->head[4];
}

void icmpv4_to_buf(struct icmp_v4 *const packet, struct skbuff *const buf)
{
    buf->head[0] = packet->type;
    buf->head[1] = packet->code;
    buf->head[2] = packet->check_sum >> 8;
    buf->head[3] = packet->check_sum & 0xff;
    memcpy(&buf->head[4], packet->data, buf->len - ICMP_V4_HEADER_SIZE);
}

/**
 * ICMPv4 is on the network layer, so the ip packet header information is needed.
 * Attention: ICMP is on the same layer as IP
 */
int icmpv4_handle(struct ip *const ip_packet, struct skbuff *const buf)
{
    int status = 0;
    struct icmp_v4 *packet = (struct icmp_v4 *)malloc(sizeof(struct icmp_v4));
    buf_to_icmpv4(buf, packet);

    switch (packet->type)
    {
    case ICMP_V4_ECHO:
    {
        packet->type = 0;
        icmpv4_to_buf(packet, buf);
        struct ip_opt *opt = (struct ip_opt *)malloc(sizeof(struct ip_opt));
        opt->protocol = IP_ICMP;
        opt->length = ip_packet->length;
        opt->dst_ip = ip_packet->src_ip;
        ip_output(buf, opt);
        free(opt);
        goto end;
    }
    default:
        status = -1;
    }
end:;
    free(packet);
    return status;
}