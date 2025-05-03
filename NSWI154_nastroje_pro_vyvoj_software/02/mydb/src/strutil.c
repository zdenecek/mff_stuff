#include "error.h"
#include "memory.h"
#include "strutil.h"


char* strtoupper(char *s)
{
	size_t size;
	int new_c;
	char *c;
	int i;

	size = strlen(s);
	c = s;
	
	for (i=0;i<size;i++)
	{
		new_c = toupper(*c);
		*c=new_c;
		c++;
	}

	return s;	
}


char* strtolower(char *s)
{
	size_t size;
	int new_c;
	char *c;
	int i;

	size = strlen(s);
	c = s;

	for (i=0;i<size;i++)
	{
    	new_c = tolower(*c);
	    *c=new_c;
		c++;
	}

  	return s;
}


char* strstr_icase(const char *s1, const char *s2)
{
	const char *s3,*s4;
  
	if (!s1 || !s2)
  	{
		set_error(ERR_STRANGE);
		return NULL;
  	}

  	while (*s1)
  	{
    	if (tolower(*s1) == tolower(*s2))
    	{
      		s3 = s2;
      		s4 = s1;
	  		/* go through the smaller string and compare all chars */
      		while ( (*s4) && (*s3) && (tolower(*s4) == tolower(*s3)) )
      		{
        		s4++;
        		s3++;
      		}
      		if (!(*s3)) return (char*)s1; /* we have found whole s2 in s1 */
      		++s1;
    	}
    	else s1++;
  	}
  	return NULL; /* haven't found anything */
}


char* str_copy(const char *start, int delim)
{
	char *ch_adr;
  	size_t size;
  	char *token;

	if (!delim) /* take whole string */
	{
		size = strlen(start);
	}
	else
	{
  		ch_adr = strchr(start,delim);
  		if (!ch_adr)
  		{
			set_error(ERR_FATIO);
    		return NULL;
  		}
  	  	size = ch_adr-start; /* size of new string */
	}

  	token = (char*)xmalloc(sizeof(char)*(size+1));
  	if (!token) 
  	{
		set_error(ERR_NOMEM);
		return NULL;
  	}
  	token = strncpy(token,start,size+1);
  	token[size]=0;

  	return token;
}


char* str_replace(const char *str, const char *from, char *to)
{
	char *tmp_str = NULL;
  	int from_size,to_size;
  	size_t size,new_size;
  	int count;
  	const char *s = NULL, *s2 = NULL;
  	char *new_s = NULL;

  	size = strlen(str);
  	from_size = strlen(from);
  	to_size = strlen(to);

  	s = str;
  	count = 0;
  	while (STR_BEGIN(s,from)) 
  	{
		count++;
		s++;
  	}

  	new_size = size + (count*to_size)-(count*from_size); /* we must add total size of inserted characters and substract the total size of replaced chars */

  	tmp_str=(char*)xmalloc(sizeof(char)*(new_size+1));
  	if (!tmp_str) return NULL;

  	s = str;
	new_s = tmp_str;
	while ((s2 = strstr(s,from)))
  	{
		/* copy characters up to the from string */
		new_s = strncat(new_s,s,s2-s);
		new_s += (s2-s);
	
		/* replace from with to */
		new_s = strncat(new_s,to,to_size);
	
    	/* move pointers behind from and to strings */
		new_s += to_size;
		s = s2 + from_size;
  	}

  	/* append the rest of input string */
  	new_s = strncat(new_s,s,(str+size)-s);

  	return tmp_str;
}


int is_string(const char *s)
{
	size_t size;
  	int i;

  	size = strlen(s);

  	/* opening quotes */
  	if ((*s != '\"') && (*s != '\'')) return 0;
  	s++;

  	for (i=0;i<(size-2);i++)
  	{
    	if (!isprint(*s)) return 0;
    	s++;
  	}

  	/* ending quotes */
  	if ((*s != '\"') && (*s != '\'')) return 0;

  	return 1;
}


int is_num(const char *s)
{
	size_t size;
  	int i;

  	size = strlen(s);

  	for (i=0;i<size;i++)
  	{
    	if (!isdigit(*s)) return 0;
    	s++;
  	}

  	return 1;
}


char* str_concat(const char *s1, const char *s2, char c)
{
	size_t size1,size2;
	char *s;

	size1 = strlen(s1);
	size2 = strlen(s2);

    s = (char*)xmalloc(sizeof(char)*(size1+size2+2));
    if (!s) return NULL;

    s = strncpy(s,s1,size1+1);
	s[size1] = c;
	s[size1+1] = 0;
    s = strncat(s,s2,size2+1);
	s[size2+size1+1] = 0;

	return s;
}
