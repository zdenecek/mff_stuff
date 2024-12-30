// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include "list.h"
#include "print.h"
#include <stdio.h>
#include <time.h>

/*
 * simple_printf tests for %pL (print list_t details).
 *
 * Note that under normal circumstances one would use unit testing framework
 * to run the tests in-program (as opposed to running diff to compare actual
 * and expected output of the whole program) and split the the tests into
 * multiple test cases. Here we prefer to make the tests as simple as possible
 * to make them more readable even for C beginners.
 */

// Do not move this structure to print.h, the printing function
// does not need to know about it at all.
typedef struct {
    const char* name;
    const char* color;
    link_t link;
} paw_t;

/*
 * For simplicity, we do not allocate any memory and assume that
 * the parameters are literals that are valid for the whole execution.
 */
static void paw_init(paw_t* paw, const char* name, const char* color) {
    link_init(&paw->link);
    paw->name = name;
    paw->color = color;
}

int main() {
    paw_t marshall, chase, skye;
    paw_init(&marshall, "Marshall", "red");
    paw_init(&chase, "Chase", "blue");
    paw_init(&skye, "Skye", "pink");

    list_t paws;
    list_init(&paws);

    /* Special case: empty list. */
    simple_printf("[EXPECTED]: %p[empty]\n", &paws);
    simple_printf("[ ACTUAL ]: %pL\n", &paws);

    /* Special case: one item. */
    list_append(&paws, &marshall.link);

    simple_printf("[EXPECTED]: %p[1: %p]\n", &paws, &marshall.link);
    simple_printf("[ ACTUAL ]: %pL\n", &paws);

    /* Append the rest of the items. */
    list_append(&paws, &chase.link);
    list_append(&paws, &skye.link);

    simple_printf("[EXPECTED]: Paw list: %p[3: %p-%p-%p]\n", &paws,
            &marshall.link, &chase.link, &skye.link);
    simple_printf("[ ACTUAL ]: Paw list: %pL\n", &paws);

    return 0;
}
