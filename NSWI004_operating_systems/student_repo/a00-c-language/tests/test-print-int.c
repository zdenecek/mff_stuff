// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include "print.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>

/*
 * simple_printf tests for %d conversion specifier.
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

    puts("[EXPECTED]: This is forty two: 42.");
    simple_printf("[ ACTUAL ]: This is forty two: %d.\n", 42);

    puts("[EXPECTED]: This is minus five: -5.");
    simple_printf("[ ACTUAL ]: This is minus five: %d.\n", -5);

    puts("[EXPECTED]: -2 -1 0 1 2.");
    simple_printf("[ ACTUAL ]: %d %d %d %d %d.\n", -2, -1, 0, 1, 2);

    puts("[EXPECTED]: INT_MIN = -2147483648 ; INT_MAX 2147483647.");
    simple_printf("[ ACTUAL ]: INT_MIN = %d ; INT_MAX %d.\n", INT_MIN, INT_MAX);

    return 0;
}
