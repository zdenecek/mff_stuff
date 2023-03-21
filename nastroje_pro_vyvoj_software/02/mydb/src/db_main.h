#ifndef __DB_MAIN_H__
#define __DB_MAIN_H__

#include "query.h"
#include "result.h"

#define PROCTABSIZE 9

typedef struct
{
	en_dbactions action;
	char *descr;
	int (*processfunc) (const st_query_parsed *query, st_result **result);
} st_proctab_elem;

extern st_proctab_elem processtab[PROCTABSIZE];

/* prepares table with function pointers */
void init_processor(void);

/* 
  processes query and returns result
  call functions from db_engine, db_admin and sql_parser
  determines which function to call for getting correct result, etc
  NULL - error */
st_result* process_query(const st_query_parsed *query);

/* 
  process SELECT query
  0 - error 
*/
int process_select(const st_query_parsed *query, st_result **res);

/* 
  process INSERT query
  0 - error 
*/
int process_insert(const st_query_parsed *query, st_result **res);

/* 
  process UPDATE query
  0 - error 
*/
int process_update(const st_query_parsed *query, st_result **res);

/* 
  process DELETE query
  0 - error 
*/
int process_delete(const st_query_parsed *query, st_result **res);

/* 
  process CREATE DATABASE query
  0 - error 
*/
int process_crdb(const st_query_parsed *query, st_result **res);

/* 
  process CREATE TABLE query
  creates all files necessary for table
  0 - error 
*/
int process_crtab(const st_query_parsed *query, st_result **res);

/* 
  process CREATE INDEX query
  0 - error 
*/
int process_crindex(const st_query_parsed *query, st_result **res);

/* 
  process DROP queries
  0 - error 
*/
int process_drop(const st_query_parsed *query, st_result **res);

/* 
  removes all data from table and records from definition and config file
  0 - error
  1 - OK 
*/
int drop_table(const char *tabname);

/* 
  process USE query
  0 - error 
*/
int process_use(const st_query_parsed *query, st_result **res);

#endif
