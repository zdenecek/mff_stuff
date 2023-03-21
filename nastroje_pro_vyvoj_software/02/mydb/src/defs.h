#ifndef __DEFS_H__
#define __DEFS_H__

/* definitions of important constants for file I/O */
/* they include place for separator characters and null terminator */
#define CHAR_COLSIZE 8 /* max number of figures for size of columns */
#define CHAR_COLTYPE 2 /* max number of figures for type of column */
#define CHAR_COLNUM 4 /* man number of figures for count of columns */

/* size of data types in bytes(chars) */
#define DATA_INT_SIZE 12
#define DATA_VARCHAR_SIZE 255
#define DATA_MAX_SIZE 255

#define TABDEF_NAME_SIZE 65
#define TABDEF_COLNUM_SIZE 3 /* count of figures in the number */
#define COLDEF_NAME_SIZE 65
#define COLDEF_DATATYPE_SIZE 3
#define COLDEF_DATASIZE_SIZE 13

#define TABDEF_COL_COUNT 64
#define TABDEF_COLCOUNT_SIZE 3
#define TABDEF_FILEPATH_SIZE 2049

#define TABDEF_SUMSIZE (TABDEF_NAME_SIZE+TABDEF_COLNUM_SIZE+TABDEF_COL_COUNT*(COLDEF_NAME_SIZE+COLDEF_DATATYPE_SIZE+COLDEF_DATASIZE_SIZE)+1)

#define TABCONF_SUMSIZE (TABDEF_NAME_SIZE+TABDEF_FILEPATH_SIZE+1)

#define ROWDATA_SUMSIZE (TABDEF_COLCOUNT_SIZE+TABDEF_COL_COUNT*(COLDEF_NAME_SIZE+DATA_MAX_SIZE)+1)

#endif
