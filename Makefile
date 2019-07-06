include ./.config
include ./mk/host.mk
include ./mk/flags.mk
export ./.config

PREBUILD_FILE = kernel arch.mk mk/env.mk

#include platform/$(PLAT)/Makefile

all: .config $(PREBUILD_FILE)

%config:
	$(Q)$(MAKE) -C scripts/kconfig menuconfig
	$(Q)./scripts/kconfig/mconf Kconfig

$(PREBUILD_FILE):
	$(Q)$(MAKE) prebuild

prebuild:
	$(Q)ln -sf -T programs.kernel/core/$(CONFIG_KERNEL_CORE) kernel
	$(Q)ln -sf -T arch/$(CONFIG_ARCH)/platform/$(CONFIG_CPU_ID)/arch.mk ./arch.mk
	$(Q)$(PYTHON) scripts/gen_mk.py --env

clean:

install:

.PHONY: all clean install prepare 
