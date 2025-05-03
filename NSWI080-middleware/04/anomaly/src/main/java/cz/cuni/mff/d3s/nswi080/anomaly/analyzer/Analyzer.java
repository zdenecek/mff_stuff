package cz.cuni.mff.d3s.nswi080.anomaly.analyzer;

import java.time.Duration;
import java.time.Instant;
import java.util.List;
import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.common.TopicPartition;
import org.apache.kafka.common.serialization.StringDeserializer;
import org.apache.kafka.common.serialization.StringSerializer;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import cz.cuni.mff.d3s.nswi080.anomaly.DoubleDeserializer;
import cz.cuni.mff.d3s.nswi080.anomaly.JacksonKafkaSerializer;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.stream.Collectors;

/**
 * The analyzer program.
 */
public class Analyzer {
    /**
     * The topic with monitor measurements.
     */
    private static final String MONITOR_TOPIC = "monitor";

    /**
     * The topic with detected anomalies.
     */
    private static final String ANOMALY_TOPIC = "anomaly";

    /**
     * The topic with analyzer states and consumer partition offsets.
     */
    public static final String ANALYZER_STATE_TOPIC = "analyzer-state";

    /**
     * The period at which analyzer states and consumer partition offsets are manually committed.
     */
    public static final Duration COMMIT_PERIOD = Duration.ofSeconds(30);

    public static final Map<String, Integer> hostnamePartitionMapping = new ConcurrentHashMap<>();

    public static void main(String[] args) {
        if (args.length == 0) {
            throw new IllegalArgumentException("Usage: <program> hostname");
        }
        Logger logger = new Logger(args[0]);
        String bootstrapServers = "localhost:9092";

        // part 1: Create the producers and consumers.
        Map<String, AnomalyAnalyzer> analyzers = new HashMap<>();
        java.util.Properties consumerProps = new java.util.Properties();
        consumerProps.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
        consumerProps.put(ConsumerConfig.GROUP_ID_CONFIG, "anomaly-analyzers");
        consumerProps.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class.getName());
        consumerProps.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, DoubleDeserializer.class.getName());
        consumerProps.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "earliest");
        consumerProps.put(ConsumerConfig.ENABLE_AUTO_COMMIT_CONFIG, "false"); 
        consumerProps.put(ConsumerConfig.GROUP_INSTANCE_ID_CONFIG, args[0]); 

        // Create the anomaly producer. 
        java.util.Properties producerProps = new java.util.Properties();
        producerProps.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
        producerProps.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, StringSerializer.class.getName());
        producerProps.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, JacksonKafkaSerializer.class.getName());
        // Create the state producer.
        java.util.Properties stateProducerProps = new java.util.Properties();
        stateProducerProps.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
        stateProducerProps.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, StringSerializer.class.getName());
        stateProducerProps.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, JacksonKafkaSerializer.class.getName());
        try (
            KafkaConsumer<String, Double> consumer = new KafkaConsumer<>(consumerProps);
            KafkaProducer<String, Anomaly> anomalyProducer = new KafkaProducer<>(producerProps);
            KafkaProducer<String, AnalyzerStateValue> stateProducer = new KafkaProducer<>(stateProducerProps)
        ) {
            // (part 1): Subscribe to MONITOR_TOPIC and call logger#subscribingToMonitorTopic.
            // Subscribe to MONITOR_TOPIC with the MonitorRebalanceListener.
            consumer.subscribe(java.util.List.of(MONITOR_TOPIC), new MonitorRebalanceListener(logger, consumer, analyzers, stateProducer, ANALYZER_STATE_TOPIC));
            logger.subscribingToMonitorTopic();
            Instant lastCommit = Instant.now();
            while (true) {
                ConsumerRecords<String, Double> records = consumer.poll(Duration.ofMillis(100));
                for (ConsumerRecord<String, Double> record : records) {
                    // (part 1): Read the measurements and call logger#monitorRecordRead(record) for each.
                    logger.monitorRecordRead(record);
                    String hostname = record.key();
                    AnomalyAnalyzer analyzer = analyzers.computeIfAbsent(hostname, AnomalyAnalyzer::new);
                    double temperature = record.value();
                    long timestamp = record.timestamp();
                    hostnamePartitionMapping.put(hostname, record.partition());
                    Anomaly anomaly = analyzer.addRecordAndDetect(temperature, timestamp);
                    if (anomaly != null) {
                        logger.anomalyDetected(anomaly);
                        ProducerRecord<String, Anomaly> anomalyRecord = new ProducerRecord<>(ANOMALY_TOPIC, hostname, anomaly);
                        anomalyProducer.send(anomalyRecord);
                    }
                }
                // TODO (part 3): Save the analyzer states and consumer positions for all assigned partitions
                //  if it is more than COMMIT_PERIOD since they were saved last.
                if (Instant.now().isAfter(lastCommit.plus(COMMIT_PERIOD))) {
                    // TODO (part 3): Call logger#periodicCommitOfStatesAndOffsets at the beginning.
                    logger.periodicCommitOfStatesAndOffsets();
                    var partitions = consumer.assignment();
                    for (TopicPartition partition : partitions) {
                        var offset = consumer.position(partition);
                        saveStateForPartition(partition, offset, analyzers, stateProducer,  ANALYZER_STATE_TOPIC);
                        logger.monitorPartitionStateCommitted(partition.partition(), offset, analyzers.values());
                    }
                    lastCommit = Instant.now();
                }
            }
        }
    }
    /**
     * Saves the state for a specific partition to Kafka.
     * 
     */
    public static void saveStateForPartition(TopicPartition partition, long offset, Map<String, AnomalyAnalyzer> analyzers, KafkaProducer<String, AnalyzerStateValue> stateProducer, String analyzerStateTopic) {
       //(method is used by both MonitorRebalanceListener and Analyzer)
        Map<String, AnomalyAnalyzer> partitionAnalyzers = analyzers.entrySet().stream()
                .filter(e -> {
                    Integer mappedPartition = Analyzer.hostnamePartitionMapping.get(e.getKey());
                    return mappedPartition != null && mappedPartition == partition.partition();
                })
                .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));
            String stateKey = "monitor-" + partition.partition();
            AnalyzerStateValue stateValue = new AnalyzerStateValue(partitionAnalyzers, offset);
            stateProducer.send(new ProducerRecord<>(analyzerStateTopic, stateKey, stateValue));
    }
}
