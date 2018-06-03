CC = arm-linux-gnueabi-gcc 
#CC = gcc 
CFLAGS = -g -Og 
LDFLAGS = 
#SOURCES = $(wildcard *.c)
#SOURCES = $(notdir $(file))
SOURCES = ${APPLET}.c
#OBJS =  $(patsubst %.c,%.o, $(SOURCES))
OBJS =  ${APPLET}.o
#APPLET = $(patsubst %.c,%, $(SOURCES))
SSRCS =  ${APPLET}.S
APPLET = tmp3

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

all: $(SSRCS) $(OBJS)
	$(CC) ${LDLAGS} -o $(APPLET) $(OBJS)

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
