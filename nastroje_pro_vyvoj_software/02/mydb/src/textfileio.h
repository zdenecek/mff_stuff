#ifndef __TEXTFILEIO_H__
#define __TEXTFILEIO_H__

#include <stdio.h>

/* 
  type of file - each has its own structure
  each one has different suffix 
*/
typedef enum { FILE_DATA = 1, FILE_INDEX, FILE_TABDEF, FILE_TABCONF } en_file_type;

/* used for creating real file name */
typedef struct
{
    char *dbname;
    char *tabname;
    en_file_type typ;
} st_file_basename;

/* holds info about one concrete file */
typedef struct
{
    int filedes;
    char *address;
    size_t size;
    size_t alignsize;
    char *path;
    char *delpos; /* position for next delete */
    char *readpos; /* position for next read */
} st_file_info;

/* 
  create new directory for database
  1 - OK
  0 - error
*/
int create_dbdir(const char *dbname);

/* 
  remove directory for database, which is to be deleted
  1 - OK, 0 - error 
*/
int remove_dbdir(const char *dbname);

/* 
  checks the existence of database
  1 - exist, 0 - not exist, -1 - error 
*/
int exist_dbdir(const char *dbname);

/* 
  create filebasename with copies of input data
  NULL - error 
*/
st_file_basename* create_filebasename(en_file_type typ, const char *dbname, const char *tabname);

/* 
  create a filename based on input data
  it will read table properties from database conf file
  NULL - error 
*/
char* create_filename(st_file_basename *basename);

/* 
  this deletes file
  1 - OK, 0 - error 
*/
int file_delete(const char *dbname,const char *tabname, en_file_type typ);

/* 
  opens file - in future check, if it isn't locked, etc
  NULL - unsuccessful 
*/
st_file_info* file_open(const char *dbname, const char *tabname, en_file_type typ);

/* 
  closes file
  also free memory for st_file_info
  0 - error, 1 - OK 
*/
int file_close(st_file_info *fi);

/*
  checks if file is bigger then specified number of megabytes 
  1 - TRUE
  0 - FALSE 
*/
int file_bigger(st_file_info *fi, int mb);

/* counts the page-aligned memory size for mmap, etc */
size_t get_mapsize(size_t filesize);

/* 
  change the file size and remap file
  1 - OK, 0 - error 
*/
int mapfile_resize(st_file_info *fi, size_t newsize);

/* reset readpos and delpos to the beginning of file */
void file_reset(st_file_info *fi);

/* 
  writes string at the end of file
  appends '\n' to the end of file
  0 - error 
*/
int file_write_string(st_file_info *fi, const char *str);

/* 
  writes string to some position
  doesn't append anything to the end of string
  make file larger if necessary
  0 - error 
*/
int file_writepos_string(st_file_info *fi, const char *str, int pos);

/* 
  reads string starting at readpos and ending with '\n' and returns it
  NULL - error 
*/
char* file_read_string(st_file_info *fi);

/* 
  deletes string starting at delpos and ending with '\n' and returns it
  NULL - error 
*/
char* file_delete_string(st_file_info *fi);

/* 
  mark string - fill it up to '\n' with mark character
  mark can be for example #
  returns original string
  NULL - error 
*/
char* file_mark_string(st_file_info *fi, char mark);

/* 
  marks string as deleted and returns it
  NULL - error
*/
char* file_str_markdel(st_file_info *fi);

/* 
  removes all strings marked as deleted - shrinks file
  should be called only sometimes(after some number of deleted strings)
  0 - error 
*/
int remove_deleted_strings(st_file_info *fi);

/* free all memory used by file_basename */
void destroy_filebasename(st_file_basename *fb);

/* free all memory used by file_info */
void destroy_fileinfo(st_file_info *fi);

#endif
