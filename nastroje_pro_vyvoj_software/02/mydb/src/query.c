#include <string.h>
#include "error.h"
#include "memory.h"
#include "linklist.h"
#include "strutil.h"
#include "query.h"

int count_tabs(const st_query_tabset *tab)
{
	int tab_count = 0;

	while (tab)
	{
		tab_count++;
		tab = tab->NEXT;
	}

	return tab_count;
}

int count_coltabs(const st_query_tabset *tabset, const char *colname, st_query_tabset **lasttab)
{
	const st_query_tabset *tab;
	int tab_count;
	int ret;

	tab_count = 0;
    tab = tabset;
	(*lasttab) = NULL;
            
    while (tab)
    {
    	ret = exist_column(tab->name,colname);
        if (ret == (-1)) return (-1);

        if (ret)
        {
	        (*lasttab) = (st_query_tabset*)tab;
            tab_count++;
        }
        
		tab = tab->NEXT;
    }

	return tab_count;
}


int coltab_exist(const char *colname, const st_query_tabset *tabset)
{
	const st_query_tabset *tab;
	int is_tab;
	size_t size;

	tab = tabset;
    is_tab = 0;

    /* check if table name is in tabset */
    while (tab)
    {
        if ((colname) && (STR_BEGIN(colname,tab->name)))
        {
	        size = strlen(tab->name);
            if (colname[size] == '.')
            {
            	is_tab = 1;
                break;
            }
        }
    	tab = tab->NEXT;
    }

	return is_tab;
}


void destroy_querycolset(st_query_colset *colset,int all)
{
	st_query_colset *cur,*head;

  	if (all)
  	{
		head = colset;
		while (head)
		{
	  		cur = head;
	  		head = head->NEXT;
	  		xfree(cur->name);
	  		xfree(cur);
		}
  	}
  	else
  	{
    	if (colset)
    	{
  	  		xfree(colset->name);
	  		xfree(colset);
    	}
  	}
}


void destroy_querytabset(st_query_tabset *tabset,int all)
{
	st_query_tabset *cur,*head;

  	if (all)
  	{
		head = tabset;
 		while (head)
 		{
	  		cur = head;
	  		head = head->NEXT;
	  		xfree(cur->name);
	  		xfree(cur);
		}
  	}
  	else
  	{
    	if (tabset)
    	{
	  		xfree(tabset->name);
	  		xfree(tabset);
    	}
  	}
}


void destroy_queryparsed(st_query_parsed *query)
{
  	if (query)
  	{
		/* choose function to destroy query parameters */
		switch (query->action)
		{
	  		case SELECT:
				destroy_queryselect(query->querydata.param_select);
				break;
	  		case INSERT:
	    		destroy_queryinsert(query->querydata.param_insert);
				break;
	  		case UPDATE:
				destroy_queryupdate(query->querydata.param_update);
				break;
	  		case DELETE:
				destroy_querydelete(query->querydata.param_delete);
				break;
	  		case CREATE_DB:
				destroy_querycrdb(query->querydata.param_crdb);
				break;
	  		case CREATE_TAB:
				destroy_querycrtab(query->querydata.param_crtab);
				break;
	  		case CREATE_INDEX:
				destroy_querycrindex(query->querydata.param_crindex);
				break;
	  		case DROP:
				destroy_querydrop(query->querydata.param_drop);
				break;
	  		case USE_DB:
				destroy_queryuse(query->querydata.param_use);
				break;
		}
		xfree(query);
  	}
}


void destroy_querydelete(st_query_delete *query)
{
	if (query)
  	{
		destroy_condnode(query->condtree);
		xfree(query->tabname);
    	xfree(query);
  	}
}


void destroy_queryupdate(st_query_update *query)
{
	if (query)
  	{
		xfree(query->tabname);
    	destroy_condnode(query->condtree);
		destroy_coldata(query->coldata,1);
		xfree(query);
  	}
}


void destroy_queryinsert(st_query_insert *query)
{
	if (query)
  	{
		xfree(query->tabname);
		destroy_coldata(query->coldata,1);
    	xfree(query);
  	}
}


void destroy_queryselect(st_query_select *query)
{
	if (query)
  	{
		destroy_condnode(query->condtree);
    	destroy_querytabset(query->tabset,1);
		destroy_querycolset(query->colset,1);
		xfree(query->ordercol);
		xfree(query);
  	}
}


void destroy_querycrindex(st_query_crindex *query)
{
	if (query)
  	{
		xfree(query->tabname);
		xfree(query->colname);
    	xfree(query);
  	}
}


void destroy_querycrtab(st_query_crtab *query)
{
	if (query)
  	{
		xfree(query->name);
		destroy_coldef(query->coldefs,1);
		xfree(query);
  	}
}


void destroy_querycrdb(st_query_crdb *query)
{
  	if (query)
  	{
		xfree(query->name);
    	xfree(query);
  	}
}


void destroy_querydrop(st_query_drop *query)
{
	if (query)
  	{
		xfree(query->targetname);
		xfree(query);
  	}
}


void destroy_queryuse(st_query_use *query)
{
	if (query)
  	{
		xfree(query->dbname);
		xfree(query);
  	}
}

