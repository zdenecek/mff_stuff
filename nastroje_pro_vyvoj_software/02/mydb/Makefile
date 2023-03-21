CC=gcc
EXTRA_CFLAGS= 
CFLAGS?= -g -Wall

export CFLAGS
export CC
export EXTRA_CFLAGS

.PHONY: clean all

all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean


%:
	$(MAKE) -C src $@
