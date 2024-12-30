// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <debug.h>
#include <debug/code.h>
#include <ktest.h>

/*
 * Tests for debug_get_stack_pointer() function. We use the volatile
 * variable value to prevent any advanced optimizations of the compiler,
 * to check that reading $sp works across callee/caller boundary we
 * use very liberal checks.
 */

#define COUNT 8
#define MAX_STACK_FRAME ((COUNT * sizeof(int)) + 12 * sizeof(native_t))

static volatile int value = 0;

/* Force no-inlining to ensure we move $sp at least a bit. */
static __attribute__((noinline)) uintptr_t get_nested_sp() {
    int a[COUNT];
    for (int i = 0; i < COUNT; i++) {
        a[i] = value;
    }
    return debug_get_stack_pointer() | a[0];
}

void kernel_test(void) {
    ktest_start("basic/stack_pointer");

    uintptr_t sp1 = debug_get_stack_pointer();
    uintptr_t sp2 = debug_get_stack_pointer();

    dprintk("sp1 = %x\n", sp1);
    dprintk("sp2 = %x\n", sp2);

    ktest_assert_in_range("$sp must be in mainmem", sp1, 0x80000000, 0x80100000);
    ktest_assert(sp1 == sp2, "$sp unexpectedly changed (0x%x => 0x%x)", sp1, sp2);

    uintptr_t sp3 = get_nested_sp();

    dprintk("sp3 = %x\n", sp3);

    ktest_assert_in_range("$sp must be in mainmen", sp3, 0x80000000, 0x80100000);
    ktest_assert(sp3 < sp2, "caller shall decrease $sp (0x%x => 0x%x)", sp2, sp3);
    ktest_assert(sp2 - MAX_STACK_FRAME < sp3, "stack frame is too big (0x%x, %dB => 0x%x)",
            sp2, MAX_STACK_FRAME, sp3);

    ktest_passed();
}
