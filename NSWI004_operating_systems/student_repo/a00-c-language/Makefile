# SPDX-License-Identifier: Apache-2.0
# Copyright 2019 Charles University

.PHONY: all check clean distclean fix-cstyle

# Settings

MAKEFLAGS += --no-builtin-rules --no-builtin-variables

CC = gcc
CFLAGS = -Iinclude -O -g -Wall -Wextra -Werror -MMD -MF "$*.dep"
LD = gcc
LDFLAGS = -O -g

VALGRIND_MEM_LEAKS = valgrind\
	--error-exitcode=99 \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--verbose

# Files

LIB_SRC = src/print.c src/freq.c src/strf-int.c
LIB_OBJS = $(addsuffix .o, $(basename $(LIB_SRC)))
LIB_DEPS = $(addsuffix .dep, $(basename $(LIB_SRC)))

EXAMPLE_LIST_SRC = src/example-list.c
EXAMPLE_LIST_OBJS = $(addsuffix .o, $(basename $(EXAMPLE_LIST_SRC)))
EXAMPLE_LIST_DEPS = $(addsuffix .dep, $(basename $(EXAMPLE_LIST_SRC)))

EXAMPLE_VARARG_SRC = src/example-vararg.c
EXAMPLE_VARARG_OBJS = $(addsuffix .o, $(basename $(EXAMPLE_VARARG_SRC)))
EXAMPLE_VARARG_DEPS = $(addsuffix .dep, $(basename $(EXAMPLE_VARARG_SRC)))

TEST_SRC = $(wildcard tests/test-*.c)
TEST_BINS = $(notdir $(basename $(TEST_SRC)))
TEST_OBJS = $(addsuffix .o, $(basename $(TEST_SRC)))
TEST_DEPS = $(addsuffix .dep, $(basename $(TEST_SRC)))

TESTS_BIN_BASE = \
	test-freq-basic \
	test-freq-cmp \
	test-freq-multi \
	test-print-hex \
	test-print-int \
	test-print-pointer \
	test-print-string

TESTS_BIN_EXTENDED = \
	test-print-buffer \
	test-print-list \
	test-print-timespec

ALL_BIN = example-vararg example-list $(TEST_BINS)

# Rules

all: $(ALL_BIN)

clean:
	find -name '*.o' -delete
	find -name '*.dep' -delete

distclean:
	find -name '*.o' -delete
	find -name '*.dep' -delete
	rm -f $(ALL_BIN)

fix-cstyle:
	clang-format --style=file -i include/*.h  src/*.c tests/*.c

example-vararg: $(EXAMPLE_VARARG_OBJS) $(LIB_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

example-list: $(EXAMPLE_LIST_OBJS) $(LIB_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(TEST_BINS): %: tests/%.o $(LIB_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

check: $(TESTS_BIN_BASE)
	./tools/run_tests.sh $^

check-extended: $(TESTS_BIN_EXTENDED)
	./tools/run_tests.sh $^

check-leaks: test-freq-multi
	$(VALGRIND_MEM_LEAKS) ./test-freq-multi

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Deps
-include $(EXAMPLE_LIST_DEPS) $(EXAMPLE_VARARG_DEPS) $(TEST_DEPS)
