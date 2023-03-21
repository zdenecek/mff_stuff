#include "error.h"
#include "memory.h"
#include "linklist.h"
#include "btree_int.h"
#include "strutil.h"

#define TOKEN_INT(C,E,T)	s2 = str_copy(s,C); \
				        	if (!s2) goto E; \
					        c = strtol(s2,NULL,10); \
					        T = c; \
					        s+=strlen(s2)+1; \

#define INT_TOKEN(T,C)	snprintf(s,11,"%d",T); \
					    size = strlen(s); \
					    for (i=0;i<size;i++) str[pos++] = s[i]; \
					    str[pos++] = C;


st_block_no* add_blockno(st_block_no *head, int block)
{
    st_block_no *cur;

    cur = (st_block_no*)xmalloc(sizeof(st_block_no));
    if (!cur) return NULL;

    cur->block = block;

	LIST_ADDHEAD(head,cur);

    return head;
}


int get_blockno(st_block_no **head)
{
    st_block_no *cur;
    int block;
    st_block_no *tmp_head;

    tmp_head = *head;

	if (!tmp_head) 
	{
		set_error(ERR_STRANGE);
		return (-1); /* error */
	}

    cur = tmp_head;
    tmp_head = tmp_head->NEXT;
    block = cur->block;
    xfree(cur);

    (*head) = tmp_head;
    return block;
}


void destroy_blockno(st_block_no *head)
{
    st_block_no *cur;

    while (head)
    {
        cur = head;
        head = head->NEXT;
        xfree(cur);
    }
}


st_node* btree_read_node(st_file_info *fi, int block)
{
	char *str;
	st_node *node;

	fi->readpos = fi->address + (BLOCK_SIZE)*block;

	str = file_read_string(fi);
	if (!str) return NULL;

	node = string_node(str);
	if (!node)
	{
		xfree(str);
		return NULL;
	}	

	node->block = block;

	return node;
}


int btree_write_node(st_file_info *fi, const st_node *node)
{
	int ret;
	char *str;

	str = node_string(node);
	if (!str) return 0;

	ret = file_writepos_string(fi,str,node->block*BLOCK_SIZE);
	if (!ret)
	{
		xfree(str);
		return 0;
	}

	return 1;
}


int btree_delete_node(st_file_info *fi, int block)
{
	char *s;
	int ret;

	s = (char*)xmalloc(sizeof(char)*(BLOCK_SIZE+1));
	if (!s) return 0;

	s = memset(s,' ',1023);

	s[1023] = '\n';
	s[0] = 'f';
	s[1] = ':';
	s[1024] = 0;

	ret = file_writepos_string(fi,s,block*BLOCK_SIZE);
	if (!ret) return 0;

	return 1;
}


st_node* btree_create_node(int block)
{
	st_node *node;
	int i;

	node = (st_node*)xmalloc(sizeof(st_node));
	if (!node) return NULL;

	node->block = block;

	node->item_count = 0;
	node->left_nghbr = -1;
	node->right_nghbr = -1;

	for (i=0;i<BTREE_MAX_KEYS;i++) node->items[i] = NULL;

	return node;	
}


int find_freenode(st_file_info *fi)
{
	char *str;
	int pos;

	file_reset(fi);
	pos = 0;

	while ((str = file_read_string(fi)))
	{
		if (*str == 'f')
		{
			xfree(str);
			return pos;
		}

		pos++;
	}

	return 0;
}


st_node* string_node(const char *str)
{
	st_node *node;
	st_item *item;
	const char *s;
	char *s2;
	int c;
	int i;

	s = str;	

	node = (st_node*)xmalloc(sizeof(st_node));
	if (!node) return NULL;

	for (i=0;i<BTREE_MAX_KEYS;i++) node->items[i] = NULL;

	if (*s == 'f')
	{
		set_error(ERR_FATIO);
		return NULL; /* free node(block) */
	}
	else if (*s == 'n') node->is_parent = 1;
	else if (*s == 'l') node->is_parent = 0;			
	else 
	{
		set_error(ERR_STRANGE);
		return NULL;
	}

	s+=2;

	TOKEN_INT(':',error1,node->item_count)
	TOKEN_INT(':',error1,node->left_nghbr)
	TOKEN_INT(':',error1,node->right_nghbr)
	
	for (i=0;i<node->item_count;i++)
	{
		item = (st_item*)xmalloc(sizeof(st_item));
		if (!item) goto error1;
	
		TOKEN_INT(';',error2,item->key)
		TOKEN_INT(';',error2,item->filepos)
		TOKEN_INT(';',error2,item->left_child)
		TOKEN_INT(':',error2,item->right_child)

		node->items[i] = item;
	}

	return node;

error2:
	xfree(item);			
error1:
	destroy_node(node);
	return NULL;
}


char* node_string(const st_node *node)
{
	char *str;
	int pos;
	char s[11];
	int i,j;
	size_t size;

	pos = 0;

	str = (char*)xmalloc(sizeof(char)*(BLOCK_SIZE+1));
	if (!str) return NULL;

	if (is_leaf(node)) str[pos++] = 'l';
	else str[pos++] = 'n';
	str[pos++] = ':';

	INT_TOKEN(node->item_count,':')
	INT_TOKEN(node->left_nghbr,':')
	INT_TOKEN(node->right_nghbr,':')

	/* loop over all items */
	for (j=0;j<node->item_count;j++)
	{
		INT_TOKEN(node->items[j]->key,';')
		INT_TOKEN(node->items[j]->filepos,';')
		INT_TOKEN(node->items[j]->left_child,';')
		INT_TOKEN(node->items[j]->right_child,':')
	}

	for (i=pos;i<(BLOCK_SIZE-1);i++) str[i] = ' ';

	str[BLOCK_SIZE-1] = '\n';
	str[BLOCK_SIZE] = 0;

	return str;
}


st_item* split_node(st_node *orig_node, st_node *newnode1, st_node *newnode2)
{
	int i;
	st_item *item;

	if (!is_full(orig_node)) 
	{
		set_error(ERR_BTREE);
		return NULL;
	}

	newnode1->item_count = BTREE_MIN_KEYS;
	for (i=0;i<BTREE_MIN_KEYS;i++) newnode1->items[i] = orig_node->items[i];

	/* create copy of item */
    item = (st_item*)xmalloc(sizeof(st_item));
    if (!item) return NULL;

    item->key = orig_node->items[BTREE_MIN_KEYS]->key;
    item->filepos = 0;
    item->left_child = newnode1->block;
    item->right_child = newnode2->block;

	/* we must preserve item in one of new leaf nodes */
	if (is_leaf(orig_node))
	{
		newnode1->item_count++;
		newnode1->items[BTREE_MIN_KEYS] = orig_node->items[BTREE_MIN_KEYS];
	}
	else xfree(orig_node->items[BTREE_MIN_KEYS]);
	
    newnode2->item_count = BTREE_MIN_KEYS;
    for (i=0;i<BTREE_MIN_KEYS;i++) newnode2->items[i] = orig_node->items[BTREE_MIN_KEYS+1+i];

	newnode1->left_nghbr = orig_node->left_nghbr;
	newnode1->right_nghbr = newnode2->block;

	newnode2->right_nghbr = orig_node->right_nghbr;
    newnode2->left_nghbr = newnode1->block;

	xfree(orig_node);

	return item;
}


st_node* join_nodes(st_node *node1, const st_node *node2)
{
	int pos;
	int i;

	if ((!is_leaf(node1)) && (!is_leaf(node2))) 
	{
		set_error(ERR_BTREE);
		return NULL;
	}

	if ((!is_empty(node1)) && (!is_empty(node2))) 
	{
		set_error(ERR_BTREE);
		return NULL;
	}

	pos = node1->item_count;
	
	for (i=0;i<node2->item_count;i++) node1->items[pos+i] = node2->items[i];

	node1->item_count += node2->item_count;
	node1->right_nghbr = node2->right_nghbr;

	return node1;
}


int is_full(const st_node *node)
{
	if (node->item_count == BTREE_MAX_KEYS) return 1;
	else return 0;
}


int is_empty(const st_node *node)
{
	if (node->item_count <= BTREE_MIN_KEYS) return 1;
    else return 0;
}


int add_item(st_node *node, int key, int filepos)
{
	st_item *item;
	int i,pos;

    if (!is_leaf(node)) 
	{
		set_error(ERR_BTREE);
		return 0;
	}
	if (is_full(node))
	{
		set_error(ERR_BTREE);
		return 0;
	}

	item = (st_item*)xmalloc(sizeof(st_item));
	if (!item) return (-1);

	item->key = key;
	item->filepos = filepos;
	item->left_child = -1;
	item->right_child = -1;

	/* search for position to put item at */
	i = 0;
	while ((i < node->item_count) && (key > node->items[i]->key)) i++;

	pos = i;

	/* move all items behind position one place */
	for (i=node->item_count-1;i>=pos;i--) node->items[i+1] = node->items[i];
	
	node->items[pos] = item;
	node->item_count++;	

	return 1;
}


int delete_item(st_node *node, int itempos)
{
	st_item *item;
    int i;

    if (!is_leaf(node)) 
	{
		set_error(ERR_BTREE);
		return 0;
	}
	if (itempos >= node->item_count) 
	{
		set_error(ERR_BTREE);
		return 0;
	}

	/* leaf node can have less than BTREE_MIN_KEYS of items */
    /* if (is_empty(node)) return 0; */

	item = node->items[itempos];	

	for (i=itempos;i<(node->item_count - 1);i++)
	{
		node->items[i] = node->items[i+1];
	}
	
	node->items[node->item_count - 1] = NULL;
	node->item_count--;

	xfree(item);

	return 1;
}


int is_leaf(const st_node *node)
{
	return (1 - node->is_parent);
}


int get_childblock(const st_node *node, int key)
{
	int i;
	
	if (is_leaf(node)) 
	{
		set_error(ERR_BTREE);
		return (-1);
	}

	i = 0;
	/* search from left side */
	/* see typical structure of b-tree */
	while (i < node->item_count)
	{
		/* if left_child == -1 then it returns -1 -> error */
		if (node->items[i]->key > key) return node->items[i]->left_child;
		i++;
	}

    /* if right_child == -1 then it returns -1 -> error */
	return node->items[node->item_count-1]->right_child;
}


int get_itempos(const st_node *node, int key, int direction)
{
	int i;

	if (!is_leaf(node)) 
	{
		set_error(ERR_BTREE);
		return (-1);
	}

	if (direction == 1)
	{
		i = 0;
		while (i < node->item_count)
		{
			if (node->items[i]->key >= key) return i;
			i++;
		}
		
		set_error(ERR_BTREE);
		return (-1); /* given key is too big */

	}
	else if (direction == 2)
	{
		i = node->item_count - 1;
        while (i >= 0)
        {
            if (node->items[i]->key <= key) return i;
            i--;
        } 

		set_error(ERR_BTREE);
        return (-1); /* given key is too small */
	}
	else /* direction should be 0 */
	{
        i = 0;
        while (i < node->item_count)
        {
            if (node->items[i]->key == key) return i;
            i++;
        } 

		set_error(ERR_BTREE);
        return (-1); /* given key not found */
	}	
}


int get_filepos(const st_node *node, int itempos)
{
	if (!is_leaf(node)) 
	{
		set_error(ERR_BTREE);
		return (-1);
	}

	if (itempos > (node->item_count - 1)) 
	{
		set_error(ERR_BTREE);
		return (-1);
	}

	return node->items[itempos]->filepos;
}


int get_neighbour(const st_node *node, int direct)
{
	if ((direct == 1) && (node->left_nghbr != (-1))) return node->left_nghbr;
	if ((direct == 2) && (node->right_nghbr != (-1))) return node->right_nghbr;

	return 0;
}


int move_item(st_node *node, st_node *target, int direct)
{
	int i;

	if ((!is_leaf(node)) || (!is_leaf(target))) 
	{
		set_error(ERR_BTREE);
		return 0;
	}

	if (is_full(target)) 
	{
		set_error(ERR_BTREE);
		return 0;
	}
	
	if (direct == 1)
	{
		for (i=target->item_count-1;i>=0;i--) target->items[i+1] = target->items[i];
		target->item_count++;
		target->items[0] = node->items[node->item_count-1];
		node->item_count--;

		return 1;
	}
    if (direct == 2)
    {
		target->items[target->item_count] = node->items[0];
		target->item_count++;

		for (i=0;i<node->item_count-1;i++) node->items[i] = node->items[i+1];
        node->item_count--;

        return 1;
    }

	set_error(ERR_BTREE);
	return 0;
}


st_node* move_overequal(st_file_info *fi, st_node *node, int key, int direct, int *pos)
{
	int i;
	st_node *node2;
	int block;

	node2 = node;

	if (direct == 1)
	{
		i = *pos;

		while ((i >=0 ) && (node2->items[i]->key == key))
		{
			i--;

			if (i == (-1))
			{
				if (node2->left_nghbr == (-1))
				{
					*pos = (-1);
					return node2;
				}
				
				block = node2->left_nghbr;
				if (node != node2) destroy_node(node2);

				node2 = btree_read_node(fi,block);
				if (!node2) return NULL;

				i = node2->item_count - 1;
			}
		}
	}
	else /* direct == 2 */
	{
        i = *pos;

        while ((i < node2->item_count) && (node2->items[i]->key == key))
        {
            i++;

            if (i == node2->item_count)
            {
                if (node2->right_nghbr == (-1))
                {
                    *pos = (-1);
                    return node2;
                }

                block = node2->right_nghbr;
                if (node != node2) destroy_node(node2);

                node2 = btree_read_node(fi,block);
                if (!node2) return NULL;

                i = 0;
            }   
        }

	}

	*pos = i;

	return node2;
}


void destroy_node(st_node *node)
{
	int i;

	if (node)
	{
		for (i=0;i<node->item_count;i++) 
		{
			if (node->items[i]) xfree(node->items[i]);
		}
		xfree(node);
	}
}


st_node* find_leafnode(st_file_info *fi,int key,st_block_no **head)
{
	st_node *node;
	st_block_no *tmp_head = NULL;
	int block = 0;
	int new_block;

	if (fi->size == 0)
	{
		node = btree_create_node(block);
		if (!node) return NULL;

		return node;
	}

	node = btree_read_node(fi,block);
    if (!node) return NULL;

    while (!is_leaf(node))
    {
        tmp_head = add_blockno(tmp_head,block);
        new_block = get_childblock(node,key);

        destroy_node(node);

        node = btree_read_node(fi,new_block);
        if (!node) return NULL;
    }

	(*head) = tmp_head;

	return node;
}


st_node* get_free_nghbr(st_file_info *fi,const st_node *node, int *direct)
{
	int block;
	st_node* node2;
	
	if (node->right_nghbr != (-1))
	{
		block = node->right_nghbr;

		*direct = 1;

        node2 = btree_read_node(fi,block);
        if (!node2) return NULL;

		if (!is_full(node2)) return node2;
	}
	else if (node->left_nghbr != (-1))
    {
        block = node->left_nghbr;

		*direct = 2;

        node2 = btree_read_node(fi,block);
        if (!node2) return NULL;

        if (!is_full(node2)) return node2;
    }
	else return NULL;

	return NULL;
}
