#include "error.h"

static st_error err_info;

/* This messages will be displayed to user(client) */
char *error_str[] = {"Out of memory", "Fatal I/O error", "File system error", "Database already exists", "Database cannot be removed", "SQL syntax error","Unspecified error","No database used","Table is too big","Database doesn't exist","Index already exist for this column","Table doesn't exist","Index doesn't exist", "Column doesn't exist","Socket error","Btree error","Condition with both columns from same table not allowed"};


void init_error(void)
{
	err_info.number = 0;
	err_info.description = "";
}


void set_error(int no)
{
	err_info.number = no;
	err_info.description = error_str[no-1];
}


char* err_descr(void)
{
	return err_info.description;
}


int err_number(void)
{
	return err_info.number;
}
