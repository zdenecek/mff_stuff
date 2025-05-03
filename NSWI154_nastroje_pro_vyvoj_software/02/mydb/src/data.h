#ifndef __DATA_H__
#define __DATA_H__

#include "textfileio.h"
#include "linklist.h"

/* all data types are internally represemted as chars */
typedef struct stcol
{
	char *col_name; /* in the form <tabname>.<colname> */
	char *data_value;
	struct stcol *NEXT;
} st_col_data;

/* table row */
typedef struct strow
{
	st_col_data *data; /* pointer to the head of linked list */
	long data_count;
	struct strow *NEXT;
} st_row_data;

/* 
  remove tablename from column names
  this should be done before writing row to a file
  0 - error
*/
int strip_tablename(const st_row_data* const row);

/* 
  add table name to column names
  this should be done after reading row from a file
  0 - error 
*/
int add_tablename(st_row_data* const row, const char *tabname);

/* 
  reads one row from current position in file
  tabname != NULL -> put tabname and '.' before colname
  NULL - error
*/
st_row_data* read_rowdata(st_file_info *fi,const char *tabname);

/* 
  write rowdata at the end of file
  0 - error
*/
int write_rowdata(st_file_info *fi,const st_row_data *rowdata);

/* 
  delete one row at current position
  returns deleted row
  NULL - error 
*/
st_row_data* delete_rowdata(st_file_info *fi);

/* 
  converts rowdata to string
  NULL - error 
*/
char* rowdata_string(const st_row_data *row);

/* 
  converts string to rowdata
  NULL - error 
*/
st_row_data* string_rowdata(const char *str);

/* 
  returns pointer to value for this column
  NULL - error 
*/
char* get_colvalue(const st_row_data *row, const char *colname);

/* free all memory used by col_data */
void destroy_coldata(st_col_data *col,int all);

/* free all memory used by row_data */
void destroy_rowdata(st_row_data *row,int all);

/* returns size of memory occupied by row data */
int rowdata_size(const st_row_data *rowdata);

/* 
  create copy of one column
  NULL - error 
*/
st_col_data* copy_coldata(const st_col_data *oldcol);

/* 
  creates a copy of row data
  NULL - error 
*/
st_row_data* copy_rowdata(const st_row_data *oldrow);

/* 
  updates data in old row and return new row with new data
  old row should be destroyed afterwards
  NULL - error
*/
st_row_data* update_rowdata(const st_row_data *oldrow, const st_col_data *newdata);

/* returns count of columns */
int count_cols(st_col_data *col);

#endif
