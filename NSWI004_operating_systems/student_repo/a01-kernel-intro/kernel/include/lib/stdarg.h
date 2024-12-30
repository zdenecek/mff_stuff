// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#ifndef _STDARG_H_
#define _STDARG_H_

/* The va_* macros and functions rely on compiler intrinsics. */

typedef __builtin_va_list va_list;

#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)
#define va_copy(dst, src) __builtin_va_copy(dst, src)

#endif
