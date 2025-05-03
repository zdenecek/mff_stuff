package cz.cuni.mff.d3s.nswi080.anomaly;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.kafka.common.serialization.Serializer;

/**
 * Serializes a Kafka key/value to JSON using Jackson.
 * <p>
 * Example usage:
 * <pre>
 * {@code
 * Properties properties = new Properties();
 * properties.setProperty(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, JacksonKafkaSerializer.class.getName());
 * }
 * </pre>
 */
public class JacksonKafkaSerializer<T> implements Serializer<T> {
    private final ObjectMapper objectMapper = new ObjectMapper();

    @Override
    public byte[] serialize(String topic, T data) {
        if (data == null) {
            return null;
        }
        try {
            return objectMapper.writeValueAsBytes(data);
        } catch (Exception e) {
            throw new RuntimeException("Error serializing object to JSON", e);
        }
    }
}
