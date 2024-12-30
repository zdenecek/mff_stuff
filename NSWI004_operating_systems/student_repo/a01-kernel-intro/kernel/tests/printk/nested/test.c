// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 Charles University

#include <ktest.h>
#include <lib/print.h>
#include <types.h>

void kernel_test(void) {
    puts(KTEST_EXPECTED "This is string with %s.");
    printk(KTEST_ACTUAL "This is string with %s.\n", "%s");

    puts(KTEST_EXPECTED "This is string with %%.");
    printk(KTEST_ACTUAL "%s.\n", "This is string with %%");

    ktest_passed();
}
