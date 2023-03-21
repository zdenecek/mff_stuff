#ifndef __TABDEF_H__
#define __TABDEF_H__

#include "linklist.h"

/* isupported data types for storing */
typedef enum { INT = 1, VARCHAR } en_col_type;

/* table column definition */
typedef struct stcoldef
{
    char *name;
    en_col_type type;
	struct stcoldef* NEXT;
} st_col_def;

/* table definition */
typedef struct
{
    char *name;
    /* pointer to the first element of array of columns */
    st_col_def *columns;
    int col_num;
} st_table_def;

/* 
  get table_def for table
  returns tabdef
  NULL - error(table does not exist) 
*/
st_table_def* get_tabledef(const char *tabname);

/* 
  writes table_def to file
  0 - error 
*/
int write_tabledef(const st_table_def *tabdef);

/* 
  delete tabdef from file
  returns deleted record
  NULL - error(table doesn't exist, etc) 
*/
st_table_def* delete_tabledef(const char *tabname);

/* returns size of memory occupied by tabledef */
int tabledef_size(const st_table_def *tbdef);

/* 
  converts st_table_def to string
  NULL - error 
*/
char* tabdef_string(const st_table_def *tabdef);

/* 
  converts string to st_table_def
  NULL - error 
*/
st_table_def* string_tabdef(const char *str);

/* free all memory used by coldef */
void destroy_coldef(st_col_def *coldef,int all);

/* free all memory used by tabdef */
void destroy_tabdef(st_table_def *tabdef);

/* 
  checks if in table exist such a column that could store this value
  checks if tested value has correct size(number of figures or chars)
  colname cannot be qualified by tabname 
  0 - column doesn't exist
  1 - OK
  -1 - error 
*/
int correct_colvalue(const st_table_def *tabdef, const char *colname, const char *value);

/*
  checks if in table exist column with this name
  0 - FALSE
  1 - OK
  -1 - error 
*/
int exist_column(const char *tabname, const char *colname);

/* 
  copy all elements in linked list of coldefs
  NULL - error 
*/
st_col_def* copy_coldef(const st_col_def *oldcldf);

/*
  checks if this column from table has this type
  1 - TRUE
  0 - FALSE
  -1 - error
*/
int column_type(const char *tabname, const char *colname, en_col_type typ);

/* returns count of columns */
int count_coldef(st_col_def *coldef);

#endif
