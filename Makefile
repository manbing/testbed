include ./.config
include ./arch.mk
include ./mk/env.mk
include ./mk/host.mk
include ./mk/flags.mk

.EXPORT_ALL_VARIABLES:

PHONY =
PREBUILD_FILE = kernel_dir

all:
	$(Q)echo "Please follow below build flow:"
	$(Q)echo "-- make prebuild"
	$(Q)echo "-- make build"
	$(Q)echo "-- make install"
	$(Q)echo "-- make release"

prebuild: .config $(PREBUILD_FILE)
	$(Q)ln -sf -T  programs.user/gpl/qemu/qemu_$(CONFIG_ARCH).sh ./qemu.sh

build: build_kernel build_userspace build_library

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
	$(Q)rm -rf $@
	$(Q)mkdir $@

rootfs:
	rm -rf $@
	mkdir $@
	cd $@;		\
	mkdir -p proc sys dev etc/init.d;	\
	cp ../scripts/rootfs/rcS etc/init.d

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

release:
	cd rootfs; find . | cpio -o --format=newc > $(TOP_DIR)/image/rootfs.img

PHONY += all clean install library rootfs

.PHONY: $(PHONY)
