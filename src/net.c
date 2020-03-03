#include "../include/net.h"
#include "../include/ops.h"

static struct netpoll _net;

uint8_t initialize_net()
{

    struct net_device *dev;
    read_lock(&dev_base_lock);
    dev = first_net_device(&init_net);
    while (dev)
    {
        printk(KERN_INFO "found %s\n", dev->name);
        dev = next_net_device(dev);
    }
    read_unlock(&dev_base_lock);

    _net.name = "EYE";
    memcpy(_net.dev_name, "ens33", 6);
    _net.remote_ip = (union inet_addr)htonl(0xffffffff);
    _net.remote_port = 1337;
    memset(_net.remote_mac, 0xff, ETH_ALEN);

    /* debug print options */
    netpoll_print_options(&_net);
    if (netpoll_setup(&_net) < 0)
    {
        printk(KERN_ERR "error with netpoll setup");
        return 1;
    }

    return 0;
}

uint8_t finalize_net()
{
    netpoll_cleanup(&_net);
    return 0;
}

uint8_t send_packet(char ch)
{
    char buff[16];
    strncpy(buff, &ch, 15);
    netpoll_send_udp(&_net, buff, strnlen(buff, 16));
    return 0;

}
