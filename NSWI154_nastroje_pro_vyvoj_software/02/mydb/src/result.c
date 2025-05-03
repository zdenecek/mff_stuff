#include <string.h>
#include "error.h"
#include "memory.h"
#include "linklist.h"
#include "strutil.h"
#include "result.h"


void destroy_resulttab(st_result_tab *restab)
{
	st_result_tab *cur;

  	while (restab)
  	{
		cur = restab;
		restab = restab->NEXT;

		if (cur->tabname) xfree(cur->tabname);
		xfree(cur);
  	}
}


void destroy_resultset(st_result_set *resset)
{
	if (resset)
  	{
    	destroy_resulttab(resset->restabs);
		destroy_rowdata(resset->resrows,1);

    	xfree(resset);
  	}
}


st_result_tab* copy_resulttab(const st_result_tab *restab)
{
	st_result_tab *newrestab;
	st_result_tab *prev,*cur;
	char *str;

	newrestab = NULL;
	prev = NULL;

	/* walk over all elements of input result_tab */
	while (restab)
  	{
		cur = (st_result_tab*)xmalloc(sizeof(st_result_tab));
		if (!cur) return NULL;

		str = str_copy(restab->tabname,0);
		if (!str) 
		{
	  		xfree(cur);
	  		return NULL;
		}
		cur->tabname = str;
		LIST_ADDTAIL(newrestab,prev,cur);

		restab = restab->NEXT;
  	}
  
  	return newrestab;
}


st_res_node* create_resnode(st_res_node *child1, st_res_node *child2, st_result_set *resset, en_logoper logop, st_cond_node *condnode)
{
	st_res_node *resnode;
	
	resnode = (st_res_node*)xmalloc(sizeof(st_res_node));
    if (!resnode) return NULL;

	resnode->child1 = child1;
    resnode->child2 = child2;
    resnode->resset = resset;
    resnode->logop = logop;
    resnode->condnode = condnode;

	return resnode;
}


void destroy_resnode(st_res_node *node)
{
	if (node)
    {
		destroy_resultset(node->resset);
		destroy_resnode(node->child1);
		destroy_resnode(node->child2);
		xfree(node);
	}
}


void destroy_result(st_result *result)
{
  if (result)
  {
	destroy_resultset(result->resultdata);
	//if (result->description) xfree(result->description);
	xfree(result);
  }	
}


