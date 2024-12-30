// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#ifndef _DEBUG_H
#define _DEBUG_H

#include <drivers/machine.h>
#include <lib/print.h>

/* Helper macros. */
#define QUOTE_ME_(x) #x
#define QUOTE_ME(x) QUOTE_ME_(x)

/*
 * dprintk works like printk but also adds source code information.
 */
#ifdef KERNEL_DEBUG
#define dprintk(fmt, ...) \
    printk("[DEBUG " __FILE__ ":" QUOTE_ME(__LINE__) " %s()] " fmt, \
            __func__, ##__VA_ARGS__)
#else
#define dprintk(...) ((void)0)
#endif

/*
 * When assertion in kernel fails, we halt the machine.
 */
#ifdef KERNEL_DEBUG
#define assert(expr) \
    do { \
        if (!(expr)) { \
            puts("Assert failed at " __FILE__ ":" QUOTE_ME(__LINE__) ": " #expr); \
            machine_halt(); \
        } \
    } while (0)
#else
#define assert(expr) ((void)0)
#endif

#endif
