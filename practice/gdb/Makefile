CC = arm-linux-gnueabi-gcc 
CFLAGS = -g -Og 
LDFLAGS = 
SOURCES = $(wildcard *.c)
#SOURCES = $(notdir $(file))
OBJS =  $(patsubst %.c,%.o, $(SOURCES))
APPLET = corrupt_stack

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

all: $(OBJS)
	$(CC) ${LDLAGS} -o $(APPLET) $^

%.o: $(SOURCES) 
	$(CC) -M -c ${CFLAGS} $< > $@.cmd
	$(CC) -c ${CFLAGS} -o $@ $<

clean:
	rm -f $(OBJS)
	rm -f ${APPLET}
	rm -f *.cmd

install:
