#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "server_comm.h"
#include "memory.h"
#include "error.h"
#include "strutil.h"
#include "data.h"

int sock_create()
{
	int sock;
  	struct sockaddr_in name;
	int ret;
	int value;

  	/* create socket */
 	sock = socket(PF_INET, SOCK_STREAM, 0);
  	if (sock == (-1))
  	{
		set_error(ERR_SOCKET);
    	return (-1); 
  	}

	value = 1;
	ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int));
	if (ret == (-1))
	{
		set_error(ERR_SOCKET);
		return (-1);
	}

  	/* give socket a name */
  	name.sin_family = AF_INET;
  	name.sin_port = htons(PORT);
  	name.sin_addr.s_addr = htonl(INADDR_ANY);
  	if (bind (sock,(struct sockaddr*)&name, sizeof(name)) == (-1))
  	{
		set_error(ERR_SOCKET);
		return (-1); /* error */
  	}

  	return sock;
}


int start_listen(int sock)
{
  	if (listen(sock,1) == (-1)) 
  	{
		set_error(ERR_SOCKET);
		return 0; /* error */
  	}
  	return 1;
}


/* -1 - error */
int wait_for_request(int sock)
{
  	int new_sock;
  	struct sockaddr_in clientname;
  	size_t size;

  	size = sizeof (clientname);
  	new_sock = accept(sock,(struct sockaddr*)&clientname,&size);
  	if (new_sock == (-1))
  	{
		set_error(ERR_SOCKET);
		return (-1);
  	}

  	return new_sock;
}


char* read_request(int sock)
{
	char *buf;
  	int n;
  	int ret;
  	char c;
	
 	buf = (char*)xmalloc(INPUT_SIZE+1);
 	if (!buf) return NULL;

  	n = 0;
  	do
  	{
		ret = read(sock,&c,1);
		if (ret == (-1))
		{
	  		xfree(buf);
			set_error(ERR_SOCKET);
	  		return NULL;
		}
		if (!ret) break;

		buf[n]=c;
		n++;
  	}
  	while ((n<=INPUT_SIZE) && (c!=';'));
  	buf[n]=0;

  	return buf;	
}


int send_result(st_result *result,int sock)
{
  	int ret;
  	char *s;
  	st_col_data *col;
  	st_result_set *resdata;
  	st_row_data *resrow;

  	/* sends each part of result as string using my protocol */
  	if (result->resultdata) /* sending result of SELECT */
  	{
		resdata = result->resultdata;

   		ret = write(sock,"Result rows\n",12);
    	if (ret == (-1)) goto error;

    	ret = write(sock,"\n",1);
    	if (ret == (-1)) goto error;

		/* loop over rows in result */
    	resrow = resdata->resrows;
    	while (resrow)
    	{
      		col = resrow->data;
	  		/* loop over all columns requested by user */
	  		/* leaves out those columns which are only in conditions, etc */
      		while (col) 
      		{
				s = str_replace(col->col_name,"#","&hash");
	
				ret = write(sock,s,strlen(s));
	    		if (ret == (-1)) goto error;

				xfree(s);

	    		ret = write(sock,"#",1);
    			if (ret == (-1)) goto error;

				s = str_replace(col->data_value,"#","&hash");

        		ret = write(sock,s,strlen(s));
        		if (ret == (-1)) goto error;

        		xfree(s);

        		ret = write(sock,"##",2);
        		if (ret == (-1)) goto error;

        		col = col->next;
      		}

	  		resrow = resrow->next;
    	}
   		ret = write(sock,"\n",1);
    	if (ret == (-1)) goto error;
  	}
  	else /* sending results of other queries */
  	{
		ret = write(sock,result->description,strlen(result->description));
		if (ret == (-1)) goto error;
	
		ret = write(sock,"##",2);
		if (ret == (-1)) goto error;

		s = (char*)xmalloc(16);
    	if (!s) return 0;

		ret = snprintf(s,15,"%d",result->resultret);

		ret = write(sock,s,strlen(s));
		if (ret == (-1))
		{
	  		xfree(s);
			goto error;
		}
		xfree(s);

		ret = write(sock,"##",2);
    	if (ret == (-1)) goto error;

    	ret = write(sock,"\n",1);
    	if (ret == (-1)) goto error;
  	}

  	return 1; /* OK */

error:
	set_error(ERR_SOCKET);
	return 0;
}


int sock_close(int sock)
{
	if (close(sock) == (-1)) 
	{
		set_error(ERR_SOCKET);
		return (-1);
	}

  	return 0;
}

