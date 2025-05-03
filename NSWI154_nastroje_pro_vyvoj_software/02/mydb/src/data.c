#include <string.h>
#include "defs.h"
#include "error.h"
#include "memory.h"
#include "linklist.h"
#include "strutil.h"
#include "textfileio.h"
#include "data.h"


int strip_tablename(const st_row_data* const row)
{
	st_col_data *col;
	char *s,*s2;

	col = row->data;

	while (col)
	{
		s = strchr(col->col_name,'.');

		if (s) /* there is table name */
		{
			s++;

			s2 = str_copy(s,0);
			if (!s2) return 0;

			xfree(col->col_name);
			col->col_name = s2;
		}

		col = col->NEXT;
	}

	return 1;
}


int add_tablename(st_row_data* const row, const char *tabname)
{
	st_col_data *col;
	char *s;

	col = row->data;
	while (col)
	{
		s = str_concat(tabname,col->col_name,'.');
		if (!s) return 0;

		xfree(col->col_name);
		col->col_name = s;

		col = col->NEXT;
	}

	return 1;
}


st_row_data* read_rowdata(st_file_info *fi,const char *tabname)
{
	char *str = NULL;
    st_row_data *rowdata = NULL;
	int ret;

    str = file_read_string(fi);
	if (!str) return NULL;
	
	rowdata = string_rowdata(str);
	if (!rowdata)
	{
		xfree(str);
		return NULL;
	}

	rowdata->NEXT = NULL;

	xfree(str);

	if (tabname)
	{
		ret = add_tablename(rowdata,tabname);
		if (!ret) 
		{
			destroy_rowdata(rowdata,0);
			return NULL;		
		}
	}

    return rowdata;
}


int write_rowdata(st_file_info *fi,const st_row_data *rowdata)
{
	int ret;
    char *str;

	ret = strip_tablename(rowdata);
	if (!ret) return 0;

    str = rowdata_string(rowdata);
    if (!str) return 0;

    ret = file_write_string(fi,str);
    if (!ret)
    {
        xfree(str);
        return 0;
    }

    return 1;
}


st_row_data* delete_rowdata(st_file_info *fi)
{
    char *str;
    st_row_data *rowdata;

    str = file_str_markdel(fi);
    if (!str) return NULL;

    rowdata = string_rowdata(str);
    if (!rowdata)
    {
        xfree(str);
        return NULL;
    }

    xfree(str);

    return rowdata;
}


char* rowdata_string(const st_row_data *row)
{
	size_t size;
	st_col_data *col;
	char s[CHAR_COLNUM+1];
	char *s2; 
	char *str;

	size = rowdata_size(row);

	/* i will put whole rowdata into this string */
	str = (char*)xmalloc(sizeof(char)*(size+1));
	if (!str) return NULL;
  
	/* get number of cols into string */
	snprintf(s,CHAR_COLNUM+1,"%ld",row->data_count);

	str = strncpy(str,s,strlen(s)+1);
	str = strncat(str,":",2); /* separator */
  
	col = row->data;

	/* loop over all columns */
	while (col)
	{
    	str = strncat(str,col->col_name,strlen(col->col_name)+1);
	    str = strncat(str,"#",2); /* separator */

		/* escape forbidden character */
		s2 = str_replace(col->data_value,"#","&hash;");
		str = strncat(str,s2,strlen(s2)+1);
		xfree(s2);

		str = strncat(str,"##",3); /* separator */
	
		col = col->NEXT;
	}

	return str;
}


st_row_data* string_rowdata(const char *str)
{
	st_row_data *row = NULL;
	st_col_data *col,*head,*prev;
	char *token = NULL;
	const char *s = NULL;    
	char *s2 = NULL,*s3 = NULL;
	int i,c;

	head = NULL;
	prev = NULL;
	col = NULL;

	if (!str) 
	{
		set_error(ERR_STRANGE);
		return NULL;
	}

	s = str;

	row = (st_row_data*)xmalloc(sizeof(st_row_data));
	if (!row) return NULL; /* error */

	token = str_copy(s,':');
	if (!token) goto error1;

	s += (strlen(token)+1);
	c = atoi(token);
	xfree(token);
 
	row->data_count = c;
 
	/* loop over all columns */
	for (i=0;i<row->data_count;i++)
	{
    	col = (st_col_data*)xmalloc(sizeof(st_col_data));
	    if (!col) goto error1;

    	token = str_copy(s,'#');
		if (!token) goto error1;
		s += (strlen(token)+1);
		col->col_name = token;

		token = str_copy(s,'#');
		if (!token) goto error1;

		/* change escaped character to original */
		s2 = str_replace(token,"&hash;","#");
		if (!s2) goto error1;

		s3 = str_copy(s2,0);
		if (!s3) goto error2;

		xfree(s2);

		s += (strlen(token)+1);
		xfree(token);
		col->data_value = s3;

		s++; /* skipping second # in separator */

		LIST_ADDTAIL(head,prev,col);		
	}

	row->data = head;

	return row;

error2:
	xfree(s2);
error1:
	destroy_rowdata(row,0);
	return NULL;
}


char* get_colvalue(const st_row_data *row, const char *colname)
{
	st_col_data *col;

	col = row->data;

	while (col)
	{
		if (STR_EQ(col->col_name,colname)) return col->data_value;

		col = col->NEXT;
	}

	return NULL;
}


void destroy_coldata(st_col_data *col,int all)
{
	st_col_data *head,*cur;

	if (all)
	{
		head = col;
		while (head)
		{
			cur = head;
			head = head->NEXT;
			if (cur->col_name) xfree(cur->col_name);
			if (cur->data_value) xfree(cur->data_value);
			xfree(cur);
		}
	}
	else
	{
		if (col)
		{
    		if (col->col_name) xfree(col->col_name);
		    if (col->data_value) xfree(col->data_value);
			xfree(col);
  		}
	}
}


void destroy_rowdata(st_row_data *row,int all)
{
	st_row_data *row2 = NULL;

	if (all)
	{
		while (row)
		{
			row2 = row;
			row = row->NEXT;
			if (row2->data) destroy_coldata(row2->data,1);
			xfree(row2);
		}	
	}
	else
	{
	    if (row)
		{
    		if (row->data) destroy_coldata(row->data,1);
    	    xfree(row);
  		}
	}
}


int rowdata_size(const st_row_data *values)
{
	st_col_data *coldata;
	size_t rowsize;
	char s[CHAR_COLNUM+1];
	char *s2;
    
	rowsize = 0;
    
	snprintf(s,CHAR_COLNUM+1,"%ld",values->data_count);
 
	rowsize += (strlen(s)+1);

	coldata = values->data;

	while (coldata)
	{
	    rowsize += (strlen(coldata->col_name)+1);
    	s2 = str_replace(coldata->data_value,"#","&hash;");
	    rowsize += (strlen(s2)+2);
    	xfree(s2);
	    coldata = coldata->NEXT;
  	}
    
	return rowsize;
}


st_col_data* copy_coldata(const st_col_data *oldcol)
{
	st_col_data *col = NULL;
	char *str = NULL;

    col = (st_col_data*)xmalloc(sizeof(st_col_data));
	if (!col) goto error1;

	str = str_copy(oldcol->col_name,0);
    if (!str) goto error1;
    col->col_name = str;

    str = str_copy(oldcol->data_value,0);
    if (!str) goto error1;
    col->data_value = str;

    col->NEXT = NULL;

	return col;
error1:
	destroy_coldata(col,0);
	return NULL;
}

st_row_data* copy_rowdata(const st_row_data *oldrow)
{
	st_row_data *row = NULL;
	st_col_data *col,*head,*prev;
	st_col_data *oldcol = NULL;

	col = NULL;
	head = NULL;
	prev = NULL;

	row = (st_row_data*)xmalloc(sizeof(st_row_data));
	if (!row) return NULL;
  
	row->data_count = oldrow->data_count;

	oldcol = oldrow->data;

	while (oldcol)
	{
		col = copy_coldata(oldcol);
		if (!col) goto error1;

		LIST_ADDTAIL(head,prev,col);

		oldcol = oldcol->NEXT;
	}

	row->data = head;

	return row;

error1:
	destroy_coldata(head,1);
	destroy_rowdata(row,0);
	return NULL;
}
  

st_row_data* update_rowdata(const st_row_data *oldrow, const st_col_data *newdata)
{   
    st_row_data *newrow;
    st_col_data *cur,*head,*prev,*old_col;
    const st_col_data *col;
    char *str;
    int update_col;

    head = NULL;
    prev = NULL;

    newrow = (st_row_data*)xmalloc(sizeof(st_row_data));
    if (!newrow) return NULL;

    newrow->data_count = oldrow->data_count;

    /* loop over all columns in old row */
    old_col = oldrow->data;
    while (old_col)
    {
        cur = (st_col_data*)xmalloc(sizeof(st_col_data*));
        if (!cur) goto error1;

        update_col = 0;
        col = newdata;
        /* check if there is new value for this column */
        while (col)
        {
            if (STR_EQ(col->col_name,old_col->col_name)) /* yes, there is */
            {
                update_col = 1;
                break;
            }
            col = col->NEXT;
        }

        if (update_col) /* we will use new value */
        {
            str = str_copy(col->col_name,0);
            if (!str) goto error2;
            cur->col_name = str;

            str = str_copy(col->data_value,0);
            if (!str) goto error2;
            cur->data_value = str;
        }
        else /* we will copy old value */
        {
            str = str_copy(old_col->col_name,0);
            if (!str) goto error2;
            cur->col_name = str;

            str = str_copy(old_col->data_value,0);
            if (!str) goto error2;
            cur->data_value = str;
        }
        old_col = old_col->NEXT;

		LIST_ADDTAIL(head,prev,cur);
    }

    newrow->data = head;

    return newrow;
error2:
    destroy_coldata(cur,0);
error1:
    destroy_coldata(head,1);
    destroy_rowdata(newrow,0);
    return NULL;
}

int count_cols(st_col_data *col)
{
	int count = 0;

    while (col)
    {
        col = col->NEXT;
        count++;
    }
	
	return count;
}

