CC = gcc 
LDFLAGS = 
CSRCS = ${APPLET}.c
OBJS =  ${APPLET}.o
SSRCS =  ${APPLET}.S
APPLET = quick_sort

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

all: $(SSRCS) $(OBJS)
	$(CC) ${LDLAGS} -o $(APPLET) $(OBJS)

$(OBJS): $(CSRCS) 
	$(CC) -M -c ${CFLAGS} $< > $@.cmd
	$(CC) -c ${CFLAGS} -o $@ $<

$(SSRCS): $(CSRCS)
	$(CC) -S ${CFLAGS} -o $@  $<

clean:
	rm -f $(OBJS)
	rm -f $(OBJS).cmd
	rm -f $(SSRCS)
	rm -f ${APPLET}
	rm -f *.cmd

install:

.PHONY: all clean install $(OBJS) $(SSRCS)
