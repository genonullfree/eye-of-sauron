#ifndef _OPS_H
#define _OPS_H

#define KERN_BUF 1024
#define MAX_CMD_LEN 6

static __attribute__((unused)) char *_cmd[] = {
    "hide",
    "unhide",
};

enum {
    HIDE,
    UNHIDE,
    MAX_CMD
};

int ops_open(struct inode *ino, struct file *fi);
ssize_t ops_write(struct file *fp, const char *buffer, unsigned long count, loff_t *offset);

#endif
