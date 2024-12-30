// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include "print.h"
#include "list.h"
#include "strf-int.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 50

int list_length(list_t* list) {
    (void)list;
    link_t* item = &list->head;
    int count = 0;
    while (item->next != &list->head) {
        count++;
        item = item->next;
    }
    return count;
}

/**
 * @brief print string via putchar
 *
 * @param string to print
 */
void put_string(const char* string) {
    while (*string != '\0')
        putchar(*string++);
}

void print_list(list_t* list) {
    simple_printf("%p[", list);
    int length = list_length(list);
    if (length == 0) {
        simple_printf("empty]", length);
        return;
    }
    link_t* item = list->head.next;
    simple_printf("%d: %p", length, item);
    for (int i = 1; i < length; i++) {
        item = item->next;
        simple_printf("-%p", item);
    }
    putchar(']');
}

void print_buffer(simple_printf_buffer_t* buffer) {
    simple_printf("%p[%p(%d):", buffer, buffer->data, buffer->size);

    if (buffer->size == 0) {
        put_string("empty]");
        return;
    }

    unsigned char* ptr = (unsigned char*)buffer->data;
    char digit_buffer[2];

    for (unsigned long remaining = buffer->size; remaining > 0; remaining--) {

        stringify_hex(*ptr, digit_buffer, 2, true);
        ptr++;

        put_string(" 0x");
        putchar(digit_buffer[1]);
        putchar(digit_buffer[0]);
    }

    putchar(']');
}

void print_timespec(struct timespec* ts) {
    simple_printf("%p[%d:%d]", ts, ts->tv_sec, ts->tv_nsec);
}

void convert(const char** format, va_list* args) {
    char string_buffer[BUFFER_SIZE];

    switch (**format) {
    case 'c': {
        const char* character = va_arg(*args, const char*);
        putchar(*character);
        break;
    }
    case 'd': {
        const int integer = va_arg(*args, const int);
        stringify_int(integer, string_buffer, BUFFER_SIZE);
        put_string(string_buffer);
        break;
    }
    case 'x': {
        const int integer = va_arg(*args, const int);
        stringify_hex(integer, string_buffer, BUFFER_SIZE, false);
        put_string(string_buffer);
        break;
    }
    case 'X': {
        const int integer = va_arg(*args, const int);
        stringify_hex(integer, string_buffer, BUFFER_SIZE, true);
        put_string(string_buffer);
        break;
    }
    case 'p': {
        (*format)++;
        switch (**format) {
        case 'T': {
            struct timespec* ts = va_arg(*args, struct timespec*);
            print_timespec(ts);
            break;
        }
        case 'L': {
            list_t* list = va_arg(*args, list_t*);
            print_list(list);
            break;
        }
        case 'B': {
            simple_printf_buffer_t* buffer = va_arg(*args, simple_printf_buffer_t*);
            print_buffer(buffer);
            break;
        }
        // default includes '\0'
        default: {
            (*format)--; // no further option, move pointer back to continue printing
            unsigned long ptr = (unsigned long)va_arg(*args, const void*);

            stringify_hex_long(ptr, string_buffer, BUFFER_SIZE, false);
            put_string("0x");
            put_string(string_buffer);
            break;
        }
        }
        break;
    }
    case 's': {
        const char* string = va_arg(*args, const char*);
        if (string == NULL) {
            put_string("(null)");
        } else {
            put_string(string);
        }
        break;
    }
    default:;
    }
}

/** Simplified printf.
 *
 * @param format Formatting string.
 * @param ... Arguments for the format string.
 */
void simple_printf(const char* format, ...) {

    va_list args;
    va_start(args, format);

    const char* it = format;
    bool conversion = false;

    while (*it != '\0') {

        if (conversion) {
            convert(&it, &args);
            conversion = false;
        } else if (*it == '%') {
            conversion = true;
        } else {
            putchar(*it);
        }
        it++;
    }

    va_end(args);
}
