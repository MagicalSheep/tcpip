#include <icmpv4.h>
#include <ip.h>
#include <skbuff.h>
#include <stdlib.h>
#include <tun.h>
#include <unistd.h>
#include <utils.h>

extern int device_fd;

void buf_to_ip(struct skbuff *const buf, struct ip *const packet)
{
    packet->version = (buf->head[0] >> 4) & 0xf;
    packet->header_length = (buf->head[0] & 0xf) << 2;
    packet->service = buf->head[1];
    packet->length = ((buf->head[2]) << 8) & 0xff00;
    packet->length |= buf->head[3] & 0xff;
    packet->identifier = ((buf->head[4]) << 8) & 0xff00;
    packet->identifier |= buf->head[5] & 0xff;
    packet->flags = (buf->head[6] >> 5) & 7;
    packet->offset = ((buf->head[6]) << 8) | buf->head[7];
    packet->offset &= 0x1fff;
    packet->ttl = buf->head[8];
    packet->protocol = buf->head[9];
    packet->check_sum = ((buf->head[10]) << 8) & 0xff00;
    packet->check_sum |= buf->head[11] & 0xff;
    packet->src_ip = buf->head[12] << 24 | buf->head[13] << 16 | buf->head[14] << 8 | buf->head[15];
    packet->dst_ip = buf->head[16] << 24 | buf->head[17] << 16 | buf->head[18] << 8 | buf->head[19];
    packet->data = &buf->head[20];
}

int route(struct ip *const ip_packet, struct skbuff *const buf)
{
    int status = 0;

    if (ip_packet->dst_ip != parse_ipv4(HOST_IP))
    {
        info(NETWORK_LAYER, "Destination is not the host, ignored\n");
        goto end;
    }

    info(NETWORK_LAYER, "Destination is the host " HOST_IP "\n");

    buf->head += IP_HEADER_SIZE;
    buf->len -= IP_HEADER_SIZE;

    switch (ip_packet->protocol)
    {
    case IP_ICMP:
        info(NETWORK_LAYER, "Packet will be handled by ICMP protocol handler\n");
        status = icmpv4_handle(ip_packet, buf);
        goto end;
    case IP_TCP:
        info(NETWORK_LAYER, "Packet will enter the transport layer and be handled by TCP protocol handler\n");
        goto end;
    case IP_UDP:
        info(NETWORK_LAYER, "Packet will enter the transport layer and be handled by UDP protocol handler\n");
        goto end;
    default:
        info(NETWORK_LAYER, "Unsupported protocol, ignored\n");
    }
end:
    return status;
}

int ipintr()
{
    while (1)
    {
        struct skbuff *buf = (struct skbuff *)malloc(sizeof(struct skbuff));
        buf->head = buf->begin = (uint8_t *)malloc(MAX_BUFFER_SIZE);

        if (tun_read(device_fd, buf) < 0)
        {
            perror("Read packet error");
            close(device_fd);
            device_fd = -1;
            return -1;
        }

        struct ip *ip_packet = (struct ip *)malloc(sizeof(struct ip));
        buf_to_ip(buf, ip_packet);

        if (ip_packet->version != 4)
        {
            if (ip_packet->version == 6)
                info(NETWORK_LAYER, "Receive an IPv6 packet, ignored\n");
            goto end;
        }

        info(NETWORK_LAYER, "Receive an IPv4 packet\n");

        // Check packet

        // Route packet
        info(NETWORK_LAYER, "Check sum completed, begin to route packet\n");
        route(ip_packet, buf);

    end:
        info(NETWORK_LAYER, "Handle packet completed\n");
        free(ip_packet);
        free(buf->begin);
        free(buf);
    }
}