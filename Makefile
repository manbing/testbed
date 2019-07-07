include ./.config
include ./arch.mk
include ./kernel.mk
include ./mk/env.mk
include ./mk/host.mk
include ./mk/flags.mk

export ./.config

PHONY =

PREBUILD_FILE = kernel_dir

all: .config $(PREBUILD_FILE) build_kernel

%config:
	$(Q)$(MAKE) -C scripts/kconfig menuconfig
	$(Q)./scripts/kconfig/mconf Kconfig

arch.mk:
	$(Q)ln -sf -T arch/$(CONFIG_ARCH)/platform/$(CONFIG_CPU_ID)/arch.mk ./arch.mk

kernel.mk:
	$(Q)ln -sf -T arch/$(CONFIG_ARCH)/kernel.mk ./kernel.mk

kernel_dir:
	$(Q)ln -sf -T programs.kernel/core/$(CONFIG_KERNEL_CORE) kernel

mk/env.mk:
	$(Q)$(PYTHON) scripts/gen_mk.py --env

clean:

install:

PHONY += all clean install

.PHONY: $(PHONY)
