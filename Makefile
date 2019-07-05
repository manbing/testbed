include ./mk/env.mk
include ./.config
export ./.config

PREBUILD_FILE = kernel arch.mk mk/env.mk

MAKE = make
Q = @

#include platform/$(PLAT)/Makefile

all: .config $(PREBUILD_FILE)

%config:
	$(Q)$(MAKE) -C scripts/kconfig menuconfig
	./scripts/kconfig/mconf Kconfig

$(PREBUILD_FILE):
	make prebuild

prebuild:
	ln -sf programs.kernel/core/$(CONFIG_KERNEL_CORE) kernel
	ln -sf arch/$(CONFIG_ARCH)/platform/$(CONFIG_CPU_ID)/arch.mk arch.mk
	$(PYTHON) gen_mk.py --env

clean:

install:

.PHONY: all clean install prepare 
