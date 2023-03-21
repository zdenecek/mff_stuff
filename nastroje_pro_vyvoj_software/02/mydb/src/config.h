#ifndef __CONFIG_H__
#define __CONFIG_H__

/* these are runtime config and status variables */
/* some of them have to be read from config file at the startup */
/* their values determine behavior of database engine and they save actual status information, selected database, etc */

/* path to directory where are database files and directories stored */
extern char *datadir;

/* currently used database */
extern char *current_db;

/* 
  reads configuration - fills datadir, etc
  1 - OK
  0 - error
*/
int read_config(void);

/* destroy all config variables */
void destroy_config(void);

#endif
