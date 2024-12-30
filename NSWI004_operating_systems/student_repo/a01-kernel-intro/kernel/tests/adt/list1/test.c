// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <adt/list.h>
#include <ktest.h>

typedef struct {
    link_t link;
    const char* name;
    const char* color;
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

void kernel_test(void) {
    ktest_start("adt/list1");

    paw_t marshall, chase, skye;
    paw_init(&marshall, "Marshall", "red");
    paw_init(&chase, "Chase", "blue");
    paw_init(&skye, "Skye", "pink");

    list_t paws;
    list_init(&paws);

    ktest_assert(list_is_empty(&paws), "list shall be empty");
    ktest_assert(list_get_size(&paws) == 0, "list shall be empty (but has %d items)", list_get_size(&paws));

    list_append(&paws, &marshall.link);
    ktest_assert(!list_is_empty(&paws), "list shall not be empty");
    ktest_assert(list_get_size(&paws) == 1, "list shall have one items (but has %d items)", list_get_size(&paws));

    list_append(&paws, &chase.link);
    list_append(&paws, &skye.link);
    ktest_assert(list_get_size(&paws) == 3, "list shall have 3 items (but has %d items)", list_get_size(&paws));

    ktest_passed();
}
