#include <string.h>
#include "memory.h"
#include "error.h"
#include "strutil.h"
#include "index.h"
#include "tabconf.h"
#include "config.h"


int create_index(const char *tabname, const char *colname)
{
	int ret;
	st_file_info *fi,*fi2;
	char *s;
	st_row_data *row;
	char *value;
	long filepos;
	st_table_conf *tabconf;

	ret = column_indexed(tabname, colname);
	if (ret == (-1)) 
	{	
		return 0;
	}
	if (ret) 
	{
		set_error(ERR_IDXEXIST);
		return 0; /* index already exist */
	}

	if (!current_db) 
	{
		set_error(ERR_NODBSEL);
		return 0;
	}

	/* add colname to tabname for the purpose of file name */
    s = str_concat(tabname,colname,'_');
	if (!s) return 0;

	fi = file_open(current_db,s,FILE_INDEX);
	if (!fi) goto error1; 

	fi2 = file_open(current_db,tabname,FILE_DATA);
	if (!fi2)
	{
		if (!file_close(fi)) destroy_fileinfo(fi);
		goto error1;
	}

	filepos = fi2->readpos - fi2->address;
	row = read_rowdata(fi2,NULL);

	while (row)
	{
		value = get_colvalue(row,colname);
		if (!value) goto error3;

		ret = index_insertrow(fi,value,filepos);
		if (!ret) goto error3;
		
	    filepos = fi2->readpos - fi2->address;
    	row = read_rowdata(fi2,NULL);
	}

	if (!file_close(fi)) destroy_fileinfo(fi);
    if (!file_close(fi2)) destroy_fileinfo(fi2);

	xfree(s);

	/* add this column to the list of indexed columns for table */
	tabconf = delete_tableconf(tabname);
	if (!tabconf) return 0;

	ret = add_indexcol(tabconf,colname);
	if (!ret) goto error4;

	ret = write_tableconf(tabconf);
	if (!ret) goto error4;

	return 1;

error3:
	destroy_rowdata(row,0);
	if (!file_close(fi)) destroy_fileinfo(fi);
	if (!file_close(fi2)) destroy_fileinfo(fi2);
error1:
	xfree(s);
	return 0;
error4:
	destroy_tabconf(tabconf);
	return 0;
}


int delete_index(const char *tabname, const char *colname)
{
	int ret;
	char *s;
	st_table_conf *tabconf;

    ret = column_indexed(tabname, colname);
    if (ret == (-1)) return 0;
    if (!ret) 
	{
		set_error(ERR_IDXNOEX);
		return 0; /* index doesn't exist */
	}

    if (!current_db)
	{
		set_error(ERR_NODBSEL);
		return 0;
	}

	/* add colname to tabname for the purpose of file name */
	s = str_concat(tabname,colname,'_');
    if (!s) return 0;
    
    ret = file_delete(current_db,s,FILE_INDEX);
    if (!ret)
	{
		xfree(s);
		return 0;
	}

	xfree(s);

    /* delete this column from the list of indexed columns for table */
    tabconf = delete_tableconf(tabname);
    if (!tabconf) return 0;

    ret = del_indexcol(tabconf,colname);
    if (!ret) goto error1;

    ret = write_tableconf(tabconf);
    if (!ret) goto error1;

	return 1;

error1:
	destroy_tabconf(tabconf);
	return 0;
}


st_index_row* get_indexrows(st_file_info *fi, const st_cond *cond)
{
	st_node *node,*newnode;
	st_block_no *head,*cur;
	int block;
	int ret;
	int key;
	int itempos;
	int direct;
	st_index_row *idxrow,*idxrow2,*tmp_idx;	

	block = 0;
	head = NULL;
	cur = NULL;

	if (!fi->address) 
	{
		set_error(ERR_FATIO);
		return NULL;
	}

    ret = get_condvalue(cond,&key);
    if (!ret) return NULL;

	node = find_leafnode(fi,key,&head);
	if (!node) return NULL;

	/* node is leaf now */

	if ((cond->oper == EQUAL) || (cond->oper == NOTEQUAL)) direct = 0;
	else if ((cond->oper == LESS) || (cond->oper == LESSEQUAL)) direct = 2;
	else direct = 1; /* cond->oper == MORE || cond->oper == MOREEQUAL */
	
	itempos = get_itempos(node,key,direct);

	switch (cond->oper)
	{
		case EQUAL:
			idxrow = index_getequal(fi,node,key,itempos);
			if (!idxrow) goto error1;
			break;			
		case NOTEQUAL:
			idxrow = index_getlower(fi,node,itempos-1);
			if (!idxrow) goto error1;

			/* skip equal items */
			newnode = move_overequal(fi,node,key,2,&itempos);
			if (!newnode) goto error1;

			if (node != newnode) destroy_node(node);
			node = newnode;
			
			idxrow2 = index_gethigher(fi,node,itempos);
			if (!idxrow2) goto error1;

			/* join two linked lists */
			tmp_idx = idxrow;
			while (tmp_idx->NEXT) tmp_idx = tmp_idx->NEXT;
			tmp_idx->NEXT = idxrow2;
	
			break;	
		case LESS:
            /* skip equal items */
            newnode = move_overequal(fi,node,key,1,&itempos);
            if (!newnode) goto error1;

            if (node != newnode) destroy_node(node);
            node = newnode;
			/* no break */
		case LESSEQUAL:
		    idxrow = index_getlower(fi,node,itempos);
            if (!idxrow) goto error1;
            break;
		case MORE:
            /* skip equal items */
            newnode = move_overequal(fi,node,key,2,&itempos);
            if (!newnode) goto error1;

            if (node != newnode) destroy_node(node);
            node = newnode;
			/* no break */
		case MOREEQUAL:
			idxrow = index_gethigher(fi,node,itempos);
			if (!idxrow) goto error1;
			break;
		default:
			goto error1;
			break;
	}
 
	destroy_node(node);
	destroy_blockno(head);

	return idxrow;
	
error1:
	destroy_node(node);
	destroy_blockno(head);
	return NULL;
}


int index_insertrow(st_file_info *fi, const char *value, long filepos)
{
	int ret;
	int key;
	st_node *node,*node2;
	int block;
    st_block_no *head,*cur;
	int parent_blk;
	st_item *item,*item2;
	int i,j;
	int direct;

	block = 0;
    head = NULL;
    cur = NULL;

    if (!fi->address) 
	{
		set_error(ERR_FATIO);
		return 0;
	}

	key = strtol(value,NULL,10);

	node = find_leafnode(fi,key,&head);
    if (!node) return 0;

    /* node is leaf now */

	if (!is_full(node))
	{
		ret = add_item(node,key,filepos);
		if (ret == (-1)) goto error1;
		if (!ret) goto error1; /* cannot add item */

		ret = btree_write_node(fi,node);
        if (!ret) goto error1;
	}
	else /* full node */
	{
		direct = 0;

		node2 = get_free_nghbr(fi,node,&direct);
		if (node2) /* we have neighbour with at least one place for item */
		{
			ret = move_item(node,node2,direct);
			if (!ret) goto error2;

			ret = add_item(node,key,filepos);
	        if (ret == (-1)) goto error2;
       		if (!ret) goto error2; /* cannot add item */

			ret = btree_write_node(fi,node);
			if (!ret) goto error2;
		
			ret = btree_write_node(fi,node2);
			if (!ret) goto error2;

			destroy_node(node2);
			destroy_node(node);
			destroy_blockno(head);

			return 1;
		}	

		/* try split */

		ret = index_splitnode(fi,node,key,filepos,NULL,&item);
		if (!ret) goto error1;
	
		/* put former middle item somewhere */
		item2 = NULL;
		while (item) /* while item hasn't been put somewhere */
		{
			parent_blk = get_blockno(&head);
			if (parent_blk == (-1)) return 0;

			node = btree_read_node(fi,parent_blk);
			if (!node) return 0;
	
			if (is_full(node))
			{
				ret = index_splitnode(fi,node,0,-1,&head,&item2);
				if (!ret) goto error1;
			}
	
			if (!is_full(node))
			{
				i = 0;
				/* find the position for new item */
				while ((i < node->item_count) && (node->items[i]->key < item->key)) i++;
				if (i == node->item_count)
				{
					node->items[i] = item;
					node->items[i-1]->right_child = item->left_child;
					node->item_count++;
				}	
				else if (i == 0)
				{
					for (j=node->item_count-1;j>=0;j--) node->items[j+1] = node->items[j];
					node->items[0] = item;
					node->items[1]->left_child = item->right_child;
					node->item_count++;
				}
				else
				{
					for (j=node->item_count-1;j>=i;j--) node->items[j+1] = node->items[j];
                    node->items[i] = item;
                    node->items[i+1]->left_child = item->right_child;
					node->items[i-1]->right_child = item->left_child;
					node->item_count++;
				}
		
				xfree(item);
				item = NULL; /* item has been put somewhere */

			    ret = btree_write_node(fi,node);
				if (!ret) goto error1;

				if (item2) item = item2;
			}
			destroy_node(node);
		}

		destroy_blockno(head);
		return 1;			
	}

	destroy_blockno(head);
	return 1;

error2:
	destroy_node(node2);
error1:
	destroy_node(node);
	destroy_blockno(head);
    return 0;
}


char* index_deleterow(st_file_info *fi, const char *value, long filepos)
{
	int ret;
    int key;
    st_node *node,*node2;
    int block; 
    st_block_no *head,*cur;
    int parent_blk;
    int i;
	char *s;

    block = 0;
    head = NULL;
    cur = NULL;

    if (!fi->address) 
	{
		set_error(ERR_FATIO);
		return NULL;
	}

    key = strtol(value,NULL,10);

	node = find_leafnode(fi,key,&head);
    if (!node) return 0;

    /* node is leaf now */

	if (node->item_count == 1)
	{
		block = node->block;

		s = (char*)xmalloc(sizeof(char)*11);
		if (!s) return NULL;

		snprintf(s,11,"%d",node->items[0]->key);

		/* remove links from neighbours */
		if (node->left_nghbr != (-1))
		{
			node2 = btree_read_node(fi,node->left_nghbr);
			if (!node2) goto error1;
		}
		node2->right_nghbr = -1;
        ret = btree_write_node(fi,node2);
        if (!ret) goto error1;
		destroy_node(node2);

		if (node->right_nghbr != (-1))
        {
            node2 = btree_read_node(fi,node->right_nghbr);
            if (!node2) goto error1;
        }
        node2->left_nghbr = -1;
        ret = btree_write_node(fi,node2);
        if (!ret) goto error1;
		destroy_node(node2);

		ret = btree_delete_node(fi,node->block);
		if (!ret) goto error1;

		destroy_node(node);

        /* remove links from parent */

		parent_blk = get_blockno(&head);
		node = btree_read_node(fi,parent_blk);

		i = 0;
		while (i < node->item_count)
		{
			if (node->items[i]->left_child == block) node->items[i]->left_child = -1;
			if (node->items[i]->right_child == block) node->items[i]->right_child = -1;
			i++;
		}

        ret = btree_write_node(fi,node);
        if (!ret) goto error1;

		destroy_node(node);
		destroy_blockno(head);

		return s;
	}
	else
	{
		i=0;
		while ((i < node->item_count) && (node->items[i]->key != key) && (node->items[i]->filepos != filepos)) i++;
		
		if (i == node->item_count) goto error1;

        s = (char*)xmalloc(sizeof(char)*11);
        if (!s) return NULL;

        snprintf(s,11,"%d",node->items[i]->key);
	
		ret = delete_item(node,i);
		if (!ret) goto error1;

		ret = btree_write_node(fi,node);
        if (!ret) goto error1;
	
		destroy_node(node);
		destroy_blockno(head);

		return s;		
	}

error1:
	destroy_node(node);
	return NULL;
}


int index_splitnode(st_file_info *fi, st_node *node, int key, int filepos, st_block_no **head,st_item **retitem)
{
    st_node *node2,*node3;
    int new_block,new_block2;
    st_item *item;
	int ret;
	int orig_block;

	orig_block = node->block;

	if (node->block == 0)
	{
    	new_block = find_freenode(fi);
    	if (!new_block)	new_block = (int)(fi->size / BLOCK_SIZE);

	    new_block2 = new_block+1;
	}	
	else
	{
		new_block = node->block;

		new_block2 = find_freenode(fi);
        if (!new_block2) new_block2 = (int)(fi->size / BLOCK_SIZE);
	}

    /* i will use block which will be free after original node */
    node2 = btree_create_node(new_block);
    if (!node2) return 0;

    node3 = btree_create_node(new_block2);
	if (!node3) goto error1;

    item = split_node(node,node2,node3);
    if (!item) goto error2;

	if (filepos > (-1))
	{
		/* decide where to put middle node */
		if (key < item->key) ret = add_item(node3,key,filepos);
		else ret = add_item(node2,key,filepos);

        if ((!ret) || (ret == (-1))) goto error2;
	}
	else (*head) = add_blockno((*head),node2->block);

    ret = btree_write_node(fi,node2);
    if (!ret) goto error3;

    ret = btree_write_node(fi,node3);
    if (!ret) goto error3;

    item->left_child = node2->block;
    item->right_child = node3->block;

    destroy_node(node2);
    destroy_node(node3);

	if (orig_block == 0)
	{
		node = btree_create_node(0);
		if (!node) goto error4;

		node->item_count = 1;
		node->items[0] = item;
		node->is_parent = 1;

		ret = btree_write_node(fi,node);
		if (!ret) 
		{
			destroy_node(node);
			goto error4;
		}

		(*retitem) = NULL;
		return 1;		
	}

	(*retitem) = item;
	return 1;
error3:
	xfree(item);
error2:
	destroy_node(node3);
error1:
	destroy_node(node2);
error4:
	(*retitem) = NULL;
	return 0;
}


st_index_row* index_getequal(st_file_info *fi, st_node *node, int key, int itempos)
{
	int i;
	st_node *node2;
	int block;
	st_index_row *head,*cur;
	char *s;

	head = NULL;

	node2 = node;

	i = itempos;
	while (node2->items[0]->key <= key)
	{
		while ((i < node2->item_count) && (node2->items[i]->key == key))
		{
			cur = (st_index_row*)xmalloc(sizeof(st_index_row));
			if (!cur) 
			{
				if (node2 != node) destroy_node(node2);
				goto error1;
			}

			s = (char*)xmalloc(sizeof(char)*11);
			if (!s)
			{
				xfree(cur);
				if (node2 != node) destroy_node(node2);
				goto error1;
			}

			snprintf(s,11,"%d",node2->items[i]->key);
			cur->value = s;
			cur->filepos = node2->items[i]->filepos;

			cur->NEXT = head;			
			head = cur;

			i++;
		}

		block = get_neighbour(node2,2);
		if (node2 != node) destroy_node(node2);
		if (!block) return head;

		node2 = btree_read_node(fi,block);
		if (!node2) goto error1;
		i = 0;
	}

	return head;

error1:
	destroy_indexrow(head,1);
	return NULL;
}


st_index_row* index_gethigher(st_file_info *fi, st_node *node, int itempos)
{
	int i;
    st_node *node2;
    int block;
	char *s;
    st_index_row *head,*cur;

	head = NULL;
	cur = NULL;

    node2 = node;

	i = itempos;
    while (node2)
    {
        while (i < node2->item_count)
        {
            cur = (st_index_row*)xmalloc(sizeof(st_index_row));
            if (!cur)
            {
                if (node2 != node) destroy_node(node2);
                goto error1;
            }

            s = (char*)xmalloc(sizeof(char)*11);
            if (!s)
            {
                xfree(cur);
                if (node2 != node) destroy_node(node2);
                goto error1;
            }

            snprintf(s,11,"%d",node2->items[i]->key);
            cur->value = s;
            cur->filepos = node2->items[i]->filepos;

            cur->NEXT = head;
            head = cur;

            i++;
        }

        block = get_neighbour(node2,2);
        if (node2 != node) destroy_node(node2);
		if (!block) return head;

        node2 = btree_read_node(fi,block);
        if (!node2) goto error1;

		i = 0;
    }

    return head;

error1:
    destroy_indexrow(head,1);
    return NULL;
}


st_index_row* index_getlower(st_file_info *fi, st_node *node, int itempos)
{
	int i;
    st_node *node2;
    int block;
    st_index_row *head,*cur;
	char *s;

	head = NULL;
	cur = NULL;

    node2 = node;

	i = itempos;
    while (node2)
    {
        while (i >= 0)
        {
            cur = (st_index_row*)xmalloc(sizeof(st_index_row));
            if (!cur)
            {
                if (node2 != node) destroy_node(node2);
                goto error1;
            }

            s = (char*)xmalloc(sizeof(char)*11);
            if (!s)
            {
                xfree(cur);
                if (node2 != node) destroy_node(node2);
                goto error1;
            }

            snprintf(s,11,"%d",node2->items[i]->key);
            cur->value = s;
            cur->filepos = node2->items[i]->filepos;

            cur->NEXT = head;
            head = cur;

            i--;
        }

        block = get_neighbour(node2,1);
        if (node2 != node) destroy_node(node2);
        if (!block) return head;

        node2 = btree_read_node(fi,block);
        if (!node2) goto error1;

		i = node2->item_count - 1;
    }

	return head;

error1:
    destroy_indexrow(head,1);
    return NULL;
}


void destroy_indexrow(st_index_row *head, int all)
{
	st_index_row *cur;

	if (all)
	{
		while (head)
		{
			cur = head;
			head = head->NEXT;
			xfree(cur->value);
			xfree(cur);
		}
	}
	else
	{
		xfree(cur->value);
		xfree(cur);
	}
}

