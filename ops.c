#include "eye.h"
#include "ops.h"

/* Global Variables */
static char _pbuff[KERN_BUF] = "";
static unsigned long _pbuff_size = 0;

int ops_show(struct seq_file *sf, void *v)
{
    seq_printf(sf, "0\n");
    return 0;
}

int ops_open(struct inode *ino, struct file *fi)
{
    return single_open(fi, ops_show, NULL);
}

ssize_t ops_write(struct file *fp, const char *buffer, unsigned long count, loff_t *offset)
{
    _pbuff_size = count;
    if (_pbuff_size > KERN_BUF)
        _pbuff_size = KERN_BUF;

    if (copy_from_user(_pbuff, buffer, _pbuff_size))
        return -EFAULT;

    _pbuff[_pbuff_size - 1] = 0;

    printk("Received cmd: %s\n", _pbuff);

    return _pbuff_size;
}

