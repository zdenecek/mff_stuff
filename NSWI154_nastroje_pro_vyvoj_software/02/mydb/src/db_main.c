#include <string.h>
#include "error.h"
#include "memory.h"
#include "db_main.h"
#include "db_engine.h"
#include "conds.h"
#include "textfileio.h"
#include "strutil.h"
#include "tabconf.h"
#include "tabdef.h"
#include "index.h"


st_proctab_elem processtab[PROCTABSIZE];


void init_processor(void)
{
	processtab[0].action = SELECT;
	processtab[0].descr = "SELECT";
	processtab[0].processfunc = &process_select;

	processtab[1].action = INSERT;
    processtab[1].descr = "INSERT";
    processtab[1].processfunc = &process_insert;

	processtab[2].action = UPDATE;
    processtab[2].descr = "UPDATE";
    processtab[2].processfunc = &process_update;

	processtab[3].action = DELETE;
    processtab[3].descr = "DELETE";
    processtab[3].processfunc = &process_delete;

	processtab[4].action = CREATE_DB;
    processtab[4].descr = "CREATE DB";
    processtab[4].processfunc = &process_crdb;

	processtab[5].action = CREATE_TAB;
    processtab[5].descr = "CREATE TAB";
    processtab[5].processfunc = &process_crtab;

	processtab[6].action = CREATE_INDEX;
    processtab[6].descr = "CREATE INDEX";
    processtab[6].processfunc = &process_crindex;

	processtab[7].action = DROP;
    processtab[7].descr = "DROP";
    processtab[7].processfunc = &process_drop;

	processtab[8].action = USE_DB;
    processtab[8].descr = "USE DB";
    processtab[8].processfunc = &process_use;
}


st_result* process_query(const st_query_parsed *query)
{
	st_result *res;
	int ret;
	int i;

	init_processor();

	res = (st_result*)xmalloc(sizeof(st_result));
	if (!res) return NULL;

	res->resultdata = NULL;
	res->description = NULL;
	res->resultret = 0;

	/* third condition is for DROP_DB */
	if ((query->action != USE_DB) && (query->action != CREATE_DB) && (!((query->action == DROP) && (query->querydata.param_drop->target == 1))) && (!current_db))
  	{
		set_error(ERR_NODBSEL);
		return NULL;
  	}

	/* choose the correct query processing function */
    i = 0;
    while (i < PROCTABSIZE)
    {
        if (query->action == processtab[i].action)
        {
            ret = (processtab[i].processfunc) (query,&res);
            if (!ret) return NULL;

			return res;
        }
        i++;
    }

    return NULL;
}


int process_select(const st_query_parsed *query, st_result **res)
{
	st_result *result;	
	st_result_set *resset,*new_resset;
	st_query_select *selectquery;
	st_res_node *resnode;
	st_row_data *row;
	int ret;


	result = *res;
	selectquery = query->querydata.param_select;

	resnode = get_resset_tree(selectquery->tabset, selectquery->condtree);
	if (!resnode)
	{
		*res = NULL;
		return 0;
	}	

	resset = compute_result(resnode);
	if (!resset)
	{
		*res = NULL;
		return 0;
	}	

	/* make cartesian product of resset and tables which have no column in any condition */
	new_resset = complete_result(resset, selectquery->tabset);
	if (!new_resset)
    {
		destroy_resultset(resset);
        *res = NULL;
        return 0;
	} 

	if (new_resset != resset) destroy_resultset(resset);
	
	resset = new_resset;

	/* order is before apply_colset because order column don't have to be in colset */
	if (selectquery->ordercol)
	{
		row = sort_rows(resset->resrows, selectquery->ordercol);
		if (!row)
		{
        	destroy_resultset(resset);
        	*res = NULL;
        	return 0;
    	} 
    	resset->resrows = row;
	}

	ret = apply_colset(selectquery->colset, resset);
	if (!ret)
	{
		destroy_resultset(resset);
        *res = NULL;
		return 0;
	}	

	result = (st_result*)xmalloc(sizeof(st_result));
	if (!result)
    {
        destroy_resultset(resset);
        *res = NULL;
        return 0;
    }
 
  	result->resultdata = resset;
  	result->description = "SELECT";
  	result->resultret = 1;

  	(*res) = result;

  	return 1;
}


int process_insert(const st_query_parsed *query, st_result **res)
{
	st_table_def *tabdef;
	st_row_data *row;
	st_col_data *col;
	st_file_info *fi;
	int ret;
	char *value;
	char *s;
	int filepos;
	st_query_insert *insertquery;
	st_result *result;

  	result = *res;
  	insertquery = query->querydata.param_insert;

  	/* test for existence of table */
  	tabdef = get_tabledef(insertquery->tabname);
  	if (!tabdef) return 0; /* table does not exist */
	destroy_tabdef(tabdef);


  	/* writing new row to data file */
  	fi = file_open(current_db,insertquery->tabname,FILE_DATA);
  	if (!fi) return 0;

	if (file_bigger(fi,64)) /* maximum size of table is 64 MB */
	{
		set_error(ERR_BIGTABLE);
		return 0;
	}
  
	row = (st_row_data*)xmalloc(sizeof(st_row_data*));
	if (!row) return 0;

	row->data = insertquery->coldata;
	row->NEXT = NULL;

	col = row->data;
	row->data_count = count_cols(col);

	filepos = fi->size; /* new row is written at the end */

  	ret = write_rowdata(fi,row);
	if (!ret) goto error3;

  	if (!file_close(fi))
  	{
		destroy_fileinfo(fi);
    	return 0;
  	}

	/* adding row to index */
	if (table_indexed(insertquery->tabname))
	{
		col = row->data;
		/* we must add row to all indexes on this table - loop over columns */
		while (col)
		{
			if (column_indexed(insertquery->tabname, col->col_name))
			{
				value = str_copy(col->data_value,0);
				if (!value) goto error3;
				
			    /* add colname to tabname for the purpose of file name */
    			s = str_concat(insertquery->tabname,col->col_name,'_');
    			if (!s) goto error1;

    			fi = file_open(current_db,s,FILE_INDEX);
    			if (!fi) goto error2;
		
				ret = index_insertrow(fi,value,filepos);
				if (!ret) goto error2;
				
				if (!file_close(fi))
				{
					destroy_fileinfo(fi);
					goto error2;
				}

				xfree(s);
				xfree(value);				
			}

			col = col->NEXT;
		}
	}


	xfree(row); /* coldata cannot be destroyed - belong to insertquery */

	result->resultret = 1;
  	result->description = "INSERT";

  	(*res) = result;

  	return 1;

error2:
	xfree(s);
error1:
	xfree(value);
error3:
    xfree(row); /* coldata cannot be destroyed - belong to insertquery */
    return 0;
}


int process_update(const st_query_parsed *query, st_result **res)
{
	st_table_def *tabdef;
	st_col_def *coldef;
	st_row_data *row,*new_row;
	st_file_info *fi;
	int ret;
	char *old_readpos;
	int satisfy;
	char *end_pos; /* boundary address for checking of rows - position of first newly written row */
	int newrow_count;
	st_result *result;
	st_query_update *updatequery;

	result = *res;

	updatequery = query->querydata.param_update;

    /* test for existence of table */
    tabdef = get_tabledef(updatequery->tabname);
    if (!tabdef) return 0; /* table does not exist */
    destroy_tabdef(tabdef);

	/* opening data file */
	fi = file_open(current_db,updatequery->tabname,FILE_DATA);
	if (!fi) return 0;
 
    if (file_bigger(fi,64)) /* maximum size of table is 64 MB */
    {
        set_error(ERR_BIGTABLE);
        return 0;
    }
 
	newrow_count = 0;
	old_readpos = fi->readpos;
	end_pos = fi->address+fi->size;

  	/* loop over all columns in data file */
  	while ( (fi->readpos < end_pos) && (row = read_rowdata(fi,NULL)) )
  	{
        /* test if row satisfy all conditions */
        satisfy = 1;
        if (updatequery->condtree)
        {
            satisfy = row_satisfy(row,updatequery->condtree);
            if (satisfy == (-1))
            {
				destroy_rowdata(row,0);
                if (!file_close(fi)) goto error1;
            }
        }

    	if (satisfy) /* row satisfy conditions - we must update it */
		{
	  		destroy_rowdata(row,0);
	  		fi->readpos = old_readpos;
			row = delete_rowdata(fi);

	  		/* apply new values */
	  		new_row = update_rowdata(row,updatequery->coldata);	  	  
	  		if (!new_row)
      		{
				destroy_rowdata(row,0);
        		if (!file_close(fi)) destroy_fileinfo(fi);
        		return 0;
      		}

	  		destroy_rowdata(row,0);

	  		/* write modified row */
	  		ret = write_rowdata(fi,new_row);

	  		if (!ret)
	  		{
        		if (!file_close(fi)) destroy_fileinfo(fi);
				destroy_rowdata(new_row,0);	
        		return 0;
	  		}
	  
     	 	destroy_rowdata(new_row,0);
	
	  		old_readpos = fi->readpos;
	  		newrow_count++;
		}
		else
		{
      		destroy_rowdata(row,0);
	  		old_readpos = fi->readpos;
		}
  	}

	ret = remove_deleted_strings(fi);
    if (!ret)
    {
        if (!file_close(fi)) goto error1;
    }
    
    if (!file_close(fi)) goto error1;

    /* updating indexes */
    /* deletes and then creates every index for table */
    if (table_indexed(updatequery->tabname))
    {
        tabdef = get_tabledef(updatequery->tabname);
        if (!tabdef) return 0;

        coldef = tabdef->columns;
    
        while (coldef)
        {
            if (column_indexed(updatequery->tabname, coldef->name))
            {
                ret = delete_index(updatequery->tabname, coldef->name);
                if (!ret) goto error3;

                ret = create_index(updatequery->tabname, coldef->name);
                if (!ret) goto error3;
            }

            coldef = coldef->NEXT;
        }

        destroy_tabdef(tabdef);
    }

  	result->resultret = newrow_count;
  	result->description = "UPDATE";
  	(*res) = result;

  	return 1;
error3:
	destroy_tabdef(tabdef);
	return 0;
error1:
  	destroy_fileinfo(fi);
  	return 0;
}


int process_delete(const st_query_parsed *query, st_result **res)
{
	st_table_def *tabdef;
	st_col_def *coldef;
	st_row_data *row;
	st_file_info *fi;
	char *old_readpos;
	int satisfy;
	int delete_count;
	st_result *result;
	st_query_delete *deletequery;
	int ret;


  	result = *res;
  	deletequery = query->querydata.param_delete;

	/* test for existence of table */
    tabdef = get_tabledef(deletequery->tabname);
    if (!tabdef) return 0; /* table does not exist */
    destroy_tabdef(tabdef);


  	/* opening data file */
  	fi = file_open(current_db,deletequery->tabname,FILE_DATA);
	if (!fi) return 0;
  
  	delete_count = 0;
  	old_readpos = fi->readpos;

  	while ((row = read_rowdata(fi,NULL)))
  	{
    	/* test if row satisfy all conditions */
    	satisfy = 1;
		if (deletequery->condtree)
		{
      		satisfy = row_satisfy(row,deletequery->condtree);
	  		if (satisfy == (-1))
	  		{
        		if (!file_close(fi)) goto error2;
	  		}		
		}

    	if (satisfy) /* row satisfy - we must delete it */
    	{
      		destroy_rowdata(row,0);
      		fi->readpos = old_readpos;
      		row = delete_rowdata(fi);

	  		destroy_rowdata(row,0);
      		old_readpos = fi->readpos;
      		delete_count++;
    	}
    	else
    	{
	  		destroy_rowdata(row,0);
      		old_readpos = fi->readpos;
    	}
  	}

	ret = remove_deleted_strings(fi);
	if (!ret)
	{
		if (!file_close(fi)) goto error1;
	}

  	if (!file_close(fi)) goto error1;

	/* updating indexes */
	/* deletes and then creates every index for table */
    if (table_indexed(deletequery->tabname))
    {
		tabdef = get_tabledef(deletequery->tabname);
		if (!tabdef) return 0;

        coldef = tabdef->columns;

        while (coldef)
        {
            if (column_indexed(deletequery->tabname, coldef->name))
            {
				ret = delete_index(deletequery->tabname, coldef->name);
				if (!ret) goto error3;

				ret = create_index(deletequery->tabname, coldef->name);
				if (!ret) goto error3;
            }

            coldef = coldef->NEXT;
        } 

		destroy_tabdef(tabdef);
    }

  	result->resultret = delete_count;
  	result->description = "DELETE";

  	(*res) = result;

  	return 1;

error3:
	destroy_tabdef(tabdef);
	return 0;
error2:
	destroy_rowdata(row,0);
error1:
  	destroy_fileinfo(fi);
  	return 0;
}


int process_crdb(const st_query_parsed *query, st_result **res)
{
	int ret;
	st_query_crdb *crdbquery;
	st_result *result;

  	result = *res;
	crdbquery = query->querydata.param_crdb;

	/* try to create directory */

  	ret = create_dbdir(crdbquery->name);
	if (!ret) return 0;
 
  	result->resultret = 1;
  	result->description = "CREATE DB";

  	(*res) = result;
 
  	return 1;
}


int process_crtab(const st_query_parsed *query, st_result **res)
{
	st_table_def *tabdef;
	st_table_conf *tabconf;
	st_col_def *coldef;
	char *s;
	size_t size;
	st_result *result;
	st_query_crtab *crtabquery;
	int ret;

  	result = *res;
	crtabquery = query->querydata.param_crtab;

	tabconf = get_tableconf(crtabquery->name);
	if (tabconf) goto error1; /* error - table already exist */

	/* creating table configuration */
    tabconf = (st_table_conf*)xmalloc(sizeof(st_table_conf));
    if (!tabconf) return 0;

	size = strlen(crtabquery->name);

	s = str_copy(crtabquery->name,0);
    if (!s) goto error1;

    tabconf->name = s;

    /* make path to datafile */
    s = (char*)xmalloc(sizeof(char)*(size+5));
    if (!s) goto error1;

    s = strncpy(s,crtabquery->name,size+1);
    s = strncat(s,".dat",5);
    tabconf->datafile = s;

	ret = write_tableconf(tabconf);
	if (!ret) goto error1;

	destroy_tabconf(tabconf);

	/* creating table definition */
	tabdef = (st_table_def*)xmalloc(sizeof(st_table_def));
	if (!tabdef) return 0;

	s = str_copy(crtabquery->name,0);
	if (!s) goto error2;
	tabdef->name = s;

	tabdef->col_num = crtabquery->col_num;

	coldef = copy_coldef(crtabquery->coldefs);
	if (!coldef) goto error2;

	tabdef->columns = coldef;

	ret = write_tabledef(tabdef);
	if (!ret) goto error2;

	destroy_tabdef(tabdef);

	result->resultret = 1;
  	result->description = "CREATE TAB";
	(*res) = result;

  	return 1;
error1:
	destroy_tabconf(tabconf);
	return 0;
error2:
	destroy_tabdef(tabdef);
	return 0;
}


int process_crindex(const st_query_parsed *query, st_result **res)
{
	st_result *result;
  	st_query_crindex *crindexquery;
  	int ret;

  	result = *res;
  	crindexquery = query->querydata.param_crindex;

	/* is column of type INT */
	ret = column_type(crindexquery->tabname,crindexquery->colname,INT);
	if (ret == (-1)) return 0;
	if (!ret) return 0;

	ret = create_index(crindexquery->tabname,crindexquery->colname);
	if (!ret) return 0;

	result->resultret = 1;
  	result->description = "CREATE INDEX";
  	(*res) = result;

  	return 1;
}


int process_drop(const st_query_parsed *query, st_result **res)
{
	int ret;
	st_result *result;
	st_query_drop *dropquery;
	char *s1,*s2;

	result = *res;
	dropquery = query->querydata.param_drop;

	switch (dropquery->target)
	{
		case 1:
	  		ret = remove_dbdir(dropquery->targetname);
			if ((current_db) && (STR_EQ(dropquery->targetname,current_db))) 
	  		{
				xfree(current_db);
	    		current_db = NULL;
	  		}
      		if (!ret) return 0;	  
	  		break;
		case 2:
	  		ret = drop_table(dropquery->targetname);
      		if (!ret) return 0;
      		break;
    	case 3:
			/* divide target name into tabname and colname */
			s1 = str_copy(dropquery->targetname,'.');
			if (!s1) return 0;

			s2 = dropquery->targetname + strlen(s1) + 1;

			ret = delete_index(s1,s2);
			if (!ret) return 0;
      		break;
    	default:
      		break;
	}

  	result->resultret = 1;
  	result->description = "DROP";
	
  	(*res) = result;

  	return 1;
}


int drop_table(const char *tabname)
{	  
	st_table_def *tabdef;
	st_col_def *coldef;
	st_table_conf *tabconf;
	int ret;

	tabdef = delete_tabledef(tabname);
	if (!tabdef) return 0;

	/* destroy all indexes for table */
	coldef = tabdef->columns;
	while (coldef)
	{
		ret = column_indexed(tabname,coldef->name);
		if (ret == (-1))
		{
			destroy_tabdef(tabdef);
            return 0;
		}

		if (ret)
		{
			ret = delete_index(tabname,coldef->name);
			if (!ret)
			{
				destroy_tabdef(tabdef);
				return 0;
			}
		}
		
		coldef = coldef->next;
	}

	destroy_tabdef(tabdef);

	
    tabconf = delete_tableconf(tabname);
    if (!tabconf) return 0;

    destroy_tabconf(tabconf);

	ret = file_delete(current_db,tabname,FILE_DATA);
    if (!ret) return 0;

	return 1;
}


int process_use(const st_query_parsed *query, st_result **res)
{
	st_result *result;
	st_query_use *usequery;

	result = *res;
	usequery = query->querydata.param_use;

	if (current_db) xfree(current_db);

	current_db = str_copy(usequery->dbname,0);
	if (!current_db) return 0;

	if (!exist_dbdir(usequery->dbname)) 
	{
		set_error(ERR_DBNOEX);
		xfree(current_db);
		current_db = NULL;
		return 0; /* error - db doesnt exist */
  	}

	result->resultret = 1;
	result->description = "USE DB";
	(*res) = result;

	return 1;
}
