LIB_OBJECTS= btree_int.o conds.o config.o data.o db_engine.o db_main.o error.o index.o memory.o parser_commands.o parser_conds.o query.o result.o sql_parser.o strutil.o tabconf.o tabdef.o textfileio.o 

all: testadmin testclient testserver libmydb.a

.PHONY: clean all

clean: 
	rm -f *.o testadmin testclient testserver libmydb.a

data.o: defs.h error.h memory.h linklist.h strutil.h textfileio.h data.h

$(LIB_OBJECTS): %.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.h : %.o

testclient: libmydb.a testclient.c client_comm.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o testclient -L. testclient.c client_comm.c  -lmydb  -lreadline -lhistory  

testadmin:  libmydb.a testadmin.c              
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o testadmin -L. testadmin.c  				   -lmydb  -lreadline -lhistory

testserver: libmydb.a testserver.c server_comm.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o testserver -L. testserver.c server_comm.c   -lmydb     

libmydb.a: $(LIB_OBJECTS)
	ar -rcvs libmydb.a $(LIB_OBJECTS)

