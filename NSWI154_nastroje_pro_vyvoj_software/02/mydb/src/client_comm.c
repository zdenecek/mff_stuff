#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "client_comm.h"
#include "memory.h"
#include "error.h"


st_sock_info* sock_create(char *address)
{
	int sock;
  	struct sockaddr_in servername;
  	struct hostent *hostinfo;
  	st_sock_info *sockinfo;

  	sockinfo = (st_sock_info*)xmalloc(sizeof(st_sock_info));
  	if (!sockinfo) return NULL;

  	/* create the socket */
  	sock = socket(PF_INET, SOCK_STREAM, 0);
  	if (sock == (-1)) goto error;

  	sockinfo->sock = sock;

  	servername.sin_family = AF_INET;
  	servername.sin_port = htons(PORT);
  	hostinfo = gethostbyname(address);
  	if (!hostinfo) goto error;

 	servername.sin_addr = *(struct in_addr*)hostinfo->h_addr;

  	sockinfo->servername = servername;

  	return sockinfo;

error:
	set_error(ERR_SOCKET);
	xfree(sockinfo);
	return NULL;
}


int sock_connect(st_sock_info *sockinfo)
{
	if (connect(sockinfo->sock,(struct sockaddr*)&(sockinfo->servername),sizeof(sockinfo->servername)) == (-1))
  	{
		set_error(ERR_SOCKET);
    	return 0;
  	}
  	return 1;
}


int write_query(int sock,char *query)
{
	int ret;

  	ret = write(sock,query,strlen(query));
  	if (ret == (-1))
	{
		set_error(ERR_SOCKET);
		return 0;
	}

  	return 1;
}


int process_result(int sock)
{
	int ret;
  	char c;
  	int is_hash=0;

  	/* read and print all input chars until EOF in this loop */
 	while (1)
  	{
    	ret = read(sock,&c,1);
    	if (ret == (-1))
    	{
	  		switch (errno)
	  		{	
				case EAGAIN:
		  			printf("EAGAIN\n");
		  			break;
				case EBADF:
          			printf("EBADF\n");
          			break;
				case EINTR:
          			printf("EINTR\n");
          			break;
				case EIO:
          			printf("EIO\n");
          			break;
				case ECONNRESET:
		  			return 1;
		  			break;
				default:
		  			printf("jina chyba %d\n",errno);
		  			break;
	  		}
      		return 0;
    	}
    	if (!ret) return 1;
	
    	if (c == '#')
		{
	  		if (!is_hash)
	  		{
				is_hash = 1;
				fputc(' ',stdout);
	  		}
	  		else fputc('\n',stdout);
		}
		else
		{
	  		is_hash = 0;
	  		fputc(c,stdout);
		}	
  	}

  	return 1;
}


int sock_close(st_sock_info *sockinfo)
{
	if (close(sockinfo->sock) == (-1)) 
  	{
		xfree(sockinfo);
		set_error(ERR_SOCKET);
		return 0;
  	}
  	xfree(sockinfo);
  	return 1;
}

