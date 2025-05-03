#ifndef __BTREE_INT_H__
#define __BTREE_INT_H__

#include "conds.h"
#include "data.h"
#include "textfileio.h"
#include "linklist.h"

#define BTREE_DEGREE 11
#define BTREE_MAX_KEYS 21
#define BTREE_MIN_KEYS 10

#define BLOCK_SIZE 1024

/* 
stores one item of btree
see typical structure of b-tree
filepos == 0 in items, which are not in leafs
left_child and right_child both == 0 in leaf 
*/
typedef struct
{
	int key; /* index key */
	int filepos; /* offset of row with this key in data file */
	/* number of block with left child in index file */
	int left_child;	/* -1 - no block */
	/* number of block with right child in index file */
	int right_child; /* -1 - no block */
} st_item;

/* left_nghbr and right_nghbr both == 0 in node, which isn't leaf */
typedef struct
{
	st_item* items[BTREE_MAX_KEYS]; /* some of them will be NULL */
	int block; /* number of block for this node */
	int item_count; /* actual count of items */
	/* no. of block with left neighbour on the same level */
	int left_nghbr;	/* -1 - no neighbour */
	/* no. of block with  right neighbour on the same level */
	int right_nghbr; /* -1 - no neighbour */
	int is_parent; /* 1 - TRUE, 0 - FALSE */
} st_node;

/* element of stack used for storing path in tree(visited nodes) */
typedef struct stblkno
{
	int block;
	struct stblkno *NEXT;
} st_block_no;


/* 
  add new element of path to the top of stack
  returns new head
  NULL - error 
*/
st_block_no* add_blockno(st_block_no *head, int block);

/* 
  returns block number from the top element
  deletes top element
  -1 - error 
*/
int get_blockno(st_block_no **head);

/* destroy all elements of stack */
void destroy_blockno(st_block_no *head);

/* 
  reads btree node from specific block
  NULL - error 
*/
st_node* btree_read_node(st_file_info *fi, int block);

/* 
  writes node to its block
  0 - error 
*/
int btree_write_node(st_file_info *fi, const st_node *node);

/* 
  deletes node from file at specific block
  0 - error 
*/
int btree_delete_node(st_file_info *fi, int block);

/* 
  create new node with this block number and initialize it
  block must exist before this function call(file must have proper size)
  NULL - error 
*/
st_node* btree_create_node(int block);

/* 
  search through whole file for a place for new node
  returns block number
  such place remained there after delete of node
  0 - no free block for node
  -1 - error 
*/
int find_freenode(st_file_info *fi);

/* 
  converts string to node
  NULL - error 
*/
st_node* string_node(const char *str);

/* 
  converts node to string
  NULL - error 
*/
char* node_string(const st_node *node);

/* 
  split full node into two smaller nodes
  nodes newnode1 and newnode2 must be created before function call
  returns item which must be inserted into parent of new two nodes
  	- former orig_node
  orig_node is destroyed 
  NULL - error 
*/
st_item* split_node(st_node *orig_node, st_node *newnode1, st_node *newnode2);

/* 
  join two empty leaf nodes into one
  node1 is used as base of new node and node2 can be destroyed
  link to node2 in parent node must be corrected
  NULL - error 
*/
st_node* join_nodes(st_node *node1, const st_node *node2);

/* 
  checks if node is full
  1 - full
  0 - not full 
*/
int is_full(const st_node *node);

/* 
  checks if node is empty(too few items)
  1 - empty
  0 - not empty 
*/
int is_empty(const st_node *node);

/* 
  adds new item to leaf node to correct pos in array of items
  0 - cannot add item to node(too many items, is not leaf, etc)
  -1 - error 
*/
int add_item(st_node *node, int key, int filepos);

/* 
  delete item with this key and filepos from leaf node
  nothing is done if such combination is not found
  0 - cannot delete item from node(node is not leaf, has too few items,etc) 
*/
int delete_item(st_node *node, int itempos);

/* 
  checks if node is leaf or not
  1 - TRUE
  0 - FALSE 
*/
int is_leaf(const st_node *node);

/* 
  returns offset of child which should hold item with this key
  node cannot be leaf(reports error in this case)
  -1 - error 
*/
int get_childblock(const st_node *node, int key);

/* 
  returns position of item in node for this key
  direction == 0 - return first item with given key
  direction == 1 - start search from first item and return first item with 
   key equal or bigger than given key (useful for lower bound of range, 
   because it's possible to move right and to neighbours to get rest of range 
  direction == 2 - start search from last item and return last item with 
   key equal or smaller than given key(useful for higher bound of range, 
   because it's possible to move left and to neighbours to get rest of range 
  node must be leaf(error otherwise) 
  -1 - error(such item not found) 
*/
int get_itempos(const st_node *node, int key, int direction);

/* 
  returns position in file for this itempos
  node must be leaf
  -1 - error 
*/
int get_filepos(const st_node *node, int itempos);

/* 
  get offset(block number) of neighbour at the same level
  direction == 1 - left, direction == 2 - right
  0 - no neighbour 
*/
int get_neighbour(const st_node *node, int direct);

/* 
  moves item to neighbour node
  both nodes must be leafs and target cannot be full
  direct == 1 - move last item to be first item in target neighbour
  direct == 2 - move first item to be last item in target neighbour
  0 - error(there is no neighbour, neighbour is full, etc 
*/
int move_item(st_node *node, st_node *target, int direct);

/* 
  moves over all items with equal keys also in neighbours
  direct == 1 - go to left, direct == 2 - go to right
  position in returned node will be in pos
  NULL - error 
*/
st_node* move_overequal(st_file_info *fi, st_node *node, int key, int direct, int *pos);

/* destroy all memory used by node */
void destroy_node(st_node *node);

/* 
  finds a node according to the key
  also returns path in the tree in **head
  NULL - error 
*/
st_node* find_leafnode(st_file_info *fi,int key,st_block_no **head);

/* 
  returns node of neighbour with free place for one item
  direct contain direction for moving after return from function
  NULL - no free neighbour or error 
*/
st_node* get_free_nghbr(st_file_info *fi,const st_node *node, int *direct);
                                               
#endif
