#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <tun.h>
#include <unistd.h>

// The host interface fd
int device_fd = -1;

int tun_alloc(char *name, int flags, char *ip)
{
    struct ifreq ifr;
    int fd, err;
    char *clonedev = "/dev/net/tun";
    char cmd[50];

    if ((fd = open(clonedev, O_RDWR)) < 0)
        return fd;

    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags |= flags;
    if (*name)
        strncpy(ifr.ifr_name, name, IFNAMSIZ);

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0)
    {
        close(fd);
        return err;
    }

    strcpy(name, ifr.ifr_name);

    cmd[0] = '\0';
    strcat(cmd, "ip addr add ");
    strcat(cmd, ip);
    strcat(cmd, " dev ");
    strcat(cmd, name);
    FILE *fp = popen(cmd, "r");
    if (fp == NULL)
        return -1;
    info(PHYSICAL_LAYER, "IP address of interface %s is set to %s\n", name, ip);

    cmd[0] = '\0';
    strcat(cmd, "ip link set ");
    strcat(cmd, name);
    strcat(cmd, " up");
    fp = popen(cmd, "r");
    if (fp == NULL)
        return -1;
    info(PHYSICAL_LAYER, "Successfully enable the interface %s\n", name);

    return fd;
}

int tun_read(const int fd, struct skbuff *buf)
{
    if ((buf->len = read(fd, buf->begin, MAX_BUFFER_SIZE)) < 0)
        return -1;
    buf->tail = buf->end = buf->begin + buf->len - 1;
    return 0;
}

int tun_write(const int fd, struct skbuff *buf)
{
    return write(fd, buf->begin, buf->len);
}