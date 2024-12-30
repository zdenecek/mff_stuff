// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include "freq.h"
#include <assert.h>
#include <stdio.h>

/*
 * freq_str tests of psuedo-parallel use.
 *
 * Note that under normal circumstances one would use unit testing framework
 * to run the tests in-program (as opposed to running diff to compare actual
 * and expected output of the whole program) and split the the tests into
 * multiple test cases. Here we prefer to make the tests as simple as possible
 * to make them more readable even for C beginners.
 */

int main() {
    freq_str_t* one = freq_str_create();
    freq_str_t* two = freq_str_create();

    assert(one != NULL);
    assert(two != NULL);

    int rc1 = freq_str_add(one, "zulu");

    puts("# After adding zulu to one");
    printf("[EXPECTED]: zulu: 1, 0\n");
    printf("[ ACTUAL ]: zulu: %d, %d\n", freq_str_get(one, "zulu"), freq_str_get(two, "zulu"));

    int rc2 = freq_str_add(two, "ALPHA");

    puts("# After adding ALPHA to two");
    printf("[EXPECTED]: zulu: 1, 0\n");
    printf("[ ACTUAL ]: zulu: %d, %d\n", freq_str_get(one, "zulu"), freq_str_get(two, "zulu"));
    printf("[EXPECTED]: ALPHA: 0, 1\n");
    printf("[ ACTUAL ]: ALPHA: %d, %d\n", freq_str_get(one, "ALPHA"), freq_str_get(two, "ALPHA"));

    int rc3 = freq_str_add(one, "zulu");
    int rc4 = freq_str_add(two, "ZULU");

    puts("# After adding zulu to one and ZULU to two");
    printf("[EXPECTED]: zulu: 2, 0\n");
    printf("[ ACTUAL ]: zulu: %d, %d\n", freq_str_get(one, "zulu"), freq_str_get(two, "zulu"));
    printf("[EXPECTED]: ALPHA: 0, 1\n");
    printf("[ ACTUAL ]: ALPHA: %d, %d\n", freq_str_get(one, "ALPHA"), freq_str_get(two, "ALPHA"));
    printf("[EXPECTED]: ZULU: 0, 1\n");
    printf("[ ACTUAL ]: ZULU: %d, %d\n", freq_str_get(one, "ZULU"), freq_str_get(two, "ZULU"));

    int rc5 = freq_str_add(one, "alpha");

    puts("# After adding alpha to one");
    printf("[EXPECTED]: alpha: 1, 0\n");
    printf("[ ACTUAL ]: alpha: %d, %d\n", freq_str_get(one, "alpha"), freq_str_get(two, "alpha"));
    printf("[EXPECTED]: zulu: 2, 0\n");
    printf("[ ACTUAL ]: zulu: %d, %d\n", freq_str_get(one, "zulu"), freq_str_get(two, "zulu"));
    printf("[EXPECTED]: ALPHA: 0, 1\n");
    printf("[ ACTUAL ]: ALPHA: %d, %d\n", freq_str_get(one, "ALPHA"), freq_str_get(two, "ALPHA"));
    printf("[EXPECTED]: ZULU: 0, 1\n");
    printf("[ ACTUAL ]: ZULU: %d, %d\n", freq_str_get(one, "ZULU"), freq_str_get(two, "ZULU"));

    puts("# Checking freq_str_add error codes");
    printf("[EXPECTED]: rc = 0, 0, 0, 0, 0\n");
    printf("[ ACTUAL ]: rc = %d, %d, %d, %d, %d\n", rc1, rc2, rc3, rc4, rc5);

    freq_str_destroy(one);
    freq_str_destroy(two);

    return 0;
}
