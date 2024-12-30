// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <ktest.h>
#include <lib/print.h>

void kernel_test(void) {
    puts(KTEST_EXPECTED "This is forty two: 42.");
    printk(KTEST_ACTUAL "This is forty two: %u.\n", 42);

    puts(KTEST_EXPECTED "0 1 2.");
    printk(KTEST_ACTUAL "%u %u %u.\n", 0, 1, 2);

    puts(KTEST_EXPECTED "MAX_UINT is 4294967295.");
    printk(KTEST_ACTUAL "MAX_UINT is %u.\n", (unsigned int)0xffffffff);

    ktest_passed();
}
