#ifndef _ICMPV4_H_
#define _ICMPV4_H_

#include <ip.h>
#include <skbuff.h>
#include <stdint.h>

#define ICMP_V4_REPLY 0x00
#define ICMP_V4_DST_UNREACHABLE 0x03
#define ICMP_V4_SRC_QUENCH 0x04
#define ICMP_V4_REDIRECT 0x05
#define ICMP_V4_ECHO 0x08
#define ICMP_V4_TIMEOUT 0x0b
#define ICMP_V4_MALFORMED 0x0c

#define ICMP_V4_HEADER_SIZE 4

/**
 * ICMPv4 Protocol
 */
struct icmp_v4
{
    uint8_t type;
    uint8_t code;
    uint16_t check_sum;
    uint8_t *data;
};

int icmpv4_handle(struct ip *const ip_packet, struct skbuff *const buf);

#endif