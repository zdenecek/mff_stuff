// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include "print.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

/*
 * simple_printf tests for %s conversion specifier.
 *
 * Note that under normal circumstances one would use unit testing framework
 * to run the tests in-program (as opposed to running diff to compare actual
 * and expected output of the whole program) and split the the tests into
 * multiple test cases. Here we prefer to make the tests as simple as possible
 * to make them more readable even for C beginners.
 */

int main() {
    puts("[EXPECTED]: This is ONE string.");
    simple_printf("[ ACTUAL ]: This is %s string.\n", "ONE");

    puts("[EXPECTED]: This is FIRST and SECOND string.");
    simple_printf("[ ACTUAL ]: This is %s and %s string.\n", "FIRST", "SECOND");

    // For completeness, test also printing not "const char*" string
    char buffer[10];
    strncpy(buffer, "Hello", 10);
    puts("[EXPECTED]: Hello, World.");
    simple_printf("[ ACTUAL ]: %s, World.\n", buffer);

    // This is actually undefined behaviour but it is probably safer
    // to test for it explicitly...
    puts("[EXPECTED]: This is NULL string: (null).");
    simple_printf("[ ACTUAL ]: This is NULL string: %s.\n", NULL);
}
