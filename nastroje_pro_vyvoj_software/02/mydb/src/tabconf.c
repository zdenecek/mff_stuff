#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "memory.h"
#include "error.h"
#include "linklist.h"
#include "strutil.h"
#include "textfileio.h"
#include "tabconf.h"


st_table_conf* get_tableconf(const char *tabname)
{
	st_file_info *fi;
    char *str;
    st_table_conf *tabconf = NULL;

    fi = file_open(current_db,NULL,FILE_TABCONF);
    if (!fi) return NULL;

    /* test for existence of table */
    while ((str = file_read_string(fi)))
    {
        tabconf = string_tabconf(str);
        if (!tabconf)
        {
            xfree(str);
            return NULL;
        }
        if (STR_EQ(tabconf->name,tabname)) break;
        destroy_tabconf(tabconf);
        tabconf = NULL;
    }

	if (!tabconf) set_error(ERR_TABNOEXIST);
	
    if (!file_close(fi))
    {
        destroy_fileinfo(fi);
        destroy_tabconf(tabconf);
        return NULL;
    }

    return tabconf;
}

int write_tableconf(const st_table_conf *tabconf)
{
	int ret;
    char *str;
    st_file_info *fi;

    fi = file_open(current_db,NULL,FILE_TABCONF);
    if (!fi) return 0;

    str = tabconf_string(tabconf);
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


st_table_conf* delete_tableconf(const char *tabname)
{
    st_file_info *fi;
    char *str;
    st_table_conf *tabconf;
    char *oldpos;

    fi = file_open(current_db,NULL,FILE_TABCONF);
    if (!fi) return 0;

    /* find correct table conf */
    do
    {
        oldpos = fi->readpos;

        str = file_read_string(fi);
        if (!str) return NULL;

        tabconf = string_tabconf(str);
        if (!tabconf)
        {
            xfree(str);
            return NULL;
        }

        if (STR_EQ(tabconf->name,tabname))
        {
            fi->readpos = oldpos;
            break;
        }
        xfree(str);
        destroy_tabconf(tabconf);
        tabconf = NULL;
    } while (1);

	xfree(str);
    destroy_tabconf(tabconf);

    str = file_delete_string(fi);
    if (!str) return NULL;

    tabconf = string_tabconf(str);
    if (!tabconf)
    {
        xfree(str);
        return NULL;
    }

    xfree(str);

    if (!file_close(fi))
    {
        destroy_fileinfo(fi);
        destroy_tabconf(tabconf);
        return NULL;
    }

    return tabconf;
}


char* tabconf_string(const st_table_conf *tabconf)
{
	size_t size;
	char *str;
	st_index_col *idxcol;
	
	size = strlen(tabconf->name)+1+strlen(tabconf->datafile);

	idxcol = tabconf->indexcols;
	while (idxcol)
	{
		size++; /* for ',' */
		size += strlen(idxcol->colname);
		idxcol = idxcol->NEXT;
	}

	/* i will put whole tabconf into this string */
	str = (char*)xmalloc(sizeof(char)*(size+1));
	if (!str) return 0;

	str = strncpy(str,tabconf->name,strlen(tabconf->name)+1);
	str = strncat(str,",",2); /* separator */
	str = strncat(str, tabconf->datafile, strlen(tabconf->datafile)+1);
	
	idxcol = tabconf->indexcols;
	while (idxcol)
	{
		str = strncat(str,",",2);
		str = strncat(str,idxcol->colname,strlen(idxcol->colname));

		idxcol = idxcol->NEXT;
	}

	return str;
}


st_table_conf* string_tabconf(const char* str)
{
	st_table_conf *tabconf;
	const char *s;
	char *token;
	st_index_col *head,*prev,*idxcol;

	head = NULL;
	prev = NULL;
	idxcol = NULL;

	if (!str) goto error1;

	s = str;

	tabconf = (st_table_conf*)xmalloc(sizeof(st_table_conf));
	if (!tabconf) return NULL; /* error */

	token = str_copy(s,',');
	if (!token) return NULL;
	s += (strlen(token)+1);
	tabconf->name = token;

	if (strchr(s,',')) /* there are some indexed columns */
	{
		token = str_copy(s,',');
        if (!token)
		{
			destroy_tabconf(tabconf);
			return NULL;
		}
        s += (strlen(token)+1);
        tabconf->datafile = token;

		while (strchr(s,',')) /* loop over all indexed columns */
		{
			idxcol = (st_index_col*)xmalloc(sizeof(st_index_col));
			if (!idxcol) goto error2;

			token = str_copy(s,',');
			if (!token) goto error3;
			idxcol->colname = token;
			s += (strlen(token)+1);

			LIST_ADDTAIL(head,prev,idxcol);
		}

		idxcol = (st_index_col*)xmalloc(sizeof(st_index_col));
        if (!idxcol) goto error2;

        token = str_copy(s,0);
        if (!token) goto error3;
        idxcol->colname = token;
        s += (strlen(token)+1);

		LIST_ADDTAIL(head,prev,idxcol);

		tabconf->indexcols = head;
	}
	else
	{
		token = str_copy(s,0);
		if (!token)
		{
			destroy_tabconf(tabconf);
			return NULL;
		}
		s += (strlen(token)+1);
		tabconf->datafile = token;

		tabconf->indexcols = NULL;
	}

	return tabconf;

error3:
	destroy_indexcol(idxcol,0);
error2:
	destroy_indexcol(head,1);
	destroy_tabconf(tabconf);
	return NULL;
error1:
	set_error(ERR_FATIO);
	return NULL;
}


int table_indexed(const char *tabname)
{
	st_table_conf *tabconf;

    tabconf = get_tableconf(tabname);
    if (!tabconf) return (-1); /* table doesn't exist */

	if (tabconf->indexcols) return 1; /* there is indexed column */

	return 0;
}


int column_indexed(const char *tabname, const char *colname)
{
	st_table_conf *tabconf;
	st_index_col *idxcol;

	tabconf = get_tableconf(tabname);
	if (!tabconf) return (-1); /* table doesn't exist */	

	if (!tabconf->indexcols) 
	{
		set_error(ERR_IDXEXIST);
		return 0;
	}

	idxcol = tabconf->indexcols;

	while (idxcol)
	{
		if (STR_EQ(idxcol->colname,colname)) 
		{
			destroy_tabconf(tabconf);
			return 1;
		}
		idxcol = idxcol->NEXT;
	}

	destroy_tabconf(tabconf);

	set_error(ERR_IDXEXIST);
	return 0;
}


int add_indexcol(st_table_conf* const tabconf,const char *colname)
{
	st_index_col *cur,*prev;
	char *s;

	cur = (st_index_col*)xmalloc(sizeof(st_index_col));
	if (!cur) return 0;

	s = str_copy(colname,0);
	if (!s)
	{
		xfree(cur);
		return 0;
	}

	cur->colname = s;
	cur->NEXT = NULL;

	if (!tabconf->indexcols)
	{
		tabconf->indexcols = cur;
		return 1;
	}
	else
	{
		prev = tabconf->indexcols;
		while (prev->NEXT) prev = prev->NEXT;

		prev->NEXT = cur;
		
		return 1;
	}

	return 1;
}


int del_indexcol(st_table_conf* const tabconf,const char *colname)
{
	st_index_col *cur,*prev;

	if (!tabconf->indexcols)
	{
		set_error(ERR_IDXNOEX);
		return 0;
	}

	cur = tabconf->indexcols;

	if (STR_EQ(cur->colname,colname))
	{
		tabconf->indexcols = cur->NEXT;
		cur->NEXT = NULL;
		destroy_indexcol(cur,0);
	}
	else
	{
		while ((cur) && (!STR_EQ(cur->colname,colname)))
		{
			prev = cur;
			cur = cur->NEXT;
		}

		if (!cur) return 0; /* no column */

		prev->NEXT = cur->NEXT;

		cur->NEXT = NULL;
		destroy_indexcol(cur,0);
	}

	return 1;
}


void destroy_tabconf(st_table_conf *tabconf)
{
	if (tabconf)
	{
    	if (tabconf->name) xfree(tabconf->name);
	    if (tabconf->datafile) xfree(tabconf->datafile);
		destroy_indexcol(tabconf->indexcols,1);
    	xfree(tabconf);
	}
}

void destroy_indexcol(st_index_col *idxcol, int all)
{
	st_index_col *head,*col;

	if (all)
	{
		head = idxcol;
		while (head)
		{
			col = head;
			head = head->NEXT;
			
			if (col->colname) xfree(col->colname);
            xfree(col);
		}
	}
	else
	{
		if (idxcol)
		{
			if (idxcol->colname) xfree(idxcol->colname);
			xfree(idxcol);
		}
	}
}
