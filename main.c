#include <ip.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <stdlib.h>
#include <tun.h>
#include <utils.h>

extern int device_fd;

int main()
{
    char *name = malloc(IFNAMSIZ);
    name[0] = '\0';
    device_fd = tun_alloc(name, IFF_TUN | IFF_NO_PI, HOST_GATEWAY);
    if (device_fd < 0)
    {
        perror("Alloc tun/tap device failed");
        exit(1);
    }
    info(PHYSICAL_LAYER, "Interface %s is working\n", name);
    info(NETWORK_LAYER, "The host IP address is " HOST_IP "\n");

    ipintr();

    return 0;
}