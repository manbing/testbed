proc-$(CONFIG_GPL_TCPDUMP)	+= gpl/tcpdump/

all:
	$(Q)for i in $(proc-y); \
                do $(MAKE) -C $$i; \
            done;
