package cz.cuni.mff.d3s.nswi080.anomaly;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.kafka.common.serialization.Deserializer;

/**
 * Deserializes a Kafka key/value from JSON to a Java object using Jackson.
 * <p>
 * Example usage:
 * <pre>
 * {@code
 * Properties properties = new Properties();
 * properties.setProperty(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, AnalyzerStateValue.AnalyzerStateValueDeserializer.class.getName());
 * }
 * </pre>
 *
 * @param <T> the type of the serialized Java class
 */
public abstract class JacksonKafkaDeserializer<T> implements Deserializer<T> {
    private final ObjectMapper objectMapper = new ObjectMapper();
    private final Class<T> type;

    public JacksonKafkaDeserializer(Class<T> type) {
        this.type = type;
    }

    @Override
    public T deserialize(String topic, byte[] data) {
        if (data == null) {
            return null;
        }
        try {
            return objectMapper.readValue(data, type);
        } catch (Exception e) {
            throw new RuntimeException("Error deserializing JSON to object", e);
        }
    }
}
