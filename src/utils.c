#include <utils.h>

uint32_t parse_ipv4(char *ip_addr)
{
    uint8_t ip[4];
    sscanf(ip_addr, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
    return ip[0] << 24 | ip[1] << 16 | ip[2] << 8 | ip[3];
}

void ipv4_to_str(const uint32_t ip_addr, char *str)
{
    str[0] = (ip_addr >> 24) & 0xff;
    str[1] = (ip_addr >> 16) & 0xff;
    str[2] = (ip_addr >> 8) & 0xff;
    str[3] = ip_addr & 0xff;
}

// FOR DEBUG
void print_ip_packet(const struct ip *const packet)
{
    printf("Version: %u\nHeader Length: %u\n", packet->version, packet->header_length);
    printf("Service: %u\nLength: %u\n", packet->service, packet->length);
    printf("Identifier: %u\nFlags: ", packet->identifier);
    for (int i = 2; i >= 0; i--)
        printf((packet->flags & (1 << i)) ? "1" : "0");
    printf("\n");
    printf("Offset: %u\nTTL: %u\n", packet->offset, packet->ttl);
    printf("Protocol: %u\nCheck Sum: %u\n", packet->protocol, packet->check_sum);
    printf("Src ip: ");
    for (int i = 24; i >= 0; i -= 8)
    {
        printf("%u", (packet->src_ip >> i) & 0xff);
        if (i)
            printf(".");
    }
    printf("\nDst ip: ");
    for (int i = 24; i >= 0; i -= 8)
    {
        printf("%u", (packet->dst_ip >> i) & 0xff);
        if (i)
            printf(".");
    }
    printf("\nData: \n");
    print_buf(packet->data, packet->length - packet->header_length);
    printf("\n\n");
}

// FOR DEBUG
void print_buf(const uint8_t *const buf, const uint32_t len)
{
    for (int i = 0; i < len; i++)
    {
        for (int j = 7; j >= 0; j--)
            printf((buf[i] & (1 << j)) ? "1" : "0");
        printf(" ");
    }
}