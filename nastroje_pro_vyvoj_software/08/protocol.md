Zdeněk Tomis
zdnek.tomis@gmail.com

Deadline: 11.12.2022

Requirements:
- You do not have to procotol absolutely everything, such as 'cd' commands,
  'rm -rf' when the task tells you to do a 'rm -rf', 'vim file', etc.
- Include output of commands only when explicitly requested.
- You can write answers in Czech.

Useful links and help:
  http://logging.apache.org/log4j/2.x/
  http://www.gnu.org/software/libc/manual/html_node/Syslog.html
  man pages of respective tools


-----
Tasks
-----

0. Tasks 1-3 have two alternatives - Java and C#/.NET.
   You should pick just one based on your favorite platform and programming language.

1/Java. The directory "daisyfs" contains a very simple concurrent file system written in Java.
        A short description and the list of relevant operations (API) is in the file "daisy.txt".
        The main class is "DaisyTest". It creates two threads which perform various operations.

2/Java. Implement logging into the "daisyfs" program. Use the Log4j library (version 2).
        Configure the Log4j library such that log messages are saved into the file "events.log".
        Log the following actions:
          - creating a new file or directory
          - opening some directory
          - closing the given directory
        For each action, record this information: 
          - a file name or inode number
          - current time (up to seconds)
          - number of the current thread
          - result (success, error status)
        Report errors also to console. Use filters on messages.

3/Java: Download the package http://d3s.mff.cuni.cz/files/teaching/nswi154/jpf-elevator.tgz, unpack, and execute using the available script run.sh.
        I tested it just on some Linux machines in the university lab and my private computer, but it should work on any common Linux distribution with recent Java installed.
        If the program does not work on the current release of Java then please try Java 8 if you can (or write me and I will update the archive jpf-elevator.tgz).
            Add full path to the Java 8 "bin" directory to invocation of "java" specified in run.sh.
        Then use the "VisualVM" tool to get the list of Java classes sorted by the size of memory (heap) fragments taken by all their instances together.
        Record the list of Java classes roughly 5 minutes after you start the jpf-elevator process.
        You can provide just around top 20 classes (bit more or less is fine), depending on how many records fit on the screen of your computer when you run jvisualvm.
        Note that the package jpf-elevator.tgz has quite large overall memory requirements (certainly more than 1 GB).
        Useful information: https://visualvm.github.io/

1/C#. The directory "deos" contains a very simple model of an operating system written in C#.
      The main class is "ChessTest". It starts the main loop of the operating system.
      Note there is no solution (project file) because the one distributed with the package was broken, so I deleted it.

Note: The chess test failed on my computer after starting three threads and panicked, see stdout. I went along as this could be a part if the task.

2/C#. Implement logging into the "deos" program. Use any framework available for C#/.NET (e.g., log4net or NLog).
      Configure the given framework such that log messages are saved into the file "events.log".
      Log the following events:
        - creating a new thread
        - start of some thread
        - termination of some thread
      For each event, record this information:
        - thread name
        - current time
        - result (success, error status)
      Report errors also to console. Use filters on messages.

I attached the events.log and a copy of stdout to the protocol.

3/C#: Use the .NET Memory Profiler (that should be available in recent Visual Studio) on the DEOS package to get the total amount of memory used by all instances of each individual class.
      Record the information (in the form of a screenshot or text file) at some point during the program execution.
      The profiler can be downloaded here: https://marketplace.visualstudio.com/items?itemName=SciTechSoftware.NETMemoryProfiler.
      If the tool does not work (or does not have the needed functionality) then you can try some other profiler for C#/.NET. A good option might be dotTrace (http://www.jetbrains.com/profiler/).

The recording can be fount in deos_memory_snapshot.txt

4. The directory "sockets" contains sources for a simple network client and server,
   and the script "build.sh" that can be used to compile everything.
   This software package was already used in lecture 3.


5. Implement logging into the "sockets" program. Use the syslog API.
   Record the IP address of each client that connects to the server.
   Also log the length of each message received from the clients.



6. Count the number of invocations of each system call and the number of invocations of each library call during execution of the server.
   You can just run the server and kill it when the message "Waiting for TCP ..." appears.
   Make also sure that names of library methods are printed in readable format (not mangled).
       Hint: use abilities of the respective tools as much as possible for this purpose, instead of shell scripts and basic linux utilities (grep, wc).
   If the tool of your choice does not show library calls, then you can try to add the "-no-pie" flag to the compiler script of the "sockets" program.
   Include all the commands that you used to fulfill this task.

    > echo 8002 | strace -c ./bin/server 2>trace.txt

% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 54.78    0.002825         565         5           openat
 12.64    0.000652          29        22           mmap
  7.37    0.000380          54         7           mprotect
  3.84    0.000198          33         6           pread64
  3.30    0.000170          24         7           fstat
  3.28    0.000169          56         3           write
  2.85    0.000147          29         5           read
  2.11    0.000109          36         3           brk
  1.92    0.000099          19         5           close
  1.84    0.000095          95         1           munmap
  1.57    0.000081          40         2           socket
  1.43    0.000074          74         1         1 accept
  0.95    0.000049          49         1           connect
  0.83    0.000043          43         1           bind
  0.66    0.000034          34         1           listen
  0.62    0.000032          16         2         1 arch_prctl
  0.00    0.000000           0         1         1 access
  0.00    0.000000           0         1           execve
------ ----------- ----------- --------- --------- ----------------
100.00    0.005157                    74         3 total

    > echo 8002 | ltrace -c  ./bin/server 2>ltrace.txt

    output:

    % time     seconds  usecs/call     calls      function
------ ----------- ----------- --------- --------------------
 15.47    0.001042        1042         1 std::istream::operator>>(unsigned short&)
 12.07    0.000813         813         1 socket
 10.13    0.000682         682         1 openlog
 10.13    0.000682         682         1 std::ostream::operator<<(std::ostream& (*)(std::ostream&))
  9.43    0.000635         635         1 htonl
  8.92    0.000601         601         1 std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
  8.15    0.000549         549         1 bind
  7.29    0.000491         491         1 std::ios_base::Init::Init()
  5.76    0.000388         388         1 listen
  4.47    0.000301         301         1 htons
  4.26    0.000287         287         1 __cxa_atexit
  3.91    0.000263         263         1 std::ostream::operator<<(unsigned short)
------ ----------- ----------- --------- --------------------
100.00    0.006734                    12 total


1. Submit an archive containing the protocol, modified sources (complete, not just
   a diff with your changes), all necessary libraries, and configuration files.


-------
Survey:
-------

We use the survey to evaluate the labs (tasks) and homework assignments.
It is completely voluntary, but useful for future tuning of the content and level of complexity of the labs.
Most questions can be answered using the scale from 1 to 10.
Write your answer under the corresponding question.

(1) How new was the topic and content of the lab for you?
     (1 - brand new, 10 - I already knew everything)


(2) Do you think that the content of this lab was useful?
     (1 - useless or too trivial, 10 - very useful)


(3) How do you evaluate the level of complexity of the homework assignment?
     (1 - too trivial, 5-6 - just about right, 10 - too demanding or complex)


(4) How do you evaluate your prior experience with the tool in the context of the homework assignment?
     (1 - I already know and can use everything, 10 - I tried everything practically for the first time)


(5) How much time (in minutes) did you spend working on the homework assignment?


(6) Additional space for your own comments on the lab and homework assignment:
 

