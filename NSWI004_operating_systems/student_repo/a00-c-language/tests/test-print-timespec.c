// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include "list.h"
#include "print.h"
#include <stdio.h>
#include <time.h>

/*
 * simple_printf tests for %pT (print struct timespec).
 *
 * Note that under normal circumstances one would use unit testing framework
 * to run the tests in-program (as opposed to running diff to compare actual
 * and expected output of the whole program) and split the the tests into
 * multiple test cases. Here we prefer to make the tests as simple as possible
 * to make them more readable even for C beginners.
 */

int main() {
    struct timespec ts = {
        .tv_sec = 21,
        .tv_nsec = 42
    };
    printf("[EXPECTED]: &ts = %p[21:42].\n", &ts);
    simple_printf("[ ACTUAL ]: &ts = %pT.\n", &ts);

    return 0;
}
