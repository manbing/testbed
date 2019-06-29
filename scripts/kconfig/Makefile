# SPDX-License-Identifier: GPL-2.0
# ===========================================================================
HOSTCC = gcc

BIN = mconf
MCONF_SRC = mconf.c confdata.c expr.c lexer.lex.c parser.tab.c preprocess.c symbol.c
LXD_SRC = $(wildcard lxdialog/*.c)
LDFLAGS = -lncurses -ltinfo
CFLAGS = -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu89 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE 

OBJS = $(patsubst %.c,%.o, $(MCONF_SRC) $(LXD_SRC))


all menuconfig: $(OBJS) $(BIN)

include ../../mk/rules.mk

lexer.lex.c: lexer.l
	flex -o$@ -L $<

lexer.lex.o: parser.tab.h

parser.tab.h: parser.y
	bison -o/dev/null --defines=$@ -t -l $<

parser.tab.c: parser.y
	bison -o$@ -t -l $<

clean:
	rm -f $(OBJS) $(BIN) lexer.lex.c parser.tab.h parser.tab.c


.PHONY: all clean

