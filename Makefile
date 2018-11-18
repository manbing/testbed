NAME = rtos

PLAT ?= stm32p103
CMSIS = external/cmsis

include platform/$(PLAT)/Makefile

# arch-specific
SSRC += arch/v7m-head.S arch/v7m-entry.S arch/v7m-svcall.S
CSRC += arch/v7m-faults.c
CSRC += kernel/syscall.c
CSRC += \
        $(wildcard kernel/*.c) \
        $(wildcard kernel/mm/*.c) \
        libc/piko/stubs.c

OBJS += $(SSRC:.S=.o) $(CSRC:.c=.o)
OBJS := $(sort $(OBJS))

deps := $(OBJS:%.o=.%.o.d)

all: $(CMSIS)/$(PLAT) $(NAME).lds $(NAME).bin

#include generic build rules
include mk/flags.mk
include mk/rules.mk
include mk/cmsis.mk

clean:
	find . -name "*.o" -type f -delete
	find . -name "*.o.d" -type f -delete
	rm -f $(deps)
	find $(CMSIS) -name "*.o" -type f -delete
	rm -f $(NAME).map $(NAME).lds
	rm -f $(NAME).elf $(NAME).bin
# Remove GEN files
	rm -f include/kernel/syscalls.h
	rm -f kernel/syscall.c
	rm -f fs/version
