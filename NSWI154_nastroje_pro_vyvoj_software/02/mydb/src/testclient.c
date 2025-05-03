#include <stdio.h>
#include <string.h>
#include "client_comm.h"
#include "memory.h"
#include "strutil.h"

#include <readline/readline.h>
#include <readline/history.h>

int main(int argc,char **argv)
{
	st_sock_info *sockinfo;
  	int ret;
  	char *line;
  	int quit = 0;

	while (!quit)
  	{
		/* connect to server */

    	sockinfo = sock_create("127.0.0.1");
    	if (!sockinfo)
    	{
      		printf("testclient - nelze vyrobit socket\n");
      		return 1;
    	}  
  
    	ret = sock_connect(sockinfo);
    	if (!ret)
    	{
      		printf("testclient - nelze se pripojit\n");
      		xfree(sockinfo);
      		return 1;
    	} 

		/*
    	line = (char*)xmalloc(INPUT_SIZE+1);  
    	if (!line)
    	{
	  		printf("testclient - nelze alokovat pamet\n");
	  		sock_close(sockinfo);
	  		xfree(sockinfo);
	  		return 1;
    	}

    	printf("zadej dotaz\n");

    	line = fgets(line,INPUT_SIZE,stdin);
    	if (!line)
    	{
	 		printf("testclient - chyba pri cteni vstupu\n");
	  		xfree(line);
	  		sock_close(sockinfo);
      		xfree(sockinfo);
      		return 1;
    	}
		*/
	
		/* read query */	
		line = readline("");
        if (!line)
        {
            printf("testclient - chyba pri cteni vstupu\n");
            sock_close(sockinfo);
            xfree(sockinfo);
            return 1;
        }
		add_history(line);

		/* send query to server */
    	ret = write_query(sockinfo->sock,line);
    	if (!ret)
    	{
	  		printf("testclient - chyba pri posilani dotazu\n");
      		xfree(line);
	  		sock_close(sockinfo);
      		xfree(sockinfo);
      		return 1;
    	}

    	printf("dotaz odeslan na server\n");

    	if (STR_BEGIN(line,"quit"))
    	{
      		xfree(line);
      		sock_close(sockinfo);
      		xfree(sockinfo);
      		quit = 1;
      		continue;
    	}

		/* get result from server and print it */
    	ret = process_result(sockinfo->sock);
    	if (!ret)
    	{
      		printf("testclient - chyba pri prijmu a vypisu odpovedi\n");
      		xfree(line);
	  		sock_close(sockinfo);
      		xfree(sockinfo);
      		return 1;
    	}
  
    	ret = sock_close(sockinfo);
    	xfree(line);
  	}
  
  	return 0;
}
