#ifndef __LINKLIST_H__
#define __LINKLIST_H__

#include <stdlib.h>

/* pointer to the next element in the list */
#define NEXT next

/* 
  H is head of the list
  T is current tail(usually "prev"
  C is inserted element
*/
#define LIST_ADDTAIL(H,T,C); \
	C->NEXT = NULL; \
	if (H) \
	{ \
		T->NEXT = C; \
		T = T->NEXT; \
	} \
	else \
	{ \
		H = C; \
		T = C; \
	} 

/* 
  H is head of the list
  C is inserted element
*/
#define LIST_ADDHEAD(H,C); \
	if (H) \
	{ \
		C->NEXT = H; \
		H = C; \
	} \
	else \
	{ \
		C->NEXT = NULL;	\
		H = C; \
	} \

		
#endif
