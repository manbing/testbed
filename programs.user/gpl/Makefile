CS_ROOT := $(word 1, $(subst /programs., /programs., $(shell pwd)))
include $(CS_ROOT)/mk/flags.mk

repository = ply

all: check_repository

check_repository: $(repository)

ply:
	git clone https://github.com/iovisor/ply.git;cd ply; \
	git branch develop; git checkout develop; cd ..;
	# ./autogen.sh
	# CC=/usr/bin/arm-linux-gnueabi-gcc ./configure --host=arm-linux --with-kerneldir=$(CS_ROOT)/programs.kernel/linux-4.11.4 \
	  alternative method, CC=$(CC) ./configure --host=arm-linux --with-kerneldir=$(CS_ROOT)/programs.kernel/linux-4.11.4)
	# make

.PHONY: all check_repository
