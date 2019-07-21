lib-$(CONFIG_LIB_GLIBC)	+= libc

all:
	$(Q)for i in $(lib-y); \
                do $(MAKE) -C $$i; \
            done;

clean:

install:
	$(Q)for i in $(lib-y); \
                do $(MAKE) -C $$i install; \
            done;


.PHONY: all clean install
