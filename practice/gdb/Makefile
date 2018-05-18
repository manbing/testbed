CC = arm-linux-gnueabi-gcc 
CFLAGS = -g -Og 
LDFLAGS = 
SOURCES = $(wildcard *.c)
#SOURCES = $(notdir $(file))
OBJS =  $(patsubst %.c,%.o, $(SOURCES))
APPLET = $(patsubst %.c,%, $(SOURCES))

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element
#	/usr/arm-linux-gnueabi/lib/

all: $(OBJS)
	$(CC) ${LDLAGS} -o $(APPLET) $^

$(OBJS): $(SOURCES) 
	$(CC) -M -c ${CFLAGS} $< > $@.cmd
	$(CC) -c ${CFLAGS} -o $@ $<

clean:
	rm -f $(OBJS)
	rm -f ${APPLET}
	rm -f *.cmd

install:
