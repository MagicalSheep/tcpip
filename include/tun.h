#ifndef _TUN_H_
#define _TUN_H_
#include <skbuff.h>
#include <utils.h>

/**
 * Alloc a tun/tap interface and set its ip address
 *
 * @param name interface name
 * @param flags tun/tap flags
 * @param ip ip address
 * @return fd
 */
int tun_alloc(char *name, int flags, char *ip);

/**
 * Read an ip packet from tun interface
 *
 * @param fd interface fd
 * @param buf buffer
 * @return status
 */
int tun_read(const int fd, struct skbuff *buf);

int tun_write(const int fd, struct skbuff *buf);

#endif