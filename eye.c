#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>


MODULE_AUTHOR("geno");
MODULE_LICENSE("GPL");

static int start_eye(void);
static void end_eye(void);

module_init(start_eye);
module_exit(end_eye);


static int start_eye(void)
{
    printk("The Eye of Sauron is upon you.\n");

    return 0;
}

static void end_eye(void)
{
    printk("You are free from the Eye.\n");
}
