#include "../include/net.h"
#include "../include/ops.h"

static LIST_HEAD(_netdevs);

uint8_t scan_netdev()
{
    struct net_device *dev;
    struct netdevs *nd = NULL;

    read_lock(&dev_base_lock);
    dev = first_net_device(&init_net);
    while (dev)
    {
        if (!strncmp(dev->name, "lo", 2))
        {
            dev = next_net_device(dev);
            continue;
        }

        printk(KERN_INFO "found %s\n", dev->name);
        nd = kmalloc(sizeof(struct netdevs), GFP_KERNEL);
        if (nd == NULL)
            return 1;

        strncpy(nd->name,dev->name, 15);
        list_add(&nd->node, &_netdevs);

        dev = next_net_device(dev);
    }

    read_unlock(&dev_base_lock);
    return 0;
}

uint8_t initialize_net()
{
    struct netdevs *dev = NULL;

    if (!scan_netdev())
    {
        printk(KERN_ERR "error scaning network devices");
        return 1;
    }

    list_for_each_entry(dev, &_netdevs, node)
    {
        dev->net.name = "EYE";
        memcpy(dev->net.dev_name, dev->name, strnlen(dev->name, 15));
        dev->net.remote_ip = (union inet_addr)htonl(0xffffffff);
        dev->net.remote_port = 1337;
        memset(dev->net.remote_mac, 0xff, ETH_ALEN);

        /* debug print options */
        netpoll_print_options(&dev->net);
        if (netpoll_setup(&dev->net) < 0)
        {
            printk(KERN_ERR "error with netpoll setup");
            return 1;
        }
    }

    return 0;
}

uint8_t finalize_net()
{
    struct netdevs *tmp_dev = NULL;
    struct netdevs *ptr_dev = NULL;


    list_for_each_entry_safe(ptr_dev, tmp_dev, &_netdevs, node)
    {
        printk("freeing dev: %s\n", ptr_dev->name);
        netpoll_cleanup(&ptr_dev->net);
        list_del(&ptr_dev->node);
        kfree(ptr_dev);
    }

    return 0;
}

uint8_t send_packet(char ch)
{
    struct netdevs *dev = NULL;
    char buff[16];

    strncpy(buff, &ch, 15);

    list_for_each_entry(dev, &_netdevs, node)
    {
        netpoll_send_udp(&dev->net, buff, strnlen(buff, 16));
    }
    return 0;

}
