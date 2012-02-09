UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
TARG += traceopen.so
traceopen.so: traceopen.c
	gcc -shared -fPIC -ldl $< -o $@ -Wall
endif
ifeq ($(UNAME), Darwin)
TARG += traceopen.dylib
traceopen.dylib: traceopen.c
	gcc -dynamiclib -undefined suppress -flat_namespace -ldl $< -o $@
endif

clean:
	rm -f $(TARG)

all: $(TARG)

