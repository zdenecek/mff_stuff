// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <debug/code.h>
#include <ktest.h>

/*
 * Test for function dumping routine. We check on loader code
 * as that is the only code that we can be sure how will be compiled
 * and where it would reside in memory.
 */

void kernel_test(void) {
    ktest_start("basic/dump_function");

    puts(KTEST_BLOCK_EXPECTED "bfc00000 <loader>:");
    puts(KTEST_BLOCK_EXPECTED "bfc00000:        3c1f8000");
    puts(KTEST_BLOCK_EXPECTED "bfc00004:        37ff1000");
    puts(KTEST_BLOCK_EXPECTED "bfc00008:        03e00008");
    puts(KTEST_BLOCK_EXPECTED "bfc0000c:        00000000");

    debug_dump_function("loader", 0xBFC00000, 4);

    ktest_passed();
}
