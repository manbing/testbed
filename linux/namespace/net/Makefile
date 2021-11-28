CC = gcc 
CFLAGS = -g -O0 
LDFLAGS = 
SOURCES = ${BIN}.c
OBJS =  ${BIN}.o
SSRCS =  ${BIN}.S
BIN = netns

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

all: $(SSRCS) $(OBJS)
	$(CC) ${LDLAGS} -o $(BIN) $(OBJS)

$(OBJS): $(SOURCES) 
	$(CC) -M -c ${CFLAGS} $< > $@.cmd
	$(CC) -c ${CFLAGS} -o $@ $<

$(SSRCS): $(SOURCES)
	$(CC) -S ${CFLAGS} -o $@  $<

clean:
	rm -f $(OBJS)
	rm -f $(OBJS).cmd
	rm -f $(SSRCS)
	rm -f ${BIN}
	rm -f *.cmd

install:

.PHONY: all clean install $(OBJS) $(SSRCS)
