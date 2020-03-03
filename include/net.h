#ifndef _NET_H
#define _NET_H

#include <linux/netpoll.h>
#include <linux/udp.h>
#include <linux/inet.h>
#include <linux/netdevice.h>

struct netdevs {
    char name[16];
    struct list_head node;
};

uint8_t scan_netdev(void);
uint8_t initialize_net(void);
uint8_t finalize_net(void);
uint8_t send_packet(char ch);

#endif
