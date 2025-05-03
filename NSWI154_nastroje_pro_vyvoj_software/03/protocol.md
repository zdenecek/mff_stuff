Zdeněk Tomis
zdnek.tomis@gmail.com 

Deadline: 13.11.2022

Requirements:
- You do not have to procotol absolutely everything, such as 'cd' commands,
  'rm -rf' when the task tells you to do a 'rm -rf', 'vim file', etc.
- Include output of commands only when explicitly requested.
- You can write answers in Czech.

Useful links and help:
  http://ant.apache.org/manual/index.html
  http://maven.apache.org/


-----
Tasks
-----

This assignment has three alternatives - Ant, Maven and MSBuild+NuGet.
You should pick just one based on your favorite platform and programming language.
Send all the build scripts and configuration files along with a complete report.

The "ftp" directory contains two subdirectories: "src" with the sources of a very simple
FTP server and "lib" with few JAR files representing external dependencies (libraries).
The main class is "org.apache.ftpserver.commandline.CommandLine".


Maven: Write the Maven build script for the FTP server - the file "ftp/pom.xml".

   - The script has to support the following actions: building, creating of a JAR file, run/execute, clean.

   - Make sure that everything works as expected.

   - Follow the standard concepts and best practices associated with Maven.

   - Test the Maven build script and paste the output of Maven here.


**Targets produce warnings which are caused by the source files and the way the dependies are specified (part of the task).**

```
$ mvn compile
[INFO] Scanning for projects...
[WARNING] 
[WARNING] Some problems were encountered while building the effective model for java.org.apache.ftpserver:apache-ftpserver:jar:1.0
[WARNING] 'dependencies.dependency.systemPath' for commons-logging:commons-logging:jar should not point at files within the project directory, ${project.basedir}/lib/commons-logging-1.
0.3.jar will be unresolvable by dependent projects @ line 26, column 25
[WARNING] 'dependencies.dependency.systemPath' for log4j:log4j:jar should not point at files within the project directory, ${project.basedir}/lib/log4j-1.2.12.jar will be unresolvable 
by dependent projects @ line 33, column 25
[WARNING]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO]
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ apache-ftpserver ---
[INFO] Deleting C:\Users\zdnek\unirepo\nastroje_pro_vyvoj_software\03\ftp\build
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.492 s
[INFO] Finished at: 2022-11-13T20:24:45+01:00
[INFO] ------------------------------------------------------------------------
PS C:\Users\zdnek\unirepo\nastroje_pro_vyvoj_software\03\ftp> mvn compile
[INFO] Scanning for projects...
[WARNING] 
[WARNING] Some problems were encountered while building the effective model for java.org.apache.ftpserver:apache-ftpserver:jar:1.0
[WARNING] 'dependencies.dependency.systemPath' for commons-logging:commons-logging:jar should not point at files within the project directory, ${project.basedir}/lib/commons-logging-1.
0.3.jar will be unresolvable by dependent projects @ line 26, column 25
[WARNING] 'dependencies.dependency.systemPath' for log4j:log4j:jar should not point at files within the project directory, ${project.basedir}/lib/log4j-1.2.12.jar will be unresolvable 
by dependent projects @ line 33, column 25
[WARNING]
[WARNING] It is highly recommended to fix these problems because they threaten the stability of your build.
[WARNING]
[WARNING] For this reason, future Maven versions might no longer support building such malformed projects.
[WARNING]
[INFO] 
[INFO] -------------< java.org.apache.ftpserver:apache-ftpserver >-------------
[INFO] Building Apache Ftp Server 1.0
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ apache-ftpserver ---
[INFO] Using 'utf-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ apache-ftpserver ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 139 source files to C:\Users\zdnek\unirepo\nastroje_pro_vyvoj_software\03\ftp\build\classes
[WARNING] /C:/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/03/ftp/src/main/java/org/apache/ftpserver/FtpConfigImpl.java: Some input files use or override a deprecated API.
[WARNING] /C:/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/03/ftp/src/main/java/org/apache/ftpserver/FtpConfigImpl.java: Recompile with -Xlint:deprecation for details.
[WARNING] /C:/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/03/ftp/src/main/java/org/apache/ftpserver/iprestrictor/FileIpRestrictor.java: Some input files use or override a deprecate
d API that is marked for removal.
[WARNING] /C:/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/03/ftp/src/main/java/org/apache/ftpserver/iprestrictor/FileIpRestrictor.java: Recompile with -Xlint:removal for details.  
[WARNING] /C:/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/03/ftp/src/main/java/org/apache/ftpserver/RequestHandler.java: Some input files use unchecked or unsafe operations.       
[WARNING] /C:/Users/zdnek/unirepo/nastroje_pro_vyvoj_software/03/ftp/src/main/java/org/apache/ftpserver/RequestHandler.java: Recompile with -Xlint:unchecked for details.
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  4.441 s
[INFO] Finished at: 2022-11-13T20:27:05+01:00
[INFO] ------------------------------------------------------------------------
```


```
$ mvn package
[INFO] Scanning for projects...
[WARNING] 
[WARNING] Some problems were encountered while building the effective model for java.org.apache.ftpserver:apache-ftpserver:jar:1.0
[WARNING] 'dependencies.dependency.systemPath' for commons-logging:commons-logging:jar should not point at files within the project directory, ${project.basedir}/lib/commons-logging-1.
0.3.jar will be unresolvable by dependent projects @ line 26, column 25
[WARNING] 'dependencies.dependency.systemPath' for log4j:log4j:jar should not point at files within the project directory, ${project.basedir}/lib/log4j-1.2.12.jar will be unresolvable 
by dependent projects @ line 33, column 25
[WARNING]
[WARNING] It is highly recommended to fix these problems because they threaten the stability of your build.
[WARNING]
[WARNING] For this reason, future Maven versions might no longer support building such malformed projects.
[WARNING]
[INFO]
[INFO] -------------< java.org.apache.ftpserver:apache-ftpserver >-------------
[INFO] Building Apache Ftp Server 1.0
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ apache-ftpserver ---
[INFO] Using 'utf-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO]
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ apache-ftpserver ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ apache-ftpserver ---
[INFO] Using 'utf-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory C:\Users\zdnek\unirepo\nastroje_pro_vyvoj_software\03\ftp\src\test\resources
[INFO]
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ apache-ftpserver ---
[INFO] Nothing to compile - all classes are up to date
[INFO]
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ apache-ftpserver ---
[INFO] No tests to run.
[INFO]
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ apache-ftpserver ---
[INFO] Building jar: C:\Users\zdnek\unirepo\nastroje_pro_vyvoj_software\03\ftp\build\apache-ftpserver-1.0.jar
[INFO] 
[INFO] --- maven-assembly-plugin:2.2-beta-5:single (make-assembly) @ apache-ftpserver ---
[INFO] Building jar: C:\Users\zdnek\unirepo\nastroje_pro_vyvoj_software\03\ftp\build\apache-ftpserver-1.0-jar-with-dependencies.jar
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  3.106 s
[INFO] Finished at: 2022-11-13T20:34:42+01:00
[INFO] ------------------------------------------------------------------------
```


```
$ mvn exec:java
[INFO] Scanning for projects...
[WARNING] 
[WARNING] Some problems were encountered while building the effective model for java.org.apache.ftpserver:apache-ftpserver:jar:1.0
[WARNING] 'dependencies.dependency.systemPath' for commons-logging:commons-logging:jar should not point at files within the project directory, ${project.basedir}/lib/commons-logging-1.
0.3.jar will be unresolvable by dependent projects @ line 26, column 25
[WARNING] 'dependencies.dependency.systemPath' for log4j:log4j:jar should not point at files within the project directory, ${project.basedir}/lib/log4j-1.2.12.jar will be unresolvable 
by dependent projects @ line 33, column 25
[WARNING] 'build.plugins.plugin.version' for org.codehaus.mojo:exec-maven-plugin is missing. @ line 67, column 21
[WARNING]
[WARNING] It is highly recommended to fix these problems because they threaten the stability of your build.
[WARNING]
[WARNING] For this reason, future Maven versions might no longer support building such malformed projects.
[WARNING]
[INFO] 
[INFO] -------------< java.org.apache.ftpserver:apache-ftpserver >-------------
[INFO] Building Apache Ftp Server 1.0
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- exec-maven-plugin:3.1.0:java (default-cli) @ apache-ftpserver ---
Usage: java org.apache.ftpserver.FtpServer <options>
  <options> := -default |
               -xml <XML configuration file> |
               -prop <properties configuration file>

There are three ways to start the FTP server.
    -default :: default configuration will be used.
    -xml     :: XML configuration will be used. User has to specify the file.
    -prop    :: properties configuration will be used. User has to specify the file.
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.454 s
[INFO] Finished at: 2022-11-13T20:31:00+01:00
[INFO] ------------------------------------------------------------------------
[INFO] Scanning for projects...
[WARNING] 
[WARNING] Some problems were encountered while building the effective model for java.org.apache.ftpserver:apache-ftpserver:jar:1.0
[WARNING] 'dependencies.dependency.systemPath' for commons-logging:commons-logging:jar should not point at files within the project directory, ${project.basedir}/lib/commons-logging-1.
0.3.jar will be unresolvable by dependent projects @ line 26, column 25
[WARNING] 'dependencies.dependency.systemPath' for log4j:log4j:jar should not point at files within the project directory, ${project.basedir}/lib/log4j-1.2.12.jar will be unresolvable 
by dependent projects @ line 33, column 25
[WARNING] 'build.plugins.plugin.version' for org.codehaus.mojo:exec-maven-plugin is missing. @ line 67, column 21
[WARNING]
[WARNING] It is highly recommended to fix these problems because they threaten the stability of your build.
[WARNING]
[WARNING] For this reason, future Maven versions might no longer support building such malformed projects.
[WARNING]
[INFO] 
[INFO] -------------< java.org.apache.ftpserver:apache-ftpserver >-------------
[INFO] Building Apache Ftp Server 1.0
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- exec-maven-plugin:3.1.0:java (default-cli) @ apache-ftpserver ---
Usage: java org.apache.ftpserver.FtpServer <options>
  <options> := -default |
               -xml <XML configuration file> |
               -prop <properties configuration file>

There are three ways to start the FTP server.
    -default :: default configuration will be used.
    -xml     :: XML configuration will be used. User has to specify the file.
    -prop    :: properties configuration will be used. User has to specify the file.
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.454 s
[INFO] Finished at: 2022-11-13T20:31:00+01:00
[INFO] ------------------------------------------------------------------------
```


```
$ mvn clean
[INFO] Scanning for projects...
[WARNING] 
[WARNING] Some problems were encountered while building the effective model for java.org.apache.ftpserver:apache-ftpserver:jar:1.0
[WARNING] 'dependencies.dependency.systemPath' for commons-logging:commons-logging:jar should not point at files within the project directory, ${project.basedir}/lib/commons-logging-1.
0.3.jar will be unresolvable by dependent projects @ line 26, column 25
[WARNING] 'dependencies.dependency.systemPath' for log4j:log4j:jar should not point at files within the project directory, ${project.basedir}/lib/log4j-1.2.12.jar will be unresolvable 
by dependent projects @ line 33, column 25
[WARNING]
[WARNING] It is highly recommended to fix these problems because they threaten the stability of your build.
[WARNING]
[WARNING] For this reason, future Maven versions might no longer support building such malformed projects.
[WARNING]
[INFO]
[INFO] -------------< java.org.apache.ftpserver:apache-ftpserver >-------------
[INFO] Building Apache Ftp Server 1.0
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ apache-ftpserver ---
[INFO] Deleting C:\Users\zdnek\unirepo\nastroje_pro_vyvoj_software\03\ftp\build
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  0.491 s
[INFO] Finished at: 2022-11-13T20:35:15+01:00
[INFO] ------------------------------------------------------------------------
```


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
2

(3) How do you evaluate the level of complexity of the homework assignment?
     (1 - too trivial, 5-6 - just about right, 10 - too demanding or complex)


(4) How do you evaluate your prior experience with the tool in the context of the homework assignment?
     (1 - I already know and can use everything, 10 - I tried everything practically for the first time)


(5) How much time (in minutes) did you spend working on the homework assignment?


(6) Additional space for your own comments on the lab and homework assignment:
 
