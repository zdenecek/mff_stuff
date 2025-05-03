#ifndef __ERROR_H__
#define __ERROR_H__

/* error numbers */
typedef enum { ERR_NOMEM=1, ERR_FATIO, ERR_FILESYS, ERR_DBEXIST, ERR_DBNORM, ERR_SQLSYNTAX, ERR_STRANGE, ERR_NODBSEL, ERR_BIGTABLE, ERR_DBNOEX, ERR_IDXEXIST, ERR_TABNOEXIST, ERR_IDXNOEX, ERR_COLNOEX, ERR_SOCKET, ERR_BTREE, ERR_NOJOIN } en_error_numbers;

/* 
  user is enecouraged to use functions instead of modyfing err_info directly
  structure can be changed at any time 
*/
typedef struct
{
  int number;
  char *description;
} st_error;

/* contains all error messages */
extern char *error_str[];

/* initialize err_info */
void init_error(void);

/* stores error info */
void set_error(int no);

/* returns error description */
char* err_descr(void);

/* returns error number */
int err_number(void);

#endif
