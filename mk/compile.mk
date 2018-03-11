CROSS_COMPILE = arm-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
AR = $(CROSS_COMPILE)ar
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJBUMP = $(CROSS_COMPILE)objdump 
READELF = $(CROSS_COMPILE)readelf 

CFLAGS_OPT = -Og

CFLAGS += \
    -std=c99 \
    -W -Wall \
    -g \
    $(CFLAGS_OPT)

LDFLAGS += \
