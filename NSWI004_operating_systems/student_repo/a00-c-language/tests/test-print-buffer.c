// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include "list.h"
#include "print.h"
#include <stdio.h>
#include <time.h>

/*
 * simple_printf tests for %pB (print simple_printf_buffer_t details).
 *
 * Note that under normal circumstances one would use unit testing framework
 * to run the tests in-program (as opposed to running diff to compare actual
 * and expected output of the whole program) and split the the tests into
 * multiple test cases. Here we prefer to make the tests as simple as possible
 * to make them more readable even for C beginners.
 */

int main() {
    unsigned char buffer[] = { 0xBE, 0xEF, 0x55, 0xAA, 0xCA, 0xFE, 0x00 };

    simple_printf_buffer_t one = { buffer, 4 };
    simple_printf_buffer_t two = { buffer, 7 };

    printf("[EXPECTED]: one = %p[%p(4): 0xBE 0xEF 0x55 0xAA].\n", &one, &buffer);
    simple_printf("[ ACTUAL ]: one = %pB.\n", &one);

    printf("[EXPECTED]: two = %p[%p(7): 0xBE 0xEF 0x55 0xAA 0xCA 0xFE 0x00].\n", &two, &buffer);
    simple_printf("[ ACTUAL ]: two = %pB.\n", &two);

    return 0;
}
