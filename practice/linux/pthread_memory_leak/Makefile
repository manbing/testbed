CC = gcc 
CFLAGS = -g -Og 
LDFLAGS = -pthread 
SOURCES = ${APPLET}.c
OBJS =  ${APPLET}.o
SSRCS =  ${APPLET}.S
APPLET = pthread_memory_leak

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

#CFLAGS += -DJOIN

all: $(SSRCS) $(OBJS)
	$(CC) ${LDFLAGS} -o $(APPLET) $(OBJS)

$(OBJS): $(SOURCES) 
	$(CC) -M -c ${CFLAGS} $< > $@.cmd
	$(CC) -c ${CFLAGS} -o $@ $<

$(SSRCS): $(SOURCES)
	$(CC) -S ${CFLAGS} -o $@  $<

clean:
	rm -f $(OBJS)
	rm -f $(OBJS).cmd
	rm -f $(SSRCS)
	rm -f ${APPLET}
	rm -f *.cmd

install:

.PHONY: all clean install $(OBJS) $(SSRCS)
