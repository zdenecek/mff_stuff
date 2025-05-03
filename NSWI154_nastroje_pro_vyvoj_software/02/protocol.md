Zdeněk Tomis
zdnek.tomis@gmail.com 

Deadline: 30.10.2022

Requirements:
- You do not have to procotol absolutely everything, such as 'cd' commands,
  'rm -rf' when the task tells you to do a 'rm -rf', 'vim file', etc.
- Include output of commands only when explicitly requested.
- You can write answers in Czech.

Useful links and help:
  http://www.gnu.org/software/make/manual/


-----
Tasks
-----

0. The "mydb" directory contains implementation of a very simple database.
   All source code files are in the "src" subdirectory.

1. Write all necessary Makefiles for the "mydb" project.
   Satisfy the following requirements:
	- create one Makefile at the top level (project root) and one Makefile in the 'src' subdirectory
		- for each target, the top-level Makefile should invoke the nested Makefile (in the 'src' directory) using the recommended approach
		- the nested Makefile should contain the actual rules for compiling source code files, removing generated object files, and so on
	- the full build process should create three binaries (programs): testadmin, testclient, testserver
		- each binary program should be linked against the static library libmydb.a (see below)
		- the binary programs 'testclient' and 'testadmin' will be also linked with the following libraries: libreadline, libhistory
		- you can use a command similar to the following one to perform correct linking: gcc -o testclient -L. testclient.c client_comm.c -lmydb -lreadline -lhistory
		- the binary program 'testclient' must be built from these source files: testclient.c, client_comm.c
		- the binary program 'testserver' must be built from these source files: testserver.c, server_comm.c
		- the binary program 'testadmin' must be built from these source files: testadmin.c
	- the build process should also create the static library libmydb.a 
		- the library must contain object files for all source files except those specific for the binary programs (see above)
		- use the following command to create the library file libmydb.a: ar -rcvs libmydb.a <list of object files to be included>
	- specify proper dependencies between object files, source files (.c), and binary programs
	- specify dependencies on header files (.h) only for "data.c" (by looking into the source code)
		- you do *not* have to specify dependencies on header files in the case of other source files, because there is too many of them
	- other requirements on both Makefiles
		- support the following targets
			all -> compiles everything (i.e., the full build)
			clean -> deletes all the generated files
		- define and use built-in variables for compiler and flags
			- variable CC with the value 'gcc'
			- variable CFLAGS with the value '-g -Wall'
			- set values of the variables in the top-level Makefile and tell Make to propagate the values to the Makefile in the 'src' subdirectory
		- use features supported by Make to eliminate duplication (static pattern rules, implicit rules, variables, functions), but do not over-generalize at the expense of clarity
 		- targets that do not correspond to real files should be marked appropriately

2. Test the Makefiles by executing "make" in the root directory of "mydb".
   Ignore all the warnings printed by GCC. The source code is quite old and does not follow the recent standards of C, but it should compile anyway.
   Paste the output of Make here.

	I pasted only stdout.

	make -C src all
	make[1]: Entering directory '/mnt/c/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/02/mydb/src'
	gcc -c -g -Wall btree_int.c -o btree_int.o
	gcc -c -g -Wall conds.c -o conds.o
	gcc -c -g -Wall config.c -o config.o
	gcc -c -g -Wall data.c -o data.o
	gcc -c -g -Wall db_engine.c -o db_engine.o
	gcc -c -g -Wall db_main.c -o db_main.o
	gcc -c -g -Wall error.c -o error.o
	gcc -c -g -Wall index.c -o index.o
	gcc -c -g -Wall memory.c -o memory.o
	gcc -c -g -Wall parser_commands.c -o parser_commands.o
	gcc -c -g -Wall parser_conds.c -o parser_conds.o
	gcc -c -g -Wall query.c -o query.o
	gcc -c -g -Wall result.c -o result.o
	gcc -c -g -Wall sql_parser.c -o sql_parser.o
	gcc -c -g -Wall strutil.c -o strutil.o
	gcc -c -g -Wall tabconf.c -o tabconf.o
	gcc -c -g -Wall tabdef.c -o tabdef.o
	gcc -c -g -Wall textfileio.c -o textfileio.o
	ar -rcvs libmydb.a btree_int.o conds.o config.o data.o db_engine.o db_main.o error.o index.o memory.o parser_commands.o parser_conds.o query.o result.o sql_parser.o strutil.o tabconf.o tabdef.o textfileio.o 
	a - btree_int.o
	a - conds.o
	a - config.o
	a - data.o
	a - db_engine.o
	a - db_main.o
	a - error.o
	a - index.o
	a - memory.o
	a - parser_commands.o
	a - parser_conds.o
	a - query.o
	a - result.o
	a - sql_parser.o
	a - strutil.o
	a - tabconf.o
	a - tabdef.o
	a - textfileio.o
	gcc -g -Wall  -o testadmin -L. testadmin.c  				   -lmydb  -lreadline -lhistory
	gcc -g -Wall  -o testclient -L. testclient.c client_comm.c  -lmydb  -lreadline -lhistory  
	gcc -g -Wall  -o testserver -L. testserver.c server_comm.c   -lmydb     
	make[1]: Leaving directory '/mnt/c/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/02/mydb/src'


3. Demonstrate that the dependencies work properly by invoking "make", "touch src/memory.h", "make", "touch src/testserver.c", "make". 
   Paste the output of each command here.

   I pasted only stdout.

   $ make

	make -C src all
	make[1]: Entering directory '/mnt/c/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/02/mydb/src'
	make[1]: Nothing to be done for 'all'.
	make[1]: Leaving directory '/mnt/c/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/02/mydb/src'

	$ touch src/memory.h
	$ makemake -C src all

	make -C src all
	make[1]: Entering directory '/mnt/c/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/02/mydb/src'
	gcc -c -g -Wall data.c -o data.o
	ar -rcvs libmydb.a btree_int.o conds.o config.o data.o db_engine.o db_main.o error.o index.o memory.o parser_commands.o parser_conds.o query.o result.o sql_parser.o strutil.o tabconf.o tabdef.o textfileio.o 
	r - btree_int.o
	r - conds.o
	r - config.o
	r - data.o
	r - db_engine.o
	r - db_main.o
	r - error.o
	r - index.o
	r - memory.o
	r - parser_commands.o
	r - parser_conds.o
	r - query.o
	r - result.o
	r - sql_parser.o
	r - strutil.o
	r - tabconf.o
	r - tabdef.o
	r - textfileio.o
	gcc -g -Wall  -o testadmin -L. testadmin.c  				   -lmydb  -lreadline -lhistory
	gcc -g -Wall  -o testclient -L. testclient.c client_comm.c  -lmydb  -lreadline -lhistory  
	gcc -g -Wall  -o testserver -L. testserver.c server_comm.c   -lmydb     
	make[1]: Leaving directory '/mnt/c/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/02/mydb/src'

	$ touch src/testserver.cmake -C src all
	$ make

	make[1]: Entering directory '/mnt/c/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/02/mydb/src'
	gcc -g -Wall  -o testserver -L. testserver.c server_comm.c   -lmydb     
	make[1]: Leaving directory '/mnt/c/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/02/mydb/src'


4. Send all the Makefiles together with the protocol in a single archive.
   Do not include binaries, object files, and other temporary files generated during the build.


-------
Survey:
-------

We use the survey to evaluate the labs (tasks) and homework assignments.
It is completely voluntary, but useful for future tuning of the content and level of complexity of the labs.
Most questions can be answered using the scale from 1 to 10.
Write your answer under the corresponding question.

(1) How new was the topic and content of the lab for you?
     (1 - brand new, 10 - I already knew everything)
5

(2) Do you think that the content of this lab was useful?
     (1 - useless or too trivial, 10 - very useful)
9

(3) How do you evaluate the level of complexity of the homework assignment?
     (1 - too trivial, 5-6 - just about right, 10 - too demanding or complex)
10

(4) How do you evaluate your prior experience with the tool in the context of the homework assignment?
     (1 - I already know and can use everything, 10 - I tried everything practically for the first time)
6

(5) How much time (in minutes) did you spend working on the homework assignment?
120

(6) Additional space for your own comments on the lab and homework assignment:
I found this assignment quite confusing. I think it would serve better if the source files were simpler.

