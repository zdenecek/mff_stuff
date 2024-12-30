// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#ifndef FREQ_H_
#define FREQ_H_

#include "list.h"

/*
 * Forward declaration of the structure.
 *
 * Note that since the API always uses only pointer
 * to this structure, we can keep it opaque for the
 * user and change it without changing the API.
 */

struct freq_str;
typedef struct freq_str freq_str_t;

extern freq_str_t* freq_str_create(void);
extern int freq_str_add(freq_str_t*, const char*);
extern int freq_str_get(freq_str_t*, const char*);
extern void freq_str_destroy(freq_str_t*);

#endif
