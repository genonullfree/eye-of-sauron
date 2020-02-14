eye-objs := ./src/eye.o ./src/ops.o
obj-m += eye.o

KVER = $(shell uname -r)
KDIR := /lib/modules/$(KVER)/build

all:
	make -C $(KDIR) M=$(PWD)

clean:
	rm -rf *.o *.ko *.symvers *.mod *.order *.unsigned .eye* *.mod.c
	rm -rf src/*.o src/*.mod.c src/.*.o.cmd
