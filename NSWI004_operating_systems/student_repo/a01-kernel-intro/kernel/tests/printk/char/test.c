// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <ktest.h>
#include <lib/print.h>

void kernel_test(void) {
    puts(KTEST_EXPECTED "Hello, World.");
    printk(KTEST_ACTUAL "%c%c%c%c%c, World.\n", 'H', 'e', 'l', 'l', 'o');

    puts(KTEST_EXPECTED "Test percent: %c with '%' prints '%'.");
    printk(KTEST_ACTUAL "Test percent: %%c with '%%' prints '%c'.\n", '%');

    ktest_passed();
}
