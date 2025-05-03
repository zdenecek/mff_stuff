#ifndef __RESULT_H__
#define __RESULT_H__

#include "data.h"
#include "tabdef.h"
#include "conds.h"

typedef struct strestab
{
    char *tabname;
    struct strestab *NEXT;
} st_result_tab;

/* 
  this stores set of data rows
  used in operations with result
  result for insert,update,etc will contain no rows 
*/
typedef struct
{
    st_result_tab *restabs;
    st_row_data *resrows;
    long row_count;
} st_result_set;

/* 
  stores one node in tree of result sets
  (child1 == NULL && child2 == NULL) => node is leaf(resset != NULL)
  resset == NULL => node isn't leaf
  condnode != NULL for nodes joining two tables 
*/
typedef struct stresnode
{
	struct stresnode *child1;
	struct stresnode *child2;
    st_result_set *resset;
    en_logoper logop;
	st_cond_node *condnode;
} st_res_node;

/* 
  db library will return the header and all selected row_data in sequence
  server then must send it to client somehow 
*/
typedef struct
{
    st_result_set *resultdata;
    int resultret;
    char *description; /* describes the result */
} st_result;


/* free all memory used by result_tab */
void destroy_resulttab(st_result_tab *restab);

/* free all memory used by result_set */
void destroy_resultset(st_result_set *resset);

/* 
  creates a copy of input result_tab
  NULL - error 
*/
st_result_tab* copy_resulttab(const st_result_tab *restab);

/* 
  creates result node with content determined by params
  NULL - error 
*/
st_res_node* create_resnode(st_res_node *child1, st_res_node *child2, st_result_set *resset, en_logoper logop, st_cond_node *condnode);

/* 
  free all memory used by st_res_node
  destroy whole tree
  can be called recursive 
*/
void destroy_resnode(st_res_node *node);

/* free all memory used by st_result */
void destroy_result(st_result *result);

#endif
