# Build and run

Build the project with Maven:

```sh
mvn install
```

```sh
mvn compile exec:exec@run-server -DargLine="-Xss128m"
```


Run a local benchmark

```sh
mvn compile exec:java@run-client -Dexec.args="cz.cuni.mff.d3s.nswi080.testsuite.suites.LocalTestSuite"
```

Specify custom params

```sh
mvn -q compile exec:java@run-client -Dexec.args="cz.cuni.mff.d3s.nswi080.testsuite.suites.BothRemoteSuite [nodes] [edges,every case divided by comma] [RMI URL] [Searcher depth]
```
