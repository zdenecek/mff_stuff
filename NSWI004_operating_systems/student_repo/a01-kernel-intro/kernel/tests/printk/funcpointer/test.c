// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 Charles University

#include <ktest.h>
#include <lib/print.h>

void kernel_test(void) {
    puts(KTEST_EXPECTED "Loader: 0xbfc00000[3c1f8000 37ff1000 03e00008 00000000].");
    printk(KTEST_ACTUAL "Loader: %pF.\n", 0xBFC00000);

    ktest_passed();
}
