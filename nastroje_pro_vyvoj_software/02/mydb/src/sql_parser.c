#include <string.h>
#include <stdlib.h>
#include "error.h"
#include "linklist.h"
#include "strutil.h"
#include "memory.h"
#include "sql_parser.h"
#include "tabdef.h"
#include "parser_commands.h"

/* S is string, P position to put char in, C1 and C2 are chars */
#define PUT_TWO_CHARS(S,P,C1,C2); \
	S[P] = C1; \
    P++; \
    S[P] = C2; \
    P++;


st_parstab_elem parsertab[TABSIZE];
st_parstab_elem parscreatetab[TABSIZE_CREATE];

void init_parser(void)
{
	parsertab[0].query = "select";
	parsertab[0].parsfunc = &parse_select;

	parsertab[1].query = "insert";
	parsertab[1].parsfunc = &parse_insert;

	parsertab[2].query = "update";
	parsertab[2].parsfunc = &parse_update;

	parsertab[3].query = "delete";
	parsertab[3].parsfunc = &parse_delete;

	parsertab[4].query = "use";
	parsertab[4].parsfunc = &parse_use;

	parsertab[5].query = "drop";
	parsertab[5].parsfunc = &parse_drop;

	parsertab[6].query = "create";
	parsertab[6].parsfunc = &parse_create;

	parscreatetab[0].query = "database";
	parscreatetab[0].parsfunc = &parse_crdb;

	parscreatetab[1].query = "table";
    parscreatetab[1].parsfunc = &parse_crtab;

	parscreatetab[2].query = "index";
    parscreatetab[2].parsfunc = &parse_crindex;
}


st_query_parsed* parse_sqlquery(const char *inputquery)
{
	char *tmp_query = NULL;
	parsed_str *query = NULL;
	st_query_parsed* parsedquery = NULL;
	int ret;
	int i;

	init_parser();
	
	/* check syntax of a query */
	ret = check_sqlsyntax(inputquery);
	if (!ret) goto error2;
    
	tmp_query = prepare_sqlquery(inputquery);
	if (!tmp_query) goto error2;

	parsedquery = (st_query_parsed*)xmalloc(sizeof(st_query_parsed));
	if (!parsedquery) goto error2; /* error */

	query = str_tokenize(tmp_query," ,;()");
	if (!query) goto error3;

	query = skip_spaces(query);
	if (!query) goto error3;

	query->token = strtolower(query->token);

	/* choose the more specific parsing function */
	i = 0;
	while (i < TABSIZE)
  	{
		if (STR_EQ(query->token,parsertab[i].query))
		{
	  		ret = (parsertab[i].parsfunc) (&query,parsedquery);
			if (!ret) goto error1;

			destroy_tokens(query,1);
			xfree(tmp_query);
			break;
		}
		i++;
  	}

	if (i == TABSIZE) goto error3;

	if (parsedquery->action == SELECT)
	{	
		ret = complete_colnames(parsedquery);
		if (ret == (-1)) return NULL; /* error */
		if (!ret) goto error2;
	}

	ret = check_parsedquery(parsedquery);
	if (!ret) goto error2;
	
	return parsedquery; /* error */

error2:
	set_error(ERR_SQLSYNTAX);
	return NULL;
error3:
	set_error(ERR_SQLSYNTAX);
error1:
	destroy_queryparsed(parsedquery);
	destroy_tokens(query,1);
	if (tmp_query) xfree(tmp_query);
	return NULL;
}


char* prepare_sqlquery(const char* inputquery)
{
	char *tmp_query = NULL, *new_query = NULL;
	size_t size,size2;
	int pos,pos2;
	int is_oper;

	size = strlen(inputquery);
	size2 = 3*size+1;

	pos = 0;
	pos2 = 0;
	
	tmp_query = (char*)xmalloc(sizeof(char)*size2);
	if (!tmp_query) return NULL;

	is_oper = 0;

	/* loop through whole inputquery */
	/* put extra spaces around operators */
	while (pos<size)
	{
		if (inputquery[pos] == '<')
		{
			if (is_oper) goto error1;
			else
			{
				PUT_TWO_CHARS(tmp_query,pos2,' ','<');
				is_oper = 1;
			}
		}
		else if (inputquery[pos] == '>')
		{
			if (is_oper)
			{
				PUT_TWO_CHARS(tmp_query,pos2,'>',' ');
				is_oper = 0;
			}
			else
			{
				PUT_TWO_CHARS(tmp_query,pos2, ' ','>');
				is_oper = 1;
			}
		}	
		else if (inputquery[pos] == '!')
		{
			if (is_oper) goto error1;
			else
			{
                PUT_TWO_CHARS(tmp_query,pos2,' ','!');
                is_oper = 1;
			}
		}
		else if (inputquery[pos] == '=')
		{
            if (is_oper)
            {
                PUT_TWO_CHARS(tmp_query,pos2,'=',' ');
                is_oper = 0;
            }
            else
            {
                PUT_TWO_CHARS(tmp_query,pos2,' ','=');
                is_oper = 1;
            }
		}
		else 
		{
			if (is_oper)
			{
				tmp_query[pos2] = ' ';
				is_oper = 0;
				pos2++;
			}
			tmp_query[pos2] = inputquery[pos];
			is_oper = 0;
			pos2++;
		}
		pos++;
	}	 				

	new_query = (char*)xmalloc(sizeof(char)*(pos2+1));
	if (!new_query) return NULL;

	new_query = strncpy(new_query,tmp_query,pos2);
	new_query[pos2] = 0;

	xfree(tmp_query);

	return new_query;

error1:
	xfree(tmp_query);
	set_error(ERR_SQLSYNTAX);
	return NULL;
}


int complete_colnames(st_query_parsed* const query)
{
	int ret;
	st_query_select *selectquery;

	selectquery = query->querydata.param_select;

	ret = complete_colsetnames(selectquery->colset,selectquery->tabset);
	if (ret == (-1)) return (-1);
	if (!ret) 
	{
		set_error(ERR_SQLSYNTAX);
		return 0;
	}

	if (selectquery->condtree)
	{
		ret = complete_condcolnames(selectquery->condtree,selectquery->tabset);
		if (ret == (-1)) return (-1);
    	if (!ret)
    	{
        	set_error(ERR_SQLSYNTAX);
        	return 0; 
    	}
	}

	if (selectquery->ordercol)
	{
		ret = complete_ordercolname(selectquery);
		if (ret == (-1)) return (-1);
        if (!ret)
        {
            set_error(ERR_SQLSYNTAX);
            return 0;
        }
	}

	return 1;	
}


int complete_colsetnames(st_query_colset *colset,st_query_tabset *tabset)
{
	st_query_colset *col = NULL;
	st_query_tabset *tab2 = NULL;
	char *s = NULL, *str = NULL;
	int tab_count;
	int is_tab;

	col = colset;

	while (col)
	{	
		if (STR_EQ(col->name,"*"))
		{
			tab_count = count_tabs(tabset);
			if (tab_count > 1) 
			{
				s = strchr(col->name,'.');
				if (!s) return 0; /* there must be tabname in this case */
			}
		
			col = col->NEXT;	
			continue;
		}

		s = strchr(col->name,'.');
		if (!s) /* there isn't tablename */
		{
			tab_count = count_coltabs(tabset,col->name,&tab2);
			if (tab_count == (-1)) return (-1);	

			if (tab_count == 1) /* only one table - add tabname to colname */
			{
				str = str_concat(tab2->name,col->name,'.');
				if (!str) return (-1);

				xfree(col->name);
				col->name = str;			
			}
			else return 0; /* no table or two and more with this column */
		}
		else /* there is tablename */
		{
			is_tab = coltab_exist(col->name,tabset);
			if (!is_tab) return 0;
		}
			
		col = col->NEXT;
	}

	return 1;
}


int complete_condcolnames(st_cond_node *condnode,st_query_tabset *tabset)
{
	int ret1,ret2;
    int i;
	st_query_tabset *tab2 = NULL;
	st_cond *cond = NULL;
    char *s = NULL, *str = NULL;
    int tab_count;
    int is_tab;

	if (!condnode) 
	{
		set_error(ERR_STRANGE);
		return (-1);
	}

	if (condnode->cond)	/* complete column arguments */
	{
		cond = condnode->cond;

		for (i=0;i<2;i++)
		{
			if (cond->arg_type[i] != COLUMN) continue;
       		
			/* arg_type == COLUMN */

			s = strchr(cond->arg_value[i],'.');
   			if (!s) /* there isn't tablename */
   			{
				tab_count = count_coltabs(tabset,cond->arg_value[i],&tab2);
		        if (tab_count == (-1)) return (-1);

				/* one table - add tabname to colname */
				if (tab_count == 1) 
        		{
           			str = str_concat(tab2->name,cond->arg_value[i],'.');
					if (!str) return (-1);

	           		xfree(cond->arg_value[i]);
               		cond->arg_value[i] = str;
       	   		}
        		else return 0; /* no table or more than one */
       		}
       		else /* there is tablename */
       		{
				is_tab = coltab_exist(cond->arg_value[i],tabset);
           		if (!is_tab) return 0;
			}
       	}

		return 1;
	}
	else
	{
		if (condnode->child_cond1)
		{
			ret1 = complete_condcolnames(condnode->child_cond1,tabset);
			if (ret1 == (-1)) return (-1);

			if (!ret1) return 0;
		}

		if (condnode->child_cond2) 
        {
    	    ret2 = complete_condcolnames(condnode->child_cond2,tabset);
        	if (ret2 == (-1)) return (-1);

            if (!ret2) return 0;
	    }

		return 1; /* both child are OK */
	}

	return (-1);
}


int complete_ordercolname(st_query_select *selectquery)
{
	st_query_tabset *tabset,*tab = NULL;
	char *s,*str;
	int tab_count;
	int is_tab;

	tabset = selectquery->tabset;

	s = strchr(selectquery->ordercol,'.');
    if (!s) /* there isn't tablename */
    {
		tab_count = count_coltabs(tabset,selectquery->ordercol,&tab);
        if (tab_count == (-1)) return (-1);

        if (tab_count == 1) /* only one table - add tabname to colname */
        {
			str = str_concat(tab->name,selectquery->ordercol,'.');
            if (!str) return (-1);

            xfree(selectquery->ordercol);
            selectquery->ordercol = str;
        }
        else return 0; /* no table or two and more with this column */
	}
    else /* there is tablename */
    {
    	is_tab = coltab_exist(selectquery->ordercol,tabset);
        if (!is_tab) return 0;
    }

	return 1;
}


int check_sqlsyntax(const char *query)
{
	size_t size;
	int is_quote;
	/* opening bracket +1, closing -1, 0 at the end and never negative */
	int brackets;
	int i;

	if (!query) return 0;

  	size = strlen(query);
	is_quote = 0;
	brackets = 0;

	for (i=0;i<(size-1);i++)
	{
        /* switch between 1 and 0 */
		if ((query[i] == '\"') || (query[i] == '\'')) is_quote = 1 - is_quote;

		if (query[i] == '(') brackets++;
		if (query[i] == ')')
		{
			brackets--;
			if (brackets < 0) return 0;	/* too many closing brackets */
		}
		if ((!is_quote) && (query[i] == ':')) return 0;
		if ((!is_quote) && (query[i] == '#')) return 0;
	}

	if (is_quote) return 0;
	if (brackets) return 0;

	/* last character other than '\n' must be ';' */
	if (query[size-1] == '\n') --size;
	if (query[size-1] != ';') return 0;

	return 1;
}


/* 1 - OK */
/* 0 - error */
int check_parsedquery(st_query_parsed *query)
{
	st_query_select *selectquery;
	st_query_delete *deletequery;
	st_query_update *updatequery;
	st_query_insert *insertquery;
	st_cond_node *condtree;
	st_col_data *coldata;
	st_query_tabset *tabset,*tabset2;
	st_table_def *tabdef;
	char *tabname;
	char *s;
	int ret;

	if ((query->action == SELECT) || (query->action == DELETE) || (query->action == UPDATE))
  	{
		if (query->action == SELECT)
		{
	  		selectquery = query->querydata.param_select;
	  		tabset = selectquery->tabset;
			tabname = NULL;	
	  		condtree = selectquery->condtree;
		}
		else if (query->action == DELETE)
		{
	  		deletequery = query->querydata.param_delete;
	  		tabname = deletequery->tabname;
	  		condtree = deletequery->condtree;
		}
		else /*if (query->action == UPDATE) */
		{
	  		updatequery = query->querydata.param_update;
	  		tabname = updatequery->tabname;
	  		condtree = updatequery->condtree;
		}
		if (tabname)
		{
			/* check for existence of table */
			tabdef = get_tabledef(tabname);
    		if (!tabdef) return 0;	

			tabset = (st_query_tabset*)xmalloc(sizeof(st_query_tabset));
			if (!tabset)
			{
				destroy_tabdef(tabdef);
				return 0;
			}
			destroy_tabdef(tabdef);

			s = str_copy(tabname,0);
			tabset->name = s;
			tabset->NEXT = NULL;
		}
		else
		{
			/* check for existence of all tables */
			tabset2 = tabset;
			while (tabset2)
			{
				tabdef = get_tabledef(tabset2->name);
				if (!tabdef) return 0;

				destroy_tabdef(tabdef);

				tabset2 = tabset2->next;
			}
		}

		/* check for correct types of params in conditions */
		if (condtree) 
		{
			ret = check_parsedconds(condtree,tabset);
			if (ret == (-1)) return 0;

			if (!ret) return 0;			
		}

		if ((query->action!=SELECT) && (tabset)) destroy_querytabset(tabset,1);
		if (query->action != UPDATE) return 1;
  	}
	if ((query->action == INSERT) || (query->action == UPDATE))
  	{
		/* check for existence of table and columns and correct types of data */
		if (query->action == INSERT)
    	{
      		insertquery = query->querydata.param_insert;
	  		tabname = insertquery->tabname;
      		coldata = insertquery->coldata;
    	}
    	else 
    	{
      		updatequery = query->querydata.param_update;
	  		tabname = updatequery->tabname;
      		coldata = updatequery->coldata;
    	}

		/* check for existence of table */
    	tabdef = get_tabledef(tabname);
    	if (!tabdef) return 0;

		while (coldata)
		{
      		ret=correct_colvalue(tabdef,coldata->col_name,coldata->data_value);
      		if (!ret)
      		{
        		destroy_tabdef(tabdef);
        		return 0;
      		}

	  		coldata = coldata->NEXT;
		}
		destroy_tabdef(tabdef);
		return 1;
  	}
  	return 1; /* other actions are OK */
}


int check_parsedconds(st_cond_node *condnode,st_query_tabset *tabset)
{
	st_cond *cond = NULL;
	int ret1,ret2,ret;
	st_query_tabset *tab = NULL;
	st_table_def *tabdef = NULL;
	int is_correct;

	if (condnode->cond)
	{
      	cond = condnode->cond;

		if ((cond->arg_type[0] == COLUMN) && (cond->arg_type[1] == VALUE))
        {
			is_correct = 0;

			tab = tabset;
			while (tab)
			{	
				tabdef = get_tabledef(tab->name);
				if (!tabdef) return 0;

          		ret = correct_colvalue(tabdef,cond->arg_value[0],cond->arg_value[1]);
				if (ret == (-1))
				{
					destroy_tabdef(tabdef);
					return (-1);
				}

          		if (ret) is_correct = 1;

				destroy_tabdef(tabdef);

				tab = tab->NEXT;
			}
			if (!is_correct) return 0;
        }
        else if ((cond->arg_type[1] == COLUMN) && (cond->arg_type[0] == VALUE))
        {
	        tab = tabset;
            while (tab)
            {
                tabdef = get_tabledef(tab->name);
                if (!tabdef) return 0;

                ret = correct_colvalue(tabdef,cond->arg_value[1],cond->arg_value[0]);
				if (ret == (-1))
				{
					destroy_tabdef(tabdef);
					return (-1);
				} 

                if (ret) is_correct = 1;

                destroy_tabdef(tabdef);

                tab = tab->NEXT;
            }
            if (!is_correct) return 0;
        }
		else if ((cond->arg_type[0]==COLUMN) && (cond->arg_type[1]==COLUMN))
		{
			/* comparing values from the same column is not allowed */ 
			if (STR_EQ(cond->arg_value[0],cond->arg_value[1])) return 0;
		}
		return 1;
	}
	else
	{	
		ret1 = check_parsedconds(condnode->child_cond1,tabset);
		if (!ret1) return 0;

        ret2 = check_parsedconds(condnode->child_cond2,tabset);
        if (!ret2) return 0;

		return 1;
	}
}


parsed_str* str_tokenize(const char *str,const char *delims)
{
	parsed_str *head,*prev,*cur;
	char *tmp_str;
	size_t size,cur_size,delim_size;
	int lastpos;
	int i,j;
	char *s = NULL;
	int is_delim;
	int is_quotes; /* determines whether we are inside quotes */

	head = NULL;
	prev = NULL;
	cur = NULL;

	size = strlen(str);
	delim_size = strlen(delims);
	cur_size = 0;
	lastpos = 0;

	tmp_str = str_copy(str,0);
	if (!tmp_str) return NULL;

	is_quotes = 0;

	/* loop through all chars in input string */
	for (i=0;i<size;i++)
	{
    	is_delim = 0;
    	/* loop over all chars in delimiters */
		for (j=0;j<delim_size;j++)
    	{
      		if ((tmp_str[i]==delims[j]) && (!is_quotes)) is_delim = 1; /* current char is delimiter - delimiter cannot be between quotes */
    	}
   		/* changes between 1 and 0 */
    	if ( (tmp_str[i]=='\"') || (tmp_str[i]=='\'')) is_quotes = 1-is_quotes;
	
    	if (is_delim)
		{
     		if (cur_size)
      		{
        		/* store new token(delimited string) */
		        cur = (parsed_str*)xmalloc(sizeof(parsed_str));
        		if (!cur) goto error2;

		        s = (char*)xmalloc(sizeof(char)*(cur_size+1));
		        if (!s) goto error1;

        		s = strncpy(s,tmp_str+lastpos,cur_size);
		        s[cur_size]=0;

        		cur->token = s;
				LIST_ADDTAIL(head,prev,cur);
      		}

      		/* store delimiter as separate string */
		    cur = (parsed_str*)xmalloc(sizeof(parsed_str));
		    if (!cur) goto error2;

		    s = (char*)xmalloc(sizeof(char)*2);
			if (!s) goto error1;

		    s[0] = tmp_str[i];
		    s[1] = 0;

			cur->token = s;
			LIST_ADDTAIL(head,prev,cur);

	      	lastpos = i+1;
		    cur_size = 0;
    	}
	    else cur_size++;
  	}

  	xfree(tmp_str);

  	return head;

error1:
	destroy_tokens(cur,0);
error2:
	xfree(tmp_str);
	return NULL;
}


void destroy_tokens(parsed_str *pars_str,int all)
{
	parsed_str *cur,*head;

	if (all)
	{
    	head = pars_str;
	    while (head)
    	{
      		cur = head;
		    head = head->NEXT;

      		xfree(cur->token);
		    xfree(cur);
    	}
  	}
  	else
  	{
		if (pars_str)
		{
      		xfree(pars_str->token);
		    xfree(pars_str);
		}
  	}
}


parsed_str* skip_spaces(parsed_str *query)
{
	parsed_str *query2;

  	while ((query) && (STR_EQ(query->token," ")))
  	{
    	query2 = query;
	    query = query->NEXT;
    	destroy_tokens(query2,0);
	}

	return query;
}


parsed_str* skip_token(parsed_str *query)
{
	parsed_str *query2;

  	if (query)
  	{
    	query2 = query;
	    query = query->NEXT;
    	destroy_tokens(query2,0);

	    return query;
  	}
  	else return NULL;
}

