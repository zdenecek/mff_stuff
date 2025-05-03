#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdlib.h>

/* allocate block of memory in more secure way */
void* xmalloc(size_t size);

/* free block of memory in more secure way */
void xfree(void *p);

/*
  checks memory status based on variables in this module
  0 - OK
  1 - some memory block not freed
  2 - too much calls of free
*/
int check_status(void);

#endif
