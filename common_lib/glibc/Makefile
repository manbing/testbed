####################################################################
TMP = $(subst learning_platform/,learning_platform/ ,$(shell pwd))
ROOT = $(word 1, $(TMP))
####################################################################
include $(ROOT)mk/compile.mk

	
SOURCES_FOLDER = glibc-2.26
BUILD_FOLDER = $(ROOT)common_lib/glibc/build/

all:
	@if [ -e ./$(SOURCES_FOLDER) ]; then \
		make -C $(BUILD_FOLDER); \
	else \
		tar zxvf  glibc-2.26.tar.gz; \
		mkdir $(BUILD_FOLDER); \
		cd $(BUILD_FOLDER);../$(SOURCES_FOLDER)/configure CC=$(CC) --host=arm-linux --prefix=$(BUILD_FOLDER);make; \
	fi

clean:

install:
