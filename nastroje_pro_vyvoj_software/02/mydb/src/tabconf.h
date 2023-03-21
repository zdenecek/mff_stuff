#ifndef __TABCONF_H__
#define __TABCONF_H__

#include "linklist.h"

/* element of linked list of indexed columns for table */
typedef struct stidxcol
{
    char *colname;
	struct stidxcol *NEXT;
} st_index_col;

/* database table configuration */
typedef struct
{
    char *name;
    char *datafile;
    st_index_col *indexcols; // elements is st_indexcol */
} st_table_conf;

/* 
  get table_conf
  returns tabconf
  NULL - error
*/
st_table_conf* get_tableconf(const char *tabname);

/* 
  writes table_conf to file
  0 - error
*/
int write_tableconf(const st_table_conf *tabconf);

/*
  delete tabconf from file
  returns deleted record
  NULL - error(table doesn't exist, etc)
*/
st_table_conf* delete_tableconf(const char *tabname);

/* 
  converts st_table_conf to string
  NULL - error 
*/
char* tabconf_string(const st_table_conf *tabconf);

/*  
  converts string to st_table_conf
  NULL - error
*/
st_table_conf* string_tabconf(const char *str);

/* 
  checks whether exist index for some column in table
  1 - TRUE
  0 - FALSE
  (-1) - error
*/
int table_indexed(const char *tabname);

/* 
  checks whether column in table is indexed
  1 - TRUE(is indexed)
  0 - FALSE
  -1 - error
*/
int column_indexed(const char *tabname, const char *colname);

/* 
  adds this column to the list of indexed columns
  0 - error
 */
int add_indexcol(st_table_conf* const tabconf,const char *colname);

/* 
  delete this column from the list of indexed columns
  0 - error(column isn't indexed, etc)
*/
int del_indexcol(st_table_conf* const tabconf,const char *colname);

/* free all memory used by tabconf */
void destroy_tabconf(st_table_conf *tabconf);

/* 
  destroy all memory used by st_index_col
  all == 1 -> destroy whole linked list
*/
void destroy_indexcol(st_index_col *idxcol, int all);

#endif
