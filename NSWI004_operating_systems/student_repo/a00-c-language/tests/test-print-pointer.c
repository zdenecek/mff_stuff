// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include "print.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

/*
 * simple_printf tests for %p conversion specifier.
 *
 * Note that under normal circumstances one would use unit testing framework
 * to run the tests in-program (as opposed to running diff to compare actual
 * and expected output of the whole program) and split the the tests into
 * multiple test cases. Here we prefer to make the tests as simple as possible
 * to make them more readable even for C beginners.
 */

int main() {
    int a = 42;
    printf("[EXPECTED]: &a = %p.\n", &a);
    simple_printf("[ ACTUAL ]: &a = %p.\n", &a);

    return 0;
}
