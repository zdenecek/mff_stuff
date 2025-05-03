# Install and run

First, download and unpack ActiveMQ Artemis from https://activemq.apache.org/components/artemis/download/.

Set up the broker using the command below. This configures the broker in the `broker` directory.

```sh
./apache-artemis-*/bin/artemis create --user user --password password --allow-anonymous broker
```

Start the broker using the command:

```sh
./broker/bin/artemis run
```

Now, install the project using Maven.

```sh
mvn install
```

Run the bank:

```sh
mvn exec:java@bank
```

Run a client (passing the client name argument using `-Dexec.args`):

```sh
mvn exec:java@client -Dexec.args="client1"
```
