CC = gcc
CFLAGS = -O2

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
TARG += traceopen.so
traceopen.so: traceopen.c
	$(CC) $(CFLAGS) -shared -fPIC -ldl $< -o $@ -Wall
ENVSRC = linuxenv
endif
ifeq ($(UNAME), Darwin)
TARG += traceopen.dylib
traceopen.dylib: traceopen.c
	$(CC) $(CFLAGS) -dynamiclib -fPIC -undefined suppress -flat_namespace -ldl $< -o $@
ENVSRC = mac
endif

clean:
	rm -f $(TARG)

all: $(TARG)

test: test/opentest $(TARG)
	test/test.sh

.PHONY: test

test/opentest: test/opentest.c
	$(CC) $< -o $@
