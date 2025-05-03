#include <string.h>
#include <stdio.h>
#include "error.h"
#include "memory.h"


/* number of currently allocated blocks of memory */
static long alloc_count=0;


void* xmalloc(size_t size)
{
	void* p;

	p = malloc(size);

	/*
	if (!p)
	{
		set_error(ERR_NOMEM);
		return NULL;
  	}
	*/
	
	if (!p)
	{
		perror("Out of memory");
		exit(EXIT_FAILURE);
		return NULL;
	}

	p = memset(p,0,size); /* clearing allocated memory to 0's */

	alloc_count++;

	return p;
}


void xfree(void *p)
{
	if (p)
	{
    	free(p);

    	alloc_count--;
	}
}


int check_status(void)
{
	if (alloc_count > 0) return 1;
	if (alloc_count < 0) return 2;

	return 0; 
}
