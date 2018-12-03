CS_ROOT := $(word 1, $(subst /programs., /programs., $(shell pwd)))
include $(CS_ROOT)/mk/flags.mk

repository = ply

all: check_repository

check_repository: $(repository)

ply:
	cd $@; \
        ./autogen.sh; \
	CC=$(CC) ./configure --host=arm-linux --with-kerneldir=$(CS_ROOT)/programs.kernel/linux-4.11.4; \
	make
	@#CC=$(CC) ./configure --host=arm-linux --with-kerneldir=$(CS_ROOT)/programs.kernel/linux-4.11.4

llvm:
	# cmake -DCMAKE_CROSSCOMPILING=True -DCMAKE_TOOLCHIAIN_FILE=/home/manbing/GitHub/Manbing/testbed/programs.user/gpl/tool_chain.cmake -DLLVM_TARGETS_TO_BUILD="BPF;X86;ARM" ..

.PHONY: all check_repository ply llvm
