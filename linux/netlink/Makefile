PWD         := $(shell pwd) 
KVERSION    := $(shell uname -r)
KERNEL_DIR   = /usr/src/linux-headers-$(KVERSION)/

MODULE_NAME  = multicast
obj-m       := $(MODULE_NAME).o
$(MODULE_NAME)-objs := 

all: multicast

multicast: multicast.ko multicast_subscribe

multicast.ko:
	make -C $(KERNEL_DIR) M=$(PWD) modules

multicast_subscribe: multicast_subscribe.c
	$(Q)gcc -o $@ $<

clean:
	rm -rf multicast.ko multicast_subscribe
	make -C $(KERNEL_DIR) M=$(PWD) clean

.PHONY: all clean multicast
