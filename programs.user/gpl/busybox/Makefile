TOP_DIR:=$(subst /programs., /programs., $(shell pwd))
TOP_DIR:=$(word 1, $(TOP_DIR))

include $(TOP_DIR)/mk/env.mk
include $(TOP_DIR)/arch.mk
include $(TOP_DIR)/mk/host.mk

SOURCE = busybox-1.26.2 

all:
	$(Q)$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(SOURCE) defconfig
	$(Q)$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(SOURCE)

install:
	$(Q)$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(SOURCE) install CONFIG_PREFIX=$(TARGET_DIR)

clean:
	$(Q)$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(SOURCE) clean
	
.PHONY: all install clean
