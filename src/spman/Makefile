LIB := ../../lib/spman.lib
all: $(LIB)

CC := sdcc
AR := sdar
CFLAGS := -mz80 --Werror -I../../include --fsigned-char --std-sdcc99 --opt-code-speed

SOURCES := $(wildcard *.c)
OBJS := $(patsubst %.c,%.rel,$(SOURCES))

$(LIB): $(OBJS)
	$(AR) -rcD $(LIB) $(OBJS)

%.rel: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $<

.PHONY: clean
clean:
	rm -f $(OBJS) $(LIB)

