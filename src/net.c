#include "../include/net.h"
#include "../include/ops.h"

static struct netpoll _net;

uint8_t initialize_net()
{
    _net.name = "EYE";
    memcpy(_net.dev_name, "ens33", 6);
    _net.local_ip = (union inet_addr)htonl(0x7f000001);
    _net.remote_ip = (union inet_addr)htonl(0xffffffff);
    _net.local_port = 1337;
    _net.remote_port = 1337;
    memset(_net.remote_mac, 0xff, ETH_ALEN);

    netpoll_print_options(&_net);
    netpoll_setup(&_net);

    return 0;
}

uint8_t send_packet(uint8_t ch)
{
    netpoll_send_udp(&_net, &ch, 1);
    return 0;

}
