// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <lib/runtime.h>

static inline unsigned long long ll_abs(long long a) {
    if (a < 0) {
        return -a;
    } else {
        return a;
    }
}

static inline int ll_simple_sgn(long long a) {
    if (a >= 0) {
        return 1;
    } else {
        return 0;
    }
}

static unsigned long long ull_div_and_mod(unsigned long long a,
        unsigned long long b, unsigned long long* rem) {

    if (b == 0) {
        *rem = 0;
        return 0;
    }

    if (a < b) {
        *rem = a;
        return 0;
    }

    int ull_size = sizeof(unsigned long long) * 8;
    *rem = 0;
    unsigned long long res = 0;

    for (int steps = ull_size; steps > 0; steps--) {
        *rem = ((*rem) << 1) | ((a >> (ull_size - 1)) & 0x1);
        res <<= 1;

        if (*rem >= b) {
            *rem -= b;
            res |= 0x1;
        }
        a <<= 1;
    }

    return res;
}

unsigned long long __udivdi3(unsigned long long a, unsigned long long b) {
    unsigned long long unused_rem;
    return ull_div_and_mod(a, b, &unused_rem);
}

unsigned long long __umoddi3(unsigned long long a, unsigned long long b) {
    unsigned long long rem;
    ull_div_and_mod(a, b, &rem);
    return rem;
}

long long __divdi3(long long a, long long b) {
    unsigned long long unused_rem;
    unsigned long long res = ull_div_and_mod(ll_abs(a), ll_abs(b), &unused_rem);
    if (ll_simple_sgn(a) == ll_simple_sgn(b)) {
        return res;
    } else {
        return -res;
    }
}

long long __moddi3(long long a, long long b) {
    unsigned long long rem;
    ull_div_and_mod(ll_abs(a), ll_abs(b), &rem);
    if (a < 0) {
        return -rem;
    } else {
        return rem;
    }
}
