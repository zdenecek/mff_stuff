#ifndef __INDEX_H__
#define __INDEX_H__

#include "conds.h"
#include "data.h"
#include "textfileio.h"
#include "btree_int.h"

/* stores index value and file position of corresponding row */
typedef struct stidxrow
{
	char *value;
	int filepos;
	struct stidxrow *next;
} st_index_row;


/* 
  creates b-tree index for this table and column
  also must change tabconf 
  0 - error 
*/
int create_index(const char *tabname, const char *colname);

/* 
  deletes b-tree index for this table and column
  also must change tabconf
  0 - error 
*/
int delete_index(const char *tabname, const char *colname);

/* 
  search index for columns which satisfy condition and return them
  value in condition must be of type INT(will be converted by atoi)
  NULL - error 
*/
st_index_row* get_indexrows(st_file_info *fi, const st_cond *cond);

/*
  insert into index new record which correspond to value and filepos
  0 - error 
*/
int index_insertrow(st_file_info *fi, const char *value, long filepos);

/* 
  delete from index row with this value and filepos
  returns the key value for this filepos
  NULL - error 
*/
char* index_deleterow(st_file_info *fi, const char *value, long filepos);

/* 
  split node and insert item with key and filepos into one of new nodes 
   or add node to blockno linked list
  filepos == -1 -> don't insert item but add to blockno
  return item which must be inserted into parent or NULL if not necessary
  0 - error 
*/
int index_splitnode(st_file_info *fi, st_node *node, int key, int filepos, st_block_no **head, st_item **retitem);

/* 
  returns index row with key equal to given key
  moves from this position to the right
  NULL - error 
*/
st_index_row* index_getequal(st_file_info *fi, st_node *node, int key, int itempos);

/* 
  returns all index row with higher keys including this one
  moves from this position to all neighbours "on the right side"
  return NULL 
*/
st_index_row* index_gethigher(st_file_info *fi, st_node *node, int itempos);

/* 
  returns all index row with lower keys including this one
  moves from this position to all neighbours "on the left side"
  return NULL 
*/
st_index_row* index_getlower(st_file_info *fi, st_node *node,int itempos);

/* 
  free all memory used by index rows
  all == 1 - destroy whole linked list 
*/
void destroy_indexrow(st_index_row *head, int all);

#endif
