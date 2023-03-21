#ifndef __QUERY_H__
#define __QUERY_H__

#include "linklist.h"
#include "data.h"
#include "tabdef.h"
#include "conds.h"

/* all supported database(SQL) queries */
typedef enum { SELECT=1 , INSERT, UPDATE, DELETE, CREATE_DB, CREATE_TAB, CREATE_INDEX, DROP, USE_DB } en_dbactions;

/* parsed set of column names */
typedef struct stcolset
{
    char *name;
	struct stcolset *NEXT;
} st_query_colset;

/* parsed set of table names */
typedef struct sttabset
{
    char *name;
	struct sttabset *NEXT;
} st_query_tabset;

/* structure for use database query */
typedef struct
{
    char *dbname;
} st_query_use;

/* common structure for all three commands */
typedef struct
{
    char *targetname;
    int target; /* 1 - DATABASE, 2 - TABLE, 3 - INDEX */
} st_query_drop;

/* structure for create database query */
typedef struct
{
    char *name;
} st_query_crdb;

/* structure for create table query */
typedef struct
{
    char *name;
    st_col_def *coldefs;
    int col_num;
} st_query_crtab;

/* structure for create index query */
typedef struct
{
    char *tabname;
    char *colname;
} st_query_crindex;

/* structure for select query */
typedef struct
{
    st_query_colset *colset;
    st_query_tabset *tabset;
    st_cond_node *condtree;
    char *ordercol;
} st_query_select;

/* structure for insert query */
typedef struct
{
    char *tabname;
    st_col_data *coldata; /* colnames are from tabname (column names) and col values from VALUES (column values) */
} st_query_insert;

/* structure for update query */
typedef struct
{
    char *tabname;
    st_col_data *coldata;
    st_cond_node *condtree;
} st_query_update;

/* structure for delete query */
typedef struct
{
    char *tabname;
    st_cond_node *condtree;
} st_query_delete;

/* union for storing query data - one at the time */
typedef union
{
    st_query_use* param_use;
    st_query_drop* param_drop;
    st_query_crdb* param_crdb;
    st_query_crtab* param_crtab;
    st_query_crindex* param_crindex;
    st_query_select* param_select;
    st_query_insert* param_insert;
    st_query_update* param_update;
    st_query_delete* param_delete;
} un_query_data;

/* stores parsed query */
typedef struct
{
    en_dbactions action;
    un_query_data querydata;
} st_query_parsed;


/* return coutn of tables */
int count_tabs(const st_query_tabset *tab);

/* 
  counts number of tables with this column
  lasttab is pointer to last table with this column or NULL 
  -1 - error 
*/
int count_coltabs(const st_query_tabset *tabset, const char *colname, st_query_tabset **lastab);

/* 
  checks if table picked up from colname is in tabset
  0 - FALSE
  1 - TRUE 
*/
int coltab_exist(const char *colname, const st_query_tabset *tabset);

/* 
  free all memory used by st_query_colset
  all == 1 -> destroy whole list 
*/
void destroy_querycolset(st_query_colset *colset, int all);

/* 
  free all memory used by st_query_tabset
  all == 1 -> destroy whole list 
*/
void destroy_querytabset(st_query_tabset *tabset, int all);

/* free all memory used by st_query_parsed */
void destroy_queryparsed(st_query_parsed *query);

/* free all memory used by st_query_delete */
void destroy_querydelete(st_query_delete *query);

/* free all memory used by st_query_update */
void destroy_queryupdate(st_query_update *query);

/* free all memory used by st_query_insert */
void destroy_queryinsert(st_query_insert *query);

/* free all memory used by st_query_select */
void destroy_queryselect(st_query_select *query);

/* free all memory used by st_query_crindex */
void destroy_querycrindex(st_query_crindex *query);

/* free all memory used by st_query_crtab */
void destroy_querycrtab(st_query_crtab *query);

/* free all memory used by st_query_crdb */
void destroy_querycrdb(st_query_crdb *query);

/* free all memory used by st_query_drop */
void destroy_querydrop(st_query_drop *query);

/* free all memory used by st_query_use */
void destroy_queryuse(st_query_use *query);

#endif
