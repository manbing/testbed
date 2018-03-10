CC = arm-linux-gnueabi-gcc 
CFLAGS = -g -Og 
LDFLAGS = 
SOURCES = corrupt_stack.c
OBJS =  $(patsubst %c,%o,${SOURCES})
APPLET = corrupt_stack

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

all: $(OBJS)
	
	$(CC) ${LDLAGS} -o $(APPLET) $^

%.o: $(SOURCES) 
	$(CC) -c ${CFLAGS} -o $@ $<

clean:
	rm $(OBJS)
	rm ${APPLET}

install:
