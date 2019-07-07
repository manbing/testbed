build_kernel: kernel/.config core module

kernel/.config:	
	$(Q)cp -f arch/$(CONFIG_ARCH)/kernel_config $@
	$(Q)$(MAKE) -C $(KERNEL_DIR) olddefconfig

core: image
	$(Q)$(MAKE) -C $(KERNEL_DIR) ARCH=$(CONFIG_ARCH) CROSS_COMPILE=$(CROSS_COMPILE)

module:
	$(Q)$(MAKE) -C $(KERNEL_DIR) ARCH=$(CONFIG_ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

install_kernel: install_core install_module

install_core:
	$(Q)cp $(KERNEL_DIR)/arch/x86/boot/bzImage ./image/

install_module:
	$(Q)$(MAKE) -C $(KERNEL_DIR) ARCH=$(CONFIG_ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules_install INSTALL_MOD_PATH=$(TARGET_DIR)

clean_kernel:

clean_core:

clean_module:

PHONY += build_kernel core module clean_kernel clean_core clean_module install_kernel install_core install_module
