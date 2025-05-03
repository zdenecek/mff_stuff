#include "error.h"
#include "memory.h"
#include "strutil.h"
#include "conds.h"


int logexp_eval(st_logexp *exp)
{
	int val1,val2;
	int ret;

 	if ( (!exp->exp1) && (!exp->exp2)) 
	{
		val1 = exp->value;
		xfree(exp);
		return val1;
	}
	else
	{
		if ((!exp->exp1) || (!exp->exp2)) 
		{
			set_error(ERR_STRANGE);
			return (-1);
		}
		
		val1 = logexp_eval(exp->exp1);
		if (val1 == (-1)) return (-1);

		val2 = logexp_eval(exp->exp2);
		if (val2 == (-1)) return (-1);

		if (exp->logop == 1)
		{
			xfree(exp);
			if (val1 && val2) ret = 1;
			else ret = 0;
			return ret;
		}
		if (exp->logop == 2)
		{
			xfree(exp);
			if ((!val1) && (!val2)) ret = 0;
			else ret = 1;
			return ret;
		}

		set_error(ERR_STRANGE);
		return (-1);
	}
}


int col_satisfy(const st_col_data *col, const st_col_data *col2, const st_cond *cond)
{
	char *value1,*value2;
	int intval1, intval2;
	char *icaseval1,*icaseval2;
	char *strval1,*strval2;
	char *str1,*str2;

	if (col && col2) /* we have joining condition */
	{
		value1 = col->data_value;
		value2 = col2->data_value;
	}
	else if (col)
	{
		/* first argument of condition */
		if (cond->arg_type[0] == COLUMN)
		{
			value1 = col->data_value;
			value2 = cond->arg_value[1]; /* constant */ 
		}
		else
 		{
			value1 = cond->arg_value[0]; /* constant */
			value2 = col->data_value;
		}
	}
	else /* two values */
	{
		value1 = cond->arg_value[0];
		value2 = cond->arg_value[1];
	}

	icaseval1 = str_copy(value1,0);
	icaseval2 = str_copy(value2,0);
	icaseval1 = strtolower(icaseval1);
	icaseval2 = strtolower(icaseval2);

	/* test for "null" value */
	if ( (STR_EQ(icaseval1,"null")) || (STR_EQ(icaseval2,"null")))	
	{
		xfree(icaseval1);
		xfree(icaseval2);
		return 0; /* "null" value makes condition not successful */
	}

	xfree(icaseval1);
	xfree(icaseval2);

	/* we have numbers */
	if ( is_num(value1) && is_num(value2) )
	{
		intval1 = strtol(value1,0,10);
		intval2 = strtol(value2,0,10);

	    switch (cond->oper)
    	{
			case EQUAL:
		        if (intval1 == intval2) return 1;
        		else return 0;
      			break;
		    case NOTEQUAL:
        		if (intval1 != intval2) return 1;
		        else return 0;
        		break;
	      	case LESS:
		        if (intval1 < intval2) return 1;
        		else return 0;
		        break;
      		case MORE:
		        if (intval1 > intval2) return 1;
        		else return 0;
		        break;
      		case LESSEQUAL:
        		if (intval1 <= intval2) return 1;
		        else return 0;
        		break;
		    case MOREEQUAL:
        		if (intval1 >= intval2) return 1;
		        else return 0;
        		break;
		    default:
        		goto error1;
    	}
  	}
	else if ( is_string(value1) && is_string(value2) ) /* we have strings */
  	{
		/* putting away quotes */
		str1 = str_copy(value1,0);
		str2 = str_copy(value2,0);
		strval1 = (str1+1);
		strval1[strlen(strval1)-1] = 0;
		strval2 = (str2+1);
		strval2[strlen(strval2)-1] = 0;

    	switch (cond->oper)
	    {
    		case EQUAL:
		        if (STR_EQ(strval1,strval2)) return 1;
        		else return 0;
		        break;
      		case NOTEQUAL:
        		if (!STR_EQ(strval1,strval2) != 0) return 1;
		        else return 0;
        		break;
		    case LESS:
        		if (strcmp(strval1,strval2) < 0) return 1;
		        else return 0;
        		break;
	      	case MORE:
		        if (strcmp(strval1,strval2) > 0) return 1;
        		else return 0;
		        break;
	      	case LESSEQUAL:
		        if (strcmp(strval1,strval2) <= 0 ) return 1;
        		else return 0;
		        break;
      		case MOREEQUAL:
		        if (strcmp(strval1,strval2) >= 0) return 1;
        		else return 0;
		        break;
      		default:
		        goto error1;
    	}	
  	}
	else if ( is_string(value1) && is_num(value2) ) /* string and number */
	{
        switch (cond->oper)
        {
            case EQUAL:
                return 0;
                break;
            case NOTEQUAL:
                return 1;
                break;
            case LESS:
                return 0;
                break;
            case MORE:
                return 1;
                break;
            case LESSEQUAL:
                return 0;
                break;
            case MOREEQUAL:
                return 1;
                break;
            default:
                goto error1;
        }
	}
	else if ( is_num(value1) && is_string(value2) ) /* number and string */
    {
        switch (cond->oper)
        {
            case EQUAL:
                return 1;
                break;
            case NOTEQUAL:
                return 0;
                break;
            case LESS:
                return 1;
                break;
            case MORE:
                return 0;
                break;
            case LESSEQUAL:
                return 1;
                break;
            case MOREEQUAL:
                return 0;
                break;
            default:
                goto error1;
        }
    }
	return (-1);
error1:
	set_error(ERR_STRANGE);
	return (-1);
}


int get_condvalue(const st_cond *cond, int *value)
{
	/* we don't want that */
	if (cond->arg_type[0] == cond->arg_type[1]) return 0;

	if (cond->arg_type[0] == VALUE)
	{
		if (is_num(cond->arg_value[0]))
		{
			*value = strtol(cond->arg_value[0],NULL,10);
			return 1;
		}
		else return 0;
	}

    if (cond->arg_type[1] == VALUE)
    {
        if (is_num(cond->arg_value[1]))
        {
            *value = strtol(cond->arg_value[1],NULL,10);
            return 1;
        }
        else return 0;
    }

	return 0;
}


char* get_condcolumn(const st_cond *cond)
{
	 /* we don't want that */
    if (cond->arg_type[0] == cond->arg_type[1]) return 0;
    
    if (cond->arg_type[0] == COLUMN)
    {
        return cond->arg_value[0];
    }

    if (cond->arg_type[1] == COLUMN)
    {
	    return cond->arg_value[1];
    }

    return NULL;
}


st_cond* create_cond(char *argval1, char *argval2, en_condarg argtype1, en_condarg argtype2, en_operator oper)
{
	st_cond *cond;

	cond = (st_cond*)xmalloc(sizeof(st_cond));
    if (!cond) return NULL;

	cond->arg_value[0] = argval1;
	cond->arg_value[1] = argval2;
	cond->arg_type[0] = argtype1;
	cond->arg_type[1] = argtype2;
	cond->oper = oper;	

	return cond;
}

st_cond_node* create_condnode(st_cond_node *child1, st_cond_node *child2, const en_logoper logop,st_cond *cond)
{
	st_cond_node *condnode;

	condnode = (st_cond_node*)xmalloc(sizeof(st_cond_node));
	if (!condnode) return NULL;

	condnode->child_cond1 = child1;
	condnode->child_cond2 = child2;
	condnode->logop = logop;
	condnode->cond = cond;

	return condnode;
}


st_logexp* create_logexp(const st_row_data *row, const st_cond_node *conds)
{
	st_logexp *logexp;

	const st_col_data *col,*col2;
	int ret;

	logexp = (st_logexp*)xmalloc(sizeof(st_logexp));
	if (!logexp) return NULL;

	if (is_condleaf(conds)) /* leaf of the tree */
	{
		/* joining condition */
		if ((conds->cond->arg_type[0] == COLUMN) && (conds->cond->arg_type[1] == COLUMN))
		{
			col = row->data;
            /* search for the correct column to take value from */
            while (col)
            {
                if (STR_EQ(conds->cond->arg_value[0],col->col_name)) break;
                col = col->NEXT;
            }
            if (!col) goto error1;

			col2 = row->data;
            /* search for the correct column to take value from */
            while (col2)
            {
                if (STR_EQ(conds->cond->arg_value[1],col2->col_name)) break;
                col2 = col2->NEXT;
            }
            if (!col2) goto error1;

            ret = col_satisfy(col,col2,conds->cond);
            if (ret == (-1)) goto error1;

			logexp->value = ret;
		}
		/* tryefirst argument of cond */
		else if (conds->cond->arg_type[0] == COLUMN)
		{ 
			col = row->data;
		    /* search for the right column to take value from */
		    while (col)
    		{
		    	if (STR_EQ(conds->cond->arg_value[0],col->col_name)) break;
		        col = col->NEXT;
    		}
		    if (!col) goto error1;

			ret = col_satisfy(col,NULL,conds->cond);
			if (ret == (-1)) goto error1;

			logexp->value = ret;
		}
		/* try second argument of cond */
		else if (conds->cond->arg_type[1] == COLUMN)
		{
			col = row->data;
    		/* search for the right column to take value from */
		    while (col)
		    {
      			if (STR_EQ(col->col_name,conds->cond->arg_value[1])) break;
      			col = col->NEXT;
		    }
    		if (!col) goto error1;

            ret = col_satisfy(col,NULL,conds->cond);
            if (ret == (-1)) goto error1;

			logexp->value = ret;
		}
		else /* both constants */
		{
			ret = col_satisfy(NULL,NULL,conds->cond);
            if (ret == (-1)) goto error1;
            logexp->value = ret;
		}

		return logexp;
	}
	else
	{
		logexp->exp1 = create_logexp(row,conds->child_cond1);
		if (!logexp->exp1) goto error1;

		logexp->exp2 = create_logexp(row,conds->child_cond2);
        if (!logexp->exp2) goto error1;

		logexp->logop = conds->logop;

		return logexp;
	}
	
error1:
	set_error(ERR_STRANGE);
	xfree(logexp);
	return NULL;
}


int row_satisfy(const st_row_data *row, const st_cond_node *conds)
{
	st_logexp *logexp;
	int ret;

	logexp = create_logexp(row,conds);
	if (!logexp) return (-1);


	ret = logexp_eval(logexp);
	if (ret == (-1)) return (-1);

	return ret;	
}


int conds_fromtable(const st_cond_node *conds, const char *tabname)
{
	char *str;
	int ret,ret2;
	st_cond *cond;

	if (conds->cond)
	{
		cond = conds->cond;

		ret = 0;

		if ((cond->arg_type[0] == COLUMN) && (cond->arg_type[1] != COLUMN))
  		{
    		str = strstr(cond->arg_value[0],tabname);
		    if (!str) ret = 0;
    		else if (str == cond->arg_value[0]) ret = 1;
			else ret = 0; 
  		}

		if ((cond->arg_type[1] == COLUMN) && (cond->arg_type[0] != COLUMN))
		{
    		str = strstr(cond->arg_value[1],tabname);
		    if (!str) return 0;
    	    else if (str == cond->arg_value[1]) ret = 1;
			else ret = 0;
  		}	
		return ret;
	}
	else if ((conds->child_cond1) && (conds->child_cond2))
	{
		ret = conds_fromtable(conds->child_cond1,tabname);
		ret2 = conds_fromtable(conds->child_cond2,tabname);

		if ((ret) && (ret2)) return 1;
		else return 0;
	}
	else return (-1);
}

int is_condleaf(const st_cond_node *condnode)
{
	if ((condnode->child_cond1) && (condnode->child_cond2)) return 0;
	else return 1;
}

st_cond* copy_cond(const st_cond *cond)
{
	char *s;
	st_cond *new_cond;

	new_cond = (st_cond*)xmalloc(sizeof(st_cond));
	if (!new_cond) return NULL;

	new_cond->oper = cond->oper;
	new_cond->arg_type[0] = cond->arg_type[0];
	new_cond->arg_type[1] = cond->arg_type[1];

	s = str_copy(cond->arg_value[0],0);
	if (!s)
	{
		destroy_cond(new_cond);
		return NULL;
	}
	new_cond->arg_value[0] = s;

	s = str_copy(cond->arg_value[1],0);
    if (!s)
    {
        destroy_cond(new_cond);
        return NULL;
    }
    new_cond->arg_value[1] = s;

	return new_cond;
}


st_cond_node* copy_condnode(const st_cond_node *condnode)
{
	st_cond_node *cnnod;
	st_cond_node *first,*second;
	st_cond *cond;

	if (is_condleaf(condnode))
	{
		cond = copy_cond(condnode->cond);
		if (!cond)
		{
			destroy_condnode(cnnod);
			return NULL;
		}
	
		cnnod = create_condnode(NULL,NULL,0,cond);	
		if (!cnnod)
		{
			destroy_condnode(cnnod);
			return NULL;
		}
		
		/* cnnod->neg_cond = condnode->neg_cond; */
		
		return cnnod;
	}	
	else
	{
		first = copy_condnode(condnode->child_cond1);
		if (!first)
	    {
            destroy_condnode(cnnod);
            return NULL;
        }

        second = copy_condnode(condnode->child_cond2);
        if (!second)
        {
            destroy_condnode(first);
            return NULL;
        }

		if (condnode->cond)
		{
        	cond = copy_cond(condnode->cond);
        	if (!cond)
        	{
            	destroy_condnode(first);
				destroy_condnode(second);
            	return NULL;
        	}
		}
		else cond = NULL;

		cnnod = create_condnode(first,second,condnode->logop,cond);
		if (!cnnod)
		{
			destroy_condnode(first);
			destroy_condnode(second);
			if (cond) destroy_cond(cond);
		}

		/* cnnod->neg_cond = condnode->neg_cond; */

		return cnnod;
	}

	return NULL;
}

void destroy_condnode(st_cond_node *condnode)
{
	if (condnode)
	{
		if (condnode->child_cond1) destroy_condnode(condnode->child_cond1);
		if (condnode->child_cond2) destroy_condnode(condnode->child_cond2);
		if (condnode->cond) destroy_cond(condnode->cond);
		xfree(condnode);
	}
}

void destroy_cond(st_cond *cond)
{
	if (cond)
	{
    	if (cond->arg_value[0]) xfree(cond->arg_value[0]);
	    if (cond->arg_value[1]) xfree(cond->arg_value[1]);
    	xfree(cond);
  	}
}

