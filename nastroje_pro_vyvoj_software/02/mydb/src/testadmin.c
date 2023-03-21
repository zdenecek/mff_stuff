#include <stdio.h>
#include <string.h>
#include "sql_parser.h"
#include "db_main.h"
#include "config.h"
#include "memory.h"
#include "error.h"
#include "strutil.h"
#include "server_comm.h"

#include <readline/readline.h>
#include <readline/history.h>

int main(int argc,char **argv)
{
	st_query_parsed *query;
 	st_result *result;
  	char *line;
  	int ret;
  	st_col_data *col;
  	st_result_set *resdata;
  	st_row_data *resrow;
  	int quit = 0;

  	init_error();

  	ret = read_config();
  	if (!ret)
  	{
		printf("testadmin - chyba v cteni konfigurace\n");
		return 1;
  	}

 	/* printf("testadmin - datadir: %s\n",datadir); */
  	/* printf("testadmin - current_db: %s\n",current_db); */


  	while (!quit)
  	{

		init_error(); /* cleaning old error info */

    	/* reading and parsing query */

		/*
    	line = (char*)xmalloc(1024);
    	if (!line) 
    	{
      		printf("testadmin - %s\n",err_descr());
      		return 1;
    	}

    	line = fgets(line,1024,stdin); 
		*/

		line = readline("");
		if (!line)
		{
			printf("testadmin - spatny dotaz\n");
			return 1;
		}
		add_history(line);

		if (STR_BEGIN(line,"quit"))
		{
	  		quit = 1;
	  		xfree(line);
	  		continue;
		}

		/* parse SQL query */
   	 	query = parse_sqlquery(line);
    	if (!query)
    	{
      		printf("testadmin - spatna query1: %s\n",err_descr());
	  		xfree(line);
	  		continue;
    	}

		printf("testadmin - spravna query\n");

		init_error(); /* clean old error info */

    	/* process query */
    	result = process_query(query);

    	if (!result)
    	{
      		printf("testadmin - spatny result: %s\n",err_descr());
	  		xfree(line);
	  		destroy_queryparsed(query);
	  		continue;
    	}

    	printf("testadmin - spravny result\n");

    	printf("%s\n",result->description);

	    /* print result */
    	if (result->resultdata)
    	{
      		resdata = result->resultdata;

      		printf("result rows\n");
     		resrow = resdata->resrows;
      		while (resrow)
      		{
        		col = resrow->data;
        		while (col)
        		{
          			printf(" %s: %s ",col->col_name,col->data_value);
			        col = col->next;
        		}
        		printf("\n");
        		resrow = resrow->next;
      		}
    	}
    	else
    	{
  	  		printf("result ret\n");
	  		printf("%d\n",result->resultret);	
    	}	

    	destroy_queryparsed(query);
    	destroy_result(result);
    	xfree(line);
  	}

  	destroy_config();

  	return 0;
}
