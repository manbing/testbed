CROSS_COMPILE ?=
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
AR = $(CROSS_COMPILE)ar
OBJCOPY = $(CROSS_COMPILE)objcopy
GDB = $(CROSS_COMPILE)gdb
HOSTCC  = gcc

CFLAGS +=
LDFLAGS +=


install_library:
	$(Q)cp -rf /lib/x86_64-linux-gnu/* $(TARGET_DIR)/lib/ 
