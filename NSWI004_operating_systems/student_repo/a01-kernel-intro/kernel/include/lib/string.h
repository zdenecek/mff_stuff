#ifndef _LIB_STRING_H
#define _LIB_STRING_H

#include <types.h>

// Trivial, copied from https://stackoverflow.com/questions/8534274/is-the-strrev-function-not-available-in-linux/8534275#8534275
char* strrev(char* str);
bool strcmp(const char* one, const char* two);
int strlen(const char* str);

#endif
