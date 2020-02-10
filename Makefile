modname := eye
obj-m := $(modname).o

eye-objs := eye.o ops.o

KVER = $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build

all:
	make -C $(KDIR) M=$(PWD) modules
