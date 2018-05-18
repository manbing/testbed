CROSS_COMPILE ?= arm-linux-gnueabi-
#CROSS_COMPILE ?= arm-none-eabi-
CC := ${CROSS_COMPILE}gcc 
AS := ${CROSS_COMPILE}as
CPU := cortex-a9 
ARCH := armv7-m

CFLAGS = -g -Og 
LDFLAGS = 
SOURCES = arm4.c
#SOURCES = $(notdir $(file))
OBJS =  $(patsubst %.c,%.o, $(SOURCES))
APPLET = $(patsubst %.c,%, $(SOURCES))

CFLAGS  += -mthumb -mcpu=$(CPU)
#LDFLAGS += -mthumb -march=$(ARCH)

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

.PHONY: all clean install

all: $(OBJS)
	$(CC) ${LDFLAGS} -o $(APPLET) $(OBJS)

$(OBJS): $(SOURCES)
	@echo objs is $(OBJS)
	$(CC) -M -c ${CFLAGS} ${CORTEX_M3} $< > $@.cmd
	$(CC) -c ${CFLAGS} ${CORTEX_M3} -o $@ $<

clean:
	rm -f $(OBJS)
	rm -f ${APPLET}
	rm -f *.cmd

install:
