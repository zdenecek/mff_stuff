package cz.cuni.mff.d3s.nswi080.anomaly.analyzer;

import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.common.TopicPartition;

import java.time.format.DateTimeFormatter;
import java.util.Collection;

/**
 * Logs the most important events to the standard output.
 * <p>
 * You should invoke all public methods with appropriate arguments.
 * <p>
 * You probably do not need to modify this class.
 */
final class Logger {
    private static final DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss.SSS");

    private final String hostname;

    Logger(String hostname) {
        this.hostname = hostname;
    }

    private void log(String message) {
        String time = formatter.format(java.time.LocalDateTime.now());
        String formatted = String.format("%s analyzer [%s]: %s", time, hostname, message);
        System.out.println(formatted);
    }

    private void log(String format, Object... args) {
        log(String.format(format, args));
    }

    public void subscribingToMonitorTopic() {
        log("Subscribing to the monitor topic.");
    }

    public void periodicCommitOfStatesAndOffsets() {
        log("Periodic commit of states and offsets.");
    }

    public void monitorRecordRead(ConsumerRecord<?, ?> record) {
        log("Read (%s, %s) from monitor partition %d at offset %d.", record.key(), record.value(), record.partition(), record.offset());
    }

    public void anomalyDetected(Anomaly anomaly) {
        log("Detected %s.", anomaly);
    }

    public void monitorPartitionsAssigned(Collection<TopicPartition> topicPartitions) {
        log("Assigned monitor partitions %s.", topicPartitions);
    }

    public void monitorPartitionStateRestored(int monitorPartition, long restoredOffset, Collection<AnomalyAnalyzer> restoredAnalyzers) {
        log("Restored state of monitor partition %d. Starting from offset %d.", monitorPartition, restoredOffset);
        for (AnomalyAnalyzer analyzer : restoredAnalyzers) {
            log("    Restored %s.", analyzer);
        }
    }

    public void monitorPartitionsRevoked(Collection<TopicPartition> topicPartitions) {
        log("Revoked monitor partitions %s.", topicPartitions);
    }

    public void monitorPartitionStateCommitted(int monitorPartition, long savedOffset, Collection<AnomalyAnalyzer> savedAnalyzers) {
        log("Committed state of monitor partition %d. Saved consumer offset is %d.", monitorPartition, savedOffset);
        for (AnomalyAnalyzer analyzer : savedAnalyzers) {
            log("    Saved %s.", analyzer);
        }
    }
}
