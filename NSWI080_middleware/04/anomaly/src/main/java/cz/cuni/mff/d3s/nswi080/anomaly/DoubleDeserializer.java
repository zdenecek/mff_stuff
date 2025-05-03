package cz.cuni.mff.d3s.nswi080.anomaly;

public class DoubleDeserializer extends JacksonKafkaDeserializer<Double> {
    public DoubleDeserializer() {
        super(Double.class);
    }
} 