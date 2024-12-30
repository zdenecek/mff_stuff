// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <ktest.h>
#include <lib/print.h>

void kernel_test(void) {
    puts(KTEST_EXPECTED "This is forty two: 42.");
    printk(KTEST_ACTUAL "This is forty two: %d.\n", 42);

    puts(KTEST_EXPECTED "This is minus five: -5.");
    printk(KTEST_ACTUAL "This is minus five: %d.\n", -5);

    puts(KTEST_EXPECTED "-2 -1 0 1 2.");
    printk(KTEST_ACTUAL "%d %d %d %d %d.\n", -2, -1, 0, 1, 2);

    puts(KTEST_EXPECTED "INT_MIN = -2147483648 ; INT_MAX 2147483647.");
    printk(KTEST_ACTUAL "INT_MIN = %d ; INT_MAX %d.\n", 0x80000000, 0x7fffffff);

    ktest_passed();
}
