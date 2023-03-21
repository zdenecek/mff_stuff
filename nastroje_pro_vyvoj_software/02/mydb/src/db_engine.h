#ifndef __DB_ENGINE_H__
#define __DB_ENGINE_H__

#include "config.h"
#include "data.h"
#include "query.h"
#include "index.h"
#include "conds.h"
#include "result.h"


/* 
  create a tree of result sets according to conditions
  conditions using(joining) two tables cannot be in leafs
   - all rows from both tables will be in leafs under that condition
  should be used recursive
  NULL - error 
*/
st_res_node* get_resset_tree(const st_query_tabset *tabset, const st_cond_node *condnode);

/* 
  return resnode with all rows from table
  NULL - error 
*/
st_res_node* get_resnode_table(const char *tabname);

/* 
  computes result set from result_set tree
  join result sets according to logical operators
  should be called recursive
  also destroys unused part of resnode
  NULL - error 
*/
st_result_set* compute_result(st_res_node* resnode);

/*
  completes result by making cartesian product with all tables that are in 
  querytabset(in FROM section of query) but not in current result created 
  by compute_result
  NULL - error
*/
st_result_set* complete_result(st_result_set *resset,st_query_tabset *querytab);

/* 
  remove unwanted columns from all rows in resset
  0 - error 
*/
int apply_colset(const st_query_colset *colset, st_result_set *resset);

/* 
  returns all rows from table, which satisfy conditions
  each condition has to contain columns only from this table or constants 
  if cond == NULL, return all rows
  NULL - error 
*/ 
st_result_set* get_tabledata(const char *tabname, const st_cond_node *condnode);

/* 
  compares result_tabs (test if they contain the same tables)
  1 - TRUE(they are same)
  0 - FALSE
*/
int compare_restab(const st_result_tab *first, const st_result_tab *second);

/* 
  joins two result_tabs into one(create one common schema
  append the second to the first
  don't destroy input
  NULL - error 
*/
st_result_tab* join_resulttabs(st_result_tab *first, st_result_tab *second);

/* 
  computes difference between first and second
  returns tables which are in first and not in second tabset
  0 - error 
*/
int diff_resulttabs(st_result_tab *first, st_result_tab *second, st_result_tab **tabset);

/* 
  joins two rows into one
  create copies and join them
  doesn't eliminate duplicate columns from the same table in both input rows
  NULL - error 
*/
st_row_data* join_resultrows(const st_row_data *first, const st_row_data *second);

/* 
  make cartesian product of two result_sets
  condnode == NULL -> don't apply condition
  NULL - error 
*/
st_result_set* cartes_product(st_result_set* first, st_result_set *second, const st_cond_node *condnode);

/* 
  make cartesian product of all tables in tabset
  NULL - error 
*/
st_result_set* table_cartes_prod(st_result_tab *tabset);

/* 
  removes row which don't satisfy condition
  condnode == NULL -> leave all rows in result_set 
  0 - error 
*/
int filter_resultset(st_result_set *resset, const st_cond_node *condnode);

/*
  appends rows from second set to the end of first
  rows from second set which are duplicates of rows from first set, are deleted
  count contains number of deleted rows after return from function
  NULL - error
*/
st_row_data* append_resrows(st_row_data *first, st_row_data *second, int *count);

/* 
  compares data in both rows
  if ordercol == NULL
  1 - EQUAL , 0 - DIFFERENT
  if ordercol != NULL, then compares only data in this column
  0 - EQUAL, 1 - FIRST SMALLER, 2 - FIRST LARGER
  -1 - error 
*/
int compare_rowdata(const st_row_data *first, const st_row_data *second, char *ordercol);

/* 
  sorts all rows with ordercol as a key
  uses quicksort algorithm
  NULL - error 
*/
st_row_data* sort_rows(st_row_data *rows,char *ordercol);

/* 
  read from table all rows stored in index rows 
  return number of rows
  -1 - error 
*/
int rows_from_index(const char *tabname, st_index_row *idxhead, st_row_data **rows);

/*
  remove duplicate rows from result set
  duplicate rows means completely identical
  0 - error
*/
int remove_duplicate_rows(st_result_set *resset);

#endif
