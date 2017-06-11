ROOTDIR:=$(word 1, $(subst /programs., /programs, $(shell pwd)))
#include $(ROOTDIR)/path.mk

DAEMON := IPCD 
DAEMON_SRCS := IPCD.c
DAEMON_OBJECT := $(patsubst %c, %o, $(DAEMON_SRCS))

CLIENT_NAME := IPCC
CLIENT_SRCS :=
CLIENT_OBJECT :=

CFLAGS := -Werror -I./include
LDFLAGS :=
LIBS :=


all: $(DAEMON) $(CLIENT)

$(DAEMON): $(DAEMON_OBJECT)
	gcc $(LDFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<
	

CLIENT:



clean:
	rm -rf *.o $(DAEMON)
