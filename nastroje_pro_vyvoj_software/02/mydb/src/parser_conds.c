#include <string.h>
#include <stdlib.h>
#include "error.h"
#include "strutil.h"
#include "memory.h"
#include "parser_conds.h"
#include "tabdef.h"


/* returns NULL - error */
st_cond_node* parse_conditions(parsed_str **inputquery)
{
	parsed_str *query;
	st_cond_node *condnode = NULL;
	st_cond_node *child_cond1 = NULL ,*child_cond2 = NULL;
	int cond_count = 0;
	int logop = 0;

	query = *inputquery;

  	query = skip_spaces(query);

	if (!query)
  	{
		set_error(ERR_SQLSYNTAX);
	    return NULL;
  	}

	/* loop over all token which forms conditions */
  	while ((query) && (!STR_EQ(query->token,"order")) && (!STR_EQ(query->token,";")))
  	{
		if (cond_count == 1)
		{
			/* there must be logical operator before second condition */
            query->token = strtolower(query->token);

            if (STR_EQ(query->token,"and")) logop = AND;
            else if (STR_EQ(query->token,"or")) logop = OR;
            else goto error3; /* no operator - syntax error */
    
            QUERY_SKIP
    
            child_cond2 = parse_condnode(&query);
            if (!child_cond2) goto error3;

			/* create another node, which will be parent of current nodes */
			/* and it will at the same level as next condition */
            condnode = create_condnode(child_cond1,child_cond2,logop,NULL);
			if (!condnode) goto error2;

            logop = 0;
            child_cond2 = NULL;
            child_cond1 = condnode;
    
            cond_count = 1;
    
            query = skip_spaces(query);
        }
        else /* cond_count == 0 */
        {
			query = skip_spaces(query);

            child_cond1 = parse_condnode(&query);
            if (!child_cond1) goto error3;

            query = skip_spaces(query);

            cond_count++;
        }
	}

	(*inputquery) = query;

	if ((child_cond1) && (!condnode)) return child_cond1;
    else return condnode;
error2:
	destroy_condnode(child_cond1);
	destroy_condnode(child_cond2);
error3:
  	(*inputquery) = query;
  	return NULL;
}


st_cond_node* parse_condnode(parsed_str **inputquery)
{
	parsed_str *query;
  	st_cond_node *condnode = NULL;
	st_cond_node *child_cond1 = NULL,*child_cond2 = NULL;
	st_cond *cond;
	int cond_count;
	en_logoper logop = 0;
	/* int neg_cond; */

	query = *inputquery;

	query = skip_spaces(query);

  	if (!query) goto error3;
  	
	cond_count = 0;

	if (STR_EQ(query->token,"(")) /* some conditions in brackets */
	{
		QUERY_SKIP

		while (!STR_EQ(query->token,")")) /* till the ending bracket */
		{
			if (cond_count == 1)
			{
				/* there must be logical operator before second condition */
        		query->token = strtolower(query->token);

        		if (STR_EQ(query->token,"and")) logop = AND;
        		else if (STR_EQ(query->token,"or")) logop = OR;
				else /* no operator - syntax error */
				{
					set_error(ERR_SQLSYNTAX);
					goto error3;
				}

				QUERY_SKIP

				child_cond2 = parse_condnode(&query);
	        	if (!child_cond2) goto error3;
			
				condnode = create_condnode(child_cond1,child_cond2,logop,NULL);
				if (!condnode) goto error2;

            	logop = 0;
            	child_cond2 = NULL;
	            child_cond1 = condnode;
    	        cond_count = 1;

				query = skip_spaces(query);
			}
			else /* cond_count == 0 */
			{
				query = skip_spaces(query);

				child_cond1 = parse_condnode(&query);
				if (!child_cond1) goto error3;

				query = skip_spaces(query);

				cond_count++;
			}
		}
	
		QUERY_SKIP
		
		(*inputquery) = query;

		if ((child_cond1) && (!condnode)) return child_cond1;
		else return condnode;
	}
	else /* condition without brackets on this level */
	{
		cond = parse_basic_cond(&query);
    	if (!cond) goto error3;

		query = skip_spaces(query);

		condnode = create_condnode(NULL,NULL,0,cond);
		if (!condnode)
		{
			destroy_cond(cond);
			goto error3;
		}
	
		/* condnode->neg_cond = neg_cond; */

		query = skip_spaces(query);

		(*inputquery) = query;

		return condnode;
	}

	return NULL;
error2:
	destroy_condnode(child_cond1);
	destroy_condnode(child_cond2);
error3:
	(*inputquery) = query;
	return NULL;
}


st_cond* parse_basic_cond(parsed_str **inputquery)
{
	parsed_str *query;
    char *arg1,*arg2;
	int type1,type2;
	int oper;
    st_cond *cond;
    int reverse_cond;

    query = *inputquery;

    query = skip_spaces(query);

    if (QUERY_NOT_OK) return NULL;

    /* test for NOT here */
    if (STR_EQ(query->token,"not"))
    {
	    reverse_cond = 1;
        query = skip_token(query);
    }
    else reverse_cond = 0;

    query = skip_spaces(query);

    if (QUERY_NOT_OK) return NULL;

    query->token = strtolower(query->token);

    /* get first member of expression */
    arg1 = str_copy(query->token,0);
    if (!arg1) return NULL;

    /* process first argument */
    if ((is_string(arg1)) || (is_num(arg1))) type1 = VALUE;
    else type1 = COLUMN; /* it is column name - string without quotes */

    QUERY_SKIP

    if (QUERY_NOT_OK) goto error1;

    /* get operator */

    query->token = strtolower(query->token);

    /* process operator */
    /* operators are reversed, if there was NOT in front of the cond */
    if (STR_EQ(query->token,"<")) oper = reverse_cond ? MOREEQUAL:LESS;
    else if (STR_EQ(query->token,">")) oper = reverse_cond ? LESSEQUAL:MORE;
    else if (STR_EQ(query->token,"=")) oper = reverse_cond ? NOTEQUAL:EQUAL;
    else if (STR_EQ(query->token,">=")) oper = reverse_cond ? LESS:MOREEQUAL;
    else if (STR_EQ(query->token,"<=")) oper = reverse_cond ? MORE:LESSEQUAL;
    else if (STR_EQ(query->token,"!=")) oper = reverse_cond ? EQUAL:NOTEQUAL;
	else if (STR_EQ(query->token,"<>")) oper = reverse_cond ? EQUAL:NOTEQUAL;
    else goto error1;

    QUERY_SKIP

    if (QUERY_NOT_OK) goto error1;

    /* get second member of query */

    query->token = strtolower(query->token);

    arg2 = str_copy(query->token,0);
    if (!arg2) goto error1;

    /* process second argument */
    if ((is_string(arg2)) || (is_num(arg2))) type2 = VALUE;
    else type2 = COLUMN; /* it is column name - string without quotes */

    cond = create_cond(arg1,arg2,type1,type2,oper);
    if (!cond) goto error2;
	
	QUERY_SKIP

	(*inputquery) = query;

    return cond;

error2:
    xfree(arg2);
error1:
    xfree(arg1);
    (*inputquery) = query;
    return NULL;
}


