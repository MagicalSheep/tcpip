#ifndef _UDP_H_
#define _UDP_H_
#include <stdint.h>

struct udp_packet{
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t check_sum;
    uint8_t *data;
};

#endif