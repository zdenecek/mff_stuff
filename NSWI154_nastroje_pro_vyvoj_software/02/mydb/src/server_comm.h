#ifndef __SERVER_COMM_H__
#define __SERVER_COMM_H__

#include "defs.h"
#include "result.h"

#define PORT 5000
#define INPUT_SIZE 1024

/* 
  creates socket
  returns socket (file descriptor)
  -1 - error 
*/
int sock_create();

/* 
  starts listening
  1 - OK
  0 - error 
*/
int start_listen(int sock);

/* 
  wait for some request in accept
  return new socket(file descriptor)
  -1 - error 
*/
int wait_for_request(int sock);

/* 
  read request from client
  NULL - error 
*/
char* read_request(int sock);

/* 
  send whole result to client
  converts result to string and then sends it
  0 - error 
*/
int send_result(st_result *result,int sock);

/* 
  closes connection
  -1 - error 
*/
int sock_close(int sock);

#endif
