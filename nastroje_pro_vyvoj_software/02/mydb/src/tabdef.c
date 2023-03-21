#include <stdlib.h>
#include <string.h>
#include "tabdef.h"
#include "defs.h"
#include "linklist.h"
#include "memory.h"
#include "error.h"
#include "config.h"
#include "strutil.h"
#include "textfileio.h"


st_table_def* get_tabledef(const char *tabname)
{
	st_file_info *fi;
	char *str = NULL;
	st_table_def *tabdef = NULL;

	if (!tabname)
	{
		set_error(ERR_STRANGE);
		return NULL;
	}

	if (!current_db)
	{
		set_error(ERR_NODBSEL);
		return NULL;
	}

	fi = file_open(current_db,NULL,FILE_TABDEF);
	if (!fi) return NULL;

	/* test for existence of table */
	while ((str = file_read_string(fi)))
	{ 
		tabdef = string_tabdef(str);
		if (!tabdef)
		{
			xfree(str);
			return NULL;
		}		
	    if (STR_EQ(tabdef->name,tabname)) break;
    	destroy_tabdef(tabdef);
	    tabdef = NULL;
  	}
 
	if (!tabdef) set_error(ERR_TABNOEXIST);
 
	if (!file_close(fi))
	{ 
		destroy_fileinfo(fi);
		destroy_tabdef(tabdef);
    	return NULL;
	}

	return tabdef;
}


int write_tabledef(const st_table_def *tabdef)
{
	int ret;
	char *str;
	st_file_info *fi;

	fi = file_open(current_db,NULL,FILE_TABDEF);
    if (!fi) return 0;

	str = tabdef_string(tabdef);
	if (!str) return 0;

	ret = file_write_string(fi,str);
	if (!ret)
	{
		xfree(str);
		return 0;
	}
	
    if (!file_close(fi))
    { 
        destroy_fileinfo(fi);
        return 0;
    }

	return 1;
}


st_table_def* delete_tabledef(const char *tabname)
{
	st_file_info *fi;
    char *str;
    st_table_def *tabdef;
	char *oldpos;

    fi = file_open(current_db,NULL,FILE_TABDEF);
    if (!fi) return 0;

	/* find correct table def */
	do
	{
		oldpos = fi->readpos;

		str = file_read_string(fi);
		if (!str) return NULL;

		tabdef = string_tabdef(str);
		if (!tabdef)
		{
			xfree(str);
			return NULL;
		}
		
        if (STR_EQ(tabdef->name,tabname))
		{
			fi->readpos = oldpos;
			break;
		}
		xfree(str);
        destroy_tabdef(tabdef);
        tabdef = NULL;
    } while (1);

	xfree(str);
	destroy_tabdef(tabdef);

	str = file_delete_string(fi);
	if (!str) return NULL;

	tabdef = string_tabdef(str);
    if (!tabdef)
    {
    	xfree(str);
        return NULL;
    }

	xfree(str);

    if (!file_close(fi))
    {
        destroy_fileinfo(fi);
        destroy_tabdef(tabdef);
        return NULL;
    }

    return tabdef;
}


int tabledef_size(const st_table_def *tbdef)
{
	st_col_def *coldef;
	size_t tbdefsize;
	char s1[CHAR_COLTYPE+1];
	char s3[CHAR_COLNUM+1];

	tbdefsize = 0;

	tbdefsize += (strlen(tbdef->name)+1); /* 1 is for separator */

	snprintf(s3,CHAR_COLNUM+1,"%d",tbdef->col_num);
	tbdefsize += (strlen(s3)+1);

	coldef = tbdef->columns;

	while (coldef)
	{
		tbdefsize += (strlen(coldef->name)+1);
	
		snprintf(s1,CHAR_COLTYPE+1,"%d",coldef->type);
	    tbdefsize += (strlen(s1)+1);

		coldef = coldef->NEXT;
	}

	return tbdefsize;
}


char* tabdef_string(const st_table_def *tabdef)
{
	char s1[CHAR_COLTYPE+1];
	char s2[CHAR_COLNUM+1];
	st_col_def *col;
	int size;
	char *str;

	size = tabledef_size(tabdef);

	/* i will put whole tabdef into this string */
	str = (char*)xmalloc(sizeof(char)*(size+1));
	if (!str) return 0;

	str = strncat(str,tabdef->name,strlen(tabdef->name)+1);
	str = strncat(str,":",2); /* separator */

	/* converts int to string */
	snprintf(s2,CHAR_COLNUM+1,"%d",tabdef->col_num);

	str = strncat(str,s2,strlen(s2)+1);
	str = strncat(str,":",2);

	col = tabdef->columns;

	/* loop over all columns */
	while (col)
	{
		str = strncat(str,col->name,strlen(col->name)+1);
		str = strncat(str,",",2);

		snprintf(s1,CHAR_COLTYPE+1,"%d",col->type);
		str = strncat(str,s1,strlen(s1)+1);

		str = strncat(str,",",2);

		col = col->NEXT;
	}

	return str;
}


st_table_def* string_tabdef(const char *str)
{
	st_table_def *tabdef = NULL;
	st_col_def *coldef = NULL, *head = NULL, *prev = NULL;
	char *token = NULL;
	const char *s;
	int i,c,j;

	if (!str) goto error1;

	s = str;

	tabdef = (st_table_def*)xmalloc(sizeof(st_table_def));
	if (!tabdef) return NULL; /* error */
  
	token = str_copy(s,':');
	if (!token) return NULL;
	s += (strlen(token)+1); /* +1 is for separator */
	tabdef->name = token;

	token = str_copy(s,':');
	if (!token) return NULL;
	s += (strlen(token)+1);
	c = atoi(token);
	tabdef->col_num = c;
	xfree(token);

	head = NULL;
	prev = NULL;
	coldef = NULL;

    /* read all columns */
	for (i=0;i<tabdef->col_num;i++)
	{
	    /* creating array */
		coldef = (st_col_def*)xmalloc(sizeof(st_col_def));
	    if (!coldef) return NULL;

    	token = str_copy(s,',');
		if (!token) return NULL;
		s += (strlen(token)+1);
		coldef->name = token;

		token = str_copy(s,',');
		if (!token) return NULL;
		s += (strlen(token)+1);
		j = atoi(token);
		coldef->type = j;
		xfree(token);

		LIST_ADDTAIL(head,prev,coldef);
	}

	tabdef->columns = head;

	return tabdef;

error1:
	set_error(ERR_FATIO);
	return NULL;
}


void destroy_coldef(st_col_def *coldef,int all)
{  
	st_col_def *head,*cur;

	if (all)
	{
		head = coldef;
		while (head)
		{
			cur = head;
			head = head->NEXT;
			if (cur->name) xfree(cur->name);
			xfree(cur);
		}
	}
	else
	{
		if (coldef)
		{ 
			if (coldef->name) xfree(coldef->name);
			xfree(coldef);
		}
	}
}   


void destroy_tabdef(st_table_def *tabdef)
{
	if (tabdef)
	{
		if (tabdef->name) xfree(tabdef->name);

		if (tabdef->columns) destroy_coldef(tabdef->columns,1);
    	xfree(tabdef);
  	}
} 

int correct_colvalue(const st_table_def *tabdef, const char *colname, const char *value)
{
	st_col_def *coldef;
	en_col_type coltype;
	size_t size;
	char *s;
	const char *str;

	coldef = tabdef->columns;

	s = strchr(colname,'.');
	if (s) str = ++s; /* skip tabname */
	else str = colname;

	while (coldef)
	{
		coltype = 0;
		if (is_num(value)) coltype = INT;
		if (is_string(value)) coltype = VARCHAR;
		if (!coltype) return 0;

		size = strlen(value);
		if ((coltype == INT) && (size > DATA_INT_SIZE)) 
			return 0; /* number is too long */
		if ((coltype == VARCHAR) && (size > DATA_VARCHAR_SIZE)) 
			return 0; /* string is too long */

		if ((STR_EQ(str,coldef->name)) && (coltype == coldef->type)) 
			return 1;

		coldef = coldef->NEXT;				
	}	

	return 0; /* no such column found */
}


int exist_column(const char *tabname, const char *colname)
{
	st_table_def *tabdef;
	st_col_def *coldef;

	tabdef = get_tabledef(tabname);
	if (!tabdef) return 0;

	coldef = tabdef->columns;

	while (coldef)
	{
		if (STR_EQ(coldef->name,colname)) 
		{
			destroy_tabdef(tabdef);
			return 1;
		}

		coldef = coldef->NEXT;
	}	
	
	return 0;
}

st_col_def* copy_coldef(const st_col_def *oldcldf)
{
	st_col_def *coldef,*head,*prev;
	char *str;

	head = NULL;
	prev = NULL;
	coldef = NULL;

	while (oldcldf)
	{
		coldef = (st_col_def*)xmalloc(sizeof(st_col_def));
		if (!coldef) goto error1;

		coldef->type = oldcldf->type;

		str = str_copy(oldcldf->name,0);
		if (!str) goto error1;
		coldef->name = str;

		LIST_ADDTAIL(head,prev,coldef);

		oldcldf = oldcldf->NEXT;
	}

	return head;

error1:
	destroy_coldef(head,1);
	return NULL;
}

int column_type(const char *tabname, const char *colname, en_col_type typ)
{
	st_table_def *tabdef;
	st_col_def *coldef;

	if (!tabname) 
	{
		set_error(ERR_STRANGE);
		return (-1);
	}
	if (!colname) 
	{
		set_error(ERR_STRANGE);
		return (-1);
	}

	tabdef = get_tabledef(tabname);
	if (!tabdef) return (-1);

	coldef = tabdef->columns;
	while (coldef)
	{
		if (STR_EQ(coldef->name,colname)) break;
		coldef = coldef->NEXT;
	}		

	if (!coldef) 
	{
		destroy_tabdef(tabdef);
		set_error(ERR_COLNOEX);
		return (-1); /* no such column */
	}

	if (coldef->type == typ) 
	{
		destroy_tabdef(tabdef);
		return 1;
	}
	else 
	{
		destroy_tabdef(tabdef);
		return 0;
	}

	destroy_tabdef(tabdef);

	set_error(ERR_COLNOEX);
	return (-1);
}

int count_coldef(st_col_def *coldef)
{
	int count = 0;

	/* count columns */
    while (coldef)
    {
        count++;
        coldef = coldef->NEXT;
    }

	return count;
}
