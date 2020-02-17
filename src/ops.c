#include "../include/eye.h"
#include "../include/ops.h"

/* Global Variables */
static char _pbuff[KERN_BUF] = "";
static unsigned long _pbuff_size = 0;

/* Function Prototypes */
void exec_commands(void);

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

    exec_commands();

    return _pbuff_size;
}

void exec_commands(void)
{
    int i = 0;

    if (_pbuff_size <= 0)
        return;

    do
    {
        if (strnstr(_cmd[i], _pbuff, MAX_CMD_LEN) != NULL)
            break;
        i++;
    }
    while (i < MAX_CMD);

    if (i == MAX_CMD)
        return;

    printk("Received %s command\n", _cmd[i]);

}
