build_kernel: kernel/.config core

kernel/.config:	
	$(Q)cp -f arch/$(CONFIG_ARCH)/kernel_config $@
	$(Q)$(MAKE) -C $(KERNEL_DIR) olddefconfig

core:
	$(Q)$(MAKE) -C $(KERNEL_DIR) ARCH=$(CONFIG_ARCH) CROSS_COMPILE=$(CROSS_COMPILE)

module:
	$(Q)$(MAKE) -C $(KERNEL_DIR) ARCH=$(CONFIG_ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules
#$(Q)$(MAKE) ARCH=$(CONFIG_ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules_install INSTALL_MOD_PATH=$(TARGET_DIR)

clean_kernel:

clean_core:

clean_module:

PHONY += build_kernel core module clean_kernel clean_core clean_module
