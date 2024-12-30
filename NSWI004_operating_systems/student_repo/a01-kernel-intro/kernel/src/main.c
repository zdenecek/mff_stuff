// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <drivers/printer.h>
#include <ktest.h>
#include <main.h>

/** This is kernel C-entry point.
 *
 * Kernel jumps here from assembly bootstrap code. Note that
 * this function runs on special stack and does not represent a
 * real thread (yet).
 *
 * When the code is compiled to run kernel test, we execute only
 * that test and terminate.
 */
void kernel_main(void) {
#ifdef KERNEL_TEST
    kernel_test();
#else
    printer_putchar('H');
    printer_putchar('e');
    printer_putchar('l');
    printer_putchar('l');
    printer_putchar('o');
    printer_putchar(',');
    printer_putchar(' ');
    printer_putchar('W');
    printer_putchar('o');
    printer_putchar('r');
    printer_putchar('l');
    printer_putchar('d');
    printer_putchar('!');
    printer_putchar('\n');
#endif
}
