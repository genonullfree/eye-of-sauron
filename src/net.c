#include "../include/net.h"
#include "../include/ops.h"

static struct netpoll _net;

uint8_t initialize_net()
{
    _net.name = "EYE";
    memcpy(_net.dev_name, "ens33", 6);
    //_net.local_ip = (union inet_addr)htonl(0x7f000001);
    _net.remote_ip = (union inet_addr)htonl(0xac103401);
    //_net.local_port = 1337;
    _net.remote_port = 1337;
    //memset(_net.remote_mac, 0xff, ETH_ALEN);
    _net.remote_mac[0] = 0x00;
    _net.remote_mac[1] = 0x50;
    _net.remote_mac[2] = 0x56;
    _net.remote_mac[3] = 0xc0;
    _net.remote_mac[4] = 0x00;
    _net.remote_mac[5] = 0x08;

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
