proc-$(CONFIG_GPL_TCPDUMP)	+= gpl/tcpdump
proc-$(CONFIG_GPL_BUSYBOX)	+= gpl/busybox

all:
	$(Q)for i in $(proc-y); \
                do $(MAKE) -C $$i; \
            done;

clean:

install:
	$(Q)for i in $(proc-y); \
                do $(MAKE) -C $$i install; \
            done;


.PHONY: all clean install
