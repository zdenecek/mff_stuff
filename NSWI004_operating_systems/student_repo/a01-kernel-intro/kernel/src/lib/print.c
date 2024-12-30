// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <adt/list.h>
#include <debug.h>
#include <drivers/printer.h>
#include <lib/print.h>
#include <lib/stdarg.h>
#include <lib/stringify.h>
#include <types.h>

#define BUFFER_SIZE 50

static inline int list_length(list_t* list) {
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
static inline void put_string(const char* string) {
    while (*string != '\0')
        printer_putchar(*string++);
}

/**
 * @brief print list
 *
 * @param list list to print
 */
static inline void print_list(const list_t* list) {
    printk("%p[", list);
    int length = list_length((list_t*)list);
    if (length == 0) {
        printk("empty]", length);
        return;
    }
    link_t* item = list->head.next;
    printk("%d: %p", length, item);
    for (int i = 1; i < length; i++) {
        item = item->next;
        printk("-%p", item);
    }
    printer_putchar(']');
}

/**
 * @brief prints string or "(null)"
 *
 * @param string to print
 */
static inline void print_string(const char* string) {
    put_string(string == NULL ? "(null)" : string);
}

/**
 * @brief prints pointer or "(nil)"
 *
 * @param ptr pointer to print
 */
static inline void print_ptr(const void* ptr) {
    if (ptr == NULL)
        put_string("(nil)");
    else
        printk("0x%x", (unsigned int)ptr);
}

/**
 * @brief dumps function address and first for instructions in format %p[%i %i %i %i].
 * Prints nothing if func is NULL
 *
 * @param func
 */
static inline void print_func(const int* func) {
    if (func == NULL)
        return;
    printk("%p[%i %i %i %i]", func, *func, *(func + 1), *(func + 2), *(func + 3));
}

static inline void convert(const char** format, va_list* args) {
    char string_buffer[BUFFER_SIZE];

    switch (**format) {
    case '%': {
        // escape
        printer_putchar('%');
        break;
    }
    case 'c': {
        // have to use int otherwise promotion rule applies, https://stackoverflow.com/questions/23983471/char-is-promoted-to-int-when-passed-through-in-c
        const int character = va_arg(*args, const int);
        printer_putchar(character);
        break;
    }
    case 'd': {
        const int integer = va_arg(*args, const int);
        stringify_int(integer, string_buffer, BUFFER_SIZE);
        put_string(string_buffer);
        break;
    }
    case 'u': {
        const unsigned int integer = va_arg(*args, const unsigned int);
        stringify_uint(integer, string_buffer, BUFFER_SIZE);
        put_string(string_buffer);
        break;
    }
    case 'x': {
        const int integer = va_arg(*args, const int);
        stringify_hex(integer, string_buffer, BUFFER_SIZE, false, true);
        put_string(string_buffer);
        break;
    }
    case 'X': {
        const int integer = va_arg(*args, const int);
        stringify_hex(integer, string_buffer, BUFFER_SIZE, true, true);
        put_string(string_buffer);
        break;
    }
    case 'i': {
        // print hex of padded to eight digits
        const int inst = va_arg(*args, const int);
        stringify_hex(inst, string_buffer, BUFFER_SIZE, false, false);
        put_string(string_buffer);
        break;
    }
    case 'p': {
        (*format)++;
        switch (**format) {
        case 'L': {
            const list_t* list = va_arg(*args, const list_t*);
            print_list(list);
            break;
        }
        case 'F': {
            const int* func = va_arg(*args, const int*);
            print_func(func);
            break;
        }
        // default includes '\0'
        default: {
            (*format)--; // no further option, move pointer back to continue printing
            const void* ptr = va_arg(*args, const void*);
            print_ptr(ptr);
        }
        }
        break;
    }
    case 's': {
        const char* string = va_arg(*args, const char*);
        print_string(string);
        break;
    }
    default:;
    }
}

/** Prints given formatted string to console.
 *
 * @param format printf-style formatting string.
 */
void printk(const char* format, ...) {
    va_list args;
    va_start(args, format);

    const char* it = format;
    bool conversion = false;

    for (; *it != 0; it++) {

        if (conversion) {
            convert(&it, &args);
            conversion = false;
        } else if (*it == '%') {
            conversion = true;
        } else {
            printer_putchar(*it);
        }
    }
    va_end(args);
}

/** Prints given string to console, terminating it with newline.
 *
 * @param s String to print.
 */
void puts(const char* s) {
    while (*s != '\0') {
        printer_putchar(*s);
        s++;
    }
    printer_putchar('\n');
}
