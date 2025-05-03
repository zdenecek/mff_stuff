#include <stdlib.h>
#include <string.h>
#include "db_engine.h"
#include "memory.h"
#include "error.h"
#include "linklist.h"
#include "strutil.h"
#include "textfileio.h"
#include "index.h"
#include "tabconf.h"
#include "result.h"


st_res_node* get_resset_tree(const st_query_tabset *tabset, const st_cond_node *condnode)
{
	const st_query_tabset *tmp_tbset = NULL;
	st_result_tab *restab = NULL, *curtab = NULL;
	st_result_set *resset = NULL;
	st_res_node *resnode = NULL,*resnode1 = NULL,*resnode2 = NULL;
	st_cond_node *cnnod = NULL;
	char *str = NULL;

	if (!condnode) /* no condition */
	{
		/* convert st_query_tabset to st_result_tab */
		restab = NULL;
		tmp_tbset = tabset;
		while (tmp_tbset)
		{
			curtab = (st_result_tab*)xmalloc(sizeof(st_result_tab));
			if (!curtab) goto error1;

			str = str_copy(tmp_tbset->name,0);
			if (!str) goto error1;

			curtab->tabname = str;
			curtab->NEXT = restab;
			restab = curtab;

			tmp_tbset = tmp_tbset->NEXT;
		}

    	resset = table_cartes_prod(restab);
        if (!resset) goto error1;

        resnode = create_resnode(NULL,NULL,resset,0,NULL);
        if (!resnode) goto error1;

		destroy_resulttab(restab);

		return resnode;
	}

	if (is_condleaf(condnode))
	{
		tmp_tbset = tabset;
		while (tmp_tbset)
		{
			if (conds_fromtable(condnode, tmp_tbset->name)) break;
			tmp_tbset = tmp_tbset->NEXT;
		}

		if (tmp_tbset) /* condition is from one table */
		{
			resset = get_tabledata(tmp_tbset->name, condnode);
			if (!resset) return NULL;			

			cnnod = copy_condnode(condnode);
			if (!cnnod) goto error1;

            resnode = create_resnode(NULL,NULL,resset,0,cnnod);
            if (!resnode) goto error1;

			return resnode;			
		}
		else /* leaf cond joining two tabs - create two nodes with whole tabs */		{
			/* first subnode */			
			str = str_copy(condnode->cond->arg_value[0],'.');
			if (!str) return NULL;

			resnode1 = get_resnode_table(str);
			if (!resnode1) goto error1;

			xfree(str);

			/* second subnode */         
			str = str_copy(condnode->cond->arg_value[1],'.');
            if (!str) goto error1;

			resnode2 = get_resnode_table(str);
			if (!resnode2) goto error1;

			xfree(str);
		
			/* main node */
			cnnod = copy_condnode(condnode);
            if (!cnnod) goto error1;

			resnode = create_resnode(resnode1,resnode2,NULL,0,cnnod);
            if (!resnode) goto error1;

			return resnode;
		}
	}
	else /* node is not leaf */
	{
	    resnode1 = get_resset_tree(tabset, condnode->child_cond1);
        if (!resnode1) return NULL;

        resnode2 = get_resset_tree(tabset, condnode->child_cond2);
        if (!resnode2) goto error1;

        cnnod = copy_condnode(condnode);
        if (!cnnod) goto error1;

        /* parent node */
		resnode = create_resnode(resnode1,resnode2,NULL,condnode->logop,cnnod);
        if (!resnode) goto error1;

        return resnode;
	}
	return NULL;

error1:
	xfree(str);
	xfree(curtab);
	destroy_resulttab(restab);
	destroy_condnode(cnnod);
	destroy_resnode(resnode2);
	destroy_resnode(resnode1);
	destroy_resultset(resset);
	return NULL;	
}


st_res_node* get_resnode_table(const char *tabname)
{
	st_result_set *resset;
	st_res_node *resnode;

	resset = get_tabledata(tabname, NULL);
    if (!resset) return NULL;

    resnode = create_resnode(NULL,NULL,resset,0,NULL);
    if (!resnode)
	{
		destroy_resultset(resset);
		return NULL;
	}

	return resnode;
}


st_result_set* compute_result(st_res_node* resnode)
{
	int ret;
	st_result_set *resset=NULL,*resset1=NULL,*resset2=NULL;
	st_result_set *tmpres1=NULL,*tmpres=NULL,*tmpres2=NULL;
	st_result_tab *tabset;
	st_row_data *cur;
	st_cond_node *condnode;
	int count;

	if ((!resnode->child1) && (!resnode->child2)) /* leaf */
	{
		destroy_condnode(resnode->condnode);
		resset = resnode->resset;
		xfree(resnode);
		return resset;
	}
	else
	{
		resset1 = compute_result(resnode->child1);
		if (!resset1) return NULL;

		resset2 = compute_result(resnode->child2);
		if (!resset2) return NULL;

		if (resnode->condnode->cond) /* joining condition */
		{
			/* create temporary condnode */
			condnode = create_condnode(NULL,NULL,0,resnode->condnode->cond);
			if (!condnode) return NULL;

			resset = cartes_product(resset1,resset2,condnode);
			if (!resset) goto error2;

			xfree(condnode);

			destroy_condnode(resnode->condnode);
			xfree(resnode);

			return resset;
		}
		else if (resnode->logop)
		{
	        resset = (st_result_set*)xmalloc(sizeof(st_result_set));
    	    if (!resset) return NULL;

            /* create result_tab */
            ret = compare_restab(resset1->restabs,resset2->restabs);
			if (ret)
			{
				tabset = resset1->restabs;
                destroy_resulttab(resset2->restabs);
                resset2->restabs = NULL;

				resset->restabs = tabset;

				resset->resrows = append_resrows(resset1->resrows,resset2->resrows,&count);
				if (!resset->resrows) goto error1;
				
				resset->row_count = resset1->row_count + resset2->row_count - count;

				xfree(resset1);
				xfree(resset2);

				resset1 = resset;
			}
            else /* result_tabs(schemas) are not same */
            {
                tabset = join_resulttabs(resset1->restabs,resset2->restabs);
                if (!tabset) goto error1;
            
            	resset->restabs = tabset;

				/* prepare first set to merging */
				ret=diff_resulttabs(resset1->restabs,resset2->restabs,&tabset);
				if (!ret) goto error1;
		
				if (tabset) /* there is some difference */
				{
					tmpres = table_cartes_prod(tabset);
					if (!tmpres) goto error1;
				
					destroy_resulttab(tabset);
			
					tmpres1 = cartes_product(resset2,tmpres,NULL);
					if (!tmpres1) goto error1;

					destroy_resultset(tmpres);
				}

            	/* prepare second set to merging */
            	ret=diff_resulttabs(resset2->restabs,resset1->restabs,&tabset);
            	if (!ret) goto error1;

            	if (tabset)
            	{
                	tmpres = table_cartes_prod(tabset);
                	if (!tmpres) goto error1;

           			destroy_resulttab(tabset);
 
            		tmpres2 = cartes_product(resset1,tmpres,NULL);
            		if (!tmpres2) goto error1;

            		destroy_resultset(tmpres);
				}

				/* append resset2 at the end of resset1 */
				cur = tmpres1->resrows;
				while (cur->NEXT) cur = cur->NEXT;
				cur->NEXT = tmpres2->resrows;
				tmpres1->row_count += tmpres2->row_count;

				destroy_resultset(resset1);
				destroy_resultset(resset2);

				resset1 = tmpres1;
			}
			
			/* apply logical operator */
			if (resnode->logop == OR)
			{
				destroy_condnode(resnode->condnode);
				xfree(resnode);

				ret = remove_duplicate_rows(resset1);
				if (!ret)
				{
					destroy_resultset(resset1);
					return NULL;
				}
				return resset1;
			}
			else if (resnode->logop == AND)
            {
				ret = filter_resultset(resset1,resnode->condnode);
				if (!ret) goto error1;

				destroy_condnode(resnode->condnode);
                xfree(resnode);

				ret = remove_duplicate_rows(resset1);
				if (!ret)
				{
					destroy_resultset(resset1);
					return NULL;
				}
				return resset1;
			}
			else 
			{
				destroy_resultset(resset1);
				destroy_resnode(resnode);
				return NULL;
			}
		}
		else return NULL;
	}

	return NULL;

error2:
	destroy_condnode(condnode);
	return NULL;
error1:
	destroy_resultset(resset);
	return NULL;
}


st_result_set* complete_result(st_result_set *resset,st_query_tabset *querytab)
{
    st_result_set *new_resset,*resset1,*resset2;
    st_result_tab *restab,*headtab,*curtab;
    char *s;

	/* find tables which are in querytab but not in resset->restab */
    headtab = NULL;
    while (querytab)
    {
        restab = resset->restabs;
        while (restab)
        {
            if (STR_EQ(restab->tabname,querytab->name)) break;
            restab = restab->next;
        }

        if (!restab) /* table not present in result tab */
        {
            curtab = (st_result_tab*)xmalloc(sizeof(st_result_tab));
            if (!curtab)
            {
                destroy_resulttab(headtab);
                return NULL;
            }

            curtab->NEXT = NULL;

            s = str_copy(querytab->name,0);
            if (!s)
            {
                destroy_resulttab(curtab);
                destroy_resulttab(headtab);
                return NULL;
            }

            curtab->tabname = s;

            curtab->NEXT = headtab;
            headtab = curtab;
        }

        querytab = querytab->next;
    }


    if (headtab)
    {
        resset2 = table_cartes_prod(headtab);
        if (!resset2)
        {
            destroy_resulttab(headtab);
            return NULL;
        }

        destroy_resulttab(headtab);

        resset1 = resset;

        new_resset = cartes_product(resset1,resset2,NULL);
        if (!new_resset)
        {
            destroy_resultset(resset2);
            destroy_resulttab(headtab);
            return NULL;
        }
    }
	else new_resset = resset;

	return new_resset;
}


int apply_colset(const st_query_colset *colset, st_result_set *resset)
{
	const st_query_colset *cols = NULL;
	st_row_data *row = NULL,*newrow = NULL;
	st_row_data *headrow = NULL,*prevrow = NULL;
	st_col_data *head = NULL,*prev = NULL,*cur = NULL,*oldcol = NULL;
	int count;
	char *s = NULL,*s2 = NULL,*tabname = NULL;

	headrow = NULL;
	prevrow = NULL;

	row = resset->resrows;
	while (row) /* loop over all rows */
	{
		newrow = (st_row_data*)xmalloc(sizeof(st_row_data));
		if (!newrow) return 0;

		count = 0;
		cols = colset;

		head = NULL;
		prev = NULL;

		/* loop over colset */
		/* order columns in row according to colset */
		while (cols) 
		{
			s = strstr(cols->name,"*");

			if (s) /* take all columns from table */
			{
				s2 = strchr(cols->name,'.');
				if (s2) /* there is tablename before * */
				{
					tabname = str_copy(cols->name,'.');
					if (!tabname) goto error;
				}
				else /* there should be only one table */
				{
					tabname = str_copy(resset->restabs->tabname,0);
					if (!tabname) goto error;
				}
	
				oldcol = row->data;

				while (oldcol)
				{
					if (STR_BEGIN(oldcol->col_name,tabname))
					{
                		cur = copy_coldata(oldcol);
                		if (!cur) goto error;
					
						LIST_ADDTAIL(head,prev,cur);	
	                	count++;
					}
					oldcol = oldcol->NEXT;
				}
			}
			else /* take only this column from table */
			{
				oldcol = row->data;
				while (oldcol && (!STR_EQ(oldcol->col_name,cols->name))) 
				{
					oldcol = oldcol->NEXT;
				}
				if (!oldcol) goto error; /* no column found */

				cur = copy_coldata(oldcol);
				if (!cur) goto error;

				LIST_ADDTAIL(head,prev,cur);

				count++;
			}

			cols = cols->NEXT;
		}

		newrow->data_count = count;
		newrow->data = head;

		LIST_ADDTAIL(headrow,prevrow,newrow);
	
		row = row->NEXT;
	}		

	destroy_rowdata(resset->resrows,1);
	resset->resrows = headrow;

	return 1;
error:
	destroy_coldata(head,1);
    destroy_rowdata(newrow,0);
    return 0;
}


st_result_set* get_tabledata(const char *tabname, const st_cond_node *condnode)
{
	st_result_set *resset = NULL;
	st_file_info *fi;
	st_row_data *cur,*head,*prev;
	st_result_tab *restab = NULL;
	st_cond *cond = NULL;
	int ret;
	long count;
	char *str = NULL,*s = NULL;
	char *colname = NULL;
	st_index_row *index_head = NULL;

	head = NULL;
	prev = NULL;
	cur = NULL;

	if (condnode) cond = condnode->cond;
	else cond = NULL;

	resset = (st_result_set*)xmalloc(sizeof(st_result_set));
	if (!resset) return NULL;

	resset->restabs = NULL;
	resset->resrows = NULL;
	resset->row_count = 0;

	/* creating result_tab */  
	restab = (st_result_tab*)xmalloc(sizeof(st_result_tab));
	if (!restab) goto error1;

	s = str_copy(tabname,0);
	if (!s)
	{
		destroy_resulttab(restab);
		goto error1;
	}
	restab->tabname = s;
	
	resset->restabs = restab;


	if (!cond) /* no condition - read whole file */
	{
		fi = file_open(current_db,tabname,FILE_DATA);
		if (!fi) goto error1;
  
  		file_reset(fi);

		count = 0;
		while ((cur = read_rowdata(fi,tabname)))
		{
			LIST_ADDTAIL(head,prev,cur);
			count++;
		}

		if (!file_close(fi)) destroy_fileinfo(fi);

		resset->resrows = head;
		resset->row_count = count;

		return resset;
	}
	else /* we have condition */
	{
		s = get_condcolumn(cond);
		if (!s) goto error1;

		/* remove table name if present */
		colname = strchr(s,'.');
		if (!colname) colname = s;
		else colname++;

		if (column_indexed(tabname,colname)) /* we have index */
		{
		    /* add colname to tabname for the purpose of file name */
    		str = str_concat(tabname,colname,'_');
    		if (!str) return 0;

    		fi = file_open(current_db,str,FILE_INDEX);
    		if (!fi) goto error1;

			/* use index */
			index_head = get_indexrows(fi, cond);
			if (!index_head)	
			{
				xfree(str);
				goto error1;

			}
			xfree(str);

			if (!file_close(fi))
			{
				destroy_fileinfo(fi);
				goto error1;
			}

			count = rows_from_index(tabname,index_head,&head);
			if (count == (-1)) goto error1;

			destroy_indexrow(index_head,1);

	        resset->resrows = head;
    	    resset->row_count = count;

        	return resset;
		}
		else /* we don't have index */
		{

			fi = file_open(current_db,tabname,FILE_DATA);
        	if (!fi) goto error1;

        	file_reset(fi);

        	count = 0;

        	while ((cur = read_rowdata(fi,tabname)))
        	{
				ret = row_satisfy(cur,condnode);
				if (ret)
				{
					LIST_ADDTAIL(head,prev,cur);				
            		count++;
				}
        	}

			if (!file_close(fi)) destroy_fileinfo(fi);

        	resset->resrows = head;
        	resset->row_count = count;

        	return resset;
		}
	}

	return NULL;

error1:
	destroy_resultset(resset);
	return NULL;
}


int compare_restab(const st_result_tab *first, const st_result_tab *second)
{
	const st_result_tab *tab1,*tab2;

	/* check if all tabs from first restab are in second */
	tab1 = first;
	while (tab1)
	{
		tab2 = second;
		while (tab2)
		{
			if (STR_EQ(tab2->tabname,tab1->tabname)) break;
			tab2 = tab2->NEXT;
		}

		if (!tab2) return 0; /* current tab1 isn't in tab2 */

		tab1 = tab1->NEXT;
	}

    /* check if all tabs from second restab are in first */
    tab2 = second;
    while (tab2)
    {
        tab1 = first;
        while (tab1)
        {
            if (STR_EQ(tab1->tabname,tab2->tabname)) break;
            tab1 = tab1->NEXT;
        }

        if (!tab1) return 0; /* current tab2 isn't in tab1 */

        tab2 = tab2->NEXT;
    }

	return 1;
}


st_result_tab* join_resulttabs(st_result_tab *first, st_result_tab *second)
{
	st_result_tab *tab1,*tab2,*prev,*cur,*copy_head;
	int in_first;

	copy_head = NULL;
	cur = NULL;
	if ((!first) || (!second)) return NULL;

	/* append to first restab those tabs from second which are not first */
	/* make copies of all appended tabs from second */
	tab2 = second;
	prev = NULL;
	while (tab2)
	{
		tab1 = first;
		in_first = 0;
		while (tab1)
		{
			/* table is in first */
			if (STR_EQ(tab1->tabname,tab2->tabname)) in_first = 1;
			tab1 = tab1->NEXT;
		}

		if (!in_first)
		{
			cur = copy_resulttab(tab2);
			if (!cur) return NULL;
			
			cur->NEXT = copy_head;
			copy_head = cur;
		}

		prev = tab2;
		tab2 = tab2->NEXT;
	}	

	/* make copy of first tabset */
	tab1 = copy_resulttab(first);
	if (!tab1) 
	{
		destroy_resulttab(copy_head);
		return NULL;
	}
	while (tab1->NEXT) tab1 = tab1->NEXT;
	tab1->NEXT = copy_head;

	return tab1;	
}


int diff_resulttabs(st_result_tab *first, st_result_tab *second, st_result_tab **tabset)
{
	st_result_tab *head,*cur,*prev;
	st_result_tab *tab;

	head = NULL;
	prev = NULL;

	while (first)
	{
		/* check if table from first tabset isn't in second */
		tab = second;
		while (tab)
		{
			if (STR_EQ(first->tabname,tab->tabname)) break;
			tab = tab->NEXT;
		}

		if (!tab) /* this table isn't in second restab */
		{
			cur = copy_resulttab(first);
			if (!cur)
			{
				destroy_resulttab(head);
				return 0;
			}

			LIST_ADDTAIL(head,prev,cur);
		}
	
		first = first->NEXT;
	}

	*tabset = head;

	return 1;
}


st_row_data* join_resultrows(const st_row_data *first,const st_row_data *second)
{
	st_row_data *row1,*row2,*row;
	st_col_data *col;

	if ((!first) || (!second)) return NULL;

	row1 = copy_rowdata(first);
	if (!row1) return NULL;

	row2 = copy_rowdata(second);
    if (!row2)
	{
		destroy_rowdata(row1,0);
		return NULL;
	}

	row = row1;
	
	/* append columns from second row to the end of first */	
	col = row->data;
	while (col->NEXT) col = col->NEXT;
	col->NEXT = row2->data;
	row->data_count += row2->data_count;

	row->NEXT = NULL;
	xfree(row2);

	return row;
}


st_result_set* cartes_product(st_result_set* first, st_result_set *second, const st_cond_node *condnode)
{
	st_result_set *resset;
	st_row_data *row1,*row2;
	st_result_tab *restab;
	st_row_data *head,*cur,*prev;
	int count;
	int ret;

	head = NULL;
	prev = NULL;

	resset = (st_result_set*)xmalloc(sizeof(st_result_set));
    if (!resset) return NULL;

    resset->restabs = NULL;
    resset->resrows = NULL;
    resset->row_count = 0;

	restab = join_resulttabs(first->restabs,second->restabs);
    if (!restab)
	{
		destroy_resultset(resset);
		return NULL;
	}

	resset->restabs = restab;

	count = 0;
	row1 = first->resrows;
	while (row1)
	{
		row2 = second->resrows;
		while (row2)
		{
			cur = join_resultrows(row1,row2);
			if (!cur)
			{
				destroy_rowdata(head,1);
				destroy_resultset(resset);
				return NULL;
			}

			if (condnode) 
			{
				ret = row_satisfy(cur,condnode);
				if (ret == (-1))
				{
					destroy_rowdata(cur,0);
					destroy_rowdata(head,1);
	                destroy_resultset(resset);
    	            return NULL;
				}
			}
			else ret = 1; 

            if (ret)
            {
                LIST_ADDTAIL(head,prev,cur);
                count++;
            }
            else destroy_rowdata(cur,0);

			row2 = row2->NEXT;
		}

		row1 = row1->NEXT;
	}

	resset->resrows = head;
	resset->row_count = count;	

	return resset;
}


st_result_set* table_cartes_prod(st_result_tab *tabset)
{
	st_result_set *resset = NULL,*resset1 = NULL, *resset2 = NULL;

	if (!tabset) return NULL;

    resset1 = get_tabledata(tabset->tabname,NULL);
    if (!resset1) return NULL;

    tabset = tabset->NEXT;

    while (tabset)
    {
    	resset2 = get_tabledata(tabset->tabname,NULL);
        if (!resset2)
		{
			destroy_resultset(resset2);
			return NULL;
		}

        resset = cartes_product(resset1,resset2,NULL);
        if (!resset)
		{
			destroy_resultset(resset1);
			destroy_resultset(resset2);
			return NULL;
		}

        tabset = tabset->NEXT;

        resset1 = resset;
	}

	return resset1;
}


int filter_resultset(st_result_set *resset, const st_cond_node *condnode)
{
	st_row_data *row1,*row2,*prev;
	int ret;
	int count = 0;

	row1 = resset->resrows;
	prev = row1;
    while (row1)
	{
    	ret = row_satisfy(row1, condnode);
		if (ret == (-1)) return 0;

        if (!ret) /* delete row from list */
        {
        	if (row1 == resset->resrows)
            {
	            resset->resrows = row1->NEXT;
                row1->NEXT = NULL;
                destroy_rowdata(row1,0);
                row1 = resset->resrows;
				prev = row1;
            }
            else
            {
				prev->next = row1->next;
            	row2 = row1;
                row1 = row1->NEXT;
                row2->NEXT = NULL;
                destroy_rowdata(row2,0);
            }

			count++;
        }
        else 
		{
			prev = row1;
			row1 = row1->NEXT;
		}
    }

	resset->row_count -= count;
	
	return 1;
}


st_row_data* append_resrows(st_row_data *first, st_row_data *second, int *count)
{
	st_row_data *row1 = NULL,*row2 = NULL;
	st_row_data *head = NULL,*prev = NULL,*cur = NULL,*prev2 = NULL;
	int ret;
	
	(*count) = 0;

	head = first;
	cur = first;
	/* move to the end of first set */
	while (cur)
	{
		prev = cur;
		cur = cur->NEXT;
	}

	prev2 = NULL;
	row2 = second;

	while (row2)
	{
		row1 = first;
		while (row1)
		{
			ret = compare_rowdata(row1,row2,NULL);
			if (ret == (-1)) return NULL;

			if (ret) break;
					
			row1 = row1->NEXT;
		}

		if (row1) /* there is some duplicate row - remove it from second set */
		{
			(*count)++;

			if (prev2)
			{
				prev2->NEXT = row2->NEXT;
				row2->NEXT = NULL;
				destroy_rowdata(row2,0);
				row2 = prev2->NEXT;
			}
			else
			{
				second = row2->NEXT;
				row2->NEXT = NULL;
				destroy_rowdata(row2,0);
				row2 = second;
				prev2 = NULL;
			}
		}
		else 
		{
			prev2 = row2;
			row2 = row2->NEXT;
		}
	}
     
	/* append second set at the end of first */
	prev->NEXT = second;

	return head;
}


int compare_rowdata(const st_row_data *first, const st_row_data *second, char *ordercol)
{
	st_col_data *col1,*col2;
	int i;
	int value1,value2;
	int ret;

	if (!ordercol) /* compare all columns */
  	{
    	if (first->data_count != second->data_count) return 0;

    	col1 = first->data;
		while (col1)
		{
			/* find corresponding column in second row */
	    	col2 = second->data;
			while (col2)
			{
				if (STR_EQ(col1->col_name,col2->col_name)) break;
				col2 = col2->NEXT;
			}
			if (!col2) return 0;
		
			if (!STR_EQ(col1->data_value,col2->data_value)) return 0;

			col1 = col1->NEXT;
		}

    	return 1; /* EQUAL DATA */
  	}
	else /* compare only order column */
  	{
		/* find correct column */
		col1 = first->data;
		i = 0;
		while ((i < first->data_count) && (!STR_EQ(col1->col_name,ordercol))) 
		{
	  		i++;
	  		col1 = col1->NEXT;
		}
		if (i == first->data_count) return (-1);
	
		/* find correct column */
		col2 = second->data;
    	i = 0;
    	while ((i < second->data_count) && (!STR_EQ(col2->col_name,ordercol))) 
    	{
      		i++;
      		col2 = col2->NEXT;
    	}
    	if (i == second->data_count) return (-1);

		/* compare values in column */
		if ((is_num(col1->data_value)) && (is_num(col2->data_value)))
		{
	  		value1 = strtol(col1->data_value,0,10);
	  		value2 = strtol(col2->data_value,0,10);
	  		if (value1 < value2) return 1;
	  		else if (value2 > value1) return 2;
	  		else return 0;		
		}
		else
		{	
	  		ret = strcmp(col1->data_value,col2->data_value);
	  		if (ret < 0) return 1;
	  		else if (ret > 0) return 2;
	  		else return 0;  
		}	
  	}

  	return (-1); /* strange error */
}


st_row_data* sort_rows(st_row_data *rows,char *ordercol)
{
	st_row_data *head,*cur,*prev,*head2,*cur2,*prev2;
  	st_row_data *pivot;
  	st_row_data *newhead,*newhead2;
  	int ret;

  	if (!rows->NEXT) return rows; /* end of recursion */
  	else
  	{
		/* first row is pivot */
		pivot = rows;
		head = rows->NEXT;
		pivot->NEXT = NULL;

		prev = head;
		head2 = NULL;
		prev2 = NULL;
		cur2 = NULL;

		/* divide linked list into two parts */
		cur = head;
		while (cur)
		{
			ret = compare_rowdata(pivot,cur,ordercol);
	  		if (ret == (-1)) return NULL;	  	

	  		/* pivot is smaller - move cur to other part(linked list 2) */
	  		if (ret == 1)
	  		{
				if (cur == head)
				{
		  			head = cur->NEXT;
					prev = head;
				}
				else prev->NEXT = cur->NEXT;
		
				LIST_ADDTAIL(head2,prev2,cur);

				cur = prev;
	  		}
	  		else /* leave cur in this part(original linked list) */
	  		{
	    		prev = cur;
				cur = cur->NEXT; 
	  		}
		}

		/* recursively sort both parts */
		if (head) newhead = sort_rows(head,ordercol);
		else newhead = NULL;
		if (head2) newhead2 = sort_rows(head2,ordercol);
		else newhead2 = NULL;

		/* merge sorted parts together */
		if (newhead)
		{
	  		cur = newhead;
	  		while (cur->NEXT) cur = cur->NEXT;
	  		cur->NEXT = pivot;
	  		cur = cur->NEXT;
	  		if (newhead2) cur->NEXT = newhead2;
	  		else cur->NEXT = NULL;
	  		return newhead;
		}
		else if (newhead2)
		{
	  		pivot->NEXT = newhead2;
	  		return pivot;
		}
		else 
		{
	 		pivot->NEXT = NULL;
	  		return pivot;
		}
  	}
}


int rows_from_index(const char *tabname, st_index_row *idxhead, st_row_data **rows)
{
	st_file_info *fi;
    st_row_data *cur,*head,*prev;
    long count;
    st_index_row *index_cur;

	head = NULL;
	prev = NULL;
	cur = NULL;

    fi = file_open(current_db,tabname,FILE_DATA);
    if (!fi) return (-1);

    index_cur = idxhead;
    count = 0;
    while (index_cur)
    {
		fi->readpos = fi->address + index_cur->filepos;

        cur = read_rowdata(fi,tabname);
        if (!cur)
        {
            if (!file_close(fi)) destroy_fileinfo(fi);
            destroy_rowdata(head,1);
			return (-1);
        }

        LIST_ADDTAIL(head,prev,cur);
        count++;

        index_cur = index_cur->NEXT;
    }

    if (!file_close(fi))
    {
    	destroy_fileinfo(fi);
		destroy_rowdata(head,1);
		return (-1);
    }

	(*rows) = head;
	return count;
}


int remove_duplicate_rows(st_result_set *resset)
{
	st_row_data *row1 = NULL,*row2 = NULL, *prev = NULL;
    int ret;
	int count = 0;
	int remove;

	row1 = resset->resrows;
    
	while (row1)
    {
		/* try to find duplicate row */
		remove = 0;
		prev = row1;
        row2 = row1->next;
        while (row2)
        {
            ret = compare_rowdata(row1,row2,NULL);
            if (ret == (-1)) return 0;

        	/* row2 is duplicate  - remove it */
			if (ret)
        	{
				prev->NEXT = row2->NEXT;
                row2->NEXT = NULL;
                destroy_rowdata(row2,0);
                row2 = prev->NEXT;

				count++;
			}
        	else
        	{
            	prev = row2;
            	row2 = row2->NEXT;
        	}
		}

		row1 = row1->NEXT;
    }

	resset->row_count -= count;

    return 1;
}

