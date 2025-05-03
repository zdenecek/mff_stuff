# Install and run

Install the project:

```sh
mvn install
```

Run a client and pass the username using `-Dexec.args=`.

```sh
mvn exec:java@client -Dexec.args=username
```
