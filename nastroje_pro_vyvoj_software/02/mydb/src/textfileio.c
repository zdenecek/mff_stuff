#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>

#include "config.h"
#include "error.h"
#include "memory.h"
#include "textfileio.h"
#include "strutil.h"


int create_dbdir(const char *dbname)
{
	char *path;
	int ret;
	size_t size1,size2;

	size1 = strlen(datadir);
	size2 = strlen(dbname);

	path = (char*)xmalloc(sizeof(char)*(size1+size2+1));
	if (!path) return 0;
  
	/* make real path */
	path = strncpy(path,datadir,size1+1);
	path = strncat(path,dbname,size2+1);

	/* setting permissions */  
	ret = mkdir(path, S_IREAD | S_IWRITE | S_IEXEC);

	if (ret == (-1)) 
	{
		if (errno == EEXIST) set_error(ERR_DBEXIST); /* database exist */
		xfree(path);
		return 0;
  	}
	
	xfree(path);
	
	return 1;
}


int remove_dbdir(const char *dbname)
{
	char *command;
	int ret;
	size_t size1,size2; 

	size1 = strlen(datadir);
	size2 = strlen(dbname);

	command = (char*)xmalloc(sizeof(char)*(size1+size2+1+7));
	if (!command) return 0;
  
	/* make shell command */
	command = strncpy(command,"rm -rf ",8); /* !! Linux/Unix specific !! */
	command = strncat(command,datadir,size1+1);
	command = strncat(command,dbname,size2+1);

	ret = system(command);
 
	if (ret == (-1) || (ret > 0))
	{
		set_error(ERR_DBNORM); /* directory cannot be removed */
		xfree(command);
		return 0;
	}

    xfree(command);
    return 1;
}


int exist_dbdir(const char *dbname)
{
	char *path;
	int ret;
	DIR *dir;
	struct dirent *direntry;
	size_t size;

	size = strlen(datadir)+1;

	path = (char*)xmalloc(sizeof(char)*size);
	if (!path) return 0;
  
	/* make real path */
	path = strncpy(path,datadir,size);

	dir = opendir(path);
	if (!dir)
	{
		set_error(ERR_FILESYS);
		return (-1);
	}
  
	ret = 0;
	/* reading directory list */
	while ((direntry = readdir(dir)))
	{
		if ((direntry->d_type == DT_DIR) && (STR_EQ(direntry->d_name,dbname))) ret = 1; /* directory exist */
	}

	closedir(dir);
	xfree(path);

	return ret; 
}


st_file_basename* create_filebasename(en_file_type typ, const char *dbname, const char *tabname)
{
	st_file_basename *fb;
	char *fb_dbname,*fb_tabname;

	 /* create file name */
    fb = (st_file_basename*)xmalloc(sizeof(st_file_basename));
    if (!fb) return NULL;

    fb->typ = typ;
    
    fb_dbname = str_copy(dbname,0);
    if (!fb_dbname) goto error1;
    fb->dbname = fb_dbname;

	if (tabname)
	{
    	fb_tabname = str_copy(tabname,0);
    	if (!fb_tabname) goto error1;
    	fb->tabname = fb_tabname;
	}
	else fb->tabname = NULL;

	return fb;

error1:
	destroy_filebasename(fb);
	return NULL;
}


char* create_filename(st_file_basename *basename)
{
	char *path;
	size_t size1,size2,size3;

	size1 = strlen(datadir);
	size2 = strlen(basename->dbname)+1;
	size3 = 0;

	if (basename->tabname) /* we will use dbname and tabname */
	{
		size3 = strlen(basename->tabname);

		path = (char*)xmalloc(sizeof(char)*(size1+size2+size3+5)); /* we also make space for file name extension */
	}
  	else /* we will use dbname twice */
	{
    	/* we also make space for file name extension */
	    path = (char*)xmalloc(sizeof(char)*(size1+(size2*2)+5));
  	}

	if (!path) return NULL;

	/* create path to the file */
	path = strncpy(path,datadir,size1+1);

  	path = strncat(path,basename->dbname,size2+1);
  	path = strncat(path,"/",2);

  	/* creating path based on requested type */
  	switch (basename->typ) 
  	{
		case FILE_DATA:
	  		path = strncat(path,basename->tabname,size3+1);
	  		path = strncat(path,".dat",5);
      		break;
		case FILE_INDEX:
	  		path = strncat(path,basename->tabname,size3+1);
	  		path = strncat(path,".idx",5);
	  		break;
		case FILE_TABDEF:
	  		path = strncat(path,basename->dbname,size2+1);
	  		path = strncat(path,".def",5);
	  		break;
	    case FILE_TABCONF:
			path = strncat(path,basename->dbname,size2+1);
    		path = strncat(path,".cfg",5);
		default:
			break;
  	}

  	return path;
}


int file_delete(const char *dbname,const char *tabname, en_file_type typ)
{
	st_file_basename *fb;
	char *fname;
	int ret;

	/* create file name */
	fb = create_filebasename(typ,dbname,tabname);
	if (!fb) return 0;

	fname = create_filename(fb);
	if (!fname) goto error1;

	destroy_filebasename(fb);

	/* delete file */
	ret = unlink(fname);
	if (ret == (-1))
	{
		xfree(fname);
		if (errno == ENOENT) return 1; /* file doesn't exist */
		set_error(ERR_FILESYS);
		return 0;
	}

	xfree(fname);
	return 1;

error1:
	destroy_filebasename(fb);
	return 0;
}


st_file_info* file_open(const char* dbname, const char *tabname, en_file_type typ)
{
	st_file_info* fi;
	struct stat *file_stat;
	int ret;
	int fd;
	char *tmp_adr;
	st_file_basename *fb;
	char *fname;
	size_t alignsize;

	/* create file name */
	fb = create_filebasename(typ,dbname,tabname);
	if (!fb) return NULL;

  	fname = create_filename(fb);
  	if (!fname) goto error1;

  	destroy_filebasename(fb);

  	fi = (st_file_info*)xmalloc(sizeof(st_file_info));
  	if (!fi) return NULL;

  	file_stat = (struct stat*)xmalloc(sizeof(struct stat));
  	if (!file_stat) return NULL;
   
  	fi->path = fname;

  	/* open file */
  	fd = open(fi->path,O_RDWR | O_CREAT);
  	if (fd == (-1)) goto error2; 

  	/* it must have correct permissions */
  	ret = chmod(fi->path, S_IRUSR | S_IWUSR);
  	if (ret == (-1)) goto error2;

  	fi->filedes = fd;

  	/* get file statistics */
  	ret = stat(fi->path,file_stat);
  	if (ret == (-1)) goto error2;

  	fi->size = file_stat->st_size;

  	/* we must use page-aligned memory size */
  	alignsize=get_mapsize(fi->size);

  	fi->alignsize = alignsize;

  	/* mapping to memory */ 
  	tmp_adr = (char*)mmap(NULL,fi->alignsize,PROT_WRITE | PROT_READ,MAP_SHARED,fi->filedes,0);

  	if (tmp_adr == (void*)MAP_FAILED)
  	{
		set_error(ERR_FATIO);
    	fi->address = NULL;
    	return NULL;
  	}

  	fi->address = tmp_adr;
  	fi->readpos = tmp_adr; /* we will read from the beginning */
  	fi->delpos = tmp_adr; /* we will delete from the beginning */

  	xfree(file_stat);

  	return fi;

error1:
  	destroy_filebasename(fb);
  	return NULL;
error2:
  	set_error(ERR_FILESYS);
  	return NULL;
}


int file_close(st_file_info *fi)
{
	int ret;
 
  	/* write memory to disk */ 
  	ret = msync(fi->address,fi->size,MS_SYNC);
  	if (ret == (-1)) goto error1;
  
  	/* unmap */
  	ret = munmap(fi->address,fi->alignsize);
  	if (ret == (-1)) goto error1;
  
  	ret = close(fi->filedes);
  	if (ret == (-1)) goto error2;

  	/* to be sure that it wil have correct permissions */
  	ret = chmod(fi->path, S_IRUSR | S_IWUSR);
  	if (ret == (-1)) goto error2;

  	destroy_fileinfo(fi);

  	return 1;

error1:
  	set_error(ERR_FATIO);
  	return 0;
error2:
  	set_error(ERR_FILESYS);
  	return 0;
}


int file_bigger(st_file_info *fi, int mb)
{
	int size;

	size = mb*1024*1024;

	if (fi->size > size) return 1;
	else return 0;
}


size_t get_mapsize(size_t filesize)
{
  	size_t page_size = (size_t)sysconf(_SC_PAGESIZE);
  	int pagecount;

  	pagecount = (int)((filesize+page_size-1) / page_size)+1;
  
  	return (size_t)(page_size*pagecount);
}


int mapfile_resize(st_file_info *fi, size_t newsize)
{
  	char *tmp_adr;
  	size_t alignnewsize;
  	int ret;

  	/* write content of memory to disk */
  	ret = msync(fi->address,fi->size,MS_SYNC);
  	if (ret == (-1)) goto error1;

  	/* unmap mapping with old filesize */
  	ret = munmap(fi->address,fi->alignsize);
  	if (ret == (-1)) goto error1;
	
  	alignnewsize=get_mapsize(newsize);
 
  	fi->alignsize = alignnewsize;

  	/* change the size of file on disk */ 
  	ret = ftruncate(fi->filedes,newsize);
  	if (ret == (-1)) goto error2;

  	/* mapping with new file size */
  	/* alignnewsize is only memory size, not the real file size */
  	tmp_adr = (char*)mmap(NULL,fi->alignsize,PROT_READ|PROT_WRITE,MAP_SHARED,fi->filedes,0);
  	if (tmp_adr == (void*)MAP_FAILED) 
  	{
		fi->address = NULL;
		goto error1;
  	}
  	fi->address = tmp_adr;
  	fi->size = newsize; /* newsize is real file size */
  	if (fi->readpos > (fi->address+fi->size) ) fi->readpos = tmp_adr; /* reset of reading position */
	if (fi->delpos > (fi->address+fi->size) ) fi->delpos = tmp_adr; /* reset of deleting position */

	return 1;

error1:
	set_error(ERR_FATIO);
	return 0;
error2:
	set_error(ERR_FILESYS);
	return 0;
}


void file_reset(st_file_info *fi)
{
	fi->readpos = fi->address;
	fi->delpos = fi->address;
}

int file_write_string(st_file_info *fi, const char *str)
{
	int ret;
	char *start;
	size_t size;
	char *s;
 
	if (!fi->address)
	{
	    set_error(ERR_FATIO);
    	return 0; /* error */
	} 
  
    size = strlen(str);

	s = (char*)malloc(sizeof(char)*(size+2));
	if (!s) return 0;

	/* we save the address we will write to */
	start = fi->address+fi->size;
  
	ret = mapfile_resize(fi, fi->size+size+1);
	if (!ret) return 0; 

	s = strncpy(s,str,size+1);
	s = strncat(s,"\n",2);

	start = strncpy(start,s,size+1);

	return 1;
}


int file_writepos_string(st_file_info *fi, const char *str, int pos)
{
	int ret;
    char *start;
    size_t size;
    char *s;

    if (!fi->address)
    {
        set_error(ERR_FATIO);
        return 0; /* error */
    }

	size = strlen(str);

	if (pos >= fi->size) /* we must make the file larger */
	{
		ret = mapfile_resize(fi, fi->size+size);
	    if (!ret) return 0;
	}

    s = (char*)malloc(sizeof(char)*(size+1));
    if (!s) return 0;

    /* we save the address we will write to */
    start = fi->address + pos;

    s = strncpy(s,str,size+1);

    start = strncpy(start,s,size);

    return 1;
}


char* file_read_string(st_file_info *fi)
{
	char *str;
	
	if (!fi->address) goto error1;
  
	if (fi->readpos >= (fi->address+fi->size)) goto error1;

	str = str_copy(fi->readpos,'\n');
	if (!str) return 0;
 
	fi->readpos += strlen(str)+1;

	return str;
error1:
	set_error(ERR_FATIO);
	return NULL;
}


char* file_delete_string(st_file_info *fi)
{
	char *str;
    int size, newsize;
	int ret;

	if (!fi->address)
	{
    	set_error(ERR_FATIO);
	    return NULL;
	}

	fi->delpos = fi->readpos;

	str = file_read_string(fi);
	if (!str) return NULL;

	size = fi->size-(fi->readpos-fi->address); /* size of the memory to move */

	fi->readpos = memmove(fi->delpos,fi->readpos,size);

	newsize = (fi->delpos - fi->address)+size; /* new file size */

	ret = mapfile_resize(fi,newsize);
	if (!ret)
	{
    	xfree(str);
		return NULL; /* error */
	}

	return str;
}


char* file_mark_string(st_file_info *fi, char mark)
{
	char *s;
	char *str;
	char *oldpos;

	if (!fi->address)
    {
        set_error(ERR_FATIO);
        return NULL;
    }

	oldpos = fi->readpos;

	str = file_read_string(fi);
    if (!str) return NULL;

	fi->readpos = oldpos;

	s = fi->readpos;
	while (*s != '\n')
	{
		*s = '#';
		s++;
	}
	s++; /* skip '\n' */
	fi->readpos = s;
	
	return str;		
}


char* file_str_markdel(st_file_info *fi)
{
	char *str;

	str = file_mark_string(fi,'#');
	
	return str;
}


int remove_deleted_strings(st_file_info *fi)
{
	char *str;

	fi->readpos = fi->address;

	while (fi->readpos < (fi->address + fi->size))
	{
		if (*(fi->readpos) == '#')
		{
			str = file_delete_string(fi);
			if (!str) return 0;
	
			xfree(str);
		}
		else
		{
			str = file_read_string(fi);
			if (!str) return 0;

			xfree(str);
		}
	}
	
	return 1;
}


void destroy_filebasename(st_file_basename *fb)
{
	if (fb)
  	{
    	if (fb->dbname) xfree(fb->dbname);
	    if (fb->tabname) xfree(fb->tabname);
    	xfree(fb);
	}
}


void destroy_fileinfo(st_file_info *fi)
{
	if (fi)
  	{
    	xfree(fi->path);
	    xfree(fi);
  	}
}

