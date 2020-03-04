# 
# Makefile
#
# Copyright (c) 2020, Martin Reames
#

src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)

LDFLAGS = -lm -lpthread
CFLAGS = -g -O2
#CFLAGS = -g
CC = gcc

all: sorts

sorts: $(obj) 
	$(CC) $(CFLAGS) -o sorts -o $@ $^ $(LDFLAGS)

-include $(dep)   # include dep files in the makefile

# generate dep files using the C preprocessor
%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) sorts
	rm -f $(dep)

