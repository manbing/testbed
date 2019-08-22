CC = gcc 
CFLAGS = -g -Og 
LDFLAGS = 
SOURCES = $(wildcard *.c)
#SOURCES = $(notdir $(file))
OBJS =  $(patsubst %.c,%.o, $(SOURCES))
BIN = $(patsubst %.c,%, $(SOURCES)) 

#	$@ => target
#	$< => first dependency element
#	$^ => all dependency element

all: $(OBJS)
	$(CC) ${LDLAGS} -o $(BIN) $^

$(OBJS): $(SOURCES) 
	$(CC) -M -c ${CFLAGS} $< > .$@.cmd
	$(CC) -c ${CFLAGS} -o $@ $<

clean:
	rm -f $(OBJS)
	rm -f ${BIN}
	rm -f *.cmd
	rm -f .*.cmd

install:
