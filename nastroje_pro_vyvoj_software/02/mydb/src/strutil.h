#ifndef __STRUTIL_H__
#define __STRUTIL_H__

#include <string.h>
#include <ctype.h>

#define STR_EQ(A,B) (strcmp((A),(B)) == 0)

/* test if B is at the beginning of A */
#define STR_BEGIN(A,B) (A == strstr((A),(B)))

/* convert all characters in string to uppercase */
char* strtoupper(char *s);

/* convert all characters in string to lowercase */
char* strtolower(char *s);

/* 
  case insensitive version of strstr
  NULL - error 
*/
char* strstr_icase(const char *s1, const char *s2);

/* 
  copy part of memory from start up to the delimiter and create new string
  delimiter is not included
  delim == 0 -> copy whole string beginning at start
  NULL - error 
*/
char* str_copy(const char *start, int delim);

/* 
  replaces string with some other string
  returns newly created string 
*/
char* str_replace(const char *str, const char *from, char *to);

/* 
  0 - no
  1 - yes(it has " or ' on the sides
*/
int is_string(const char *s);

/* 
  0 - no
  1 - yes(all chars are numeric 
*/
int is_num(const char *s);

/* 
  concatenates two strings and puts char c between them
  NULL - error 
*/
char* str_concat(const char *s1, const char *s2, char c);

#endif
