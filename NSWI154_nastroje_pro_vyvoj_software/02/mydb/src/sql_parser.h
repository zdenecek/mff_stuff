#ifndef __SQL_PARSER_H__
#define __SQL_PARSER_H__

#include "query.h"

#define TABSIZE 7
#define TABSIZE_CREATE 3


#define QUERY_NOT_OK  (!query) || ((query) && (STR_EQ(query->token,";")))

#define QUERY_SKIP query = skip_token(query); \
				   query = skip_spaces(query);


/* one item of list of parsed-string tokens */
typedef struct parstr
{
	char *token;
    struct parstr *NEXT;
} parsed_str;

/* element of table of parsing functions */
typedef struct
{
    char *query;
    int (*parsfunc) (parsed_str**,st_query_parsed*);
} st_parstab_elem;


extern st_parstab_elem parsertab[TABSIZE];
extern st_parstab_elem parscreatetab[TABSIZE_CREATE];

/* prepares table with keywords and function pointers */
void init_parser(void);

/* 
  parse SQL query
  NULL - unsuccessful 
*/
st_query_parsed* parse_sqlquery(const char* inputquery);

/* 
  prepare SQL query for further parsing
  put spaces around operators and in other necessary places
  returns modified query string
  NULL - error 
*/
char* prepare_sqlquery(const char* inputquery);

/* 
  complete columns names with table names
  used only for colset and conditions in select
  0 in the case of conflict
  -1 - error 
*/
int complete_colnames(st_query_parsed* const query);

/* 
  complete column names with table names in query_colset
  0 - conflict or nonexistence of column/table
  -1 - error 
*/
int complete_colsetnames(st_query_colset *colset, st_query_tabset *tabset);

/* 
  complete column names with table names in conditions
  can be called recursive
  0 - conflict or nonexistence of column/table
  -1 - error 
*/
int complete_condcolnames(st_cond_node *condnode, st_query_tabset *tabset);

/*
  complete order column with table name 
  0 - conflict
  -1 - error 
*/
int complete_ordercolname(st_query_select *selectquery);

/* 
  check query syntax(brackets,quotes,semicolon)
  checks for forbidden characters in whole query except arguments in quotes
  0 - error 
*/
int check_sqlsyntax(const char *query);

/* 
  checks query against definition of table
  existence of table and columns, correct data types, etc
  1 - OK
  0 - error 
*/
int check_parsedquery(st_query_parsed *query);

/* 
  checks parsed conditions against definition of table
  existence of columns, correct data types, etc
  could be called recursively
  1 - OK
  0 - error 
*/
int check_parsedconds(st_cond_node *condnode,st_query_tabset *tabset);

/* 
  parses string and divides it into array of strings
  returns this array in structure along with other info
  NULL - error 
*/
parsed_str* str_tokenize(const char *str,const char *delims);

/* 
  destroy linked list of tokens
  all == 1 - destroy all elements in list
  all == 0 - destroy only this element 
*/
void destroy_tokens(parsed_str* pars_str,int all);

/* skip all tokens containing only space and destroy skipped items in list */
parsed_str* skip_spaces(parsed_str *query);

/* skip one element of list and destroy this element */
parsed_str* skip_token(parsed_str *query);

#endif
