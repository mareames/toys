# 
# Makefile
#
# Copyright (c) 2020, Martin Reames
#

src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)

LDFLAGS = -lm 
CFLAGS = -g -O2
#CFLAGS = -g
CC = gcc

all: trees

trees: $(obj) 
	$(CC) $(CFLAGS) -o trees -o $@ $^ $(LDFLAGS)

-include $(dep)   # include dep files in the makefile

# generate dep files using the C preprocessor
%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) trees
	rm -f $(dep)

