// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 Charles University

#include "list.h"
#include <stdio.h>

/*
 * Example list usage.
 */

typedef struct {
    link_t link;
    const char* name;
    const char* color;
} paw_t;

/*
 * For simplicity, we assume that the arguments are string literals
 * that are valid for the whole duration of the program execution.
 * This means that we don't need to copy their values and only need
 * to allocate the list item.
 */
static paw_t* paw_create(const char* name, const char* color) {
    paw_t* paw = malloc(sizeof(paw_t));
    if (paw == NULL) {
        return NULL;
    }

    link_init(&paw->link);
    paw->name = name;
    paw->color = color;

    return paw;
}

static void print_paw_list(list_t* paws) {
    puts("Paw list:");
    list_foreach(*paws, paw_t, link, paw) {
        printf(" -> %p: Paw %s.\n", paw, paw->name);
    }
}

int main() {
    paw_t* marshall = paw_create("Marshall", "red");
    paw_t* chase = paw_create("Chase", "blue");
    paw_t* skye = paw_create("Skye", "pink");

    list_t paws;
    list_init(&paws);

    printf("list_is_empty = %d\n", list_is_empty(&paws));

    list_append(&paws, &marshall->link);

    printf("list_is_empty = %d\n", list_is_empty(&paws));

    print_paw_list(&paws);

    /* Append the rest of the items. */
    list_append(&paws, &chase->link);
    list_append(&paws, &skye->link);
    print_paw_list(&paws);

    /* Remove and free items one by one. */
    while (!list_is_empty(&paws)) {
        link_t* first = list_pop(&paws);
        paw_t* paw = list_item(first, paw_t, link);
        free(paw);
    }

    printf("list_is_empty = %d\n", list_is_empty(&paws));

    return 0;
}
