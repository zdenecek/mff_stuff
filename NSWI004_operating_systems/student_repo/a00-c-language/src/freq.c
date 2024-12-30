// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "freq.h"

struct freq_str {
    list_t list;
    link_t link;
    int occurences;
    char* str;
};

/** Create new string frequency counter.
 *
 * @return Pointer to opaque structure containing the list.
 * @retval NULL Out of memory.
 */
freq_str_t* freq_str_create(void) {

    freq_str_t* freq_list = malloc(sizeof(freq_str_t));
    if (freq_list == NULL) {
        return NULL;
    }

    // avoid leaks (valgrind)
    freq_list->str = NULL;

    list_init(&freq_list->list);

    return freq_list;
}

/** Destroy previously created frequency counter.
 *
 * @param freq Frequency counter previously created with freq_str_create.
 */
void freq_str_destroy(freq_str_t* freq) {

    freq_str_t* current = freq;

    list_foreach(freq->list, freq_str_t, link, i) {

        free(current->str);
        free(current);

        current = i;
    }
    if (current != NULL) {
        free(current->str);
        free(current);
    }
}

freq_str_t* freq_str_try_get_record(freq_str_t* freq_list, const char* str) {
    list_foreach(freq_list->list, freq_str_t, link, i) {
        if (strcmp(i->str, str) == 0) {

            return i;
        }
    }
    return NULL;
}

/** Count in the given string.
 *
 * @param freq Frequency counter.
 * @param str String to count-in.
 * @return Error value.
 * @retval 0 Success.
 */
int freq_str_add(freq_str_t* freq, const char* str) {

    freq_str_t* record = freq_str_try_get_record(freq, str);
    if (record != NULL) {
        record->occurences++;
        return 0;
    }

    char* str_copy = malloc(strlen(str) + 1);
    if (str_copy == NULL) {
        return ENOMEM;
    }
    strcpy(str_copy, str);

    freq_str_t* newly_appended = malloc(sizeof(freq_str_t));
    if (newly_appended == NULL) {
        free(str_copy);
        return ENOMEM;
    }

    newly_appended->str = str_copy;
    newly_appended->occurences = 1;

    link_init(&newly_appended->link);
    list_append(&freq->list, &newly_appended->link);

    return 0;
}

/** Get frequency of given string.
 *
 * @freq Frequency counter.
 * @param str String that is queried.
 * @return Number of occurences.
 */
int freq_str_get(freq_str_t* freq, const char* str) {
    freq_str_t* record = freq_str_try_get_record(freq, str);

    return record == NULL ? 0 : record->occurences;
}