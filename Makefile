include ./.config
include ./arch.mk
include ./mk/env.mk
include ./mk/host.mk
include ./mk/flags.mk

export ./.config
export

PHONY =
PREBUILD_FILE = kernel_dir

all prebuild: .config $(PREBUILD_FILE)

build_all: build_kernel build_userspace build_library

include ./kernel.mk

%config:
	$(Q)$(MAKE) -C scripts/kconfig menuconfig
	$(Q)./scripts/kconfig/mconf Kconfig

arch.mk:
	$(Q)ln -sf -T arch/$(CONFIG_ARCH)/platform/$(CONFIG_CPU_ID)/arch.mk ./arch.mk

arch_build.mk:
	$(Q)ln -sf -T arch/$(CONFIG_ARCH)/platform/$(CONFIG_CPU_ID)/arch_build.mk ./arch_build.mk

kernel.mk:
	$(Q)ln -sf -T arch/$(CONFIG_ARCH)/kernel.mk ./kernel.mk

kernel_dir:
	$(Q)ln -sf -T programs.kernel/core/$(CONFIG_KERNEL_CORE) kernel

mk/env.mk:
	$(Q)$(PYTHON) scripts/gen_mk.py --env

image:
	$(Q)mkdir $@

rootfs:
	$(Q)rm -rf rootfs
	$(Q)mkdir rootfs
	$(Q)cd rootfs;		\
	$(Q)mkdir -p proc sys dev etc/init.d

build_userspace:
	$(Q)$(MAKE) -C programs.user 

install_userspace:
	$(Q)$(MAKE) -C programs.user install

clean_userspace:
	$(Q)$(MAKE) -C programs.user clean

build_library:
	$(Q)$(MAKE) -C lib 

install_library:
	$(Q)$(MAKE) -C lib install 

clean_library:
	$(Q)$(MAKE) -C lib clean 

clean: clean_kernel clean_userspace clean_library

install: image rootfs install_kernel install_userspace install_library

PHONY += all clean install rootfs library

.PHONY: $(PHONY)
