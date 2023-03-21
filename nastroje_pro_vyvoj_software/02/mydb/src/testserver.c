#include <stdio.h>
#include <string.h>
#include "sql_parser.h"
#include "db_main.h"
#include "memory.h"
#include "error.h"
#include "strutil.h"
#include "config.h"
#include "server_comm.h"

int main(int argc,char **argv)
{
	st_query_parsed *query;
  	st_result *result;
  	char *line;
  	int ret;
  	int quit = 0;
  	int sock,newsock;

  	init_error();

 	ret = read_config();
 	if (!ret)
  	{
		printf("testserver - chyba v cteni konfigurace\n");
		return 1;
  	}

  	printf("testserver - datadir: %s\n",datadir);
  	printf("testserver - current_db: %s\n",current_db);


  	sock = sock_create();
  	if (sock == (-1))
  	{
		printf("testserver - nelze vyrobit socket\n");
		return 1;
	}

  	ret = start_listen(sock);
  	if (!ret)
  	{
		printf("testserver - nelze nastartovat listening\n");
		return 1;
  	}

  	while (!quit)
  	{

		newsock = wait_for_request(sock);
		if (newsock == (-1))
		{
	  		printf("testserver - chyba pri cekani na request\n");
	  		return 1;
		}

    	/* reading query */
    	line = read_request(newsock);
    	if (!line) 
    	{
      		printf("testserver - chyba pri cteni dotazu\n");
	  		sock_close(newsock);
      		return 1;
    	}

    	printf("dotaz %s\n",line);

		if (STR_BEGIN(line,"quit"))
		{
	  		quit = 1;
	  		xfree(line);
	  		sock_close(newsock);
	  		continue;
		}

		init_error(); /* cleaning old error info */

		/* parsing SQL query */
    	query = parse_sqlquery(line);
    	if (!query)
    	{
      		printf("testserver - spatna query1: %s\n",err_descr());
	  		xfree(line);
	  		sock_close(newsock);
	  		continue;
    	}

    	printf("testserver - spravna query\n");

		init_error(); /* cleaning old error info */

    	/* processing query */
    	result = process_query(query);

    	if (!result)
    	{
      		printf("testserver - spatny result: %s\n",err_descr());
	  		xfree(line);
	  		destroy_queryparsed(query);
	  		sock_close(newsock);
	  		continue;
    	}

		/* sending result */
		ret = send_result(result,newsock);
		if (!ret)
		{
	  		printf("testserver - chyba pri posilani resultu\n");
      		destroy_queryparsed(query);
      		destroy_result(result);
      		xfree(line);
      		sock_close(newsock);
	  		continue;
		}

    	destroy_queryparsed(query);
    	destroy_result(result);
    	xfree(line);
		sock_close(newsock);
  	}

  	sock_close(sock);

  	destroy_config();

  	return 0;
}
