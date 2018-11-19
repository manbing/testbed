CS_ROOT := $(word 1, $(subst /practice, /practice, $(shell pwd)))
include $(CS_ROOT)/mk/flags.mk

BIN = tracex1_user
OBJS = tracex1_user.o bpf_load.o
CSRCS = 
SSRCS =
CFLAGS = -I$(CS_ROOT)/kernel/samples/bpf \
         -I$(CS_ROOT)/kernel/tools/lib



#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

all: $(OBJS)
	$(CC) ${LDFLAGS} -o $(BIN) $(OBJS)

include $(CS_ROOT)/mk/rules.mk

clean:
	rm -f $(OBJS)
	rm -f $(OBJS).cmd
	rm -f ${BIN}
	rm -f *.cmd

install:

.PHONY: all clean install
