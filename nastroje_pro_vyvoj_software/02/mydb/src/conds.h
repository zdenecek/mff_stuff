#ifndef __CONDS_H__
#define __CONDS_H__

#include "data.h"

/* types of arguments in conditions */
typedef enum { COLUMN = 1, VALUE } en_condarg;

/* strings will be compared lexicographically */
typedef enum { EQUAL = 1 , NOTEQUAL, LESS, MORE, LESSEQUAL, MOREEQUAL } en_operator;

/* logical operators */
typedef enum { AND = 1, OR } en_logoper;

/* 
  structure for logical expression
  if (exp1 == NULL) && (exp2 == NULL) then value is taken 
*/
typedef struct stlogexp
{
    int value; /* 1 - true, 0 - false */
	struct stlogexp *exp1;
	struct stlogexp *exp2;
    en_logoper logop; /* 1 - AND, 2 - OR */
} st_logexp;

/* condition */
typedef struct stcond
{
    char *arg_value[2];
    en_condarg arg_type[2];
    en_operator oper;
} st_cond;

/* 
  node of condition tree
  there can be both child nodes, logop and cond for joins or only cond(in leaf) 
*/
typedef struct stcondnode
{
	struct stcondnode *child_cond1;
	struct stcondnode *child_cond2;
	en_logoper logop; /* 0 - no operator */
	st_cond *cond;
	/* int neg_cond; 1 - negate result, 0 - don't change it */
} st_cond_node;


/* 
  evaluates tree of logical expression
  1 - TRUE
  0 - FALSE
  -1 - error 
*/
int logexp_eval(st_logexp *exp);

/* 
  test if this col satisfy condition
  col2 != NULL - condition using columns from different tables
  1 - TRUE
  0 - FALSE
  -1 - error 
*/
int col_satisfy(const st_col_data *col, const st_col_data *col2, const st_cond *cond);

/* 
  puts variable argument of condition to value
  value must be INT
  0 - error(there is no INT in cond, etc 
*/
int get_condvalue(const st_cond *cond, int *value);

/* 
  returns pointer to column used in condition
  doesn't create new string
  NULL - error 
*/
char* get_condcolumn(const st_cond *cond);

/* 
  create condition
  NULL - error 
*/
st_cond* create_cond(char *argval1, char *argval2, en_condarg argtype1, en_condarg argtype2, en_operator oper);

/* 
  create cond_node
  NULL - error 
*/
st_cond_node* create_condnode(st_cond_node *child1, st_cond_node *child2, const en_logoper logop, st_cond *cond);

/* 
  create logexp for this row and condition tree
  NULL - error 
*/
st_logexp* create_logexp(const st_row_data *row, const st_cond_node *conds);

/* 
  test if data in this row satisfy all conditions in tree
  1 - TRUE
  0 - FALSE
  -1 - error 
*/
int row_satisfy(const st_row_data *row, const st_cond_node *conds);

/* 
  checks if conditions in tree uses columns only from one table
  1 - TRUE
  0 - FALSE
  -1 - error 
*/
int conds_fromtable(const st_cond_node *conds, const char *tabname);

/* 
  test for leaf of the tree
  0 - isn't leaf
  1 - leaf 
*/
int is_condleaf(const st_cond_node *condnode);

/* 
  create a copy of condition
  NULL - error 
*/
st_cond* copy_cond(const st_cond *cond);

/* 
  create copy of whole condtree
  NULL - error 
*/
st_cond_node* copy_condnode(const st_cond_node *condnode);

/* free all memory used by cond_node and its childs */
void destroy_condnode(st_cond_node *condnode);

/* free all memory used by condition */
void destroy_cond(st_cond *cond);

#endif
