// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <adt/list.h>
#include <ktest.h>

/*
 * Test for printk on lists.
 */

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
    ktest_start("printk/list");

    paw_t marshall, chase, skye;
    paw_init(&marshall, "Marshall", "red");
    paw_init(&chase, "Chase", "blue");
    paw_init(&skye, "Skye", "pink");

    list_t paws;
    list_init(&paws);

    /* Special case: empty list. */
    printk(KTEST_EXPECTED "%p[empty]\n", &paws);
    printk(KTEST_ACTUAL "%pL\n", &paws);

    /* Special case: one item. */
    list_append(&paws, &marshall.link);

    printk(KTEST_EXPECTED "%p[1: %p]\n", &paws, &marshall.link);
    printk(KTEST_ACTUAL "%pL\n", &paws);

    /* Append rest of items. */
    list_append(&paws, &chase.link);
    list_append(&paws, &skye.link);

    printk(KTEST_EXPECTED "Paw list: %p[3: %p-%p-%p]\n", &paws,
            &marshall.link, &chase.link, &skye.link);
    printk(KTEST_ACTUAL "Paw list: %pL\n", &paws);

    list_foreach(paws, paw_t, link, it) {
        dprintk("%p: Paw %s.\n", it, it->name);
    }

    ktest_passed();
}
