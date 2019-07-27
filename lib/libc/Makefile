TOP_DIR:= ../..
include $(TOP_DIR)/mk/env.mk
include $(TOP_DIR)/arch.mk
include $(TOP_DIR)/mk/host.mk
include $(TOP_DIR)/mk/flags.mk

SRC_DIR = glibc
BUILD = build

all: $(BUILD)
	$(Q)$(MAKE) -C $(BUILD)

$(BUILD):
	mkdir $@; cd $@; \
        ../$(SRC_DIR)/configure --host=$(HOST) --prefix=$(TARGET_DIR) --disable-werror

clean:
	rm -rf $(BUILD)

install:
	$(Q)$(MAKE) -C $(BUILD) install

.PHONY: all clean install
