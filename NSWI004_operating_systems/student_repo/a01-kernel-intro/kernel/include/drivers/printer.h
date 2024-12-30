// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#ifndef _DRIVERS_PRINTER_H_
#define _DRIVERS_PRINTER_H_

/** Console hardware address.
 *
 * Note that this is virtual address
 * (compare with settings in msim.conf).
 */
#define PRINTER_ADDRESS (0x90000000)

/** Display a single character.
 *
 * @param c The character to display.
 */
static inline void printer_putchar(const char c) {
    (*(volatile char*)PRINTER_ADDRESS) = c;
}

#endif
