// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#ifndef _ADT_LIST_H
#define _ADT_LIST_H

#include <debug.h>
#include <types.h>

/*
 * A doubly linked list whose link pointers are embedded inside the item type.
 *
 * To define an item type that can be placed inside the list, include the link_t member in the item type:
 *
 * typedef struct my_item {
 *     ...
 *     link_t my_list_link;
 *     ...
 * } my_item_t;
 *
 * The list is represented by the list_t type, which contains the head and tail pointers.
 * An empty list is initialized using the list_init function, which is the same as declaring
 * a variable of the list_t type and setting the head and tail pointers to itself:
 *
 * list_t my_list;
 * list_init(&my_list);
 *
 * Functions that manipulate the list use pointers to the link member,
 * rather than pointers to the list item, to address the list elements:
 *
 * my_item_t *item;
 * list_append(&my_list, &item->my_list_link);
 *
 * The list_item macro converts a pointer to the link member to a pointer to the list item:
 *
 * link_t *link = list_pop(&my_list);
 * my_item_t *item = list_item(link, my_item_t, my_list_link);
 *
 * The list_foreach macro iterates over the list elements similar to a for loop:
 *
 * list_foreach (my_list, my_item_t, my_list_link, item) {
 *     do_something_with(item);
 * }
 *
 */

/** Compute the offset of a member in a structure
 *
 * @param type   The type of the container structure a member is embedded in.
 * @param member The name of the member within the structure.
 *
 * @return Offset of the member in the structure.
 *
 */
#define list_offset_of(type, member) \
    ((size_t) & ((type*)0)->member)

/** Cast a member of a structure to the containing structure
 *
 * @param ptr    The pointer to the member.
 * @param type   The type of the container structure a member is embedded in.
 * @param member The name of the member within the structure.
 *
 * @return Container structure.
 *
 */
#define list_container_of(ptr, type, member) \
    ({ \
        const __typeof__(((type*)0)->member)* __mptr = (ptr); \
        (type*)((void*)__mptr - list_offset_of(type, member)); \
    })

/** Get list item
 *
 * Get a typed structure from the given list item.
 *
 * @param link   Pointer to the list item.
 * @param type   The type of the structure the item is embedded in.
 * @param member The name of the member of the item within
 *               the containing structure.
 *
 * @return Typed structure corresponding to the list item.
 *
 */
#define list_item(link, type, member) \
    list_container_of(link, type, member)

/** Iterate over list members
 *
 * @param list     The list to iterate over.
 * @param type     The type of the structure the item is embedded in.
 * @param member   The name of the member of the item within
 *                 the containing structure.
 * @param iterator The name of the iterator to declare.
 *
 */
#define list_foreach(list, type, member, iterator) \
    for (type* iterator = NULL; iterator == NULL; iterator = (type*)1) \
        for (link_t* _link = (list).head.next; \
                iterator = list_item(_link, type, member), \
                     _link != &(list).head; \
                _link = _link->next)

/** Polymorphic list link
 *
 */
typedef struct link {
    struct link* prev;
    struct link* next;
} link_t;

/** Polymorphic list link
 *
 */
typedef struct {
    link_t head;
} list_t;

/** Checked list link initialization
 *
 * Initialize a list item.
 *
 * @param link The list item to initialize.
 *
 */
static inline void link_init(link_t* link) {
    assert(link != NULL);

    link->prev = NULL;
    link->next = NULL;
}

/** Checked list initialization
 *
 * Initialize a list. An empty list points back
 * to itself for easy runtime checks.
 *
 * @param list The list head to initialize.
 *
 */
static inline void list_init(list_t* list) {
    assert(list != NULL);

    list->head.prev = &list->head;
    list->head.next = &list->head;
}

/** Test whether a list item is connected
 *
 * @param link The list item to examine.
 *
 * @return True if the list item is connected to a list.
 *
 */
static inline bool link_is_connected(const link_t* link) {
    assert(link != NULL);

    return ((link->prev != NULL) && (link->next != NULL));
}

/** Test whether a list is empty
 *
 * @param list The list to examine.
 *
 * @return True if the list is empty.
 *
 */
static inline bool list_is_empty(const list_t* list) {
    assert(list != NULL);

    return (list->head.next == &list->head);
}

static inline size_t list_get_size(const list_t* list) {
    size_t result = 0;
    for (link_t* link = list->head.next; link != &(list->head); link = link->next) {
        result++;
    }
    return result;
}

/** Prepend item to the list
 *
 * Prepend a new item to the beginning of a list.
 *
 * @param list The list to prepend to.
 * @param link The new item link.
 *
 */
static inline void list_prepend(list_t* list, link_t* link) {
    assert(list != NULL);
    assert(link != NULL);

    link->next = list->head.next;
    link->prev = &list->head;

    list->head.next->prev = link;
    list->head.next = link;
}

/** Append item to the list
 *
 * Append a new item to the endof a list.
 *
 * @param list The list to append to.
 * @param link The new item link.
 *
 */
static inline void list_append(list_t* list, link_t* link) {
    assert(list != NULL);
    assert(link != NULL);

    link->next = &list->head;
    link->prev = list->head.prev;

    list->head.prev->next = link;
    list->head.prev = link;
}

/** Remove an item from a list
 *
 * Remove an item from any list it is currently part of.
 *
 * @param link The item link to remove.
 *
 */
static inline void list_remove(link_t* link) {
    assert(link != NULL);

    if (link_is_connected(link)) {
        link->next->prev = link->prev;
        link->prev->next = link->next;
    }

    link_init(link);
}

/** Pop an item from the list
 *
 * Remove the first item from the list.
 *
 * @param list The list to pop from.
 *
 * @return The first item.
 *
 */
static inline link_t* list_pop(list_t* list) {
    assert(list != NULL);

    if (list_is_empty(list))
        return NULL;

    link_t* item = list->head.next;

    list_remove(item);
    return item;
}

/** Rotate the list by making its head into its tail
 *
 * @param list The list to rotate.
 *
 * @return The rotated item.
 *
 */
static inline link_t* list_rotate(list_t* list) {
    assert(list != NULL);

    if (list_is_empty(list))
        return NULL;

    /*
     * Simply remove the first item from the list
     * and append it back to the end of the list.
     * Not the most efficient way but functional
     * nonetheless.
     */

    link_t* item = list_pop(list);
    list_append(list, item);

    return item;
}

#endif
