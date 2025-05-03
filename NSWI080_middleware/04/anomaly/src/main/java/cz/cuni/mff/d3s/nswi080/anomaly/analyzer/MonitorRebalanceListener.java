package cz.cuni.mff.d3s.nswi080.anomaly.analyzer;

import org.apache.kafka.clients.consumer.ConsumerRebalanceListener;
import org.apache.kafka.common.TopicPartition;
import org.apache.kafka.common.serialization.StringDeserializer;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerRecord;
import java.util.Collection;
import java.util.Map;
import java.util.stream.Collectors;
import org.apache.kafka.clients.consumer.ConsumerConfig;

/**
 * Listens to monitor partition reassignment events and stores/restores the partition offsets and analyzer states.
 */
class MonitorRebalanceListener implements ConsumerRebalanceListener {
    private final Logger logger;
    private final KafkaConsumer<String, Double> consumer;
    private final Map<String, AnomalyAnalyzer> analyzers;
    private final KafkaProducer<String, AnalyzerStateValue> stateProducer;
    private final String analyzerStateTopic;
    private final KafkaConsumer<String, AnalyzerStateValue> stateConsumer;

    MonitorRebalanceListener(Logger logger, KafkaConsumer<String, Double> consumer, Map<String, AnomalyAnalyzer> analyzers, KafkaProducer<String, AnalyzerStateValue> stateProducer, String analyzerStateTopic) {
        this.logger = logger;
        this.consumer = consumer;
        this.analyzers = analyzers;
        this.stateProducer = stateProducer;
        this.analyzerStateTopic = analyzerStateTopic;
        java.util.Properties stateConsumerProps = new java.util.Properties();
        stateConsumerProps.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, "localhost:9092");
        stateConsumerProps.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class.getName());
        stateConsumerProps.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, AnalyzerStateValue.AnalyzerStateValueDeserializer.class.getName());
        stateConsumerProps.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "earliest");
        this.stateConsumer = new KafkaConsumer<>(stateConsumerProps);
    }

    @Override
    public void onPartitionsRevoked(Collection<TopicPartition> partitions) {
        for (TopicPartition partition : partitions) {
            long offset = consumer.position(partition);
            Analyzer.saveStateForPartition(partition, offset, analyzers, stateProducer, analyzerStateTopic);
            logger.monitorPartitionStateCommitted(partition.partition(), offset, analyzers.values());
            
        }
        logger.monitorPartitionsRevoked(partitions);
    }

    @Override
    public void onPartitionsAssigned(Collection<TopicPartition> monitorPartitions) {
        var partitionInfo = stateConsumer.partitionsFor(analyzerStateTopic);
        if (partitionInfo == null || partitionInfo.isEmpty()) {
            logger.monitorPartitionsAssigned(monitorPartitions);
            return;
        }
        var statePartitions = partitionInfo.stream()
            .map(info -> new TopicPartition(info.topic(), info.partition()))
            .collect(Collectors.toList());
        stateConsumer.assign(statePartitions);
        stateConsumer.seekToBeginning(statePartitions);

        var monitorPartitionKeys = monitorPartitions.stream()
            .collect(Collectors.toMap(
                topicPartition -> "monitor-" + topicPartition.partition(),
                topicPartition -> topicPartition
            ));

        while (true) {
            var records = stateConsumer.poll(java.time.Duration.ofMillis(100));
            if (records.isEmpty()) break;
            for (var record : records) {
                TopicPartition monitorPartition = monitorPartitionKeys.get(record.key());
                if (monitorPartition != null) {
                    var state = record.value();
                    analyzers.putAll(state.analyzers());
                    consumer.seek(monitorPartition, state.offset());
                    logger.monitorPartitionStateRestored(
                        monitorPartition.partition(), state.offset(), state.analyzers().values()
                    );
                }
            }
        }
        logger.monitorPartitionsAssigned(monitorPartitions);
    }
}
