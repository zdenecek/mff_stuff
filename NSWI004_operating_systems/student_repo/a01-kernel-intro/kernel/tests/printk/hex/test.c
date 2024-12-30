// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <ktest.h>
#include <lib/print.h>

void kernel_test(void) {
    puts(KTEST_EXPECTED "Zero is still zero: 0.");
    printk(KTEST_ACTUAL "Zero is still zero: %x.\n", 0);

    puts(KTEST_EXPECTED "This is forty two: 2a.");
    printk(KTEST_ACTUAL "This is forty two: %x.\n", 42);

    puts(KTEST_EXPECTED "This is minus five: fffffffb.");
    printk(KTEST_ACTUAL "This is minus five: %x.\n", -5);

    puts(KTEST_EXPECTED "INT_MIN in hexa: 80000000 ; INT_MAX in hexa: 7fffffff.");
    printk(KTEST_ACTUAL "INT_MIN in hexa: %x ; INT_MAX in hexa: %x.\n", 0x80000000, 0x7fffffff);

    puts(KTEST_EXPECTED "UINT_MAX in hexa: ffffffff.");
    printk(KTEST_ACTUAL "UINT_MAX in hexa: %x.\n", 0xffffffff);

    puts(KTEST_EXPECTED "Capital letters: 0xABCD.");
    printk(KTEST_ACTUAL "Capital letters: 0x%X.\n", 0xabcd);

    ktest_passed();
}
