CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
TARGET = my_vfs
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

create_files:
	touch image
	mkdir dir

clean_files:
	rm image
	rm -rf dir

load:
	sudo insmod $(TARGET).ko

unload:
	sudo rmmod $(TARGET).ko

info:
	lsmod | grep $(TARGET)

logs:
	dmesg | grep $(TARGET)

mount_vfs:
	sudo mount -o loop -t $(TARGET) ./file ./dir

unmount:
	sudo umount ./dir
