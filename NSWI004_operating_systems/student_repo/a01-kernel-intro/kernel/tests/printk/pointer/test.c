// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 Charles University

#include <ktest.h>
#include <lib/print.h>
#include <types.h>

void kernel_test(void) {
    char* beef = (char*)0xbeef;
    char* deadbeef = (char*)0xdeadbeef;

    puts(KTEST_EXPECTED "beef pointer: 0xbeef.");
    printk(KTEST_ACTUAL "beef pointer: %p.\n", beef);

    puts(KTEST_EXPECTED "dead beef pointer: 0xdeadbeef.");
    printk(KTEST_ACTUAL "dead beef pointer: %p.\n", deadbeef);

    puts(KTEST_EXPECTED "This is NULL pointer: (nil).");
    printk(KTEST_ACTUAL "This is NULL pointer: %p.\n", NULL);

    ktest_passed();
}
