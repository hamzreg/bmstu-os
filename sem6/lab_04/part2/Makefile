CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
# TARGET = fortunes
TARGET = seqfile
obj-m := $(TARGET).o

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order
	@rm -f .*.*.cmd *~ *.*~ TODO.*
	@rm -fR .tmp*
	@rm -rf .tmp_versions

disclean: clean
	@rm -f *.ko *.symvers *.mod

load:
	sudo insmod $(TARGET).ko

unload:
	sudo rmmod $(TARGET).ko

info:
	lsmod | grep $(TARGET)

logs:
	dmesg | grep +

show:
	ls -ail /proc | grep $(TARGET)
