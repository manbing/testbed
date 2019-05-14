CROSS_COMPILE ?= 
CC := ${CROSS_COMPILE}gcc 
AS := ${CROSS_COMPILE}as
CPU := 
ARCH := 

CFLAGS = -g -Og 
LDFLAGS = 
SOURCES = get_ip_addr.c
OBJS =  $(patsubst %.c,%.o, $(SOURCES))
APPLET = $(patsubst %.c,%, $(SOURCES))

CFLAGS  +=
LDFLAGS += 

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

.PHONY: all clean install

all: $(OBJS)
	$(CC) ${LDFLAGS} -o $(APPLET) $(OBJS)

$(OBJS): $(SOURCES)
	$(CC) -M -c ${CFLAGS} $< > $@.cmd
	$(CC) -c ${CFLAGS} -o $@ $<

clean:
	rm -f $(OBJS)
	rm -f ${APPLET}
	rm -f *.cmd

install:
