#ifndef __PARSER_COMMANDS_H__
#define __PARSER_COMMANDS_H__

#include "query.h"
#include "sql_parser.h"

/* 
  parse USE query
  0 - error 
*/
int parse_use(parsed_str **inputquery, st_query_parsed* const parsquery);

/* 
  parse DROP queries
  0 - error 
*/
int parse_drop(parsed_str **inputquery, st_query_parsed* const parsquery);

/* 
  parse CREATE queries
  0 - error 
*/
int parse_create(parsed_str **inputquery, st_query_parsed* const parsquery);

/* 
  parse CREATE DATABASE query
  0 - error 
*/
int parse_crdb(parsed_str **inputquery, st_query_parsed* const parsquery);

/* 
  parse CREATE TABLE QUERY
  0 - error 
*/
int parse_crtab(parsed_str **inputquery, st_query_parsed* const parsquery);

/* 
  parse CREATE INDEX query
  0 - error 
*/
int parse_crindex(parsed_str **inputquery, st_query_parsed* const parsquery);

/* 
  parse SELECT query
  0 - error 
*/
int parse_select(parsed_str **inputquery, st_query_parsed* const parsquery);

/* 
  parse INSERT query
  0 - error 
*/
int parse_insert(parsed_str **inputquery, st_query_parsed* const parsquery);

/* 
  parse UPDATE query
  0 - error 
*/
int parse_update(parsed_str **inputquery, st_query_parsed* const parsquery);

/* 
  parse DELETE query
  0 - error 
*/
int parse_delete(parsed_str **inputquery, st_query_parsed* const parsquery);

/* 
  returns NULL - error
  parses all up to the and including ")" 
*/
st_col_def* parse_coldef(parsed_str **inputquery);

/* 
  returns NULL - error
  parses all up to the and excluding "from" 
*/
st_query_colset* parse_colset(parsed_str **inputquery);

/* 
  returns NULL - error
  parses all up to the and excluding "where" or "order by" 
*/
st_query_tabset* parse_tabset(parsed_str **inputquery);

/* 
  returns NULL - error
  also checks equality of count of column names and supplied values
  parses all up to the and including ";" 
*/
st_col_data* parse_insertdata(parsed_str **inputquery);

/* 
  returns NULL - error
  also checks equality of count of column names and supplied values
  parses everything up to and excluding "where" or ";" 
*/
st_col_data* parse_updatedata(parsed_str **inputquery);

#endif
