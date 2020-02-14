#ifndef _EYE_H
#define _EYE_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/notifier.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

/* Struct for remembering sysfs grp info */
struct module_sect_attr {
    struct module_attribute mattr;
    char *name;
    unsigned long address;
};

/* Struct for remembering module stats */
struct module_sect_attrs {
    struct attribute_group grp;
    unsigned int nsections;
    struct module_sect_attr attrs[0];
};

#endif
