#ifndef __PARSER_CONDS_H__
#define __PARSER_CONDS_H__

#include "conds.h"
#include "sql_parser.h"

/* 
  parses all conditions up to the and excluding "order by" or ";"
  calls parse_condnode or parse_basic_cond 
  NULL - error 
*/
st_cond_node* parse_conditions(parsed_str **inputquery);

/* 
  parse one condition without surrounding brackets or all conditions in bracket 
  pair recursively and return them in condition tree
  can be called recursive on eclosed conds in brackets
  NULL - error 
*/
st_cond_node* parse_condnode(parsed_str **inputquery);

/* 
  parse condition in form argument1 oper argument2
  NULL - error 
*/ 
st_cond* parse_basic_cond(parsed_str **inputquery);

#endif
