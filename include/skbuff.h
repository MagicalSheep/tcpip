#ifndef _SKBUFF_H_
#define _SKBUFF_H_

#include <stdint.h>

#define MAX_BUFFER_SIZE 1500

/**
 * Packet
 */
struct skbuff
{
    uint8_t *begin; // packet head
    uint8_t *head;  // packet payload head
    uint8_t *tail;  // packet payload end
    uint8_t *end;   // packet end
    uint32_t len;   // packet length
};

#endif