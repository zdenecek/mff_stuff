# Anomaly Assignment

Start a Kafka broker.

```shell
docker run --rm \
     --name kafka-broker \
     -p 9092:9092 \
     -e KAFKA_NODE_ID=1 \
     -e KAFKA_PROCESS_ROLES=broker,controller \
     -e KAFKA_LISTENERS=PLAINTEXT://:9092,CONTROLLER://:9093 \
     -e KAFKA_ADVERTISED_LISTENERS=PLAINTEXT://localhost:9092 \
     -e KAFKA_CONTROLLER_LISTENER_NAMES=CONTROLLER \
     -e KAFKA_LISTENER_SECURITY_PROTOCOL_MAP=CONTROLLER:PLAINTEXT,PLAINTEXT:PLAINTEXT \
     -e KAFKA_CONTROLLER_QUORUM_VOTERS=1@localhost:9093 \
     -e KAFKA_OFFSETS_TOPIC_REPLICATION_FACTOR=1 \
     -e KAFKA_TRANSACTION_STATE_LOG_REPLICATION_FACTOR=1 \
     -e KAFKA_TRANSACTION_STATE_LOG_MIN_ISR=1 \
     -e KAFKA_GROUP_INITIAL_REBALANCE_DELAY_MS=0 \
     -e KAFKA_NUM_PARTITIONS=3 \
     apache/kafka:4.0.0
```

Install the dependencies and compile the project.

```shell
mvn install
```

Run a monitor, passing a hostname as an argument.

```shell
mvn exec:java@monitor -Dexec.args="hostname"
```

Run an analyzer, passing a hostname as an argument.

```shell
mvn exec:java@analyzer -Dexec.args="hostname"
```
