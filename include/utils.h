#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdint.h>
#include <stdio.h>
#include <ip.h>
#define PHYSICAL_LAYER "\033[0m\033[1;31m[Physical Layer] \033[0m"
#define DATA_LINK_LAYER "\033[0m\033[1;32m[Data Link Layer] \033[0m"
#define NETWORK_LAYER "\033[0m\033[1;35m[Network Layer] \033[0m"
#define TRANSPORT_LAYER "\033[0m\033[1;34m[Transport Layer] \033[0m"
#define DEBUG "[Debug] "

#define HOST_GATEWAY "192.168.1.1/24"
#define HOST_IP "192.168.1.2"
#define HOST_TTL 64

#define info(prefix, msg, ...) \
    printf(prefix msg, ##__VA_ARGS__)
#define debug(msg, ...) \
    printf(DEBUG msg, ##__VA_ARGS__)
#define print_ip(ip)                                           \
    {                                                          \
        char str[4];                                           \
        ipv4_to_str(ip, str);                                  \
        printf("%u.%u.%u.%u", str[0], str[1], str[2], str[3]); \
    }

uint32_t parse_ipv4(char *ip_addr);

void ipv4_to_str(const uint32_t ip_addr, char *str);

void print_ip_packet(const struct ip *const packet);

void print_buf(const uint8_t *const buf, const uint32_t len);

#endif