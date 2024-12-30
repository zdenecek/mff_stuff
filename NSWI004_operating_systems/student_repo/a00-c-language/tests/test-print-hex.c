// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include "print.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>

/*
 * simple_printf tests for %x and %X conversion specifiers.
 *
 * Note that under normal circumstances one would use unit testing framework
 * to run the tests in-program (as opposed to running diff to compare actual
 * and expected output of the whole program) and split the the tests into
 * multiple test cases. Here we prefer to make the tests as simple as possible
 * to make them more readable even for C beginners.
 */

int main() {
    // Our tests assumes we are on architecture with 32bit int
    if (sizeof(int) != 4) {
        puts("Tests not suitable for architecture with sizeof(int) != 32bits.");
        return 1;
    }

    puts("[EXPECTED]: This is forty two: 2a.");
    simple_printf("[ ACTUAL ]: This is forty two: %x.\n", 42);

    puts("[EXPECTED]: This is minus five: fffffffb.");
    simple_printf("[ ACTUAL ]: This is minus five: %x.\n", -5);

    puts("[EXPECTED]: -2 -1 0 1 2.");
    simple_printf("[ ACTUAL ]: %d %d %d %d %d.\n", -2, -1, 0, 1, 2);

    puts("[EXPECTED]: INT_MIN in hexa: 80000000 ; INT_MIN in hexa: 7fffffff.");
    simple_printf("[ ACTUAL ]: INT_MIN in hexa: %x ; INT_MIN in hexa: %x.\n", 0x80000000, 0x7fffffff);

    puts("[EXPECTED]: Capital letters: 0xABCD.");
    simple_printf("[ ACTUAL ]: Capital letters: 0x%X.\n", 0xabcd);
}
