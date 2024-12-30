// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include "freq.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/*
 * freq_str tests of proper string comparison.
 *
 * Note that under normal circumstances one would use unit testing framework
 * to run the tests in-program (as opposed to running diff to compare actual
 * and expected output of the whole program) and split the the tests into
 * multiple test cases. Here we prefer to make the tests as simple as possible
 * to make them more readable even for C beginners.
 */

int main() {
    freq_str_t* one = freq_str_create();
    char zulu_static[] = { 'z', 'u', 'l', 'u', 0 };
    char* zulu_dynamic = malloc(8);

    assert(one != NULL);
    assert(zulu_dynamic != NULL);

    strncpy(zulu_dynamic, "zulu", 5);

    int rc1 = freq_str_add(one, "al");
    int rc2 = freq_str_add(one, "alpha");
    int rc3 = freq_str_add(one, "zulu");
    int rc4 = freq_str_add(one, zulu_static);
    int rc5 = freq_str_add(one, zulu_dynamic);

    printf("[EXPECTED]: al: 1\n");
    printf("[ ACTUAL ]: al: %d\n", freq_str_get(one, "al"));
    printf("[EXPECTED]: alpha: 1\n");
    printf("[ ACTUAL ]: alpha: %d\n", freq_str_get(one, "alpha"));
    printf("[EXPECTED]: lima: 0\n");
    printf("[ ACTUAL ]: lima: %d\n", freq_str_get(one, "lima"));
    printf("[EXPECTED]: zulu: 3\n");
    printf("[ ACTUAL ]: zulu: %d\n", freq_str_get(one, zulu_static));
    printf("[EXPECTED]: lima: 0\n");
    printf("[ ACTUAL ]: lima: %d\n", freq_str_get(one, "xylima" + 2));

    printf("[EXPECTED]: rc = 0, 0, 0, 0, 0\n");
    printf("[ ACTUAL ]: rc = %d, %d, %d, %d, %d\n", rc1, rc2, rc3, rc4, rc5);

    freq_str_destroy(one);

    return 0;
}
