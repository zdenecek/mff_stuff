#ifndef __CLIENT_COMM_H__
#define __CLIENT_COMM_H__

#include <netinet/in.h>

#define PORT 5000
#define INPUT_SIZE 1024

typedef struct
{
	int sock;
  	struct sockaddr_in servername;	
} st_sock_info;

/* 
  create socket(set address,atd)
  return new socket
  NULL - error 
*/
st_sock_info* sock_create(char *address);

/* 
  opens connection
  0 - error 
*/
int sock_connect(st_sock_info *sockinfo);

/* 
  send a query to server
  0 - error 
*/
int write_query(int sock,char *query);

/* 
  read and print result
  0 - error 
*/
int process_result(int sock);

/* 
  closes connection
  0 - error 
*/
int sock_close(st_sock_info *sockinfo);

#endif
