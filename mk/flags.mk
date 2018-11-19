#CROSS_COMPILE ?= arm-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
AR = $(CROSS_COMPILE)ar
OBJCOPY = $(CROSS_COMPILE)objcopy
GDB = $(CROSS_COMPILE)gdb
HOSTCC  = gcc
WGET = wget
PYTHON ?= python3
QEMU_SYSTEM_ARM ?= /home/manbing/GitHub/Jserv/qemu_stm32/arm-softmmu/qemu-system-arm

CFLAGS_OPT = -O0

CFLAGS += \
    -W -Wall \
    -Iinclude -Iinclude/libc -I. \
    -I$(CS_ROOT)/kernel/include	 \
    -ggdb \
    $(CFLAGS_OPT)

LDFLAGS +=

#CFLAGS  += -mthumb -mcpu=$(CPU)
#LDFLAGS += -mthumb -march=$(ARCH)
