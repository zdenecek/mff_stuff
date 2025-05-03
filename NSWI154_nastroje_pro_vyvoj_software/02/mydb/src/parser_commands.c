#include <string.h>
#include <stdlib.h>
#include "linklist.h"
#include "error.h"
#include "strutil.h"
#include "memory.h"
#include "parser_commands.h"
#include "parser_conds.h"


int parse_drop(parsed_str **inputquery, st_query_parsed *parsquery)
{
	parsed_str *query;
	char *s;
	st_query_drop *dropquery;

	query = *inputquery;

	dropquery = (st_query_drop*)xmalloc(sizeof(st_query_drop));
	if (!dropquery) return 0; /* error */

	query = skip_spaces(query);

	/* get the first word */
	query->token = strtolower(query->token);

	if (!STR_EQ(query->token,"drop")) goto error1;

	QUERY_SKIP /* skip drop */

	if (QUERY_NOT_OK) goto error1;

	query->token = strtolower(query->token);

	/* choose the right action */
	if (STR_EQ(query->token,"database")) dropquery->target = 1;
	else if (STR_EQ(query->token,"table")) dropquery->target = 2;
	else if (STR_EQ(query->token,"index")) dropquery->target = 3;
	else goto error1;

	QUERY_SKIP /* skip "database","table" or "index" */

	if (QUERY_NOT_OK) goto error1;

	query->token = strtolower(query->token);

	s = str_copy(query->token,0);
	if (!s) goto error2;
    
	dropquery->targetname = s;

	query = skip_token(query);

	(*inputquery) = query;
	parsquery->querydata.param_drop = dropquery;
	parsquery->action = DROP;

  	return 1;

error1:
	set_error(ERR_SQLSYNTAX);
error2:
  	destroy_querydrop(dropquery);
	(*inputquery) = query;
	return 0;
}


/* 0 - error */
int parse_use(parsed_str **inputquery, st_query_parsed* const parsquery)
{
	parsed_str *query;
	char *s;
	st_query_use* usequery;

	query = *inputquery;

	usequery = (st_query_use*)xmalloc(sizeof(st_query_use));
	if (!usequery) return 0; /* error */

	query = skip_spaces(query);

	/* get the first word */
  	query->token = strtolower(query->token);

	if (!STR_EQ(query->token,"use")) goto error1;

	QUERY_SKIP /* skip "use" */

	if (QUERY_NOT_OK) goto error1;

	query->token = strtolower(query->token);

	s = str_copy(query->token,0);
	if (!s) goto error2;

	usequery->dbname = s;

	(*inputquery) = query;
	parsquery->querydata.param_use = usequery;
	parsquery->action = USE_DB;
	return 1;
error1:
	set_error(ERR_SQLSYNTAX);
error2:
  	destroy_queryuse(usequery);
  	(*inputquery) = query;
  	return 0;
}


int parse_create(parsed_str **inputquery, st_query_parsed* const parsquery)
{
	parsed_str *query;
	int i;
	int ret;

	query = *inputquery;

	QUERY_SKIP

	if (!query) return 0;

	query->token = strtolower(query->token);

	i = 0;
	while (i < TABSIZE_CREATE)
	{
		if (STR_EQ(query->token,parscreatetab[i].query))
        {
			QUERY_SKIP

            ret = (parscreatetab[i].parsfunc) (&query,parsquery);
            if (!ret) 
			{
				(*inputquery) = query;
				return 0;
    		}
            break;
        }
        i++;
	}

	if (i == TABSIZE_CREATE) 
    {
        set_error(ERR_SQLSYNTAX);
		(*inputquery) = query;
        return 0;
    }

	(*inputquery) = query;
  	return ret;
}


/* returns NULL - error */
int parse_crdb(parsed_str **inputquery, st_query_parsed* const parsquery)
{
	parsed_str *query;
	char *s;
	st_query_crdb* crdbquery;

	query = *inputquery;

	crdbquery = (st_query_crdb*)xmalloc(sizeof(st_query_crdb));
	if (!crdbquery) return 0; /* error */

	query = skip_spaces(query);

	if (QUERY_NOT_OK) goto error1;

	query->token = strtolower(query->token);

	s = str_copy(query->token,0);
	if (!s) goto error2;

	crdbquery->name = s;

    (*inputquery) = query;
    parsquery->querydata.param_crdb = crdbquery;
    parsquery->action = CREATE_DB;
    return 1;
error1:
	set_error(ERR_SQLSYNTAX);
error2:
	destroy_querycrdb(crdbquery);
	(*inputquery) = query;
	return 0;
}


/* returns NULL - error */
int parse_crtab(parsed_str **inputquery, st_query_parsed* const parsquery)
{
	parsed_str *query;
	char *s;
	st_query_crtab *crtabquery;
	st_col_def *coldef;

	query = *inputquery;

	crtabquery = (st_query_crtab*)xmalloc(sizeof(st_query_crtab));
	if (!crtabquery) return 0; /* error */
  
	query = skip_spaces(query);

	if (QUERY_NOT_OK) goto error1;

	/* get table name */
	query->token = strtolower(query->token);

	s = str_copy(query->token,0);
	if (!s) goto error2;

	crtabquery->name = s;

	QUERY_SKIP

	if (QUERY_NOT_OK) goto error1;

	/* get columns definition */
	coldef = parse_coldef(&query);
	if (!coldef) goto error1;

	crtabquery->coldefs = coldef;
	crtabquery->col_num = count_coldef(coldef);

    (*inputquery) = query;
    parsquery->querydata.param_crtab = crtabquery;
    parsquery->action = CREATE_TAB;
    return 1;
error1:
	set_error(ERR_SQLSYNTAX);
error2:
	destroy_querycrtab(crtabquery);
	(*inputquery) = query;
	return 0;
}


/* returns NULL - error */
int parse_crindex(parsed_str **inputquery, st_query_parsed* const parsquery)
{
	parsed_str *query;
	char *s;
	st_query_crindex *crindexquery;

	query = *inputquery;

	crindexquery = (st_query_crindex*)xmalloc(sizeof(st_query_crindex));
	if (!crindexquery) return 0; /* error */

	query = skip_spaces(query);

	if (QUERY_NOT_OK) goto error1;

	query->token = strtolower(query->token);

	if (!STR_EQ(query->token,"on")) goto error1;

	QUERY_SKIP

	if (QUERY_NOT_OK) goto error1;

	s = str_copy(query->token,0);
	if (!s) goto error2;

	crindexquery->tabname = s;

	QUERY_SKIP

	if (QUERY_NOT_OK) goto error1;

	QUERY_SKIP /* we must skip '(' */
	
	s = str_copy(query->token,0);
	if (!s) goto error2;

	crindexquery->colname = s;

	QUERY_SKIP

	if (!query) goto error1;

	if (!STR_EQ(query->token,")")) goto error1;

    (*inputquery) = query;
    parsquery->querydata.param_crindex = crindexquery;
    parsquery->action = CREATE_INDEX;
    return 1;
error1:
	set_error(ERR_SQLSYNTAX);
error2:
	destroy_querycrindex(crindexquery);
	(*inputquery) = query;
  	return 0;
}


int parse_select(parsed_str **inputquery, st_query_parsed* const parsquery)
{
	parsed_str *query = NULL;
	char *s = NULL;
	st_query_select *selectquery = NULL;
	st_cond_node *condtree = NULL;
	st_query_colset *colset = NULL;
	st_query_tabset *tabset = NULL;

	query = *inputquery;

	selectquery = (st_query_select*)xmalloc(sizeof(st_query_select));
	if (!selectquery) return 0; /* error */

	query = skip_spaces(query);

	/* get the first word */
	query->token = strtolower(query->token);

	if (!STR_EQ(query->token,"select")) goto error1; 

	QUERY_SKIP

	if (QUERY_NOT_OK) goto error1;

	/* colset is required */
	colset = parse_colset(&query);
	if (!colset) goto error3;

  	selectquery->colset = colset;

  	query = skip_spaces(query);

  	if (QUERY_NOT_OK) goto error1;

	if (!STR_EQ(query->token,"from")) goto error1;

	/* we must skip "from" */
  	QUERY_SKIP

  	if (QUERY_NOT_OK) goto error1;

	tabset = parse_tabset(&query);
	if (!tabset) goto error3;

 	selectquery->tabset = tabset;
  
	query = skip_spaces(query);

	if (query) query->token = strtolower(query->token);

	if ((query) && (STR_EQ(query->token,"where")))
  	{
		/* we must skip "where" */
		QUERY_SKIP

		condtree = parse_conditions(&query);
	    if (!condtree) goto error3;

		selectquery->condtree = condtree;
	}
	else selectquery->condtree = NULL; /* no conditions */

	query = skip_spaces(query);

 	if (query)
  	{
    	query = skip_spaces(query);
  
    	if (STR_EQ(query->token,"order"))
    	{
	  		QUERY_SKIP

      		if (QUERY_NOT_OK) goto error1;

			/* we must skip "by" */
	  		QUERY_SKIP

      		if (QUERY_NOT_OK) goto error1;

      		s = str_copy(query->token,0);
      		if (!s) goto error1;

	  		selectquery->ordercol = s;
    	}
		else if (STR_EQ(query->token,";")) selectquery->ordercol = NULL;
		else goto error1;
	}
   	else selectquery->ordercol = NULL;


	(*inputquery) = query;
  	parsquery->action = SELECT;
  	parsquery->querydata.param_select = selectquery;
  	return 1;

error1:
  	set_error(ERR_SQLSYNTAX);
error3:
  	destroy_queryselect(selectquery);
  	(*inputquery) = query;
  	return 0;
}


int parse_insert(parsed_str **inputquery, st_query_parsed* const parsquery)
{
	parsed_str *query;
  	char *s;
  	st_query_insert *insertquery;
  	st_col_data *coldata;
 
	query = *inputquery;
 
	insertquery = (st_query_insert*)xmalloc(sizeof(st_query_insert));
  	if (!insertquery) return 0; /* error */

  	query = skip_spaces(query);
  
  	/* get the first word */
  	query->token = strtolower(query->token);
   	if (!STR_EQ(query->token,"insert")) goto error1;

  	QUERY_SKIP
  	if (QUERY_NOT_OK) goto error1;

  	query->token = strtolower(query->token);
  	if (!STR_EQ(query->token,"into")) goto error1;
 
  	QUERY_SKIP 
  	if (QUERY_NOT_OK) goto error1;

	/* get table name */
  	s = str_copy(query->token,0);
  	if (!s) goto error2;

	insertquery->tabname = s;
  
	QUERY_SKIP

	/* parse columns */
  	/* they are required */
  	coldata = parse_insertdata(&query);
  	if (!coldata) goto error2;

  	insertquery->coldata = coldata;

  	(*inputquery) = query;

  	parsquery->action = INSERT;
  	parsquery->querydata.param_insert = insertquery;
  	return 1;
error1:
  	set_error(ERR_SQLSYNTAX);
error2:
  	destroy_queryinsert(insertquery);
  	(*inputquery) = query;
  	return 0;
}


int parse_update(parsed_str **inputquery, st_query_parsed* const parsquery)
{
	parsed_str *query;
  	char *s;
  	st_query_update *updatequery;
  	st_cond_node *condtree;
  	st_col_data *coldata;

  	query = *inputquery;

  	updatequery = (st_query_update*)xmalloc(sizeof(st_query_update));
	if (!updatequery) return 0; /* error */

	query = skip_spaces(query);
  
  	/* get the first word */
  	query->token = strtolower(query->token);
  	if (!STR_EQ(query->token,"update")) goto error1;

  	QUERY_SKIP
  	if (QUERY_NOT_OK) goto error1;

  	/* get table name */
  	s = str_copy(query->token,0);
  	if (!s) goto error2;

	updatequery->tabname = s;
	
	QUERY_SKIP
	if (QUERY_NOT_OK) goto error1;

	query->token = strtolower(query->token);
	if (!STR_EQ(query->token,"set")) goto error1;

	QUERY_SKIP

  	/* columns to update/change */
  	/* they are required */
  	coldata = parse_updatedata(&query);
  	if (!coldata) goto error2;
  
  	updatequery->coldata = coldata;

  	query = skip_spaces(query);

	if (!query) goto error1;
 
  	if (STR_EQ(query->token,";")) /* end of query */
  	{
		updatequery->condtree = NULL;
    	(*inputquery) = query;
    	parsquery->action = UPDATE;
    	parsquery->querydata.param_update = updatequery;
    	return 1;
  	}

	if (!STR_EQ(query->token,"where")) goto error1;

	/* we must skip "where" */
	QUERY_SKIP
 
	condtree = parse_conditions(&query);
	if (!condtree) goto error2;
  
  	updatequery->condtree = condtree;

	(*inputquery) = query;
	parsquery->action = UPDATE;
	parsquery->querydata.param_update = updatequery;
  	return 1;
error1:
	set_error(ERR_SQLSYNTAX);
error2:
	destroy_queryupdate(updatequery);
  	(*inputquery) = query;
  	return 0; /* error */
}


int parse_delete(parsed_str **inputquery, st_query_parsed* const  parsquery)
{
	parsed_str *query;
	char *s;
	st_query_delete *deletequery;
	st_cond_node *condtree;
  
	query = *inputquery;

	deletequery = (st_query_delete*)xmalloc(sizeof(st_query_delete));
	if (!deletequery) return 0; /* error */

	query = skip_spaces(query);

	/* get the first word */
  	query->token = strtolower(query->token);
  	if (!STR_EQ(query->token,"delete")) goto error1;

	QUERY_SKIP
  	if (QUERY_NOT_OK) goto error1;

	query->token = strtolower(query->token);
	if (!STR_EQ(query->token,"from")) goto error1;

	QUERY_SKIP
  	if (QUERY_NOT_OK) goto error1;

  	/* get table name */
  	s = str_copy(query->token,0);
	if (!s) goto error2;
  
  	deletequery->tabname = s;
  
  	QUERY_SKIP
  	if (!query) goto error1;

  	if (STR_EQ(query->token,";")) /* we are at the end */
  	{
		deletequery->condtree = NULL;
		(*inputquery) = query;
    	parsquery->action = DELETE;
    	parsquery->querydata.param_delete = deletequery;
    	return 1;
  	}
  	else
  	{
    	query->token = strtolower(query->token);
    	if (!STR_EQ(query->token,"where")) goto error1; 
 
    	QUERY_SKIP

    	condtree = parse_conditions(&query);
	    if (!condtree) goto error2;

		deletequery->condtree = condtree;

		(*inputquery) = query;
    	parsquery->action = DELETE;
    	parsquery->querydata.param_delete = deletequery;
    	return 1;
  	}

error1:
  	set_error(ERR_SQLSYNTAX);
error2:
  	destroy_querydelete(deletequery);
  	(*inputquery) = query;
  	return 0; /* error */
}


/* returns NULL - error */
st_col_def* parse_coldef(parsed_str **inputquery)
{
	parsed_str *query;
	st_col_def *coldef;
	st_col_def *head,*prev;
	char *s;
 
	head = NULL;
	prev = NULL;
	query = *inputquery;

	query = skip_spaces(query);

	if (QUERY_NOT_OK) goto error3;

	query = skip_token(query);
 
	while (1)
	{
    	coldef = (st_col_def*)xmalloc(sizeof(st_col_def));
	    if (!coldef) goto error4;

    	query = skip_spaces(query);
		if (QUERY_NOT_OK) goto error1;

		/* get the name of the column */
	
		query->token = strtolower(query->token);
		s = str_copy(query->token,0);
		if (!s) goto error2;

		coldef->name = s;

		QUERY_SKIP
		if (QUERY_NOT_OK) goto error1;

    	/* get the data type of column */
    
		query->token = strtolower(query->token);

		if (STR_EQ(query->token,"int")) coldef->type = INT;
		else if (STR_EQ(query->token,"varchar")) coldef->type = VARCHAR;
		else goto error1;

		query = skip_token(query);

		LIST_ADDTAIL(head,prev,coldef); 

		query = skip_spaces(query);

    	if ((query) && (STR_EQ(query->token,")")))  /* end of coldef */
		{
	  		query = skip_token(query);
			(*inputquery) = query;
			return head; 
		}
		else if ((query) && (STR_EQ(query->token,","))) query = skip_token(query); /* next column */
		else goto error1;
  	}
 
error1: 
	set_error(ERR_SQLSYNTAX);
error2:
	destroy_coldef(coldef,0);
	destroy_coldef(head,1);
	goto error4;
error3:
	set_error(ERR_SQLSYNTAX);
error4:
  	(*inputquery) = query;
  	return NULL; /* syntax error */
}


/* returns NULL - error */
st_query_colset* parse_colset(parsed_str **inputquery)
{
	parsed_str *query = NULL;	
	st_query_colset *col,*head,*prev;
	char *s = NULL;

	head = NULL;
	prev = NULL;
	col = NULL;

	query = *inputquery;

	query = skip_spaces(query);

	if (QUERY_NOT_OK) goto error1;

	while ((query) && (!STR_EQ(query->token,"from")) && (!STR_EQ(query->token,";")))
	{
		query = skip_spaces(query);

    	col = (st_query_colset*)xmalloc(sizeof(st_query_colset));
	    if (!col) goto error3;

    	/* get column name */
		s = str_copy(query->token,0);
	    if (!s) goto error2;
    
		col->name = s;

		query = skip_token(query);

		LIST_ADDTAIL(head,prev,col);
		
		query = skip_spaces(query);	

		if ((query) && (STR_EQ(query->token,","))) query = skip_token(query);
  	}

  	(*inputquery) = query;
  	return head;

error1:
  	set_error(ERR_SQLSYNTAX);
  	goto error4;
error2:
  	destroy_querycolset(col,0);
error3:
  	destroy_querycolset(head,1);
error4:
  	(*inputquery) = query;
  	return NULL;
}


/* returns NULL - error */
st_query_tabset* parse_tabset(parsed_str **inputquery)
{
	parsed_str *query = NULL;
	char *s = NULL;
	st_query_tabset *tab,*head,*prev;

	head = NULL;
	prev = NULL;
	tab = NULL;

	query = *inputquery;

	query = skip_spaces(query);

	if (QUERY_NOT_OK) goto error1;

	while ((query) && (!STR_EQ(query->token,"where")) && (!STR_EQ(query->token,"order")) && (!STR_EQ(query->token,";")))
  {
	    tab = (st_query_tabset*)xmalloc(sizeof(st_query_tabset));
    	if (!tab) goto error3;

		/* get table name */
    	s = str_copy(query->token,0);
	    if (!s) goto error2;

	    tab->name = s;

		query = skip_token(query);

		LIST_ADDTAIL(head,prev,tab);

		query = skip_spaces(query);
    	if ((query) && (STR_EQ(query->token,","))) query = skip_token(query);
		query = skip_spaces(query);
  	}

  	(*inputquery) = query;
  	return head;
error1:
  	set_error(ERR_SQLSYNTAX);
  	goto error4;
error2:
  	destroy_querytabset(tab,0);
error3:
  	destroy_querytabset(head,1);
error4:
  	(*inputquery) = query;
  	return NULL;
}


/* returns NULL - error */
/* also checks equality of count of column names and supplied values */
st_col_data* parse_insertdata(parsed_str **inputquery)
{
	parsed_str *query;
	char *s;
	st_col_data *col,*head,*prev;
	
	head = NULL;
	prev = NULL;
	col = NULL;

	query = *inputquery;
 
	query = skip_spaces(query);  
  	if ((query) && (STR_EQ(query->token,"("))) query = skip_token(query);
  
  	query = skip_spaces(query);

  	if (!query)
  	{
		set_error(ERR_SQLSYNTAX);
		goto error4;
  	}

  	/* loop over all column names */
  	while ((query) && (!STR_EQ(query->token,"values")) && (!STR_EQ(query->token,";")))
  	{
    	col = (st_col_data*)xmalloc(sizeof(st_col_data));
	    if (!col) goto error2;

		if (STR_EQ(query->token,";"))
    	{
	  		set_error(ERR_SQLSYNTAX);
	  		goto error2;
    	}

		/* get column name */
    	query->token = strtolower(query->token);

    	s = str_copy(query->token,0);
	    if (!s) goto error3;

    	col->col_name = s;

		QUERY_SKIP
	
		LIST_ADDTAIL(head,prev,col);

		if ((query) && (STR_EQ(query->token,",")))
    	{
	  		QUERY_SKIP
    	}

		if ((query) && (STR_EQ(query->token,")")))
    	{
	  		QUERY_SKIP
    	}
  	}

  	/* skip over "values" */
  	QUERY_SKIP
  	if (!query) goto error1;

  	if ((query) && (STR_EQ(query->token,"("))) query = skip_token(query);
 
  	/* loop over all values - also checks equality of count of both colnames and values */
	col = head;
  	while (col)
  	{
		query = skip_spaces(query);
    	if (QUERY_NOT_OK) goto error1;

		/* get column value */
    	s = str_copy(query->token,0);
	    if (!s) goto error2;
    
		col->data_value = s;

		QUERY_SKIP

		/* we must skip over ',' or ')'*/
		QUERY_SKIP

		col = col->NEXT;
  	}

  	(*inputquery) = query;
  	return head;

error3:
  	destroy_coldata(col,0);
  	goto error2;
error1:
  	set_error(ERR_SQLSYNTAX);
error2:
  	destroy_coldata(head,1);
error4:
  	(*inputquery) = query;
  	return NULL; /* error - too few values */
}


/* returns NULL - error */
st_col_data* parse_updatedata(parsed_str **inputquery)
{
	parsed_str *query;
	char *s;
	st_col_data *col,*head,*prev;

	head = NULL;
	prev = NULL;

	query = *inputquery;

	query = skip_spaces(query);

  	/* loop over all column */
  	while ((query) && (!STR_EQ(query->token,"where")) && (!STR_EQ(query->token,";")))
  	{
		query = skip_spaces(query);
    	if (QUERY_NOT_OK) goto error1;

    	col = (st_col_data*)xmalloc(sizeof(st_col_data));
    	if (!col) goto error2;

		query->token = strtolower(query->token);

		/* get column name */
    	s = str_copy(query->token,0);
	    if (!s) goto error2;

	    col->col_name = s;

		QUERY_SKIP

    	if ((query) && (!STR_EQ(query->token,"="))) goto error1;

		/* we must skip '=' */
		QUERY_SKIP
    	if (QUERY_NOT_OK) goto error1; /* too few values */

		/* get column data value */
    	s = str_copy(query->token,0);
	    if (!s) goto error2;

		col->data_value = s;

		query = skip_token(query);

		LIST_ADDTAIL(head,prev,col);

		query = skip_spaces(query);

		if ((query) && (STR_EQ(query->token,",")))
    	{
	  		QUERY_SKIP
    	}
  	}

  	(*inputquery) = query;

  	return head;
error1:
  	set_error(ERR_SQLSYNTAX);
error2:
  	destroy_coldata(head,1);
  	(*inputquery) = query;
  	return NULL; /* error - too few values */
}


