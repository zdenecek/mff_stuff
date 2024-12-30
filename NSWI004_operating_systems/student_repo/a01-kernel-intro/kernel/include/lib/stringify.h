#ifndef _LIB_STRINGIFY_H
#define _LIB_STRINGIFY_H

#include <types.h>

extern int stringify_int(int number, char* buffer, size_t buffersize);
extern int stringify_uint(unsigned int number, char* buffer, size_t buffersize);
extern int stringify_hex(long number, char* buffer, size_t buffersize, bool upper, bool remove_zeroes);

#endif
