// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <stdarg.h>
#include <stdio.h>

#include "print.h"

static void print_arguments(const char* header, ...) {
    puts(header);

    va_list args;
    va_start(args, header);
    const char* argument = va_arg(args, const char*);
    while (argument != NULL) {
        printf(" - '%s'\n", argument);
        argument = va_arg(args, const char*);
    }
    va_end(args);
}

int main() {
    print_arguments("Start of alphabet", "a", "b", "c", NULL);
    print_arguments("No arguments", NULL);
    print_arguments("Bad usage", "a", "b", NULL, "never printed", NULL);

    return 0;
}
