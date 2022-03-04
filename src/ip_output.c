#include <ip.h>
#include <skbuff.h>
#include <tun.h>
#include <utils.h>

extern int device_fd;

int ip_output(struct skbuff *const buf, const struct ip_opt *const opt)
{
    buf->head -= IP_HEADER_SIZE;
    buf->len += IP_HEADER_SIZE;
    buf->head[2] = (opt->length >> 8) & 0xff;
    buf->head[3] = opt->length & 0xff;
    buf->head[8] = HOST_TTL;
    buf->head[9] = opt->protocol;
    buf->head[16] = (opt->dst_ip >> 24) & 0xff;
    buf->head[17] = (opt->dst_ip >> 16) & 0xff;
    buf->head[18] = (opt->dst_ip >> 8) & 0xff;
    buf->head[19] = opt->dst_ip & 0xff;

    // choose routing, finish source ip
    uint32_t src_ip = parse_ipv4(HOST_IP);
    buf->head[12] = (src_ip >> 24) & 0xff;
    buf->head[13] = (src_ip >> 16) & 0xff;
    buf->head[14] = (src_ip >> 8) & 0xff;
    buf->head[15] = src_ip & 0xff;

    // send to data link layer
    tun_write(device_fd, buf);

    info(NETWORK_LAYER, "Send an IP packet successfully(%hhu.%hhu.%hhu.%hhu --> %hhu.%hhu.%hhu.%hhu)\n",
         buf->head[12], buf->head[13], buf->head[14], buf->head[15],
         buf->head[16], buf->head[17], buf->head[18], buf->head[19]);
}