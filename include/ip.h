#ifndef _IP_H_
#define _IP_H_

#include <skbuff.h>
#include <stdint.h>

#define IP_ICMP 0x01
#define IP_TCP 0x04
#define IP_UDP 0x11

#define IP_HEADER_SIZE 20

/**
 * IP Protocol
 */
struct ip
{
    uint8_t version;
    uint8_t header_length;
    uint8_t service;
    uint16_t length;
    uint16_t identifier;
    uint8_t flags;
    uint16_t offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check_sum;
    uint32_t src_ip;
    uint32_t dst_ip;
    uint8_t *data;
};

/**
 * Options for ip_output
 */
struct ip_opt
{
    uint16_t length;
    uint8_t protocol;
    uint32_t dst_ip;
};

/**
 * Read ip packet from data link layer
 */
int ipintr();

int ip_output(struct skbuff *const buf, const struct ip_opt *const opt);

#endif