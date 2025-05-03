#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memory.h"
#include "strutil.h"
#include "config.h"

char *datadir = NULL;

char *current_db = NULL;


int read_config(void)
{
	FILE *file;
	char *line;
	char *token;
	size_t size;
	
	file = fopen("mydb.cfg","r");
	if (!file) return 0;

	line = (char*)xmalloc(sizeof(char)*256);

	line = fgets(line,255,file);
	if (!line) return 0;

	/* reads key */
	token = strtok(line," ");
	if (!token) goto error;
 
	if (!STR_EQ(token,"datadir")) goto error;

	/* reads value */
	token = strtok(0," ");
	if (!token) goto error;

	size = strlen(token);
	
	datadir = (char*)xmalloc(size*sizeof(char));
	if (!datadir) goto error;

	/* set the path to data files, etc */
	datadir = strncpy(datadir,token,size-1);

	/* we change ending '\n' to NULL character */	
	datadir[size-1]=0; 	

	if (fclose(file) == EOF) goto error;

	xfree(line);
  
	return 1;
error:
	xfree(line);
	return 0;
}

void destroy_config(void)
{
	if (datadir) xfree(datadir);
	if (current_db) xfree(current_db);
}

