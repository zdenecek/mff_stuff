// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#ifndef _LIB_RUNTIME_H
#define _LIB_RUNTIME_H

/*
 * Declarations of functions needed for division of long long values.
 */

unsigned long long __udivdi3(unsigned long long, unsigned long long);
unsigned long long __umoddi3(unsigned long long, unsigned long long);
long long __divdi3(long long, long long);
long long __moddi3(long long, long long);

#endif
