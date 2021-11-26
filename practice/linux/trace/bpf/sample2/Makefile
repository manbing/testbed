CS_ROOT := $(word 1, $(subst /practice, /practice, $(shell pwd)))
include $(CS_ROOT)/mk/flags.mk

BIN = bpf_loader 
OBJS = test_bpf_user.o
CSRCS = 
SSRCS =

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
