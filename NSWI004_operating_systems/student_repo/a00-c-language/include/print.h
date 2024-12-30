// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#ifndef PRINT_H_
#define PRINT_H_

typedef struct {
    void* data;
    unsigned long size;
} simple_printf_buffer_t;

extern void simple_printf(const char* format, ...);

#endif
