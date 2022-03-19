obj-m += md1.o md2.o md3.o

CURRENT = $(shell uname -r)
KDIR ?= /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)

all default: modules
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KDIR) M=$(PWD) $@

clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order
	@rm -f .*.*.cmd *~ *.*~ TODO.*
	@rm -fR .tmp*
	@rm -rf .tmp_versions

disclean: clean
	@rm -f *.ko *.symvers *.mod

load:
	sudo insmod md1.ko
	sudo insmod md2.ko
	sudo insmod md3.ko

unload:
	sudo rmmod md2.ko
	sudo rmmod md1.ko

info:
	lsmod | grep md

logs:
	dmesg | grep -
