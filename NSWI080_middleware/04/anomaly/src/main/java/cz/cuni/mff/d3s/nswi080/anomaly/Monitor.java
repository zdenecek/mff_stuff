package cz.cuni.mff.d3s.nswi080.anomaly;

import oshi.SystemInfo;
import oshi.hardware.HardwareAbstractionLayer;
import oshi.hardware.Sensors;

import java.time.Duration;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Random;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.kafka.common.serialization.StringSerializer;

/**
 * The monitor program.
 */
public class Monitor {
    /**
     * The period between taking the measurements.
     */
    private static final Duration SAMPLE_PERIOD = Duration.ofSeconds(3);

    /**
     * The topic where measurements are sent.
     */
    private static final String MONITOR_TOPIC = "monitor";

    public static void main(String[] args) throws InterruptedException {
        // PLEASE NOTE
        // FOR SOME REASON, ALL THE MEASUREMENTS ARE 0, I AM NOT SURE WHY (anomalies are different)

        if (args.length == 0) {
            throw new IllegalArgumentException("Usage: <program> hostname");
        }
        String hostname = args[0];
        SystemInfo si = new SystemInfo();
        HardwareAbstractionLayer hal = si.getHardware();
        Sensors sensors = hal.getSensors();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss.SSS");
        Random random = new Random();
        String bootstrapServers = "localhost:9092";

        // (part 1): Carefully select the key and value and create a producer.
        java.util.Properties props = new java.util.Properties();
        props.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, StringSerializer.class.getName());
        props.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG, JacksonKafkaSerializer.class.getName());
        props.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);

        try (KafkaProducer<String, Double> producer = new KafkaProducer<>(props)) {
            while (true) {
                LocalDateTime now = LocalDateTime.now();
                double cpuTemp = sensors.getCpuTemperature();
                // Simulate random failures of the sensor (do not remove this).
                if (random.nextInt() % 31 == 0) {
                    cpuTemp = Double.longBitsToDouble(random.nextLong());
                }
                System.out.printf("%s monitor [%s]: measured %s%n", formatter.format(now), hostname, cpuTemp);

                // (part 1): Send the data to the monitor topic.
                ProducerRecord<String, Double> record = new ProducerRecord<>(MONITOR_TOPIC, hostname, cpuTemp);
                producer.send(record);
                
                Thread.sleep(SAMPLE_PERIOD);
            }
        }
    }
}
