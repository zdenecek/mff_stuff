// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include "freq.h"
#include <assert.h>
#include <stdio.h>

/*
 * freq_str base test.
 *
 * Note that under normal circumstances one would use unit testing framework
 * to run the tests in-program (as opposed to running diff to compare actual
 * and expected output of the whole program) and split the the tests into
 * multiple test cases. Here we prefer to make the tests as simple as possible
 * to make them more readable even for C beginners.
 */

int main() {
    freq_str_t* words = freq_str_create();

    int rc1 = freq_str_add(words, "bravo");
    int rc2 = freq_str_add(words, "alpha");
    int rc3 = freq_str_add(words, "bravo");
    int rc4 = freq_str_add(words, "alpha");
    int rc5 = freq_str_add(words, "bravo");

    printf("[EXPECTED]: 0, 0, 0, 0, 0\n");
    printf("[ ACTUAL ]: %d, %d, %d, %d, %d\n", rc1, rc2, rc3, rc4, rc5);

    printf("[EXPECTED]: alpha = 2\n");
    printf("[ ACTUAL ]: alpha = %d\n", freq_str_get(words, "alpha"));

    printf("[EXPECTED]: bravo = 3\n");
    printf("[ ACTUAL ]: bravo = %d\n", freq_str_get(words, "bravo"));

    printf("[EXPECTED]: charlie = 0\n");
    printf("[ ACTUAL ]: charlie = %d\n", freq_str_get(words, "charlie"));

    // The same again, doubling the amount.

    rc1 = freq_str_add(words, "bravo");
    rc2 = freq_str_add(words, "alpha");
    rc3 = freq_str_add(words, "bravo");
    rc4 = freq_str_add(words, "alpha");
    rc5 = freq_str_add(words, "bravo");

    printf("[EXPECTED]: 0, 0, 0, 0, 0\n");
    printf("[ ACTUAL ]: %d, %d, %d, %d, %d\n", rc1, rc2, rc3, rc4, rc5);

    printf("[EXPECTED]: alpha = 4\n");
    printf("[ ACTUAL ]: alpha = %d\n", freq_str_get(words, "alpha"));

    printf("[EXPECTED]: bravo = 6\n");
    printf("[ ACTUAL ]: bravo = %d\n", freq_str_get(words, "bravo"));

    printf("[EXPECTED]: charlie = 0\n");
    printf("[ ACTUAL ]: charlie = %d\n", freq_str_get(words, "charlie"));

    freq_str_destroy(words);

    return 0;
}
