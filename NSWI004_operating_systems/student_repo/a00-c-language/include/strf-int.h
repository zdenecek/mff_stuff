#ifndef STRFINT_H
#define STRFINT_H

#include <stdbool.h>
#include <stdio.h>

extern int stringify_int(int number, char* buffer, size_t buffersize);
extern int stringify_hex(int number, char* buffer, size_t buffersize, bool upper);
extern int stringify_hex_long(long number, char* buffer, size_t buffersize, bool upper);

#endif
