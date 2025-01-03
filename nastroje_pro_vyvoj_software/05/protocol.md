Zdeněk Tomis
zdnek.tomis@gmail.com 

Deadline: 27.11.2022

Requirements:
- You do not have to procotol absolutely everything, such as 'cd' commands,
  'rm -rf' when the task tells you to do a 'rm -rf', 'vim file', etc.
- Include output of commands only when explicitly requested.
- You can write answers in Czech.

Useful links and help:
  http://junit.org/junit5/
  https://github.com/junit-team/junit/wiki


-----
Tasks
-----

0. You need to download all the required libraries from the web or from the Maven repository.

1. Write unit tests for the class java.util.TreeMap.
   See http://docs.oracle.com/javase/8/docs/api/java/util/TreeMap.html.

   In the case you plan to use a different language (C#, C/C++, Python, ...), find the corresponding class with similar behavior (e.g., SortedDictionary for C#) and then adapt tasks 1-3 for that particular class (e.g., for C# replace "TreeMap" with "SortedDictionary" everywhere, and so on).

   Cover the following basic use cases:
     - adding new mapping with the key different from all present mappings
     - adding new mapping with a key equal to some already present mapping
     - removing some element (mapping) already present in the map
     - removing all elements from the map and checking for emptiness
     - getting the value associated with a specific key
     - attempt to get a value for a key not present in the map

   Always verify the current state of the map (content, size) and, if necessary, also the state of the associated iterator.

   Note that the Java package of your test classes must be different than "java.util", because application programs cannot modify or extend this package.
   This is not a problem since your tests will be calling only the public methods (you will make black-box tests).

   Follow the recommended practices for writing unit tests.

2. Build all tests for TreeMap and verify that they pass.
   Run tests from the command line, either by calling the appropriate runner class or use the "main" method in your test class (paste the command and test output here).
	
```
$ mvn test 
[INFO] Scanning for projects...
	
[INFO] 
[INFO] -------------------------< org.example:task05 >-------------------------
[INFO] Building task05 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ task05 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ task05 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ task05 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory C:\Users\zdnek\unirepo\nastroje_pro_vyvoj_software\05\task05\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ task05 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.22.0:test (default-test) @ task05 ---
[INFO] 
[INFO] -------------------------------------------------------
[INFO]  T E S T S
[INFO] -------------------------------------------------------
[INFO] Running org.example.TreeTest
[INFO] Tests run: 6, Failures: 0, Errors: 0, Skipped: 0, Time elapsed: 0.077 s - in org.example.TreeTest
[INFO] 
[INFO] Results:
[INFO]
[INFO] Tests run: 6, Failures: 0, Errors: 0, Skipped: 0
[INFO]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  3.192 s
[INFO] Finished at: 2022-11-26T23:08:46+01:00
[INFO] ------------------------------------------------------------------------  
```  


1. Add another test for TreeMap that fails and prints a custom error message.
   For example, you can assert that the map has a key-value mapping which it does not really have.
   Run the tests and paste the output here. 
   In the output, mark the line of the output that contains the custom error message, and mark the line that identifies the place in the test code that has failed.

```
 
$ mvn test
[INFO] Scanning for projects...
[INFO] 
[INFO] -------------------------< org.example:task05 >-------------------------
[INFO] Building task05 1.0-SNAPSHOT
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ task05 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ task05 ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ task05 ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory C:\Users\zdnek\unirepo\nastroje_pro_vyvoj_software\05\task05\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ task05 ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 1 source file to C:\Users\zdnek\unirepo\nastroje_pro_vyvoj_software\05\task05\target\test-classes
[INFO] 
[INFO] --- maven-surefire-plugin:2.22.0:test (default-test) @ task05 ---
[INFO] 
[INFO] -------------------------------------------------------
[INFO]  T E S T S
[INFO] -------------------------------------------------------
[INFO] Running org.example.TreeTest
(the line that identifies the place in the test code that has failed)
[ERROR] Tests run: 7, Failures: 1, Errors: 0, Skipped: 0, Time elapsed: 0.067 s <<< FAILURE! - in org.example.TreeTest
org.opentest4j.AssertionFailedError: Map should contain key Hi ==> expected: <true> but was: <false>         
        at org.example.TreeTest.checkNonexistentMapping(TreeTest.java:118)

[INFO] 
[INFO] Results:
[INFO]
[ERROR] Failures: 
 [ERROR] checkNonexistentMapping  Time elapsed: 0.012 s  <<< FAILURE!
( the line of the output that contains the custom error message)[ERROR]   TreeTest.checkNonexistentMapping:118 Map should contain key Hi ==> expected: <true> but was: <false>
[INFO]
[ERROR] Tests run: 7, Failures: 1, Errors: 0, Skipped: 0
[INFO]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD FAILURE
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  4.043 s
[INFO] Finished at: 2022-11-26T23:12:41+01:00
[INFO] ------------------------------------------------------------------------
[ERROR] Failed to execute goal org.apache.maven.plugins:maven-surefire-plugin:2.22.0:test (default-test) on project task05: There are test failures.
[ERROR]
[ERROR] Please refer to C:\Users\zdnek\unirepo\nastroje_pro_vyvoj_software\05\task05\target\surefire-reports for the individual test results.
[ERROR] Please refer to dump files (if any exist) [date]-jvmRun[N].dump, [date].dumpstream and [date]-jvmRun[N].dumpstream.
[ERROR] -> [Help 1]
[ERROR]
[ERROR] To see the full stack trace of the errors, re-run Maven with the -e switch.
[ERROR] Re-run Maven using the -X switch to enable full debug logging.
[ERROR]
[ERROR] For more information about the errors and possible solutions, please read the following articles:
[ERROR] [Help 1] http://cwiki.apache.org/confluence/display/MAVEN/MojoFailureException
```

1. Define interface Employees with the following methods in your favorite language (Java, C#, C/C++, Python, ...).
   List of required methods:
     int add(String name, int salary); // returns ID
     Set<Integer> getAll(); // returns a set of IDs
     String getName(int id);
     int getSalary(int id);
     void changeSalary(int id, int newSalary);
   It represents a simple database of employees.
   You do not have to create a real implementation of this interface (e.g., using an SQL database).




2. Define and implement the class Statistics that satisfies the following requirements. Use your favorite language (Java, C#, C/C++, Python, ...).
   Constructor should have an argument of type Employees that represents an instance to be used inside the class.
   List of required methods that you should implement:
     int computeAverageSalary();
     int getMinSalary();
     void printSalariesByName(); // prints the list of pairs <name, salary> that is sorted by employee names

3. Write unit tests for methods of the Statistics class in any unit testing framework that is available for the selected language.
   In the unit tests, properly capture (model) the dependency on the instance of Employees given to the constructor.
   Define reasonable properties for the three methods and check whether they hold (using assertions in your unit tests).
   You should create unit tests that really check behavior of methods in the Statistics class (i.e., an empty stub implementation of the Employee interface is not sufficient).



4. Write unit tests for selected methods in the class java.text.SimpleDateFormat (http://docs.oracle.com/javase/8/docs/api/java/text/SimpleDateFormat.html), respectively for the corresponding class in the standard library of the language that you choose (C#, C/C++, Python, ...). For C#, it should be the DateTime class.
   You need to cover only the constructor that accepts "String pattern", the method "String format(Date)", and the method "Date parse(String)". Note that methods "format" and "parse" are defined in the superclass.
   Use advanced features of JUnit, respectively the framework of your choice for other languages, especially parameterized tests.

5. Submit an archive containing your tests (source code) and the build scripts along with the protocol.


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
  
 

