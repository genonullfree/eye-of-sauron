#ifndef _NET_H
#define _NET_H

#include <linux/netpoll.h>
#include <linux/udp.h>
#include <linux/inet.h>

uint8_t initialize_net(void);
uint8_t finalize_net(void);
uint8_t send_packet(char ch);

#endif
