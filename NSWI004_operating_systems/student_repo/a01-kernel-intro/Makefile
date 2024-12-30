# SPDX-License-Identifier: Apache-2.0
# Copyright 2019 Charles University

GDB_PORT = 10001
DIFF = diff

-include config.mk

### Phony targets

.PHONY: all clean distclean kernel run-msim-gdb run-gdb cstyle fix-cstyle check-config



### Default target

all: kernel

kernel: check-config
	$(MAKE) -C kernel

clean:
	$(MAKE) -C kernel clean

distclean:
	$(MAKE) -C kernel distclean
	rm -f config.mk

run-msim-gdb:
	msim -g $(GDB_PORT)

run-gdb:
	$(GDB) -ix gdbinit -iex "target remote :$(GDB_PORT)"

cstyle:
	./tools/cstyle.sh kernel/

fix-cstyle:
	find kernel/ -name '*.[ch]' -exec clang-format -style=file -i {} \;


### Usability target

check-config:
    ifeq (,$(wildcard ./config.mk))
	$(error Missing 'config.mk'. Run 'configure.py' to create it)
    endif
